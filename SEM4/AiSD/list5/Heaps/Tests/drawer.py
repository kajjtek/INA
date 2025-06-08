import matplotlib.pyplot as plt
import pandas as pd

# Wczytaj dane z pliku CSV
try:
    df_exp1 = pd.read_csv('comparisons_per_operation_n500.csv')
except FileNotFoundError:
    print("Błąd: Plik 'comparisons_per_operation_n500.csv' nie został znaleziony.")
    print("Upewnij się, że uruchomiłeś program C++ i plik został wygenerowany.")
    exit()

plt.figure(figsize=(14, 8)) # Ustawia rozmiar wykresu

# Rysuj dane dla każdego eksperymentu
for i in sorted(df_exp1['experiment_id'].unique()):
    subset = df_exp1[df_exp1['experiment_id'] == i]
    plt.plot(subset['operation_index'], subset['comparisons_count'], label=f'Eksperyment {i}', alpha=0.7)

plt.xlabel('Indeks operacji (kolejność wykonania)')
plt.ylabel('Liczba porównań kluczy')
plt.title('Liczba porównań na operację dla n=500 (5 eksperymentów)')
plt.legend(title='ID eksperymentu')
plt.grid(True, linestyle='--', alpha=0.6)
plt.tight_layout() # Dopasuj układ, aby zapobiec przycinaniu etykiet
plt.show()

# Wczytaj dane z pliku CSV
try:
    df_exp2 = pd.read_csv('n_vs_average_cost.csv')
except FileNotFoundError:
    print("Błąd: Plik 'n_vs_average_cost.csv' nie został znaleziony.")
    print("Upewnij się, że uruchomiłeś program C++ i plik został wygenerowany.")
    exit()

plt.figure(figsize=(12, 7)) # Ustawia rozmiar wykresu
plt.plot(df_exp2['n'], df_exp2['average_cost'], marker='o', linestyle='-', color='skyblue', markersize=4, linewidth=1.5)

plt.xlabel('Wartość n')
plt.ylabel('Średni koszt operacji (całkowita liczba porównań / n)')
plt.title('Zależność średniego kosztu operacji od n')
plt.grid(True, linestyle='--', alpha=0.6)
plt.tight_layout()
plt.show()
