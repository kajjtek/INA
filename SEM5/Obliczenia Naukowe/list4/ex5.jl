#KAJETAN PLEWA
using Plots
using Printf

include("./ex4.jl") 

f_a(x) = exp(x)
f_b(x) = x^2 * sin(x)

N_TESTS = [5, 10, 15]
OUTPUT_DIR = "graphs"

println("=== ROZPOCZĘCIE TESTÓW ZADANIA 5 (Zapis do plików) ===")

mkpath(OUTPUT_DIR)
println("Katalog docelowy został utworzony/sprawdzony: $OUTPUT_DIR/")

println("\n--- TEST (a): f(x) = e^x w [0, 1] (równoodległe) ---")
a_a = 0.0
b_a = 1.0
f_tag_a = "fa_exp"

for n in N_TESTS
    nodes_type = :equidistant
    nodes_short = "eq"
    
    println("Generowanie wykresu dla n = $n...")
    
    p = plotNewtonInterp(f_a, a_a, b_a, n, nodes_type = nodes_type)
    
    filename = @sprintf("%s/%s_n%02d_%s.png", OUTPUT_DIR, f_tag_a, n, nodes_short)
    
    Plots.savefig(p, filename)
    println("Zapisano: $filename")
end

println("\n--- TEST (b): f(x) = x^2 * sin(x) w [-1, 1] (równoodległe) ---")
a_b = -1.0
b_b = 1.0
f_tag_b = "fb_x2sinx"

for n in N_TESTS
    nodes_type = :equidistant
    nodes_short = "eq"
    
    println("Generowanie wykresu dla n = $n...")
    
    # 1. Wygenerowanie wykresu
    p = plotNewtonInterp(f_b, a_b, b_b, n, nodes_type = nodes_type)
    
    # 2. Konstrukcja nazwy pliku
    filename = @sprintf("%s/%s_n%02d_%s.png", OUTPUT_DIR, f_tag_b, n, nodes_short)
    
    # 3. Zapisanie do pliku
    Plots.savefig(p, filename)
    println("Zapisano: $filename")
end

println("\n=== KONIEC TESTÓW ZADANIA 5 ===")
println("Wszystkie 6 wykresów zapisano w folderze $OUTPUT_DIR/.")