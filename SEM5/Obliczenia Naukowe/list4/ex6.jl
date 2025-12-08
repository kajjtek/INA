#KAJETAN PLEWA
using Plots
using Printf 

include("./ex4.jl") 

f_a(x) = abs(x)
f_b(x) = 1.0 / (1.0 + x^2)

N_TESTS = [5, 10, 15]
OUTPUT_DIR = "graphs"

println("=== ROZPOCZĘCIE TESTÓW ZADANIA 6 (Rozbieżności) ===")

mkpath(OUTPUT_DIR)
println("Katalog docelowy: $OUTPUT_DIR/")

println("\n--- TEST (a): f(x) = |x| w [-1, 1] (Zjawisko rozbieżności/wygładzania) ---")
a = -1.0
b = 1.0
f_tag = "fa_absX"

for n in N_TESTS
    nodes_type = :equidistant
    nodes_short = "eq"
    p_eq = plotNewtonInterp(f_a, a, b, n, nodes_type = nodes_type)
    filename_eq = @sprintf("%s/%s_n%02d_%s.png", OUTPUT_DIR, f_tag, n, nodes_short)
    Plots.savefig(p_eq, filename_eq)
    println("Zapisano: $filename_eq")
    
    nodes_type = :chebyshev
    nodes_short = "czeb"
    p_czeb = plotNewtonInterp(f_a, a, b, n, nodes_type = nodes_type)
    filename_czeb = @sprintf("%s/%s_n%02d_%s.png", OUTPUT_DIR, f_tag, n, nodes_short)
    Plots.savefig(p_czeb, filename_czeb)
    println("Zapisano: $filename_czeb")
end

println("\n--- TEST (b): f(x) = 1 / (1 + x^2) w [-5, 5] (Zjawisko Rungego) ---")
a = -5.0
b = 5.0
f_tag = "fb_runge"

for n in N_TESTS
    nodes_type = :equidistant
    nodes_short = "eq"
    p_eq = plotNewtonInterp(f_b, a, b, n, nodes_type = nodes_type)
    filename_eq = @sprintf("%s/%s_n%02d_%s.png", OUTPUT_DIR, f_tag, n, nodes_short)
    Plots.savefig(p_eq, filename_eq)
    println("Zapisano: $filename_eq")
    
    nodes_type = :chebyshev
    nodes_short = "czeb"
    p_czeb = plotNewtonInterp(f_b, a, b, n, nodes_type = nodes_type)
    filename_czeb = @sprintf("%s/%s_n%02d_%s.png", OUTPUT_DIR, f_tag, n, nodes_short)
    Plots.savefig(p_czeb, filename_czeb)
    println("Zapisano: $filename_czeb")
end

println("\n=== KONIEC TESTÓW ZADANIA 6 ===")
println("Wszystkie 12 wykresów zapisano w folderze $OUTPUT_DIR/.")