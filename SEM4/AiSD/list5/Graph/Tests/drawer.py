import matplotlib.pyplot as plt
import pandas as pd

try:
    df = pd.read_csv('information_passing_results.csv')
except FileNotFoundError:
    print("Error: 'message_propagation_analysis.csv' not found.")
    print("Please ensure you have run the C++ program and the file was generated.")
    exit()

plt.figure(figsize=(14, 8))

plt.plot(df['n'], df['avg_rounds'], marker='o', linestyle='-', color='blue', label='Średnia liczba rund', markersize=4, linewidth=1.5)
plt.plot(df['n'], df['max_rounds'], marker='x', linestyle='--', color='red', label='Maksymalna liczba rund', markersize=4, linewidth=1.5)
plt.plot(df['n'], df['min_rounds'], marker='^', linestyle=':', color='green', label='Minimalna liczba rund', markersize=4, linewidth=1.5)

plt.xlabel('Liczba wierzchołków (n)')
plt.ylabel('Liczba rund')
plt.title('Analiza rozesłania wiadomości w drzewach MST')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.6)
plt.tight_layout()
plt.show()

print("Wykres 'Analiza rozesłania wiadomości w drzewach MST' został wygenerowany.")



# Wczytaj dane z pliku CSV
try:
    df = pd.read_csv('results.csv') # Zakładana nazwa pliku
except FileNotFoundError:
    print("Błąd: Plik 'mst_times_comparison.csv' nie został znaleziony.")
    print("Upewnij się, że plik CSV z danymi jest w tym samym katalogu co skrypt.")
    exit()

plt.figure(figsize=(12, 7))

# Wykres dla algorytmu Prima
plt.plot(df['n'], df['AvgPrim(ms)'], marker='o', linestyle='-', color='blue', label='Średni czas Prima (ms)', markersize=4, linewidth=1.5)

# Wykres dla algorytmu Kruskala
plt.plot(df['n'], df['AvgKruskal(ms)'], marker='x', linestyle='--', color='red', label='Średni czas Kruskala (ms)', markersize=4, linewidth=1.5)

plt.xlabel('Liczba wierzchołków (n)')
plt.ylabel('Średni czas (ms)')
plt.title('Porównanie średnich czasów wykonania algorytmów Prima i Kruskala')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.6)
plt.tight_layout()
plt.show()

print("Wykres 'Porównanie średnich czasów wykonania algorytmów Prima i Kruskala' został wygenerowany.")
