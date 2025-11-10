using JuMP
import HiGHS

function read_assignment_data(filepath::String)
    
    # Inicjalizacja wynikow
    N_AIRPORTS = 0
    N_COMPANIES = 0
    C_matrix = Array{Float64}(undef, 0, 0)
    Min_Limits = Vector{Float64}()
    Max_Limits = Vector{Float64}()

    # Flagi do sledzenia aktualnie przetwarzanej sekcji
    current_section = :dimensions
    
    lines = readlines(filepath)
    line_index = 1
    
    while line_index <= length(lines)
        line = strip(lines[line_index])
        line_index += 1
        
        # Ignoruj komentarze i puste linie, ale uzywaj ich jako markery sekcji
        if startswith(line, "#") || isempty(line)
            if occursin("KOSZTÓW C", line)
                current_section = :cost_matrix
            elseif occursin("MINIMALNE WYMAGANIA LOTNISK", line)
                current_section = :min_limits
            elseif occursin("MAKSYMALNE ZDOLNOŚCI FIRM", line)
                current_section = :max_limits
            end
            continue
        end

        # --- Parsowanie sekcji ---
        
        if current_section == :dimensions
            parts = split(line, '='; limit=2)
            if length(parts) == 2
                key = strip(parts[1])
                value = parse(Int, strip(parts[2]))
                if key == "N_AIRPORTS"
                    N_AIRPORTS = value
                elseif key == "N_COMPANIES"
                    N_COMPANIES = value
                end
            end
            
        elseif current_section == :cost_matrix
            # Wczytywanie macierzy C (N_AIRPORTS x N_COMPANIES)
            if N_AIRPORTS > 0 && N_COMPANIES > 0
                temp_rows = Vector{Vector{Float64}}()
                
                # Wczytuj N_AIRPORTS wierszy macierzy C
                for _ in 1:N_AIRPORTS
                    # Wyczytanie, parsowanie i dodanie wiersza
                    row_data = parse.(Float64, split(lines[line_index-1]))
                    if length(row_data) != N_COMPANIES
                        error("Bład: Wiersz kosztów ma $(length(row_data)) kolumn, oczekiwano $N_COMPANIES.")
                    end
                    push!(temp_rows, row_data)
                    line_index += 1 # Przesuniecie indeksu po udanym parsowaniu wiersza
                end
                
                # Zmiana z Array of Arrays na Macierz
                C_matrix = reduce(vcat, transpose.(temp_rows))
                current_section = :after_cost_matrix # Zmien sekcje, aby nie czytac dalej wierszy macierzy
                line_index -= 1 # Cofniecie, poniewaz ostatni wiersz zostal juz przetworzony
            end
            
        elseif current_section == :min_limits
            # Wczytanie wektora minimalnych wymagan lotnisk
            Min_Limits = parse.(Float64, split(line))
            
        elseif current_section == :max_limits
            # Wczytanie wektora maksymalnych zdolnosci firm
            Max_Limits = parse.(Float64, split(line))
        end
    end

    # Walidacja koncowa
    if isempty(C_matrix) || isempty(Min_Limits) || isempty(Max_Limits)
        error("Brak wszystkich wymaganych danych (C, Min/Max Limits).")
    end
    if length(Min_Limits) != N_AIRPORTS || length(Max_Limits) != N_COMPANIES
        error("Bład wymiarów: Wektory granic nie pasuja do N_AIRPORTS / N_COMPANIES.")
    end

    return N_AIRPORTS, N_COMPANIES, C_matrix, Max_Limits, Min_Limits
end

function createModel(c::Array, max_limits::Array, min_limits::Array, n_companies::Int, n_airports::Int)
    model = Model(HiGHS.Optimizer)

    @variable(model, x[i in 1:n_airports, j in 1:n_companies] >= 0)

    @constraint(model, comp_limit[i in 1:n_companies], sum(x[j,i] for j in 1:n_airports) <= max_limits[i])

    @constraint(model, air_limit[i in 1:n_airports],sum(x[i,j] for j in 1:n_companies) >= min_limits[i])

    @objective(model, Min, sum(c[i,j]*x[i,j] for i in 1:n_airports, j in 1:n_companies))

    optimize!(model)

    if termination_status(model) == MOI.OPTIMAL
        println("OPTIMAL")
    else
        println("NON OPTIMAL: ", termination_status)
    end
end 

c = [
    10 7 8;
    10 11 14;
    9 12 4;
    11 13 9
]

min_limits = [1.1e5, 2.2e5, 3.3e5, 4.4e5]
max_limits = [2.75e5, 5.5e5, 6.6e5]



n_companies = 3
n_airports = 4

const DATA_FILE = "assignment_data.txt"

# Wczytanie danych z pliku
try
    # Odczyt danych
    N_A, N_C, C, Max_L, Min_L = read_assignment_data(DATA_FILE)
    
    # Uruchomienie rozwiazania
    createModel(C, Max_L, Min_L, N_A, N_C)

catch e
    println("\nBLAD PRZY URUCHAMIANIU MODELU:")
    println(e)
    println("\nUpewnij sie, ze plik $DATA_FILE istnieje i zawiera poprawne dane.")
end