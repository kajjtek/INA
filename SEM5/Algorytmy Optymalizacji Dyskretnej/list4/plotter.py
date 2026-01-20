import pandas as pd
import matplotlib.pyplot as plt
import os
import numpy as np

# Konfiguracja folderów
OUTPUT_DIR = "./graphs"
if not os.path.exists(OUTPUT_DIR):
    os.makedirs(OUTPUT_DIR)

def load_data(path):
    if not os.path.exists(path):
        return None
    try:
        df = pd.read_csv(path)
        # Konwersja danych na numeryczne z obsługą błędów
        cols_to_convert = ['k', 'i', 'run_id', 'time_seconds', 'max_flow', 
                           'augmenting_paths', 'augmenting_paths_dfs_hits', 'matching_size']
        for col in cols_to_convert:
            if col in df.columns:
                df[col] = pd.to_numeric(df[col], errors='coerce')
        return df
    except Exception as e:
        print(f"Błąd podczas ładowania {path}: {e}")
        return None

def plot_comparison_tasks(df1, df4):
    """Porównuje wyniki zadania 1 (EK) i 4 (Dinic) na wspólnych wykresach."""
    if df1 is None or df4 is None:
        return

    # Agregacja danych
    s1 = df1.groupby('k').agg({'time_seconds': 'mean', 'augmenting_paths': 'mean', 'max_flow': 'mean'}).reset_index()
    s4 = df4.groupby('k').agg({'time_seconds': 'mean', 'augmenting_paths_dfs_hits': 'mean', 'max_flow': 'mean'}).reset_index()

    # 1. Porównanie czasu (skala logarytmiczna)
    plt.figure(figsize=(10, 6))
    plt.plot(s1['k'], s1['time_seconds'], 'o-', label='Edmonds-Karp', color='blue')
    plt.plot(s4['k'], s4['time_seconds'], 's-', label='Dinic', color='red')
    plt.yscale('log')
    plt.xlabel('Wymiar k')
    plt.ylabel('Średni czas [s] (skala log)')
    plt.title('Porównanie czasu wykonania: Edmonds-Karp vs Dinic')
    plt.legend()
    plt.grid(True, which="both", ls="-", alpha=0.3)
    plt.savefig(f"{OUTPUT_DIR}/comp_time_ek_dinic.png")
    plt.close()

    # 2. Porównanie liczby ścieżek
    plt.figure(figsize=(10, 6))
    plt.plot(s1['k'], s1['augmenting_paths'], 'o-', label='EK (Augmenting Paths)', color='blue')
    plt.plot(s4['k'], s4['augmenting_paths_dfs_hits'], 's-', label='Dinic (DFS Hits)', color='red')
    plt.yscale('log')
    plt.xlabel('Wymiar k')
    plt.ylabel('Liczba operacji powiększających (skala log)')
    plt.title('Liczba ścieżek/operacji powiększających')
    plt.legend()
    plt.grid(True, which="both", ls="-", alpha=0.3)
    plt.savefig(f"{OUTPUT_DIR}/comp_paths_ek_dinic.png")
    plt.close()

    # 3. Średni Max Flow (powinny być identyczne)
    plt.figure(figsize=(10, 6))
    plt.plot(s1['k'], s1['max_flow'], 'o-', label='Średni Max Flow', color='green')
    plt.yscale('log')
    plt.xlabel('Wymiar k')
    plt.ylabel('Wartość przepływu (skala log)')
    plt.title('Średnia wartość maksymalnego przepływu w funkcji k')
    plt.grid(True, which="both", ls="-", alpha=0.3)
    plt.savefig(f"{OUTPUT_DIR}/trend_max_flow.png")
    plt.close()

def plot_task_2():
    """Generuje wykresy dla zadania 2 zgodnie z wytycznymi PDF."""
    df = load_data("./ex2/wyniki_zad2_matching.csv")
    if df is None:
        print("Pominięto zadanie 2: brak danych.")
        return

    stats = df.groupby(['k', 'i']).agg({'time_seconds': 'mean', 'matching_size': 'mean'}).reset_index()

    # Wykres 1: Rozmiar skojarzenia vs i (dla każdego k)
    plt.figure(figsize=(10, 6))
    for k_val in sorted(stats['k'].unique()):
        subset = stats[stats['k'] == k_val]
        plt.plot(subset['i'], subset['matching_size'], marker='o', label=f'k={k_val}')
    plt.xlabel('Stopień wierzchołka i')
    plt.ylabel('Średni rozmiar skojarzenia')
    plt.title('Rozmiar skojarzenia w zależności od i')
    plt.legend(title="Wymiar k", bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(f"{OUTPUT_DIR}/ex2_matching_vs_i.png")
    plt.close()

    # Wykres 2: Czas działania vs k (dla każdego i)
    plt.figure(figsize=(10, 6))
    for i_val in sorted(stats['i'].unique()):
        subset = stats[stats['i'] == i_val]
        if len(subset) > 0:
            plt.plot(subset['k'], subset['time_seconds'], marker='s', label=f'i={i_val}')
    plt.yscale('log')
    plt.xlabel('Wymiar k')
    plt.ylabel('Średni czas [s] (log)')
    plt.title('Czas wyznaczania skojarzenia w funkcji k')
    plt.legend(title="Stopień i", bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.grid(True, which="both", ls="-", alpha=0.3)
    plt.tight_layout()
    plt.savefig(f"{OUTPUT_DIR}/ex2_time_vs_k.png")
    plt.close()

def plot_individual_runs(task_id, df):
    """Generuje wykresy słupkowe dla pojedynczych prób."""
    if df is None: return
    
    unique_keys = ['k'] if task_id != '2' else ['k', 'i']
    grouped = df.groupby(unique_keys)

    for name, group in grouped:
        # Obsługa klucza grupowania (może być krotką lub pojedynczą wartością)
        if isinstance(name, (tuple, list)):
            k_val = name[0]
            i_val = name[1] if len(name) > 1 else 0
        else:
            k_val = name
            i_val = 0
        
        plt.figure(figsize=(8, 5))
        plt.bar(group['run_id'], group['time_seconds'], color='gray', alpha=0.6)
        mean_v = group['time_seconds'].mean()
        plt.axhline(mean_v, color='red', linestyle='--', label=f'Średnia: {mean_v:.4f}s')
        
        plt.title(f"Zadanie {task_id} | k={k_val}" + (f", i={i_val}" if task_id == '2' else ""))
        plt.xlabel("Próba")
        plt.ylabel("Czas [s]")
        plt.legend()
        
        filename = f"ex{task_id}-{k_val}-{i_val}.png"
        plt.savefig(f"{OUTPUT_DIR}/{filename}")
        plt.close()

if __name__ == "__main__":
    print("Ładowanie danych...")
    df1 = load_data("./ex1/wyniki_zad1_edmonds_karp.csv")
    df2 = load_data("./ex2/wyniki_zad2_matching.csv")
    df4 = load_data("./ex4/wyniki_zad4_dinic.csv")

    print("Generowanie wykresów porównawczych (Zadania 1 & 4)...")
    plot_comparison_tasks(df1, df4)
    
    print("Generowanie wykresów dla Zadania 2...")
    plot_task_2()

    print("Generowanie wykresów jednostkowych dla sprawozdania...")
    plot_individual_runs('1', df1)
    plot_individual_runs('2', df2)
    plot_individual_runs('4', df4)

    print(f"Sukces! Wszystkie wykresy zapisano w folderze {OUTPUT_DIR}")