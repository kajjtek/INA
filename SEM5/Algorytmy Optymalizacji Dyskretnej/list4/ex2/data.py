import subprocess
import csv
import re
import platform

# Konfiguracja
BINARY_NAME = "./ex2"
if platform.system() == "Windows":
    BINARY_NAME = "ex2.exe"

OUTPUT_CSV = "wyniki_zad2_matching.csv"
K_MIN = 3
K_MAX = 10
REPEATS = 5

def parse_output(stdout, stderr):
    # Szukamy: "Rozmiar maksymalnego skojarzenia: X" oraz "Czas wykonania: Y s"
    match_size_match = re.search(r"Rozmiar maksymalnego skojarzenia:\s+(\d+)", stdout)
    # Dopasowujemy "Czas wykonania" z ewentualnym dopiskiem w nawiasie i liczbami w notacji zwykłej lub naukowej
    time_match = re.search(r"Czas wykonania(?:[^:]*):\s*([0-9]*\.?[0-9]+(?:[eE][-+]?[0-9]+)?)", stderr)

    match_size = int(match_size_match.group(1)) if match_size_match else None
    time_sec = float(time_match.group(1)) if time_match else None

    return match_size, time_sec

def run_experiment():
    print(f"Rozpoczynam eksperymenty dla Zadania 2. Wyniki w: {OUTPUT_CSV}")
    
    with open(OUTPUT_CSV, mode='w', newline='') as csv_file:
        writer = csv.writer(csv_file)
        writer.writerow(["k", "i", "run_id", "matching_size", "time_seconds"])

        # Pętla po k (wymiar)
        for k in range(K_MIN, K_MAX + 1):
            print(f"Testowanie k = {k}...")
            
            # Pętla po i (stopień wierzchołka, od 1 do k)
            for degree_i in range(1, k + 1):
                
                for r in range(REPEATS):
                    try:
                        result = subprocess.run(
                            [BINARY_NAME, "--size", str(k), "--degree", str(degree_i)],
                            capture_output=True,
                            text=True,
                            check=True
                        )
                        
                        match_size, time_sec = parse_output(result.stdout, result.stderr)
                        
                        if match_size is not None:
                            writer.writerow([k, degree_i, r + 1, match_size, time_sec])
                        else:
                            print(f"Blad parsowania dla k={k}, i={degree_i}")

                    except subprocess.CalledProcessError as e:
                        print(f"Blad uruchomienia: {e}")
                    except FileNotFoundError:
                        print(f"Nie znaleziono pliku {BINARY_NAME}. Skompiluj kod C++!")
                        return

    print("Zakonczono eksperymenty zadania 2.")

if __name__ == "__main__":
    run_experiment()