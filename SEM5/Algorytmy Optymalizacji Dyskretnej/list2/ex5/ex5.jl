using JuMP
import HiGHS

function read_flow_data(filepath::String)
    
    # Inicjalizacja wynikow
    N_D = 0
    N_S = 0
    
    # Macierze przeplywu D->S (D x S)
    Min_Flow_DS = Array{Float64}(undef, 0, 0)
    Max_Flow_DS = Array{Float64}(undef, 0, 0)
    
    # Wektory przeplywu Source->D (N_D)
    Min_Flow_SD = Vector{Float64}()
    Max_Flow_SD = Vector{Float64}()

    # Wektory przeplywu S->Final (N_S)
    Min_Flow_SF = Vector{Float64}()
    Max_Flow_SF = Vector{Float64}()

    current_section = :dimensions
    lines = readlines(filepath)
    line_index = 1
    
    # Funkcja pomocnicza do parsowania pojedynczej linii z liczbami
    function parse_numbers(str)
        return parse.(Float64, split(str))
    end

    # Funkcja pomocnicza do czytania macierzy (N_ROWS x N_COLS)
    function read_matrix(N_ROWS, N_COLS, current_line_index)
        temp_rows = Vector{Vector{Float64}}()
        
        for _ in 1:N_ROWS
            if current_line_index > length(lines)
                error("Plik danych zakonczyl sie zbyt wczesnie podczas czytania macierzy.")
            end
            
            row_data = parse_numbers(lines[current_line_index])
            if length(row_data) != N_COLS
                error("Bład: Wiersz macierzy ma $(length(row_data)) kolumn, oczekiwano $N_COLS.")
            end
            push!(temp_rows, row_data)
            current_line_index += 1
        end
        
        # Zmiana z Array of Arrays na Macierz
        return reduce(vcat, transpose.(temp_rows)), current_line_index
    end

    while line_index <= length(lines)
        line = strip(lines[line_index])
        line_index += 1
        
        # Ignoruj komentarze i puste linie, uzywaj ich jako markery sekcji
        if startswith(line, "#") || isempty(line)
            if occursin("D->S", line)
                current_section = :flow_ds_min
            elseif occursin("SOURCE->D", line)
                current_section = :flow_sd
            elseif occursin("S->FINAL", line)
                current_section = :flow_sf
            end
            continue
        end

        # --- Parsowanie sekcji ---
        
        if current_section == :dimensions
            parts = split(line, '='; limit=2)
            if length(parts) == 2
                key = strip(parts[1])
                value = parse(Int, strip(parts[2]))
                if key == "N_DISTRICT"
                    N_D = value
                elseif key == "N_SHIFT"
                    N_S = value
                end
            end
            
        elseif current_section == :flow_ds_min
            # Czytanie macierzy MIN_FLOW_D_S
            Min_Flow_DS, line_index = read_matrix(N_D, N_S, line_index - 1)
            current_section = :flow_ds_max

        elseif current_section == :flow_ds_max
            # Czytanie macierzy MAX_FLOW_D_S
            Max_Flow_DS, line_index = read_matrix(N_D, N_S, line_index - 1)
            current_section = :after_flow_ds
            
        elseif current_section == :flow_sd
            # Czytanie wektorow Source->D
            if isempty(Min_Flow_SD)
                Min_Flow_SD = parse_numbers(line)
            elseif isempty(Max_Flow_SD)
                Max_Flow_SD = parse_numbers(line)
                current_section = :after_flow_sd
            end
            
        elseif current_section == :flow_sf
            # Czytanie wektorow S->Final
            if isempty(Min_Flow_SF)
                Min_Flow_SF = parse_numbers(line)
            elseif isempty(Max_Flow_SF)
                Max_Flow_SF = parse_numbers(line)
                current_section = :after_flow_sf
            end
        end
    end

    # Walidacja koncowa
    if N_D == 0 || N_S == 0 || size(Min_Flow_DS) != (N_D, N_S) || length(Min_Flow_SD) != N_D || length(Min_Flow_SF) != N_S
        error("Błąd w danych: Niepoprawne wymiary lub brakujące macierze/wektory.")
    end

    return N_D, N_S, Min_Flow_DS, Max_Flow_DS, Min_Flow_SD, Max_Flow_SD, Min_Flow_SF, Max_Flow_SF
end

function createEdges(n_district::Int, n_shift::Int, max_array::Array, min_array::Array)
    temp = Dict{Tuple{Int, Int}, Tuple{Float64, Float64}}()
    for i in 1:n_district
        for j in 1:n_shift 
            temp[(i, j)] = (min_array[i,j], max_array[i, j])
        end 
    end
    return temp;
end 

function graphModel(n_district::Int, n_shift::Int, max_array::Array, min_array::Array, source_min::Array, source_max::Array, final_min::Array, final_max::Array)
    
    edges = createEdges(n_district, n_shift, max_array, min_array)
    
    model = Model(HiGHS.Optimizer)

    @variable(model, edges[(i, j)][1] <= f[i in 1:n_district, j in 1:n_shift] <= edges[(i, j)][2])

    @variable(model, source_min[i] <= from_source[i in 1:n_district] <= source_max[i])

    @variable(model, final_min[i] <= to_final[i in 1:n_district] <= final_max[i])

    @variable(model, backwardedge >= 0)

    @constraint(model, flow, sum(x[i]*edges[i][4] for i in 1:edges.size()) <= T)

    @constraint(model, flowStart, backwardedge - sum(from_source[i] for i in 1:n_district) == 0)
    
    @constraint(model, flow1stlayer[i in 1:n_district], from_source[i] - sum(f[i, j] for j in 1:n_shift)==0)
    
    @constraint(model, flow2ndlayer[i in 1:n_shift], sum(f[j, i] for j in 1:n_district) - to_final[i] ==0)

    @constraint(model, flowFinal, sum(to_final[i] for i in 1:n_shift) - backwardedge == 0)

    @objective(model, Min, backwardedge)
    
    optimize!(model)

    if termination_status(model) == MOI.OPTIMAL
        println("OPTIMAL")
    else
        println("NON OPTIMAL: ", termination_status)
    end
end

const DATA_FILE = "flow_data.txt"

# Wczytanie danych z pliku
try
    # Odczyt danych
    N_D, N_S, Min_DS, Max_DS, Min_SD, Max_SD, Min_SF, Max_SF = read_flow_data(DATA_FILE)
    
    # Uruchomienie rozwiazania
    create_flow_model(N_D, N_S, Min_DS, Max_DS, Min_SD, Max_SD, Min_SF, Max_SF)

catch e
    println("\nBLAD PRZY URUCHAMIANIU MODELU:")
    println(e)
    println("\nUpewnij sie, ze plik $DATA_FILE istnieje i zawiera poprawne dane.")
end