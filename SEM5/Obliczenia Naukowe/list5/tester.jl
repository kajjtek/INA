#KAJETAN PLEWA
include("parser.jl")
include("structure.jl")

using LinearAlgebra
using Statistics

"""
Zapisuje wynik do pliku tekstowego zgodnie ze specyfikacją z PDF (strona 3).
"""
function save_results(filepath::String, x::Vector{Float64}, err::Float64, is_computed_b::Bool)
    open(filepath, "w") do f
        # Zapisujemy zawsze błąd jako pierwszą linię (dla obu scenariuszy)
        println(f, err)
        for val in x
            println(f, val)
        end
    end
end

"""
Mnoży BlockMatrix przez wektor x, zwraca wektor Ax.
"""
function multiply_blockmatrix(BM::BlockMatrix, x::Vector{Float64})
    n = BM.n
    l = BM.l
    v = BM.v
    y = zeros(Float64, n)

    for k in 1:v
        offset = (k - 1) * l
        A = BM.AList[k]
        # A block (l x l)
        for i in 1:l
            gi = offset + i
            s = 0.0
            for j in 1:l
                gj = offset + j
                if gj <= n
                    s += A.fields[i, j] * x[gj]
                end
            end
            y[gi] += s
        end
        # C block -> contributes to same offset rows (columns in next block)
        if k < v
            C = BM.CList[k]
            next_offset = k * l
            for i in 1:l
                gi = offset + i
                for j in 1:l
                    gj = next_offset + j
                    if gj <= n
                        y[gi] += C.fields[i, j] * x[gj]
                    end
                end
            end
        end
        # B block -> contributions from previous block's variables
        if k > 1
            B = BM.BList[k]
            prev_offset = (k - 2) * l
            for i in 1:l
                gi = offset + i
                for j in 1:l
                    gj = prev_offset + j
                    if gj >= 1 && gj <= n
                        y[gi] += B.fields[i, j] * x[gj]
                    end
                end
            end
        end
    end
    return y
end

"""
Główna funkcja testująca, wykonująca wszystkie wymagane warianty:
- Gauss (bez i z wyborem elementu głównego)
- LU + solver (bez i z wyborem elementu głównego)
Dla dwóch scenariuszy: b z pliku oraz b obliczane (Ax).
"""
function run_comprehensive_tests(name::String, stats_file::IOStream)
    mat_path = "testfiles/Dane$(name)/A.txt"
    vec_path = "testfiles/Dane$(name)/B.txt" 
    
    if !isfile(mat_path)
        println("Pominięto Dane$name - brak pliku macierzy.")
        return
    end

    println("\n========================================")
    println("PRZETWARZANIE: Dane$name")
    println("========================================")

    scenarios = [(:file, "z_pliku"), (:computed, "wyliczone")]

    for (scen_type, scen_suffix) in scenarios
        is_computed = scen_type == :computed
        
        if scen_type == :file
            if !isfile(vec_path)
                println("  [Info] Pominięto scenariusz 'z pliku' (brak $vec_path)")
                continue
            end
            b_base = read_vector(vec_path)
        else
            BM_for_b = read_matrix(mat_path)
            b_base = compute_b_ones(BM_for_b)
        end

        for pivot in [false, true]
            p_label = pivot ? "pivot" : "no_pivot"

            # --- WARMUP: wywołania poza pomiarem aby wymusić kompilację metod (elim./back_sub)
            try
                BM_w = read_matrix(mat_path)
                b_w = copy(b_base)
                p_w = collect(1:BM_w.n)
                gaussianElimination(BM_w, b_w, pivot, p_w)
                _ = back_substitution(BM_w, b_w)
                BM_w2 = read_matrix(mat_path)
                b_w2 = copy(b_base)
                p_w2 = collect(1:BM_w2.n)
                LU!(BM_w2, pivot, p_w2)
                _ = solve_LU(BM_w2, b_w2, p_w2)
            catch e
                # warmup may fail for some reason, but we continue to timed runs
            end
            
            # --- TEST 1: Eliminacja Gaussa ---
            BM_gauss = read_matrix(mat_path)
            b_gauss = copy(b_base)
            p_gauss = collect(1:BM_gauss.n)
            
            println("  -> Gauss ($p_label, $scen_suffix)...")
            # zachowaj oryginalną macierz i oryginalny b (przed modyfikacją) do obliczenia reszty
            BM_orig = read_matrix(mat_path)
            b_before = copy(b_gauss)
            t_gauss = @elapsed begin
                gaussianElimination(BM_gauss, b_gauss, pivot, p_gauss)
                x_gauss = back_substitution(BM_gauss, b_gauss)
            end
            if is_computed
                err_gauss = norm(ones(BM_gauss.n) - x_gauss) / norm(ones(BM_gauss.n))
            else
                # obliczamy resztę ||A*x - b|| / ||b|| używając oryginalnej macierzy i oryginalnego b
                Ax = multiply_blockmatrix(BM_orig, x_gauss)
                denom = norm(b_before)
                err_gauss = denom == 0.0 ? norm(Ax - b_before) : norm(Ax - b_before) / denom
            end
            out_gauss = "testfiles/Wynik$(name)_gauss_$(p_label)_$(scen_suffix).txt"
            save_results(out_gauss, x_gauss, err_gauss, is_computed)
            
            # Zapis czasu do CSV: plik, metoda, pivot, scenariusz, czas, błąd
            println(stats_file, "$(name),Gauss,$(pivot),$(scen_suffix),$(t_gauss),$(err_gauss)")

            # --- TEST 2: Rozkład LU + Rozwiązanie ---
            BM_lu = read_matrix(mat_path)
            b_lu = copy(b_base)
            p_lu = collect(1:BM_lu.n)
            
            println("  -> LU ($p_label, $scen_suffix)...")
            BM_orig_lu = read_matrix(mat_path)
            b_before_lu = copy(b_lu)
            t_lu = @elapsed begin
                LU!(BM_lu, pivot, p_lu)
                x_lu = solve_LU(BM_lu, b_lu, p_lu)
            end
            if is_computed
                err_lu = norm(ones(BM_lu.n) - x_lu) / norm(ones(BM_lu.n))
            else
                Ax_lu = multiply_blockmatrix(BM_orig_lu, x_lu)
                denom_lu = norm(b_before_lu)
                err_lu = denom_lu == 0.0 ? norm(Ax_lu - b_before_lu) : norm(Ax_lu - b_before_lu) / denom_lu
            end
            out_lu = "testfiles/Wynik$(name)_lu_$(p_label)_$(scen_suffix).txt"
            save_results(out_lu, x_lu, err_lu, is_computed)
            
            # Zapis czasu do CSV
            println(stats_file, "$(name),LU,$(pivot),$(scen_suffix),$(t_lu),$(err_lu)")
            
            if is_computed
                println("     Błąd względny (Gauss): $err_gauss (Czas: $(t_gauss)s)")
                println("     Błąd względny (LU):    $err_lu (Czas: $(t_lu)s)")
            end
        end
    end
end

# Lista plików do testów
test_files = [
    "16_1_1", 
    "10000_1_1", 
    "50000_1_1", 
    "100000_1_1", 
    "500000_1_1", 
    "750000_1_1", 
    "1000000_1_1"
]

# Otwarcie pliku zbiorczego na statystyki czasu
open("testfiles/czasy.csv", "w") do stats_file
    println(stats_file, "Plik,Metoda,Pivot,Scenariusz,Czas_s,Blad")
    for f in test_files
        try
            run_comprehensive_tests(f, stats_file)
        catch e
            println("Krytyczny błąd przy pliku $f: $e")
        end
    end
end

println("\nTesty zakończone. Wyniki i czasy zapisano w folderze testfiles/.")