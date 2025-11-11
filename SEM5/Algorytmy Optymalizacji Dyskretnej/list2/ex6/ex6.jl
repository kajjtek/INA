using JuMP
import HiGHS
using DelimitedFiles

function parse_camera_data_simple(filename::String)
    # Wczytanie wszystkich danych z pliku (oczekujemy tylko jednej linii)
    local_data = readdlm(filename, ' ', Int)

    # 1. Wymiary siatki z pierwszego wiersza
    if size(local_data, 1) < 1 || size(local_data, 2) < 2
        error("Plik danych musi zawierać w pierwszym wierszu: m n (np. 5 5).")
    end
    
    m = local_data[1, 1] # Liczba kolumn (jak w Twoim modelu)
    n = local_data[1, 2] # Liczba wierszy (jak w Twoim modelu)
    k = local_data[1, 3]

    println("--- Wczytane Dane (Zadanie 6) ---")
    println("Wymiary siatki (m x n): $m x $n")
    println("-----------------------")

    # Zwracamy wymiary. Kontenery (Y) będą zdefiniowane w części głównej programu.
    return m, n, k
end

function scan_squares(x::Array, k::Int, i_cur::Int, j_cur::Int, m::Int, n::Int)
    i_start = max(1, i_cur-k)
    i_end = min(n, i_cur+k)
    j_start = max(1, j_cur-k)
    j_end = min(m, j_cur+k)



    return sum(x[i_cur,j] for j in j_start:j_end) + sum(x[i, j_cur] for i in i_start:i_end)
end 

function createModel(m::Int, n::Int, k::Int) #kwadrat mxn
    model = Model(HiGHS.Optimizer)

    @variable(model, x[i in 1:n, j in 1:m], Bin) #0 - kontener #1 kamera

    @constraint(model, observed[i in 1:n, j in 1:m], scan_squares(x, k, i, j, m, n)>=1)

    @objective(model, Min, sum(x[i, j] for i in 1:n, j in 1:m))

    optimize!(model)

    println("\n--- Wyniki Modelu Umieszczenia Kamer ---")
    status = termination_status(model)
    if status == MOI.OPTIMAL
        println("STATUS: OPTYMALNY")
        println("Minimalna liczba kamer: ", objective_value(model))
        
        x_val = value.(x)
        camera_positions = [(i, j) for i in 1:n, j in 1:m if round(Int, x_val[i,j]) == 1]
        
        println("\nPositions kamer (wiersz, kolumna):")
        for pos in camera_positions
            println(" ", pos)
        end
        
        println("\nSiatka (1=kamera, 0=brak):")
        for i in 1:n
            for j in 1:m
                print(round(Int, x_val[i,j]))
            end
            println()
        end
        
        println("\nZasięg każdej kamery (kwadrat o boku ", 2*k+1, "):")
        for (cam_i, cam_j) in camera_positions
            i_start = max(1, cam_i - k)
            i_end = min(n, cam_i + k)
            j_start = max(1, cam_j - k)
            j_end = min(m, cam_j + k)
            println(" Kamera at ($cam_i, $cam_j) pokrywa od ($i_start, $j_start) do ($i_end, $j_end)")
        end
    else
        println("STATUS: NIEOPTYMALNY (", status, ")")
    end
end 


const DATA_FILE = "coverage_data.txt"

# Wczytanie danych z pliku
try
    m, n, k = parse_camera_data_simple(DATA_FILE)
    
    # Uruchomienie rozwiazania
    createModel(m, n, k)

catch e
    println("\nBLAD PRZY URUCHAMIANIU MODELU:")
    println(e)
    println("\nUpewnij sie, ze plik $DATA_FILE istnieje i zawiera poprawne dane.")
end