using Plots
using Printf 

# Wczytanie funkcji z Zadania 4, która musi obsługiwać oba typy węzłów
# (Zakładamy, że zawiera definicję plotNewtonInterp/rysujNnfx i jej zależności)
include("./ex4.jl") 

# 1. DEFINICJE FUNKCJI DO TESTOWANIA
# (a) f(x) = |x|
f_a(x) = abs(x)
# (b) f(x) = 1 / (1 + x^2)
f_b(x) = 1.0 / (1.0 + x^2)

# 2. KONFIGURACJA TESTÓW
N_TESTS = [5, 10, 15]
OUTPUT_DIR = "graphs"

println("=== ROZPOCZĘCIE TESTÓW ZADANIA 6 (Rozbieżności) ===")

# Upewnienie się, że katalog docelowy istnieje.
mkpath(OUTPUT_DIR)
println("Katalog docelowy: $OUTPUT_DIR/")

# ====================================================================
# TEST (a): f(x) = |x| w [-1, 1]
# ====================================================================
println("\n--- TEST (a): f(x) = |x| w [-1, 1] (Zjawisko rozbieżności/wygładzania) ---")
a = -1.0
b = 1.0
f_tag = "fa_absX"

for n in N_TESTS
    # --- Węzły równoodległe ---
    nodes_type = :equidistant
    nodes_short = "eq"
    p_eq = plotNewtonInterp(f_a, a, b, n, nodes_type = nodes_type)
    filename_eq = @sprintf("%s/%s_n%02d_%s.png", OUTPUT_DIR, f_tag, n, nodes_short)
    Plots.savefig(p_eq, filename_eq)
    println("Zapisano: $filename_eq")
    
    # --- Węzły Czebyszewa ---
    nodes_type = :chebyshev
    nodes_short = "czeb"
    p_czeb = plotNewtonInterp(f_a, a, b, n, nodes_type = nodes_type)
    filename_czeb = @sprintf("%s/%s_n%02d_%s.png", OUTPUT_DIR, f_tag, n, nodes_short)
    Plots.savefig(p_czeb, filename_czeb)
    println("Zapisano: $filename_czeb")
end


# ====================================================================
# TEST (b): f(x) = 1 / (1 + x^2) w [-5, 5] (Funkcja Rungego)
# ====================================================================
println("\n--- TEST (b): f(x) = 1 / (1 + x^2) w [-5, 5] (Zjawisko Rungego) ---")
a = -5.0
b = 5.0
f_tag = "fb_runge"

for n in N_TESTS
    # --- Węzły równoodległe ---
    nodes_type = :equidistant
    nodes_short = "eq"
    p_eq = plotNewtonInterp(f_b, a, b, n, nodes_type = nodes_type)
    filename_eq = @sprintf("%s/%s_n%02d_%s.png", OUTPUT_DIR, f_tag, n, nodes_short)
    Plots.savefig(p_eq, filename_eq)
    println("Zapisano: $filename_eq")
    
    # --- Węzły Czebyszewa ---
    nodes_type = :chebyshev
    nodes_short = "czeb"
    p_czeb = plotNewtonInterp(f_b, a, b, n, nodes_type = nodes_type)
    filename_czeb = @sprintf("%s/%s_n%02d_%s.png", OUTPUT_DIR, f_tag, n, nodes_short)
    Plots.savefig(p_czeb, filename_czeb)
    println("Zapisano: $filename_czeb")
end

println("\n=== KONIEC TESTÓW ZADANIA 6 ===")
println("Wszystkie 12 wykresów zapisano w folderze $OUTPUT_DIR/.")