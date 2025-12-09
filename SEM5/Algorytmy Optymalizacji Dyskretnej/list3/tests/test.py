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
# !!! RADIX HEAP WYKOMENTOWANY ZGODNIE Z PROŚBĄ !!!
EXECUTABLES = {
    "dijkstra": "../bin/dijkstra",
    "dial": "../bin/dial",
    # "radix": "./bin/radixheap" 
}

# Folder wejściowy - szukamy rekurencyjnie w podkatalogach
INPUT_DIR = "../inputs"
# Folder na wyniki
OUTPUT_DIR = "../results"

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
    
    # Przekazujemy -c tylko dla Diala (Radix Heap jest wyłączony, ale w razie potrzeby dodać: or algo_name == "radix")
    
    if mode == "SSSP":
        cmd.extend(["-ss", config_file, "-oss", res_file])
    elif mode == "P2P":
        cmd.extend(["-p2p", config_file, "-op2p", res_file])
    
    try:
        start_t = time.time()
        # Uruchomienie procesu (Timeout 10 minut (600s))
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
    # Ustawienie seed'a, aby losowania źródeł były powtarzalne dla różnych uruchomień testów
    # Jeśli chcesz za każdym razem inne źródła, usuń tę linię
    
    ensure_dirs()
    
    # Rekurencyjne wyszukiwanie plików .gr
    graph_files = glob.glob(os.path.join(INPUT_DIR, '**', '*.gr'), recursive=True)
    if not graph_files:
        print(f"Nie znaleziono plików .gr w folderze {INPUT_DIR} lub jego podkatalogach!")
        return

    tasks = []
    
    print("Uwaga: test.py już nie generuje plików .ss/.p2p. Uruchom tests/createssp2p.py aby je wygenerować, jeśli potrzeba.")
    print(f"Rozpoczynam testy na {MAX_WORKERS} wątkach dla {len(EXECUTABLES)} algorytmów...")
    results_data = []

    # Zbuduj listę grafów do testów tylko wtedy, gdy istnieją odpowiadające pliki .ss i .p2p
    graph_metadata = {}
    for graph_path in graph_files:
        base_name = os.path.relpath(graph_path, INPUT_DIR).replace(os.sep, '_').replace('.gr', '')
        ss_file = os.path.join(OUTPUT_DIR, base_name + ".ss")
        p2p_file = os.path.join(OUTPUT_DIR, base_name + ".p2p")
        if not os.path.exists(ss_file) or not os.path.exists(p2p_file):
            print(f"  Pomijam graf {graph_path}: brak {'.ss' if not os.path.exists(ss_file) else ''}{' i ' if (not os.path.exists(ss_file) and not os.path.exists(p2p_file)) else ''}{'.p2p' if not os.path.exists(p2p_file) else ''}")
            continue
        node_count, max_cost = get_graph_metadata(graph_path)
        graph_metadata[base_name] = {'N': node_count, 'C': max_cost, 'original_path': graph_path}

    try:
        with ThreadPoolExecutor(max_workers=MAX_WORKERS) as executor:
            for base_name, metadata in graph_metadata.items():
                graph_path = metadata['original_path']
                ss_file = os.path.join(OUTPUT_DIR, base_name + ".ss")
                p2p_file = os.path.join(OUTPUT_DIR, base_name + ".p2p")

                # Dla każdego z WŁĄCZONYCH algorytmów
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
                try:
                    res = future.result()
                except Exception as e:
                    # Log exception and continue collecting other tasks
                    res = ("UNKNOWN", "", "", "EXC", f"Task raised exception: {e}")
                results_data.append(res)
                completed += 1
                status_text = f"Czas: {res[3]}s" if isinstance(res[3], float) else f"STATUS: {res[3]} ({res[4]})"
                print(f"[{completed}/{total_tasks}] Zakończono: {res[0]} na {res[1]} ({res[2]}) -> {status_text}")
    except KeyboardInterrupt:
        print("\nTest runner interrupted by user (KeyboardInterrupt). Attempting to flush results...")
    except Exception as e:
        print(f"\nUnexpected error in test runner: {e}")
        import traceback
        traceback.print_exc()
    
    # Zapisz wszystko do CSV
    csv_file = os.path.join(OUTPUT_DIR, "wyniki_testow_Dijkstra_Dial.csv")
    with open(csv_file, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(["Algorytm", "Graf", "Tryb", "Czas [s]", "Status/Błąd"])
        writer.writerows(results_data)
        
    print(f"\nTesty zakończone! Wyniki zapisano w {csv_file}")

if __name__ == "__main__":
    main()