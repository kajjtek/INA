using JuMP
import HiGHS

function read_production_data(filepath::String)
    
    # Inicjalizacja wynikow
    N_M = 0
    N_P = 0
    Time_Per_KG = Array{Float64}(undef, 0, 0)
    Max_Demand = Vector{Float64}()
    Product_Profit = Vector{Float64}()
    Machine_Cost = Vector{Float64}()
    Max_Time = 0.0

    current_section = :dimensions
    lines = readlines(filepath)
    line_index = 1
    
    # Przechowywanie danych macierzy i wektorow tymczasowo
    temp_matrix_lines = []
    
    # Funkcja pomocnicza do parsowania pojedynczej linii z liczbami
    function parse_numbers(str)
        return parse.(Float64, split(str))
    end

    while line_index <= length(lines)
        line = strip(lines[line_index])
        line_index += 1
        
        # Ignoruj komentarze i puste linie
        if startswith(line, "#") || isempty(line)
            if occursin("PARAMETRY CZASOWE", line)
                current_section = :time_matrix; temp_matrix_lines = []
            elseif occursin("PARAMETRY RYNKOWE", line)
                current_section = :market_params; temp_matrix_lines = []
            elseif occursin("PARAMETRY KOSZTOWE", line)
                current_section = :cost_params; temp_matrix_lines = []
            end
            continue
        end

        # --- Parsowanie sekcji ---
        
        if current_section == :dimensions
            parts = split(line, '='; limit=2)
            if length(parts) == 2
                key = strip(parts[1])
                value = parse(Int, strip(parts[2]))
                if key == "N_MACHINES"
                    N_M = value
                elseif key == "N_PRODUCT"
                    N_P = value
                end
            end
            
        elseif current_section == :time_matrix
            # Wczytywanie Macierzy Czasow (N_PRODUCT x N_MACHINES)
            push!(temp_matrix_lines, line)
            if length(temp_matrix_lines) == N_P && N_P > 0
                Time_Per_KG = reduce(vcat, transpose.(parse_numbers.(temp_matrix_lines)))
                current_section = :after_time_matrix # Sekcja odczytana
            end

        elseif current_section == :market_params
            # Wczytywanie wektorow popytu i zysku
            if isempty(Max_Demand)
                Max_Demand = parse_numbers(line)
            elseif isempty(Product_Profit)
                Product_Profit = parse_numbers(line)
                current_section = :after_market_params
            end
            
        elseif current_section == :cost_params
            # Wczytywanie wektora kosztu maszyny i stałej czasu
            if isempty(Machine_Cost)
                Machine_Cost = parse_numbers(line)
            else
                Max_Time = parse(Float64, line)
            end
        end
    end

    # --- Walidacja i upewnienie sie, ze jest to Array{Float64} ---
    if N_M == 0 || N_P == 0 || isempty(Time_Per_KG) || isempty(Max_Demand) || isempty(Product_Profit) || isempty(Machine_Cost) || Max_Time == 0.0
        error("Brak wszystkich wymaganych danych (wymiary, macierz, wektory lub limit czasu).")
    end
    
    if size(Time_Per_KG) != (N_P, N_M)
        error("Błąd wymiarów: Macierz czasów ma rozmiar $(size(Time_Per_KG)), oczekiwano ($N_P, $N_M).")
    end
    
    return N_M, N_P, Time_Per_KG, Max_Demand, Product_Profit, Machine_Cost, Max_Time
end

function createModel(n_machines::Int, n_product::Int, max_product_demand::Array, product_profit::Array, machine_hour_cost::Array, time_per_kg::Array)
    model = Model(HiGHS.Optimizer)

    @variable(model, p[i in 1:n_product, j in 1:n_machines] >= 0) #w minutach

    @constraint(model, max_time[i in 1:n_machines], sum(time_per_kg[j, i]*p[j,i] for j in 1:n_product) <= 3600)

    @constraint(model, max_kgs[i in 1:n_product], sum(p[i, j] for j in 1:n_machines) <=max_product_demand[i])

    @objective(model, Max, sum(product_profit[i]*sum(p[i, j] for j in 1:n_machines) for i in 1:n_product)-sum(machine_hour_cost[i]*sum(p[j, i] * time_per_kg[j, i] / 60 for j in 1:n_product) for i in 1:n_machines))

    optimize!(model)

    if termination_status(model) == MOI.OPTIMAL
        println("OPTIMAL")
    else
        println("NON OPTIMAL: ", termination_status)
    end
end


n_machines = 3
n_product = 4

# minuty na 1kg wyrobu i - produkt, j -maszyma
time_per_kg = [
    5 10 6;
    3 6 4;
    4 5 3;
    4 2 1
]

max_product_demand = [400, 100, 150, 500]

product_profit = [5, 6, 5, 4]

machine_hour_cost = [2, 2, 3]

const DATA_FILE = "production_data.txt"

# Wczytanie danych z pliku
try
    # Odczyt danych
    N_M, N_P, Time_KG, Max_D, Profit, Cost, Max_T = read_production_data(DATA_FILE)
    
    # Uruchomienie rozwiazania
    create_production_model(N_M, N_P, Time_KG, Max_D, Profit, Cost, Max_T)

catch e
    println("\nBLAD PRZY URUCHAMIANIU MODELU:")
    println(e)
    println("\nUpewnij sie, ze plik $DATA_FILE istnieje i zawiera poprawne dane.")
end