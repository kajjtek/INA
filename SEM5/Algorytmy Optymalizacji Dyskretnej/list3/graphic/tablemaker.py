import pandas as pd
import re
import os
import numpy as np

# --- KONFIGURACJA ---
INPUT_FILE_CSV = '../actual_results/radix/wyniki_radix_dlugosci_sciezek_p2p.csv' # Plik CSV z wynikami P2P (dystansami)
ALGORYTM_DO_TABELI = 'radix'       # Algorytm, którego wyniki dystansu chcemy pokazać.

# SCHEMAT NAZEWNICTWA PLIKÓW Z ZAPYTANIAMI
# Na podstawie Twojego przykładu 'generated/Long-C.1.0.p2p5', gdzie 'Long-C.1.0' to nazwa grafu.
QUERIES_DIR = '../generated'
QUERIES_SUFFIX = '.p2p5'


def get_query_file_path(graph_name):
    """Generuje oczekiwaną ścieżkę do pliku z zapytaniami dla danego grafu."""
    # Używamy tylko części przed '.p2p5' do znalezienia pliku
    return os.path.join(QUERIES_DIR, graph_name + QUERIES_SUFFIX)

def load_st_pairs(file_path):
    """Wczytuje pary (s, t) z pliku z zapytaniami."""
    queries = []
    if not os.path.exists(file_path):
        # W przypadku braku pliku, zwracamy puste pary
        print(f"Brak pliku z zapytaniami: {file_path}. Używam domyślnych par.")
        return [('N/A', 'N/A')] * 5
        
    try:
        with open(file_path, 'r') as f:
            for i, line in enumerate(f):
                parts = line.strip().split()
                if len(parts) >= 3 and parts[0].lower() == 'q':
                    s = parts[1]
                    t = parts[2]
                    queries.append((s, t))
                # Przerwij po 5-ciu
                if i >= 4 and len(queries) == 5:
                    break
        
        # Jeśli załadowano mniej niż 5, uzupełnij
        while len(queries) < 5:
             queries.append(('N/A', 'N/A'))
             
        return queries

    except Exception as e:
        print(f"Błąd podczas czytania pliku z zapytaniami {file_path}: {e}")
        return [('N/A', 'N/A')] * 5

def parse_graph_info(graph_name):
    """Próbuje wyciągnąć Rodzinę i Instancję (wartość i)."""
    if 'USA-road' in graph_name:
        # USA-road-d.NY -> ('USA-road', 'NY', 0) (0 jako minimalne i)
        return 'USA-road', graph_name.split('.')[-1], 0
    
    # Schemat Long-C.1.0, Long-n.10.0, Square-C.2.0
    match = re.match(r"([a-zA-Z0-9-]+)\.(\d+)\.0", graph_name)
    if match:
        family = match.group(1)
        # Bierzemy i jako liczbę całkowitą
        i_val = int(match.group(2)) 
        # Zwracamy całą nazwę instancji (np. Long-C.1.0) oraz wartość i
        return family, graph_name, i_val
    
    # Domyślny przypadek
    return graph_name, graph_name, 0

def get_max_i_instance(df):
    """Filtruje DataFrame, zostawiając tylko instancję z największym 'i' dla każdej Rodziny."""
    
    # Dodanie kolumn pomocniczych
    df[['Rodzina', 'Instancja_Pelna', 'i']] = df['Graf'].apply(lambda x: pd.Series(parse_graph_info(x)))
    
    # Grupowanie i znalezienie maksymalnego 'i'
    idx = df.groupby('Rodzina')['i'].idxmax()
    df_filtered = df.loc[idx]
    
    # Przywrócenie pierwotnej kolejności po rodzinach (opcjonalne)
    df_filtered = df_filtered.sort_values(by='Rodzina', ascending=True)
    
    return df_filtered

def generate_latex_table():
    try:
        # Dodanie konwersji, aby upewnić się, że kolumny dystansów są numeryczne
        df = pd.read_csv(INPUT_FILE_CSV)
        p2p_columns_csv = [f"Para_{i+1}" for i in range(5)]
        for col in p2p_columns_csv:
             # Wymuś konwersję do typu float, błędy zamień na NaN
             df[col] = pd.to_numeric(df[col], errors='coerce') 

    except FileNotFoundError:
        print(f"Błąd: Nie znaleziono pliku CSV: {INPUT_FILE_CSV}")
        return

    # 1. FILTROWANIE DANYCH
    if ALGORYTM_DO_TABELI:
        df = df[df['Algorytm'].str.contains(ALGORYTM_DO_TABELI, case=False, na=False)]

    if df.empty:
        print(f"UWAGA: Pusty DataFrame po filtrowaniu. Sprawdź nazwę algorytmu ('{ALGORYTM_DO_TABELI}') w CSV.")
        return

    # Wybór instancji z największym 'i'
    df_max_i = get_max_i_instance(df.copy())
    
    # 2. PRZYGOTOWANIE STRUKTURY TABELI LATEX (jak na obrazku)
    latex_headers = [r"\textbf{Rodzina}", r"\textbf{Źródło ($s$)}", r"\textbf{Cel ($t$)}", r"\textbf{Dystans ($d$)}"]
    col_spec = "|l|r|r|r|"
    col_spec = col_spec.strip('|') 
    
    latex_output = []
    latex_output.append(r"\begin{table}[h!]")
    latex_output.append(r"\caption{Tabela 1: Wyniki dystansów P2P}") # Jak na obrazku
    latex_output.append(r"\centering")
    # Zrezygnowanie z resizebox, jeśli kolumny są węższe
    latex_output.append(r"\begin{tabular}{" + col_spec + r"}")
    latex_output.append(r"\hline")
    latex_output.append(" & ".join(latex_headers) + r" \\ \hline")

    # 3. GENEROWANIE WIERSZY
    p2p_columns_csv = [f"Para_{i+1}" for i in range(5)]
    current_family = ""
    first_row_family = True # Flaga do obsługi multirow

    for index, row in df_max_i.iterrows():
        family = row['Rodzina']
        graph_name = row['Graf']
        
        # Wczytanie faktycznych par (s, t) dla tej instancji
        queries_path = get_query_file_path(graph_name)
        st_pairs = load_st_pairs(queries_path)

        # Sprawdzenie, czy dystanse są dostępne
        distances = [row[col] for col in p2p_columns_csv if col in row]
        
        # Upewnienie się, że mamy 5 dystansów (w razie potrzeby uzupełnienie NaN)
        distances.extend([np.nan] * (5 - len(distances)))

        # Wiersze dla 5 par P2P
        for i in range(5):
            s, t = st_pairs[i]
            d = distances[i]

            # Formatowanie dystansu (bez notacji naukowej)
            # Użycie formatu bez miejsc po przecinku (jeśli to liczba)
            distance_str = f"{d:.0f}" if pd.notna(d) else "N/A"

            # Obsługa pierwszej kolumny (Rodzina)
            if first_row_family:
                if family != current_family and current_family != "":
                    # Dodanie linii oddzielającej rodziny
                    latex_output.append(r"\hline")
                
                # Użycie multirow, by nazwa rodziny była wyśrodkowana na 5 wierszach
                # Uwaga: multirow w LaTeX wymaga pakietu \usepackage{multirow}
                line = f"\\multirow{{5}}{{*}}{{{family}}} & {s} & {t} & {distance_str} \\\\"
                first_row_family = False
            else:
                # Pozostałe wiersze w tej samej rodzinie
                line = f"& {s} & {t} & {distance_str} \\\\"

            latex_output.append(line)
        
        current_family = family
        first_row_family = True # Reset dla kolejnej rodziny
        
    # 4. ZAMKNIĘCIE TABELI
    latex_output.append(r"\hline")
    latex_output.append(r"\end{tabular}")
    latex_output.append(r"\label{tab:results_p2p_final}")
    latex_output.append(r"\end{table}")
    
    print('\n'.join(latex_output))

if __name__ == "__main__":
    generate_latex_table()