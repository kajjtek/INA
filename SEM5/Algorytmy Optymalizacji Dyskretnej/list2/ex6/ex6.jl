using JuMP
import HiGHS


function read_coverage_data(filepath::String)
    data = Dict{String, Int}()
    
    # Otwiera i czyta plik, ignorując linie puste i komentarze
    open(filepath, "r") do file
        for line in eachline(file)
            line = strip(line)
            if startswith(line, "#") || isempty(line)
                continue
            end
            
            # Parsowanie linii w formacie KLUCZ = WARTOSC
            parts = split(line, '='; limit=2)
            if length(parts) == 2
                key = strip(parts[1])
                try
                    value = parse(Int, strip(parts[2]))
                    data[key] = value
                catch e
                    println("Blad parsowania wartosci dla klucza '$key': $e")
                end
            end
        end
    end
    
    # Walidacja danych
    if !haskey(data, "N") || !haskey(data, "M") || !haskey(data, "K")
        error("Plik danych musi zawierac N, M i K.")
    end

    # N to wiersze, M to kolumny
    return data["N"], data["M"], data["K"]
end

function scan_squares(x::Array, k::Int, i_cur::Int, j_cur::Int, m::Int, n::Int)
    i_start = max(1, i_cur-k)
    i_end = min(n, i_cur+k)
    j_start = max(1, j_cur-k)
    j_end = min(m, j_cur+k)

    return sum(x[i,j] for i in i_start:i_end, j in j_start:j_end)
end 

function createModel(m::Int, n::Int, k::Int) #kwadrat mxn
    model = Model(HiGHS.Optimizer)

    @variable(model, x[i in 1:n, j in 1:m], Bin) #0 - kontener #1 kamera

    @constraint(model, observed[i in 1:n, j in 1:m], scan_squares(x, k, i, j, m, n)>=1)

    @objective(model, min, sum(x[i, j] for i in 1:n, j in 1:m))

    optimize!(model)

    if termination_status(model) == MOI.OPTIMAL
        println("OPTIMAL")
    else
        println("NON OPTIMAL: ", termination_status)
    end
end 


const DATA_FILE = "coverage_data.txt"

# Wczytanie danych z pliku
try
    N, M, K = read_coverage_data(DATA_FILE)
    
    # Uruchomienie rozwiazania
    createModel(N, M, K)

catch e
    println("\nBLAD PRZY URUCHAMIANIU MODELU:")
    println(e)
    println("\nUpewnij sie, ze plik $DATA_FILE istnieje i zawiera poprawne dane.")
end