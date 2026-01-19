import subprocess
import csv
import re
import platform

# Konfiguracja
BINARY_NAME = "./ex4"
if platform.system() == "Windows":
    BINARY_NAME = "ex4.exe"

OUTPUT_CSV = "wyniki_zad4_dinic.csv"
K_MIN = 1
K_MAX = 16 # Zmien na 16 dla pelnego testu
REPEATS = 5

def parse_output(stdout, stderr):
    # Uwaga: w kodzie Dynica stdout ma dopisek "(Dinic)"
    # cout << "Maksymalny przeplyw (Dinic): " << maxFlowValue << endl;
    flow_match = re.search(r"Maksymalny przeplyw(?: \(Dinic\))?:\s+(\d+)", stdout)
    
    time_match = re.search(r"Czas wykonania:\s+([\d\.]+)", stderr)
    
    # cerr << "Liczba sciezek powiekszajacych (DFS hits): " ...
    paths_match = re.search(r"Liczba sciezek powiekszajacych \(DFS hits\):\s+(\d+)", stderr)

    flow = int(flow_match.group(1)) if flow_match else None
    time_sec = float(time_match.group(1)) if time_match else None
    paths = int(paths_match.group(1)) if paths_match else None

    return flow, time_sec, paths

def run_experiment():
    print(f"Rozpoczynam eksperymenty dla Zadania 4 (Dinic). Wyniki w: {OUTPUT_CSV}")
    
    with open(OUTPUT_CSV, mode='w', newline='') as csv_file:
        writer = csv.writer(csv_file)
        writer.writerow(["k", "run_id", "max_flow", "time_seconds", "augmenting_paths_dfs_hits"])

        for k in range(K_MIN, K_MAX + 1):
            print(f"Testowanie k = {k}...")
            
            for i in range(REPEATS):
                try:
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

    print("Zakonczono eksperymenty zadania 4.")

if __name__ == "__main__":
    run_experiment()