import subprocess
import re
import csv

GLPSOL_PATH = "glpsol" 

def run_experiment(k, task="task1", i=2):
    exe = "./ex1/ex1" if task == "task1" else "./ex2/ex2"
    model_file = "temp_model.mod"
    
    # Budowanie komendy: odpalamy raz, tworzymy plik i czytamy wynik z konsoli
    cmd = [exe, "--size", str(k), "--glpk", model_file]
    if task == "task2":
        cmd += ["--degree", str(i)]
    
    try:
        # 1. URUCHOM C++ (oblicza przepływ i zapisuje plik .mod)
        res_cpp = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
        
        # Wyciąganie wyniku z C++ (szukamy liczby po dwukropku)
        cpp_val_match = re.search(r"(?:przeplyw|skojarzenie):\s*(\d+)", res_cpp.stdout, re.IGNORECASE)
        cpp_time_match = re.search(r"(?:Czas|Czas wykonania):\s*(\d+)", res_cpp.stderr, re.IGNORECASE)

        if not cpp_val_match:
            return None

        cpp_val = cpp_val_match.group(1)
        cpp_time = cpp_time_match.group(1) if cpp_time_match else "0"

        # 2. URUCHOM GLPK na tym samym pliku, który przed chwilą zapisał C++
        res_glpk = subprocess.run([GLPSOL_PATH, "-m", model_file], capture_output=True, text=True, timeout=60)
        
        # Wyciąganie wyniku z GLPK
        glpk_val_match = re.search(r"Objective:\s+\w+\s+=\s+(\d+)", res_glpk.stdout)
        if not glpk_val_match:
            glpk_val_match = re.search(r"obj\s+=\s+([\d.e+]+)", res_glpk.stdout)

        glpk_val = str(int(float(glpk_val_match.group(1)))) if glpk_val_match else "N/A"
        
        return {
            "k": k,
            "task": task,
            "i": i if task == "task2" else "-",
            "cpp_res": cpp_val,
            "glpk_res": glpk_val,
            "time_us": cpp_time,
            "status": "OK" if cpp_val == glpk_val else "ERROR"
        }
    except Exception as e:
        print(f"Błąd dla k={k}: {e}")
        return None

# Pętla testowa
results = []
print("Uruchamiam testy (C++ generuje model, GLPK go sprawdza)...")

for k in range(1, 9):
    res = run_experiment(k, "task1")
    if res:
        results.append(res)
        print(f"k={k}: C++={res['cpp_res']}, GLPK={res['glpk_res']} -> {res['status']}")

# Zapis do CSV
with open('results.csv', 'w', newline='', encoding='utf-8') as f:
    writer = csv.DictWriter(f, fieldnames=["k", "task", "i", "cpp_res", "glpk_res", "time_us", "status"])
    writer.writeheader()
    writer.writerows(results)

print("\nWyniki gotowe w results.csv!")