import pandas as pd
import matplotlib.pyplot as plt
import os

# Tworzenie folderu na wykresy, jeśli nie istnieje
OUTPUT_DIR = "./graphs"
if not os.path.exists(OUTPUT_DIR):
    os.makedirs(OUTPUT_DIR)

def plot_task_1_and_4():
    """Generuje wykresy dla zadania 1 (Edmonds-Karp) i 4 (Dinic)."""
    files = {
        "1": "./ex1/wyniki_zad1_edmonds_karp.csv",
        "4": "./ex4/wyniki_zad4_dinic.csv"
    }

    for task_id, filename in files.items():
        if not os.path.exists(filename):
            print(f"Pominięto zadanie {task_id}: brak pliku {filename}")
            continue

        df = pd.read_csv(filename)
        # Obliczanie średnich dla każdego k
        stats = df.groupby('k').agg({
            'time_seconds': 'mean',
            'augmenting_paths': 'mean' if 'augmenting_paths' in df.columns else 'augmenting_paths_dfs_hits'
        }).reset_index()

        for index, row in stats.iterrows():
            k_val = int(row['k'])
            
            # 1. Wykres czasu dla danego k
            plt.figure(figsize=(10, 6))
            k_data = df[df['k'] == k_val]
            plt.bar(k_data['run_id'], k_data['time_seconds'], color='skyblue')
            plt.axhline(y=row['time_seconds'], color='red', linestyle='--', label=f"Średnia: {row['time_seconds']:.4f}s")
            plt.title(f"Zadanie {task_id} - Czas wykonania dla k={k_val}")
            plt.xlabel("Numer próby")
            plt.ylabel("Czas [s]")
            plt.legend()
            plt.grid(axis='y', linestyle='--', alpha=0.7)
            
            save_path = f"{OUTPUT_DIR}/ex{task_id}-{k_val}-0.png"
            plt.savefig(save_path)
            plt.close()

        # Dodatkowo: Zbiorczy wykres zależności czasu od k (skala logarytmiczna)
        plt.figure(figsize=(10, 6))
        plt.plot(stats['k'], stats['time_seconds'], marker='o', linestyle='-', color='green')
        plt.title(f"Zadanie {task_id} - Średni czas vs k")
        plt.xlabel("Wymiar k")
        plt.ylabel("Średni czas [s]")
        plt.yscale('log')
        plt.grid(True, which="both", ls="-", alpha=0.5)
        plt.savefig(f"{OUTPUT_DIR}/ex{task_id}-trend-time.png")
        plt.close()

def plot_task_2():
    """Generuje wykresy dla zadania 2 (Matching)."""
    filename = "./ex2/wyniki_zad2_matching.csv"
    if not os.path.exists(filename):
        print(f"Pominięto zadanie 2: brak pliku {filename}")
        return

    df = pd.read_csv(filename)
    
    # Grupowanie po k i i
    stats = df.groupby(['k', 'i']).agg({
        'time_seconds': 'mean',
        'matching_size': 'mean'
    }).reset_index()

    for k_val in df['k'].unique():
        k_df = df[df['k'] == k_val]
        
        for i_val in k_df['i'].unique():
            ki_data = k_df[k_df['i'] == i_val]
            avg_time = ki_data['time_seconds'].mean()

            # Wykres czasu dla konkretnej pary (k, i)
            plt.figure(figsize=(10, 6))
            plt.bar(ki_data['run_id'], ki_data['time_seconds'], color='salmon')
            plt.axhline(y=avg_time, color='blue', linestyle='--', label=f"Średnia: {avg_time:.4f}s")
            plt.title(f"Zadanie 2 - Czas wykonania dla k={k_val}, i={i_val}")
            plt.xlabel("Numer próby")
            plt.ylabel("Czas [s]")
            plt.legend()
            plt.grid(axis='y', linestyle='--', alpha=0.7)
            
            save_path = f"{OUTPUT_DIR}/ex2-{k_val}-{i_val}.png"
            plt.savefig(save_path)
            plt.close()

    # Wykres zależności rozmiaru skojarzenia od stopnia i dla różnych k
    plt.figure(figsize=(12, 8))
    for k_val in stats['k'].unique():
        subset = stats[stats['k'] == k_val]
        plt.plot(subset['i'], subset['matching_size'], marker='s', label=f"k={k_val}")
    
    plt.title("Zadanie 2 - Rozmiar skojarzenia vs stopień wierzchołka i")
    plt.xlabel("Stopień i")
    plt.ylabel("Średni rozmiar skojarzenia")
    plt.legend(title="Wymiar k")
    plt.grid(True)
    plt.savefig(f"{OUTPUT_DIR}/ex2-matching-comparison.png")
    plt.close()

if __name__ == "__main__":
    print("Generowanie wykresów...")
    plot_task_1_and_4()
    plot_task_2()
    print(f"Gotowe! Wykresy znajdują się w folderze {OUTPUT_DIR}")