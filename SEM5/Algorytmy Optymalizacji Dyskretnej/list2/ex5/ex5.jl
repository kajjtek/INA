using JuMP
import HiGHS

using DelimitedFiles

function parse_circulation_data(filename::String)
    lines = readlines(filename)
    
    # Row 1: dimensions
    dims = parse.(Int, split(lines[1]))
    n_district = dims[1]
    n_shift = dims[2]

    # Rows 2 to 1+n_district: min_array
    min_array = zeros(Int, n_district, n_shift)
    for i in 1:n_district
        values = parse.(Int, filter!(!isempty, split(lines[1 + i])))
        min_array[i, :] = values
    end
    
    # Rows 2+n_district to 1+2*n_district: max_array
    max_array = zeros(Int, n_district, n_shift)
    for i in 1:n_district
        values = parse.(Int, filter!(!isempty, split(lines[1 + n_district + i])))
        max_array[i, :] = values
    end
    
    # Row 2+2*n_district: source_min
    source_min = parse.(Int, filter!(!isempty, split(lines[2 + 2*n_district])))
    
    # Row 3+2*n_district: source_max
    source_max = parse.(Int, filter!(!isempty, split(lines[3 + 2*n_district])))
    
    # Row 4+2*n_district: final_min
    final_min = parse.(Int, filter!(!isempty, split(lines[4 + 2*n_district])))
    
    # Row 5+2*n_district: final_max
    final_max = parse.(Int, filter!(!isempty, split(lines[5 + 2*n_district])))

    println("--- Wczytane Dane (Zadanie 5) ---")
    println("Minimalny przydział P -> Z:\n", min_array)
    println("Maksymalny przydział P -> Z:\n", max_array)
    println("Minima dla Dzielnic (Source): ", source_min)
    println("Maksima dla Dzielnic (Source): ", source_max)
    println("Minima dla Zmian (Final): ", final_min)
    println("Maksima dla Zmian (Final): ", final_max)
    println("-----------------------")

    return n_district, n_shift, max_array, min_array, source_min, source_max, final_min, final_max
end

# Przykładowe użycie:
# n_d, n_s, max_a, min_a, s_min, s_max, f_min, f_max = parse_circulation_data("dane_cyrkulacja.txt")
# graphModel(n_d, n_s, max_a, min_a, s_min, s_max, f_min, f_max)

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

    @constraint(model, flowStart, backwardedge - sum(from_source[i] for i in 1:n_district) == 0)
    
    @constraint(model, flow1stlayer[i in 1:n_district], from_source[i] - sum(f[i, j] for j in 1:n_shift)==0)
    
    @constraint(model, flow2ndlayer[i in 1:n_shift], sum(f[j, i] for j in 1:n_district) - to_final[i] ==0)

    @constraint(model, flowFinal, sum(to_final[i] for i in 1:n_shift) - backwardedge == 0)

    @objective(model, Min, backwardedge)
    
    optimize!(model)

    println("\n--- Wyniki Modelu Cyrkulacji Przepływu ---")
    status = termination_status(model)
    if status == MOI.OPTIMAL
        println("STATUS: OPTYMALNY")
        println("Minimalny przepływ wsteczny (backward edge): ", objective_value(model))
        
        f_val = value.(f)
        from_source_val = value.(from_source)
        to_final_val = value.(to_final)
        backward_val = value.(backwardedge)

        println("\nPrzepływ z źródła (Source -> Dzielnice):")
        for i in 1:n_district
            println(" Dzielnica ", i, ": ", round(from_source_val[i]; digits=3))
        end

        println("\nPrzepływ między warstwami (Dzielnice -> Zmiany):")
        for i in 1:n_district
            for j in 1:n_shift
                if round(f_val[i,j]; digits=3) > 0.001
                    println(" Z-> ", i, " do ", j, ": ", round(f_val[i,j]; digits=3), " [min: ", min_array[i,j], ", max: ", max_array[i,j], "]")
                end
            end
        end

        println("\nPrzepływ do końca (Zmiany -> Final):")
        for j in 1:n_shift
            println(" Zmiana ", j, ": ", round(to_final_val[j]; digits=3))
        end

        println("\nBackward edge (przepływ wsteczny): ", round(backward_val; digits=3))
        
        # Sprawdzenie bilansu
        total_from_source = sum(from_source_val)
        total_to_final = sum(to_final_val)
        println("\nBilans:")
        println(" Razem z źródła: ", round(total_from_source; digits=3))
        println(" Razem do końca: ", round(total_to_final; digits=3))
        println(" Backward edge:  ", round(backward_val; digits=3))
    else
        println("STATUS: NIEOPTYMALNY (", status, ")")
    end
end

const DATA_FILE = "flow_data.txt"

# Wczytanie danych z pliku
try
    # Odczyt danych
    n_district, n_shift, max_array, min_array, source_min, source_max, final_min, final_max = parse_circulation_data(DATA_FILE)
    
    # Uruchomienie rozwiazania
    graphModel(n_district, n_shift, max_array, min_array, source_min, source_max, final_min, final_max)

catch e
    println("\nBLAD PRZY URUCHAMIANIU MODELU:")
    println(e)
    println("\nUpewnij sie, ze plik $DATA_FILE istnieje i zawiera poprawne dane.")
end