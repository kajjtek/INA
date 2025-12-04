using Plots
using Printf # Potrzebne do formatowania nazwy pliku
# Używamy Base.Filesystem.mkpath, ale po prostu mkpath() wystarczy w nowszych wersjach
# Julia, jeśli nie ma konfliktu nazw.

# Upewnij się, że plik ex4.jl zawiera plotNewtonInterp i wszystkie zależności
include("./ex4.jl") 

# 1. Definicja funkcji
f_a(x) = exp(x)
f_b(x) = x^2 * sin(x)

# 2. Konfiguracja testów
N_TESTS = [5, 10, 15]
OUTPUT_DIR = "graphs"

println("=== ROZPOCZĘCIE TESTÓW ZADANIA 5 (Zapis do plików) ===")

# --- UTWORZENIE KATALOGU DOCELOWEGO ---
# mkpath jest bezpieczne - tworzy katalog, jeśli nie istnieje, i nie zgłasza błędu, 
# jeśli już jest.
mkpath(OUTPUT_DIR)
println("Katalog docelowy został utworzony/sprawdzony: $OUTPUT_DIR/")

# --- TEST (a): f(x) = e^x w [0, 1] (równoodległe) ---
println("\n--- TEST (a): f(x) = e^x w [0, 1] (równoodległe) ---")
a_a = 0.0
b_a = 1.0
f_tag_a = "fa_exp" # Krótka etykieta dla funkcji

for n in N_TESTS
    nodes_type = :equidistant
    nodes_short = "eq"
    
    println("Generowanie wykresu dla n = $n...")
    
    # 1. Wygenerowanie wykresu (obiekty P zostają w pamięci, ale nie są wyświetlane)
    p = plotNewtonInterp(f_a, a_a, b_a, n, nodes_type = nodes_type)
    
    # 2. Konstrukcja nazwy pliku: np. graphs/fa_exp_n05_eq.png
    # %02d zapewnia, że n jest formatowane jako 2 cyfry (np. 05)
    filename = @sprintf("%s/%s_n%02d_%s.png", OUTPUT_DIR, f_tag_a, n, nodes_short)
    
    # 3. Zapisanie do pliku
    Plots.savefig(p, filename)
    println("Zapisano: $filename")
end

# --- TEST (b): f(x) = x^2 * sin(x) w [-1, 1] (równoodległe) ---
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