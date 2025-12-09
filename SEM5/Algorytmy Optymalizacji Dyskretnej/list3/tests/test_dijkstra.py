import os
import subprocess
import glob
import csv
import time
import re
from concurrent.futures import ThreadPoolExecutor, as_completed
import argparse
import sys
import pandas as pd

# --- KONFIGURACJA ALGORYTMU ---
ALGORITHM_NAME = "dijkstra"
BINARY_PATH = "../bin/dijkstra"

# --- KONFIGURACJA OGÓLNA ---
INPUT_DIR = "../inputs"
OUTPUT_DIR = "../results"
NUM_SOURCES_SS5 = 5 
NUM_P2P_PAIRS = 5
TIMEOUT_SECONDS = 3600 # 1 godzina na test
MAX_WORKERS = 8 # Maksymalna liczba równoległych procesów 

# Pliki wynikowe CSV (unikalne dla Dijkstry)
CSV_SS1_FILE = os.path.join(OUTPUT_DIR, f"wyniki_{ALGORITHM_NAME}_czasow_sssp1.csv")
CSV_SS5_FILE = os.path.join(OUTPUT_DIR, f"wyniki_{ALGORITHM_NAME}_czasow_sssp5.csv")
CSV_P2P_FILE = os.path.join(OUTPUT_DIR, f"wyniki_{ALGORITHM_NAME}_dlugosci_sciezek_p2p.csv")

# --- FUNKCJE POMOCNICZE (identyczne jak w oryginalnym test.py) ---

def ensure_dirs():
    """Tworzy katalog wynikowy, jeśli nie istnieje."""
    if not os.path.exists(OUTPUT_DIR):
        os.makedirs(OUTPUT_DIR)

def get_graph_metadata(graph_file):
    """Zczytuje N (liczba wierzchołków), M (liczba krawędzi) i max wagę (C) z pliku .gr"""
    node_count = 0
    max_weight = 0
    edge_count = 0
    try:
        with open(graph_file, 'r') as f:
            for line in f:
                if line.startswith('p sp'):
                    parts = line.split()
                    if len(parts) >= 4:
                        node_count = int(parts[2])
                        edge_count = int(parts[3])
                elif line.startswith('a '):
                    parts = line.split()
                    if len(parts) >= 4:
                        weight = int(parts[3])
                        if weight > max_weight:
                            max_weight = weight
    except Exception:
        return {'N': 0, 'M': 0, 'C': 0}
    
    # Dodanie metadata dla łatwiejszego filtrowania
    base_name = os.path.basename(graph_file).replace('.gr', '')
    family = base_name.split('_')[0]
    
    return {
        'N': node_count, 
        'M': edge_count, 
        'C': max_weight, 
        'name': base_name,
        'family': family,
        'path': graph_file
    }

def run_single_test(alg_name, binary, graph_path, test_type, query_file, res_file, metadata):
    """Uruchamia pojedynczy test i zwraca czas (SSSP) LUB długość ścieżki (P2P)."""
    
    N, C = metadata['N'], metadata['C']
    
    # Ustalanie parametrów
    cmd = [binary, "-d", graph_path]
    # Dijkstra nie wymaga parametru C, więc pomijamy jego dodanie
    # if alg_name in ["dial", "radix"]:
    #     cmd.extend(["-c", str(C)]) 

    if test_type.startswith("SSSP"):
        cmd.extend(["-ss", query_file, "-oss", res_file])
    elif test_type == "P2P":
        cmd.extend(["-p2p", query_file, "-op2p", res_file])
    else:
        return alg_name, metadata['name'], test_type, "ERROR", "Nieznany typ testu", 0
    
    start_time = time.monotonic()
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=TIMEOUT_SECONDS)
        elapsed_time = time.monotonic() - start_time
        
        if result.returncode != 0:
            return alg_name, metadata['name'], test_type, "FAILED", f"Kod wyjścia {result.returncode}. STDERR: {result.stderr.strip()}", elapsed_time

    except subprocess.TimeoutExpired:
        return alg_name, metadata['name'], test_type, "TIMEOUT", f"Przekroczono czas {TIMEOUT_SECONDS}s", TIMEOUT_SECONDS
    except FileNotFoundError:
        return alg_name, metadata['name'], test_type, "ERROR", "Nie znaleziono pliku binarnego", 0
    except Exception as e:
        return alg_name, metadata['name'], test_type, "ERROR", f"Nieoczekiwany błąd: {e}", elapsed_time

    # --- Parsowanie wyniku ---
    try:
        with open(res_file, 'r') as f:
            content = f.read()
        
        # 1. Parsowanie CZASU (dla SSSP-1 i SSSP-5)
        if test_type.startswith("SSSP"):
            time_match = re.search(r'^t\s+(\d+\.\d+)', content, re.MULTILINE)
            if time_match:
                total_time = float(time_match.group(1))
                
                if test_type == "SSSP-5":
                    # ŚREDNIA CZASÓW
                    result_value = total_time / NUM_SOURCES_SS5
                else:
                    result_value = total_time
                
                # Zwraca: alg_name, graph_name, test_type, status, czas/avg_czas, N_count
                return alg_name, metadata['name'], test_type, "OK", result_value, metadata['N']
        
        # 2. Parsowanie DŁUGOŚCI ŚCIEŻEK (dla P2P)
        elif test_type == "P2P":
            # Zbieramy wszystkie długości 'd'
            distance_matches = re.findall(r'^d\s+(\d+)', content, re.MULTILINE)
            
            if len(distance_matches) >= NUM_P2P_PAIRS: 
                distances = [int(d) for d in distance_matches[:NUM_P2P_PAIRS]]
                # Zwraca: alg_name, graph_name, test_type, status, lista_dlugosci, N_count
                return alg_name, metadata['name'], test_type, "OK", distances, metadata['N']
            
            return alg_name, metadata['name'], test_type, "FAILED", f"Znaleziono {len(distance_matches)} długości, oczekiwano {NUM_P2P_PAIRS}.", elapsed_time
            
        return alg_name, metadata['name'], test_type, "FAILED", "Nie znaleziono czasu lub długości w pliku wynikowym", elapsed_time

    except Exception as e:
        return alg_name, metadata['name'], test_type, "ERROR", f"Błąd parsowania: {e}", elapsed_time


def find_largest_graphs(graph_metadata):
    """Znajduje największą instancję (max N, max M/C) w każdej rodzinie grafów."""
    
    families = {}
    for meta in graph_metadata.values():
        family = meta['family']
        if family not in families:
            families[family] = []
        families[family].append(meta)

    largest_graphs = {}
    
    for family, graphs in families.items():
        # Sortujemy głównie po N, a następnie po M lub C
        graphs.sort(key=lambda x: (x['N'], x['M'] if 'C' not in x['name'] else x['C']), reverse=True)
        
        largest_graphs[family] = graphs[0]['name']

    return largest_graphs

def main():
    global INPUT_DIR, OUTPUT_DIR, MAX_WORKERS
    
    parser = argparse.ArgumentParser(description=f'Skrypt do testowania algorytmu {ALGORITHM_NAME} dla SSSP/P2P.')
    parser.add_argument('--input', '-i', default=INPUT_DIR, help='Katalog wejściowy (domyślnie: ../inputs)')
    parser.add_argument('--output', '-o', default=OUTPUT_DIR, help='Katalog wyjściowy (domyślnie: ../results)')
    parser.add_argument('--workers', '-w', type=int, default=MAX_WORKERS, help='Maksymalna liczba równoległych procesów (domyślnie: 8)')
    
    args = parser.parse_args()
    
    INPUT_DIR = args.input
    OUTPUT_DIR = args.output
    MAX_WORKERS = args.workers
    
    ensure_dirs()
    
    # 1. Zbieranie metadanych o wszystkich grafach
    all_graph_files = glob.glob(os.path.join(INPUT_DIR, '**', '*.gr'), recursive=True)
    graph_metadata = {}
    
    print(f"Zbieranie metadanych dla {len(all_graph_files)} grafów...")
    for graph_path in all_graph_files:
        meta = get_graph_metadata(graph_path)
        if meta['N'] > 0:
            graph_metadata[meta['name']] = meta

    # 2. Identyfikacja największych instancji dla P2P
    largest_graphs_names = find_largest_graphs(graph_metadata)
    print(f"Identyfikacja największych instancji dla testów P2P: {list(largest_graphs_names.values())}")
    
    tasks = []
    ss1_results_data = [] 
    ss5_results_data = []  
    p2p_results_data = [] 
    
    try:
        with ThreadPoolExecutor(max_workers=MAX_WORKERS) as ex:
            
            alg_name, binary = ALGORITHM_NAME, BINARY_PATH
            
            for base_name, meta in graph_metadata.items():
                
                # --- Weryfikacja plików zapytań ---
                ss1_file = os.path.join(OUTPUT_DIR, base_name + ".ss1")
                ss5_file = os.path.join(OUTPUT_DIR, base_name + ".ss5")
                p2p5_file = os.path.join(OUTPUT_DIR, base_name + ".p2p5")

                if not os.path.exists(ss1_file) or not os.path.exists(ss5_file):
                    print(f"OSTRZEŻENIE: Pomijam SSSP dla {base_name} - brakuje plików .ss1/.ss5.")
                    continue
                
                # --- A. Testy SSSP (SSSP-1 i SSSP-5) ---
                
                # 1. SSSP-1 (jedno źródło)
                res_file_1 = os.path.join(OUTPUT_DIR, f"{base_name}_{alg_name}_ss1.res")
                tasks.append(ex.submit(run_single_test, alg_name, binary, meta['path'], 
                                       "SSSP-1", ss1_file, res_file_1, meta))

                # 2. SSSP-5 (pięć źródeł, średni czas)
                res_file_5 = os.path.join(OUTPUT_DIR, f"{base_name}_{alg_name}_ss5.res")
                tasks.append(ex.submit(run_single_test, alg_name, binary, meta['path'], 
                                       "SSSP-5", ss5_file, res_file_5, meta))
                                       
                # --- B. Testy P2P (Tylko największe instancje) ---
                if base_name in largest_graphs_names.values():
                    if not os.path.exists(p2p5_file):
                        print(f"OSTRZEŻENIE: Pomijam P2P dla największego {base_name} - brakuje pliku .p2p5.")
                        continue
                        
                    res_file_p2p = os.path.join(OUTPUT_DIR, f"{base_name}_{alg_name}_p2p5.res")
                    tasks.append(ex.submit(run_single_test, alg_name, binary, meta['path'], 
                                           "P2P", p2p5_file, res_file_p2p, meta))


            # Zbieranie wyników
            total_tasks = len(tasks)
            completed = 0

            for future in as_completed(tasks):
                try:
                    alg_name, graph_name, test_type, status, result, N_count = future.result()
                except Exception as e:
                    alg_name, graph_name, test_type, status, result, N_count = (ALGORITHM_NAME, "UNKNOWN", "UNKNOWN", "EXC", f"Task raised exception: {e}", 0)
                
                completed += 1
                
                # Oddzielne przetwarzanie wyników
                if status == "OK":
                    if test_type == "SSSP-1":
                        ss1_results_data.append([alg_name, graph_name, N_count, graph_metadata[graph_name]['C'], result, status])
                        status_text = f"CZAS: {result:.4f}s"
                    elif test_type == "SSSP-5":
                        ss5_results_data.append([alg_name, graph_name, N_count, graph_metadata[graph_name]['C'], result, status])
                        status_text = f"ŚR. CZAS: {result:.4f}s"
                    elif test_type == "P2P":
                        p2p_results_data.append({'alg': alg_name, 'graph': graph_name, 'distances': result})
                        status_text = f"DŁUGOŚCI: {result}"
                else:
                    if test_type == "SSSP-1":
                        ss1_results_data.append([alg_name, graph_name, N_count, graph_metadata[graph_name]['C'], result, status])
                    elif test_type == "SSSP-5":
                        ss5_results_data.append([alg_name, graph_name, N_count, graph_metadata[graph_name]['C'], result, status])
                    # P2P błędy są ignorowane, ponieważ brakuje im pełnego zestawu danych do tabeli
                    status_text = f"STATUS: {status} ({result})"
                    
                print(f"[{completed}/{total_tasks}] Zakończono: {alg_name} na {graph_name} ({test_type}) -> {status_text}")
    
    except KeyboardInterrupt:
        print("\nTest runner przerwany przez użytkownika. Zapisuję zebrane wyniki...")
    except Exception as e:
        print(f"\nNieoczekiwany błąd w test runnerze: {e}")
        import traceback
        traceback.print_exc()
        
    # --- Zapisywanie wyników SSSP-1 (Czas) do CSV ---
    print(f"\nZapisywanie wyników SSSP-1 do: {CSV_SS1_FILE}")
    with open(CSV_SS1_FILE, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(["Algorytm", "Graf", "N_Wierzchołków", "C_Max_Waga", "Czas_s", "Status"])
        writer.writerows(ss1_results_data)

    # --- Zapisywanie wyników SSSP-5 (Średni Czas) do CSV ---
    print(f"Zapisywanie wyników SSSP-5 do: {CSV_SS5_FILE}")
    with open(CSV_SS5_FILE, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(["Algorytm", "Graf", "N_Wierzchołków", "C_Max_Waga", "Sredni_Czas_s", "Status"])
        writer.writerows(ss5_results_data)

    # --- Zapisywanie wyników P2P (Długości ścieżek) do CSV (Format tabelaryczny) ---
    if p2p_results_data:
        print(f"Zapisywanie wyników długości ścieżek P2P do: {CSV_P2P_FILE}")
        
        p2p_header = ["Algorytm", "Graf"]
        p2p_header.extend([f"Para_{i+1}" for i in range(NUM_P2P_PAIRS)])
        
        p2p_output = []
        for res in p2p_results_data:
            row = [res['alg'], res['graph']]
            row.extend(res['distances'])
            p2p_output.append(row)

        with open(CSV_P2P_FILE, 'w', newline='') as f:
            writer = csv.writer(f)
            writer.writerow(p2p_header)
            writer.writerows(p2p_output)


if __name__ == '__main__':
    main()