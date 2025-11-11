using JuMP
import HiGHS

using DelimitedFiles

function parse_graph_data(filename::String)
    data_lines = readlines(filename)
    
    # 1. Wczytanie stałych (Linia 1)
    constants = parse.(Int, filter!(!isempty, split(data_lines[1])))
    n_vertices = constants[1]
    n_edges = constants[2]
    start = constants[3]
    final = constants[4]
    T = constants[5]

    # 2. Wczytanie krawędzi (Linia 2 do końca)
    # Każda krawędź to (src, dst, cost, time)
    edges = zeros(Int, n_edges, 4)
    
    for k in 1:n_edges
        edges[k, :] = parse.(Int, filter!(!isempty, split(data_lines[1 + k])))
    end

    return edges, start, final, T, n_vertices, n_edges
end



function flow_out(v::Int, x::Array{<:Any, 1}, edges::Array)
    return sum((x[k] for k in 1:size(edges, 1) if edges[k, 1] == v); init=0)
end

function flow_in(v::Int, x::Array{<:Any, 1}, edges::Array)
    return sum((x[k] for k in 1:size(edges, 1) if edges[k, 2] == v); init=0)
end

function sumOccurences(edges::Array, cur::Int, x::Array)
    return flow_out(cur, x, edges)  - flow_in(cur, x, edges)
end

function graphModel(edges::Array, start::Int, final::Int, T::Int, n_vertices::Int, n_edges::Int)
    model = Model(HiGHS.Optimizer)

    @variable(model, x[i in 1:n_edges], Bin)

    @constraint(model, timeLimit, sum(x[i]*edges[i, 4] for i in 1:n_edges) <= T)

    @constraint(model, flowStart, sumOccurences(edges, start, x)==1)
    
    @constraint(model, flowEnd, sumOccurences(edges, final, x)==-1)
    
    @constraint(model, flow[i in 1:n_vertices; i != start && i != final], sumOccurences(edges, i, x)==0)

    @objective(model, Min, sum(x[i]*edges[i,3] for i in 1:n_edges))
    
    optimize!(model)

    println("\n--- Wyniki Optymalizacji ---")
    status = termination_status(model)
    if status == MOI.OPTIMAL
        println("STATUS: OPTYMALNY")
        println("Minimalny koszt ścieżki: ", objective_value(model))

        X_val = value.(x)
        sel = [k for k in 1:n_edges if round(Int, X_val[k]) == 1]

        if isempty(sel)
            println("Brak wybranych krawędzi w rozwiązaniu.")
        else
            println("\nWybrane krawędzie (idx: src -> dst | koszt, czas):")
            for k in sel
                println(" ", k, ": ", edges[k,1], " -> ", edges[k,2], " | koszt=", edges[k,3], ", czas=", edges[k,4])
            end

            total_cost = sum(edges[k,3] for k in sel)
            total_time = sum(edges[k,4] for k in sel)
            println("\nCałkowity koszt: ", total_cost)
            println("Całkowity czas: ", total_time, " (Limit: ", T, ")")

            # Rekonstrukcja ścieżki (kolejno od start do final)
            path = [start]
            cur = start
            visited = Set([start])
            for _ in 1:n_edges
                idx_in_sel = findfirst(i -> edges[i,1] == cur, sel)
                if idx_in_sel === nothing
                    break
                end
                edge_idx = sel[idx_in_sel]
                nxt = edges[edge_idx, 2]
                push!(path, nxt)
                if nxt == final
                    break
                end
                if nxt in visited
                    println("Uwaga: wykryto cykl podczas rekonstruowania ścieżki.")
                    break
                end
                push!(visited, nxt)
                cur = nxt
            end
            println("\nZrekonstruowana ścieżka (wierzchołki): ", join(path, " -> "))
        end
    else
        println("STATUS: NIEOPTYMALNY (", status, ")")
    end
end

DATA_FILE = "graph2.txt"
try
    
    # Odczyt danych
    edges, start, final, T, n_vertices, n_edges = parse_graph_data(DATA_FILE)
    
    # Uruchomienie rozwiazania
    graphModel(edges, start, final, T, n_vertices, n_edges)

catch e
    println("\nBLAD PRZY URUCHAMIANIU MODELU:")
    println(e)
    println("\nUpewnij sie, ze plik $DATA_FILE istnieje i zawiera poprawne dane.")
end