import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Wczytanie danych
df = pd.read_csv("../csvfiles/mergesort100.csv")  # <-- tu zmień ścieżkę do pliku
df_qs = pd.read_csv("../csvfiles/mysort100.csv")
# Grupowanie po 'n' i liczenie średnich
grouped = df.groupby('Size of array').mean().reset_index()
grouped_qs = df_qs.groupby('Size of array').mean().reset_index()

n_values = grouped['Size of array']
cmps_avg = grouped[' Number of comparisons']
swps_avg = grouped[' Number of swaps']

qs_n_values = grouped_qs['Size of array']
qs_cmps_avg = grouped_qs[' Number of comparisons']
qs_swp_avg = grouped_qs[' Number of swaps']

# Ilorazy
c_over_n = cmps_avg / n_values
s_over_n = swps_avg / n_values

qs_c_over_n = qs_cmps_avg / qs_n_values
qs_s_over_n = qs_swp_avg / qs_n_values

fig, axes = plt.subplots(2, 2, figsize=(12, 12))

# Wykres średnia liczba porównań (c) w zależności od n
axes[0, 0].plot(n_values, cmps_avg, label='mergesort', color='blue', marker='o')
axes[0, 0].plot(qs_n_values, qs_cmps_avg, label='mysort', color='orange', marker='x')
axes[0, 0].set_title('Average Number of Comparisons (c) vs n')
axes[0, 0].set_xlabel('Size of Array (n)')
axes[0, 0].set_ylabel('Average Comparisons (c)')
axes[0, 0].legend()

# Wykres średnia liczba przestawień (s) w zależności od n
axes[0, 1].plot(n_values, swps_avg, label='mergesort', color='blue', marker='o')
axes[0, 1].plot(qs_n_values, qs_swp_avg, label='mysort', color='orange', marker='x')
axes[0, 1].set_title('Average Number of Swaps (s) vs n')
axes[0, 1].set_xlabel('Size of Array (n)')
axes[0, 1].set_ylabel('Average Swaps (s)')
axes[0, 1].legend()

# Wykres iloraz c/n w zależności od n
axes[1, 0].plot(n_values, c_over_n, label='mergesort', color='blue', marker='o')
axes[1, 0].plot(qs_n_values, qs_c_over_n, label='mysort', color='orange', marker='x')
axes[1, 0].set_title('c/n Ratio vs n')
axes[1, 0].set_xlabel('Size of Array (n)')
axes[1, 0].set_ylabel('c/n')
axes[1, 0].legend()

# Wykres iloraz s/n w zależności od n
axes[1, 1].plot(n_values, s_over_n, label='mergesort', color='blue', marker='o')
axes[1, 1].plot(qs_n_values, qs_s_over_n, label='mysort', color='orange', marker='x')
axes[1, 1].set_title('s/n Ratio vs n')
axes[1, 1].set_xlabel('Size of Array (n)')
axes[1, 1].set_ylabel('s/n')
axes[1, 1].legend()

plt.tight_layout()
plt.show()