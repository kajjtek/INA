import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import math

name = 'Select'
sigma = 16
# Wczytanie danych
df = pd.read_csv("../csvfiles/dualsort.csv")  # <-- tu zmień ścieżkę do pliku



# Grupowanie po 'n' i liczenie średnich
grouped = df.groupby('n').mean().reset_index()




n_values = grouped['n']
cmps_avg = grouped['cmps']
swps_avg = grouped['time']



# Ilorazy
c_over_n = cmps_avg / n_values
s_over_n = [cmp/(n*math.log2(n)) for cmp, n in zip(swps_avg, n_values)]










fig, axes = plt.subplots(2, 2, figsize=(12, 12))

ax = axes[0, 0]

# Plot the raw data (lines without markers because you're drawing estimated functions)
# ax.plot(n_values, cmps_avg, label='k = 16', color='blue')
# ax.plot(qs_n_values, qs_cmps_avg, label='k = 32', color='orange')
# ax.plot(k50_n_values, k50_cmps_avg, label='k = 50', color='black')
# ax.plot(k100_n_values, k100_cmps_avg, label='k = 75', color='red')

# Now add best-fit lines for each dataset using np.polyfit
# For k = 16 (first dataset)
coeffs = np.polyfit(n_values, cmps_avg,3)  # degree=1 => linear fit
poly = np.poly1d(coeffs)
ax.plot(n_values, poly(n_values), color='blue', linestyle='-', label='dual pivot quick sort', linewidth=2)

# For k = 32 (qs dataset)




# For k = 50




# For k = 75 (from k100 dataset)




ax.set_title(name + ': Average Number of Comparisons (c) vs n')
ax.set_xlabel('Size of Array (n)')
ax.set_ylabel('Average Comparisons (c)')
ax.legend()

# --- Subplot 2: Average Swaps vs n ---
ax = axes[0, 1]
# ax.plot(n_values, swps_avg, label='k = 16', color='blue')
# ax.plot(qs_n_values, qs_swp_avg, label='k = 32', color='orange')
# ax.plot(k50_n_values, k50_swp_avg, label='k = 50', color='black')
# ax.plot(k100_n_values, k100_swp_avg, label='k = 75', color='red')

# Best-fit lines for swaps
coeffs = np.polyfit(n_values, swps_avg,3)
poly = np.poly1d(coeffs)
ax.plot(n_values, poly(n_values), color='blue', linestyle='-', label='dual pivot quick sort', linewidth=2)

ax.set_title(name + ': Time vs n')
ax.set_xlabel('Size of Array (n)')
ax.set_ylabel('Average Swaps (s)')
ax.legend()

# --- Subplot 3: c/n Ratio vs n ---
ax = axes[1, 0]
# ax.plot(n_values, c_over_n, label='k = 16', color='blue')
# ax.plot(qs_n_values, qs_c_over_n, label='k = 32', color='orange')
# ax.plot(k50_n_values, k50_c_over_n, label='k = 50', color='black')
# ax.plot(k100_n_values, k100_c_over_n, label='k = 75', color='red')

coeffs = np.polyfit(n_values, c_over_n,3)
poly = np.poly1d(coeffs)
ax.plot(n_values, poly(n_values), color='blue', linestyle='-', label='dual pivot quick sort', linewidth=2)

ax.set_title(name + ': c/n Ratio vs n')
ax.set_xlabel('Size of Array (n)')
ax.set_ylabel('c/n')
ax.legend()

# --- Subplot 4: s/n Ratio vs n ---
ax = axes[1, 1]
# ax.plot(n_values, s_over_n, label='k = 16', color='blue')
# ax.plot(qs_n_values, qs_s_over_n, label='k = 32', color='orange')
# ax.plot(k50_n_values, k50_s_over_n, label='k = 50', color='black')
# ax.plot(k100_n_values, k100_s_over_n, label='k = 75', color='red')

coeffs = np.polyfit(n_values, s_over_n,3)
poly = np.poly1d(coeffs)
ax.plot(n_values, poly(n_values), color='blue', linestyle='-', label='dual pivot quick sort', linewidth=2)

Ymax = 1.1 * max(s_over_n)

ax.set_ylim(0, Ymax)
ax.set_title(name + ': Time Ratio vs n')
ax.set_xlabel('Size of Array (n)')
ax.set_ylabel('Time')
ax.legend()

plt.tight_layout()
plt.show()