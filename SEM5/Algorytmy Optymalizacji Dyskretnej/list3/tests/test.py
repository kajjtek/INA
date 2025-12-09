import os
import subprocess
import glob
import random 
import csv
import time
from concurrent.futures import ThreadPoolExecutor, as_completed
from math import log2, ceil

# --- KONFIGURACJA (DOSTOSOWANA DO NOWEJ STRUKTURY) ---

# Ścieżki do Twoich skompilowanych programów w katalogu /bin
EXECUTABLES = {
    "dijkstra": "./bin/dijkstra",
    "dial": "./bin/dial",
    "radix": "./bin/radixheap"
}

# Folder wejściowy - używamy '**/', aby szukać rekurencyjnie w podkatalogach np. /inputs/nazwarodzaju/
INPUT_DIR = "./inputs"
# Folder na wyniki
OUTPUT_DIR = "./results"

# Wymagania testów SSSP
NUM_SOURCES = 5 # Liczba losowych źródeł

# Maksymalna liczba równoległych procesów 
MAX_WORKERS = 8 

# --- FUNKCJE POMOCNICZE ---

def ensure_dirs():
    if not os.path.exists(OUTPUT_DIR):
        os.makedirs(OUTPUT_DIR)

def get_graph_metadata(graph_file):
    """
    Zczytuje N (liczba wierzchołków) i maksymalną wagę (C) z pliku .gr
    """
    node_count = 0
    max_weight = 0
    try:
        with open(graph_file, 'r') as f:
            for line in f:
                if line.startswith('p sp'):
                    parts = line.split()
                    node_count = int(parts[2])
                elif line.startswith('a '):
                    # a <u_idx> <v_idx> <weight>
                    weight = int(line.split()[3])
                    if weight > max_weight:
                        max_weight = weight
    except Exception as e:
        print(f"Błąd przy parsowaniu metadanych z {graph_file}: {e}")
        return 0, 0
    return node_count, max_weight

def create_ss_file(filename, node_count, num_sources):
    """
    Generuje plik .ss z NUM_SOURCES wierzchołkami, losowanymi dla danego grafu.
    """
    if node_count < 1:
        return 0

    sources = set()
    # Dodajemy losowe, unikalne wierzchołki, aż osiągniemy NUM_SOURCES
    while len(sources) < num_sources and len(sources) < node_count:
        sources.add(random.randint(1, node_count)) 
        
    final_sources = sorted(list(sources))

    with open(filename, 'w') as f:
        f.write(f"p aux sp ss {len(final_sources)}\n")
        for s in final_sources:
            f.write(f"s {s}\n")
            
    return len(final_sources)

def create_p2p_file(filename, node_count):
    """
    Generuje plik .p2p z jednym zapytaniem: najmniejszy (1) do największego (N).
    """
    if node_count < 1:
        with open(filename, 'w') as f:
            f.write("p aux sp p2p 0\n")
        return 0

    source = 1
    target = node_count
    
    with open(filename, 'w') as f:
        f.write("p aux sp p2p 1\n")
        f.write(f"q {source} {target}\n")
    return 1


def run_single_test(algo_name, binary_path, graph_file, mode, config_file, res_file, max_cost):
    """
    Uruchamia pojedynczy test (binarkę C++).
    """
    cmd = [binary_path, "-d", graph_file]
    
    if algo_name in ["dial", "radix"]:
        cmd.extend(["-c", str(max_cost)]) 
    
    if mode == "SSSP":
        cmd.extend(["-ss", config_file, "-oss", res_file])
    elif mode == "P2P":
        cmd.extend(["-p2p", config_file, "-op2p", res_file])
    
    try:
        start_t = time.time()
        # Timeout 10 minut (600s)
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=600) 
        
        if result.returncode != 0:
            return (algo_name, os.path.basename(graph_file), mode, "ERROR", f"ExitCode:{result.returncode}, Stderr: {result.stderr.strip()}")

        parsed_time = "N/A"
        if os.path.exists(res_file):
            with open(res_file, 'r') as f:
                for line in f:
                    if line.startswith('t '):
                        parsed_time = float(line.split()[1])
                        break
        
        return (algo_name, os.path.basename(graph_file), mode, parsed_time, "OK")

    except subprocess.TimeoutExpired:
        return (algo_name, os.path.basename(graph_file), mode, "TIMEOUT", "Process timed out after 600s")
    except Exception as e:
        return (algo_name, os.path.basename(graph_file), mode, "EXC", f"Exception: {str(e)}")

# --- GŁÓWNA PĘTLA ---

def main():
    ensure_dirs()
    
    # Zmiana dla rekurencyjnego wyszukiwania
    # glob.glob szuka w ./inputs, a '/**/*.gr' rekurencyjnie znajduje pliki w podkatalogach
    graph_files = glob.glob(os.path.join(INPUT_DIR, '**', '*.gr'), recursive=True)
    if not graph_files:
        print(f"Nie znaleziono plików .gr w folderze {INPUT_DIR} lub jego podkatalogach!")
        return

    tasks = []
    
    print("Generowanie plików konfiguracyjnych (.ss, .p2p)...")
    graph_metadata = {}

    for graph_path in graph_files:
        # Używamy ścieżki względnej do nazwy pliku, aby uniknąć problemów z długimi nazwami
        base_name = os.path.relpath(graph_path, INPUT_DIR).replace(os.sep, '_').replace('.gr', '')
        node_count, max_cost = get_graph_metadata(graph_path)
        graph_metadata[base_name] = {'N': node_count, 'C': max_cost, 'original_path': graph_path}
        
        ss_file = os.path.join(OUTPUT_DIR, base_name + ".ss")
        p2p_file = os.path.join(OUTPUT_DIR, base_name + ".p2p")
        
        # Wygenerowanie unikalnych źródeł dla tego grafu
        create_ss_file(ss_file, node_count, NUM_SOURCES) 
        create_p2p_file(p2p_file, node_count)
        print(f"  Graf {graph_path}: Wierzchołków={node_count}, MaxCost={max_cost}. Pliki .ss/.p2p gotowe.")

    print(f"Rozpoczynam testy na {MAX_WORKERS} wątkach...")
    results_data = []

    with ThreadPoolExecutor(max_workers=MAX_WORKERS) as executor:
        for base_name, metadata in graph_metadata.items():
            graph_path = metadata['original_path']
            ss_file = os.path.join(OUTPUT_DIR, base_name + ".ss")
            p2p_file = os.path.join(OUTPUT_DIR, base_name + ".p2p")
            
            # Dla każdego algorytmu
            for algo, binary in EXECUTABLES.items():
                if not os.path.exists(binary):
                    print(f"Ostrzeżenie: Nie znaleziono pliku wykonywalnego {binary}, pomijam.")
                    continue
                
                # Zleć zadanie SSSP
                res_ss = os.path.join(OUTPUT_DIR, f"{base_name}_{algo}_ss.res")
                tasks.append(executor.submit(run_single_test, algo, binary, graph_path, "SSSP", ss_file, res_ss, metadata['C']))
                
                # Zleć zadanie P2P
                res_p2p = os.path.join(OUTPUT_DIR, f"{base_name}_{algo}_p2p.res")
                tasks.append(executor.submit(run_single_test, algo, binary, graph_path, "P2P", p2p_file, res_p2p, metadata['C']))

        # Zbieranie wyników w czasie rzeczywistym
        total_tasks = len(tasks)
        completed = 0
        
        for future in as_completed(tasks):
            res = future.result()
            results_data.append(res)
            completed += 1
            status_text = f"Czas: {res[3]}s" if isinstance(res[3], float) else f"STATUS: {res[3]} ({res[4]})"
            print(f"[{completed}/{total_tasks}] Zakończono: {res[0]} na {res[1]} ({res[2]}) -> {status_text}")

    # Zapisz wszystko do CSV
    csv_file = "wyniki_testow.csv"
    with open(csv_file, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(["Algorytm", "Graf", "Tryb", "Czas [s]", "Status/Błąd"])
        writer.writerows(results_data)
        
    print(f"\nTesty zakończone! Wyniki zapisano w {csv_file}")

if __name__ == "__main__":
    main()