#KAJETAN PLEWA
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

function run_lu_test()
    # 1. Parametry
    n = 16  # mała macierz, żeby łatwo było debugować
    l = 4
    ck = 1.0
    filename = "A_lu_test.txt"

    println("--- KROK 1: Generowanie i wczytywanie ---")
    matrixgen.blockmat(n, l, ck, filename)
    BM = read_matrix(filename)

    println("--- KROK 2: Generowanie wektora b (sumy wierszy) ---")
    # b tworzymy na podstawie ORYGINALNEJ macierzy, zanim ją zniszczymy faktoryzacją
    b_original = compute_b_ones(BM) 
    
    println("--- KROK 3: Faktoryzacja LU (In-place) ---")
    # Wywołujemy Twoją funkcję gaussianElimination, ale przerobioną na LU
    # Upewnij się, że w środku wywołuje eliminateLU!
    @time LU!(BM, false) 

    println("--- KROK 4: Forward Substitution (Ly = b) ---")
    @time y = forward_substitution(BM, b_original)

    println("--- KROK 5: Backward Substitution (Ux = y) ---")
    @time x_calc = back_substitution(BM, y)

    println("--- KROK 6: Weryfikacja ---")
    x_true = ones(n)
    rel_error = norm(x_true - x_calc) / norm(x_true)
    
    println("Błąd względny LU: $rel_error")
    
    if rel_error < 1e-12
        println("✅ TEST LU ZALICZONY!")
    else
        println("❌ TEST LU NIEUDANY. Sprawdź indeksy w forward_substitution lub pętlę w C.")
        # Opcjonalnie wyświetl wyniki, żeby zobaczyć gdzie puchnie błąd
        # println("Obliczone x: ", x_calc)
    end
end

run_test()
run_lu_test()