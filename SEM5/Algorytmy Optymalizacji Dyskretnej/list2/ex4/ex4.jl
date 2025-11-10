using JuMP
import HiGHS

function read_graph_data(filepath::String)
    
    # Inicjalizacja skalarnych
    N_VERTICES = 0
    START_NODE = 0
    FINAL_NODE = 0
    MAX_TIME = 0.0

    # Inicjalizacja macierzy krawedzi
    Edges_List = Vector{Vector{Float64}}()

    current_section = :dimensions
    lines = readlines(filepath)
    line_index = 1
    
    function parse_numbers(str)
        # Parsuje liczby, ale oczekuje Int dla Source/Destination, Float dla Cost/Time
        return parse.(Float64, split(str))
    end

    while line_index <= length(lines)
        line = strip(lines[line_index])
        line_index += 1
        
        # Ignoruj komentarze i puste linie
        if startswith(line, "#") || isempty(line)
            if occursin("LISTA KRAWĘDZI", line)
                current_section = :edges
            end
            continue
        end

        # --- Parsowanie sekcji ---
        
        if current_section == :dimensions
            parts = split(line, '='; limit=2)
            if length(parts) == 2
                key = strip(parts[1])
                # Wymiary sa Int, MAX_TIME jest Float
                if key == "MAX_TIME"
                    MAX_TIME = parse(Float64, strip(parts[2]))
                else
                    value = parse(Int, strip(parts[2]))
                    if key == "N_VERTICES"
                        N_VERTICES = value
                    elseif key == "START_NODE"
                        START_NODE = value
                    elseif key == "FINAL_NODE"
                        FINAL_NODE = value
                    end
                end
            end
            
        elseif current_section == :edges
            # Wczytywanie macierzy krawedzi
            push!(Edges_List, parse_numbers(line))
        end
    end

    # Walidacja
    if N_VERTICES == 0 || START_NODE == 0 || FINAL_NODE == 0 || MAX_TIME == 0.0 || isempty(Edges_List)
        error("Brak wszystkich wymaganych danych (wymiary, węzły, limit czasu lub krawędzie).")
    end
    
    # Konwersja Edges_List do Array{Float64, 2}
    Edges_Matrix = reduce(vcat, transpose.(Edges_List))

    # Walidacja krawedzi (powinny miec 4 kolumny)
    if size(Edges_Matrix, 2) != 4
         error("Błąd: Macierz krawędzi musi mieć 4 kolumny (Source, Destination, Cost, Time).")
    end

    return Edges_Matrix, N_VERTICES, START_NODE, FINAL_NODE, MAX_TIME
end

function flow_out(v::Int, x::Array{<:Any, 1}, edges::Array)
    return sum(x[k] for (k, edge) in enumerate(edges) if edge[1] == v)
end

function flow_in(v::Int, x::Array{<:Any, 1}, edges::Array)
    return sum(x[k] for (k, edge) in enumerate(edges) if edge[2] == v)
end

function sumOccurences(edges::Array, cur::Int, x::Array)
    return flow_in(cur, x, edges) - flow_out(cur, x, edges)
end

function graphModel(edges::Array, start::Int, final::Int, T::Int, n_vertices::Int)
    model = Model(HiGHS.Optimizer)

    @variable(model, x[i in 1:edges.size()], Bin)

    @constraint(model, timeLimit, sum(x[i]*edges[i][4] for i in 1:edges.size()) <= T)

    @constraint(model, flowStart, sumOccurences(edges, start, edges)==1)
    
    @constraint(model, flowEnd, sumOccurences(edges, final, x)==-1)
    
    @constraint(model, flow[i in 1:n_vertices], sumOccurences(edges, i, x)==0)

    @objective(model, min, sum(x[i]*edges[i][3]))
    
    optimize!(model)

    println("\n--- Wyniki Optymalizacji ---")
    if termination_status(model) == MOI.OPTIMAL
        println("STATUS: OPTYMALNY")
        println("Minimalny koszt ścieżki: ", objective_value(model))
        
        X_val = value.(x)
        
        println("\nWybrana ścieżka (krawędź -> koszt, czas):")
        total_cost = 0.0
        total_time = 0.0
        
        for k in 1:n_edges
            if round(Int, X_val[k]) == 1
                src = Int(edges[k, 1])
                dst = Int(edges[k, 2])
                cost = costs[k]
                time = times[k]
                
                println("Krawędź $src -> $dst (Koszt: $cost, Czas: $time)")
                total_cost += cost
                total_time += time
            end
        end
        println("\nCałkowity koszt: $total_cost")
        println("Całkowity czas: $total_time (Limit: $T)")

    else
        println("STATUS: NIEOPTYMALNY (", termination_status(model), ")")
    end
end

try
    # Odczyt danych
    Edges, N_V, S_Node, F_Node, Max_T = read_graph_data(DATA_FILE)
    
    # Uruchomienie rozwiazania
    createGraphModel(Edges, S_Node, F_Node, Max_T, N_V)

catch e
    println("\nBLAD PRZY URUCHAMIANIU MODELU:")
    println(e)
    println("\nUpewnij sie, ze plik $DATA_FILE istnieje i zawiera poprawne dane.")
end