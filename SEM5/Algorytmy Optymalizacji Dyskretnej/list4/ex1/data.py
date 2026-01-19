import subprocess
import csv
import re
import platform

# Konfiguracja
BINARY_NAME = "./ex1" 
if platform.system() == "Windows":
    BINARY_NAME = "ex1.exe"

OUTPUT_CSV = "wyniki_zad1_edmonds_karp.csv"
K_MIN = 1
K_MAX = 16 # Zmien na 16 dla pełnego testu (może trwać długo)
REPEATS = 5  # Liczba powtórzeń dla uśrednienia wyników

def parse_output(stdout, stderr):
    """Wyciąga dane z wyjścia programu C++."""
    # Szukamy wzorców zdefiniowanych w kodzie C++
    flow_match = re.search(r"Maksymalny przeplyw:\s+(\d+)", stdout)
    time_match = re.search(r"Czas wykonania:\s+([\d\.]+)", stderr)
    paths_match = re.search(r"Liczba sciezek powiekszajacych:\s+(\d+)", stderr)

    flow = int(flow_match.group(1)) if flow_match else None
    time_sec = float(time_match.group(1)) if time_match else None
    paths = int(paths_match.group(1)) if paths_match else None

    return flow, time_sec, paths

def run_experiment():
    print(f"Rozpoczynam eksperymenty dla Zadania 1. Wyniki w: {OUTPUT_CSV}")
    
    with open(OUTPUT_CSV, mode='w', newline='') as csv_file:
        writer = csv.writer(csv_file)
        # Nagłówki kolumn CSV
        writer.writerow(["k", "run_id", "max_flow", "time_seconds", "augmenting_paths"])

        for k in range(K_MIN, K_MAX + 1):
            print(f"Testowanie k = {k}...")
            
            for i in range(REPEATS):
                try:
                    # Uruchomienie procesu C++
                    result = subprocess.run(
                        [BINARY_NAME, "--size", str(k)],
                        capture_output=True,
                        text=True,
                        check=True
                    )
                    
                    flow, time_sec, paths = parse_output(result.stdout, result.stderr)
                    
                    if flow is not None:
                        writer.writerow([k, i + 1, flow, time_sec, paths])
                    else:
                        print(f"Blad parsowania dla k={k}, run={i}")

                except subprocess.CalledProcessError as e:
                    print(f"Blad uruchomienia dla k={k}: {e}")
                except FileNotFoundError:
                    print(f"Nie znaleziono pliku {BINARY_NAME}. Skompiluj kod C++!")
                    return

    print("Zakonczono eksperymenty zadania 1.")

if __name__ == "__main__":
    run_experiment()