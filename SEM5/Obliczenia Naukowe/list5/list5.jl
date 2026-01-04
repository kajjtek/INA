include("parser.jl")
include("structure.jl")
include("matrixgen.jl")

using LinearAlgebra # potrzebne do funkcji norm()

# include("parser.jl")
# include("structure.jl")
# include("solvers.jl") # tu trzymaj gaussianElimination i back_substitution

function run_test()
    # 1. Parametry testu
    n = 100
    l = 4
    ck = 10.0
    filename = "A_test.txt"

    println("--- KROK 1: Generowanie macierzy ---")
    # Zakładam, że moduł matrixgen jest załadowany
    matrixgen.blockmat(n, l, ck, filename)

    println("--- KROK 2: Wczytywanie macierzy do struktury ---")
    BM = read_matrix(filename)

    println("--- KROK 3: Generowanie wektora b (sumy wierszy) ---")
    # Tworzymy b tak, by rozwiązaniem były same jedynki
    b = compute_b_ones(BM)
    
    # Robimy kopię b, bo eliminacja Gaussa go zmodyfikuje
    b_work = copy(b)

    println("--- KROK 4: Rozwiązywanie układu (n = $n) ---")
    
    # Pomiar czasu dla Gaussa
    println("Eliminacja Gaussa:")
    @time gaussianElimination(BM, b_work, false)

    # Pomiar czasu dla Back Substitution
    println("Podstawienie wsteczne:")
    @time x_calc = back_substitution(BM, b_work)

    println("--- KROK 5: Weryfikacja wyniku ---")
    x_true = ones(n)
    
    # Liczymy błąd względny: ||x_true - x_calc|| / ||x_true||
    rel_error = norm(x_true - x_calc) / norm(x_true)
    
    println("Błąd względny: $rel_error")
    
    if rel_error < 1e-10
        println("✅ TEST ZALICZONY: Wynik jest poprawny!")
    else
        println("❌ TEST NIEUDANY: Błąd jest zbyt duży. Sprawdź indeksowanie.")
    end

    println("Pierwsze 5 obliczonych x: ", x_calc[1:min(5, n)])
end

run_test()