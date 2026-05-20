import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import re
import os

# --- KONFIGURACJA ---
INPUT_FILE = './ss5.csv'
OUTPUT_DIR = '../graphs'
# Filtrowanie "złych" wyników (np. timeouty/błędy oznaczone inaczej niż OK)
FILTER_STATUS_OK = True 

if not os.path.exists(OUTPUT_DIR):
    os.makedirs(OUTPUT_DIR)

def parse_graph_name(graph_name):
    """
    Rozbija nazwę grafu na rodzinę i parametr.
    Np. 'Square-C.13.0' -> ('Square-C', 13)
    Np. 'USA-road-t.NY' -> ('USA-road-t', 'NY')
    """
    # Sprawdzenie czy to USA-road
    if 'USA-road' in graph_name:
        parts = graph_name.split('.')
        family = parts[0]
        region = parts[1] if len(parts) > 1 else 'Unknown'
        return family, region
    
    # Standardowe rodziny generowane (np. Random4-n.10.0)
    match = re.match(r"([a-zA-Z0-9-]+)\.(\d+)", graph_name)
    if match:
        return match.group(1), int(match.group(2))
    
    return graph_name, 0

def generate_plots():
    # 1. Wczytanie danych
    df = pd.read_csv(INPUT_FILE)
    df['Czas_s'] = pd.to_numeric(df['Czas_s'], errors='coerce')
    df = df.dropna(subset=['Czas_s'])
    
    
    if FILTER_STATUS_OK and 'Status' in df.columns:
        df = df[df['Status'] == 'OK']

    # 2. Przetwarzanie danych
    # Konwersja czasu na ms (jeśli w CSV jest w sekundach)
    df['Czas_ms'] = df['Czas_s'] * 1000

    # Ekstrakcja rodziny i parametru
    df[['Rodzina', 'Parametr']] = df['Graf'].apply(
        lambda x: pd.Series(parse_graph_name(x))
    )

    # Agregacja (średnia z różnych seedów/źródeł dla tego samego parametru i algorytmu)
    df_agg = df.groupby(['Rodzina', 'Algorytm', 'Parametr'])['Czas_ms'].mean().reset_index()

    # Lista unikalnych rodzin
    rodziny = df_agg['Rodzina'].unique()

    # Ustawienie stylu wykresów
    sns.set_theme(style="whitegrid")
    
    for rodzina in rodziny:
        data_rodzina = df_agg[df_agg['Rodzina'] == rodzina]
        
        plt.figure(figsize=(10, 6))
        
        # Sprawdzenie czy to USA-road (wykres słupkowy) czy generowane (liniowy)
        if 'USA-road' in rodzina:
            # Sortowanie regionów alfabetycznie lub wg czasu
            chart = sns.barplot(
                data=data_rodzina,
                x='Parametr', # Tutaj 'Parametr' to nazwa regionu (np. NY, COL)
                y='Czas_ms',
                hue='Algorytm',
                palette='viridis'
            )
            plt.yscale('log')
            plt.xlabel('Region')
            plt.title(f'Czas wykonania dla rodziny: {rodzina}')
        else:
            # Wykres liniowy dla parametrów liczbowych
            # Upewnij się, że parametr jest liczbowy do sortowania
            data_rodzina['Parametr'] = pd.to_numeric(data_rodzina['Parametr'])
            data_rodzina = data_rodzina.sort_values('Parametr')

            chart = sns.lineplot(
                data=data_rodzina,
                x='Parametr',
                y='Czas_ms',
                hue='Algorytm',
                style='Algorytm',
                markers=True,
                dashes=False,
                palette='viridis',
                linewidth=2.5,
                markersize=8
            )
            plt.yscale('log')
            plt.xlabel(r'Parametr $i$ (skala logarytmiczna rozmiaru)')
            plt.title(f'Czas wykonania dla rodziny: {rodzina}')

        plt.ylabel('Czas wykonania [ms] (log)')
        plt.grid(True, which="both", ls="-", alpha=0.5)
        
        filename = f"{OUTPUT_DIR}/plot_{rodzina}.png"
        plt.savefig(filename, dpi=300, bbox_inches='tight')
        print(f"Wygenerowano wykres: {filename}")
        plt.close()

if __name__ == "__main__":
    generate_plots()