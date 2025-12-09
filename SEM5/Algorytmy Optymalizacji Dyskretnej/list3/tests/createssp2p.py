import os
import glob
import random
import argparse

# --- KONFIGURACJA ---
INPUT_DIR = os.path.join('..', 'inputs')
OUTPUT_DIR = os.path.join('..', 'results')
NUM_SOURCES_SS1 = 1  # Liczba źródeł dla SSSP-1
NUM_SOURCES_SS5 = 5  # Liczba źródeł dla SSSP-5
NUM_P2P_PAIRS = 5    # Liczba par wierzchołków dla P2P

def ensure_dirs():
    """Tworzy katalog wynikowy, jeśli nie istnieje."""
    if not os.path.exists(OUTPUT_DIR):
        os.makedirs(OUTPUT_DIR)

def get_graph_metadata(graph_file):
    """Zczytuje N (liczba wierzchołków) i max wagę (C) z pliku .gr"""
    node_count = 0
    max_weight = 0
    try:
        with open(graph_file, 'r') as f:
            for line in f:
                if line.startswith('p sp'):
                    parts = line.split()
                    if len(parts) >= 4:
                        node_count = int(parts[2])
                elif line.startswith('a '):
                    weight = int(line.split()[3])
                    if weight > max_weight:
                        max_weight = weight
    except Exception as e:
        print(f"Błąd przy parsowaniu metadanych z {graph_file}: {e}")
        return 0, 0
    return node_count, max_weight

def create_ss_file(filename, node_count, num_sources):
    """Generuje plik z losowymi wierzchołkami źródłowymi dla SSSP."""
    if node_count < 1:
        return 0
    
    # Wierzchołki są numerowane od 1 do N
    all_nodes = list(range(1, node_count + 1))
    
    # Wybieramy unikalne, losowe źródła
    k = min(num_sources, node_count)
    sources = random.sample(all_nodes, k=k)
    
    try:
        with open(filename, 'w') as f:
            for source in sources:
                f.write(f"s {source}\n") # format: s [wierzchołek_źródłowy]
        return k
    except Exception as e:
        print(f"Błąd zapisu do pliku {filename}: {e}")
        return 0

def create_p2p_file(filename, node_count, num_pairs):
    """Generuje plik z losowymi parami (start, cel) dla P2P."""
    if node_count < 2:
        return 0
    
    all_nodes = list(range(1, node_count + 1))
    
    pairs_created = 0
    generated_pairs = set()
    
    try:
        with open(filename, 'w') as f:
            while pairs_created < num_pairs:
                # Losujemy unikalne źródło i cel
                source, target = random.sample(all_nodes, k=2)
                pair = (source, target)
                
                # Upewniamy się, że para jest unikalna (w obie strony)
                if pair not in generated_pairs and (target, source) not in generated_pairs:
                    f.write(f"q {source} {target}\n") # format: q [źródło] [cel]
                    generated_pairs.add(pair)
                    pairs_created += 1
        return pairs_created
    except Exception as e:
        print(f"Błąd zapisu do pliku {filename}: {e}")
        return 0

def main():
    global INPUT_DIR, OUTPUT_DIR
    parser = argparse.ArgumentParser(description='Generuje pliki zapytań (.ss1, .ss5, .p2p5) dla grafów .gr.')
    parser.add_argument('--input', '-i', default=INPUT_DIR, help='Katalog wejściowy z plikami .gr (domyślnie: ../inputs)')
    parser.add_argument('--output', '-o', default=OUTPUT_DIR, help='Katalog wyjściowy dla plików zapytań (domyślnie: ../results)')
    parser.add_argument('--force', '-f', action='store_true', help='Wymusza nadpisanie istniejących plików zapytań.')
    
    args = parser.parse_args()
    
    
    INPUT_DIR = args.input
    OUTPUT_DIR = args.output
    force = args.force
    
    ensure_dirs()
    
    # Znajdujemy wszystkie pliki .gr rekurencyjnie
    all_graph_files = glob.glob(os.path.join(INPUT_DIR, '**', '*.gr'), recursive=True)
    
    if not all_graph_files:
        print(f"Nie znaleziono plików .gr w katalogu wejściowym: {INPUT_DIR}")
        return

    print(f"Znaleziono {len(all_graph_files)} grafów. Generowanie plików zapytań...")

    for graph_path in all_graph_files:
        # Prawidłowe utworzenie nazwy bazowej: tylko nazwa pliku bez rozszerzenia
        base_name = os.path.basename(graph_path).replace('.gr', '')
        node_count, _ = get_graph_metadata(graph_path)
        
        if node_count == 0:
            print(f"OSTRZEŻENIE: Pomijam {base_name} - nie udało się odczytać liczby wierzchołków.")
            continue
            
        # --- SSSP-1 (jedno źródło) ---
        ss1_file = os.path.join(OUTPUT_DIR, base_name + ".ss1")
        if force or not os.path.exists(ss1_file):
            created = create_ss_file(ss1_file, node_count, NUM_SOURCES_SS1)
            print(f"  {base_name}: Utworzono .ss1 ({created} źr.).")
        # else:
            # print(f"  {base_name}: Plik .ss1 już istnieje, pomijam.")

        # --- SSSP-5 (pięć źródeł) ---
        ss5_file = os.path.join(OUTPUT_DIR, base_name + ".ss5")
        if force or not os.path.exists(ss5_file):
            created = create_ss_file(ss5_file, node_count, NUM_SOURCES_SS5)
            print(f"  {base_name}: Utworzono .ss5 ({created} źr.).")
        # else:
            # print(f"  {base_name}: Plik .ss5 już istnieje, pomijam.")
            
        # --- P2P-5 (pięć par) ---
        p2p5_file = os.path.join(OUTPUT_DIR, base_name + ".p2p5")
        if force or not os.path.exists(p2p5_file):
            created = create_p2p_file(p2p5_file, node_count, NUM_P2P_PAIRS)
            print(f"  {base_name}: Utworzono .p2p5 ({created} par).")
        # else:
            # print(f"  {base_name}: Plik .p2p5 już istnieje, pomijam.")

if __name__ == '__main__':
    main()