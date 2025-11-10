using JuMP
import HiGHS

function read_inventory_data(filepath::String)
    
    # Inicjalizacja skalarnych
    N_INTERVALS = 0
    CAPACITY = 0
    I0 = 0
    INV_COST = 0.0

    # Inicjalizacja wektorow
    PC = Vector{Float64}() # Production Cost
    MO = Vector{Float64}() # Max Overflow
    CO = Vector{Float64}() # Cost Overflow
    D = Vector{Float64}()  # Demand

    current_section = :dimensions
    lines = readlines(filepath)
    line_index = 1
    
    function parse_numbers(str)
        return parse.(Float64, split(str))
    end
    
    while line_index <= length(lines)
        line = strip(lines[line_index])
        line_index += 1
        
        # Ignoruj komentarze i puste linie
        if startswith(line, "#") || isempty(line)
            if occursin("PARAMETRY KOSZTOWE I POPYTU", line)
                current_section = :vectors
            elseif occursin("KOSZT ZAPASU", line)
                current_section = :inv_cost
            end
            continue
        end

        # --- Parsowanie sekcji ---
        
        if current_section == :dimensions
            parts = split(line, '='; limit=2)
            if length(parts) == 2
                key = strip(parts[1])
                value = parse(Int, strip(parts[2]))
                if key == "N_INTERVALS"
                    N_INTERVALS = value
                elseif key == "CAPACITY"
                    CAPACITY = value
                elseif key == "I0"
                    I0 = value
                end
            end
            
        elseif current_section == :vectors
            # Wczytywanie wektorow, jeden po drugim
            numbers = parse_numbers(line)
            if isempty(PC) && length(numbers) == N_INTERVALS
                PC = numbers
            elseif isempty(MO) && length(numbers) == N_INTERVALS
                MO = numbers
            elseif isempty(CO) && length(numbers) == N_INTERVALS
                CO = numbers
            elseif isempty(D) && length(numbers) == N_INTERVALS
                D = numbers
                current_section = :after_vectors # Przejscie do nastepnej sekcji
            end

        elseif current_section == :inv_cost
            INV_COST = parse(Float64, line)
        end
    end

    # Walidacja
    if N_INTERVALS == 0 || CAPACITY == 0 || INV_COST == 0.0 || isempty(PC) || isempty(D)
        error("Brak wszystkich wymaganych danych lub niepoprawne wymiary wektorów.")
    end
    
    return N_INTERVALS, PC, MO, CO, D, CAPACITY, I0, INV_COST
end


function createModel(n_intervals::Int, production_cost::Array, max_overflow::Array, cost_overflow::Array, demand::Array, capacity::Int, I0::Int)
    model = Model(HiGHS.Optimizer)

    @variable(model, 0<= x[i in 1:n_intervals] <= 100)
    @variable(model, 0<= y[i in 1:n_intervals] <= max_overflow[i])
    @variable(model, 0<= I[i in 1:n_intervals] <=capacity)

    @constraint(model, bilans_init, I[1] == I0 + x[1] + y[1] - demand[1])

    @constraint(model, bilans[i in 2:n_intervals], I[i] == I[i-1] + x[i] + y[i] - demand[i])

    @objective(model, Min, sum(x[i]*production_cost[i] + y[i]*cost_overflow[i] + 1500*I[i] for i in 1:n_intervals))


    optimize!(model)

    if termination_status(model) == MOI.OPTIMAL
        println("OPTIMAL")
    else
        println("NON OPTIMAL: ", termination_status)
    end
end 

const DATA_FILE = "inventory_data.txt"

# Wczytanie danych z pliku
try
    # Odczyt danych
    N_T, PC, MO, CO, D, C, I0_val, Inv_Cost = read_inventory_data(DATA_FILE)
    
    # Uruchomienie rozwiazania
    create_inventory_model(N_T, PC, MO, CO, D, C, I0_val, Inv_Cost)

catch e
    println("\nBLAD PRZY URUCHAMIANIU MODELU:")
    println(e)
    println("\nUpewnij sie, ze plik $DATA_FILE istnieje i zawiera poprawne dane.")
end

n_intervals = 4

production_cost = [6000, 4000, 8000, 9000]
max_overflow = [60, 65, 70, 60]
cost_overflow = [8000, 6000, 10000, 11000]
demand = [130, 80, 125, 195]
capacity = 70
I0 = 15

