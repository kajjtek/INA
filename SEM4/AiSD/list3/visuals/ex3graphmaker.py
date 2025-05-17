import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

name = 'Select'
sigma = 16
# Wczytanie danych
df = pd.read_csv("../csvfiles/customSelect3_32.csv")  # <-- tu zmień ścieżkę do pliku
df_qs = pd.read_csv("../csvfiles/customSelect5_32.csv")
df_50 = pd.read_csv("../csvfiles/customSelect7_32.csv")
df_100 = pd.read_csv("../csvfiles/customSelect9_32.csv")
# Grupowanie po 'n' i liczenie średnich
grouped = df.groupby('n').mean().reset_index()
grouped_qs = df_qs.groupby('n').mean().reset_index()
grouped_50 = df_50.groupby('n').mean().reset_index()
grouped_100 = df_100.groupby('n').mean().reset_index()

n_values = grouped['n']
cmps_avg = grouped['cmps']
swps_avg = grouped['swps']

k50_n_values = grouped_50['n']
k50_cmps_avg = grouped_50['cmps']
k50_swp_avg = grouped_50['swps']


k100_n_values = grouped_100['n']
k100_cmps_avg = grouped_100['cmps']
k100_swp_avg = grouped_100['swps']


qs_n_values = grouped_qs['n']
qs_cmps_avg = grouped_qs['cmps']
qs_swp_avg = grouped_qs['swps']

# Ilorazy
c_over_n = cmps_avg / n_values
s_over_n = swps_avg / cmps_avg

qs_c_over_n = qs_cmps_avg / qs_n_values
qs_s_over_n = qs_swp_avg / qs_cmps_avg

k100_c_over_n = k100_cmps_avg / k100_n_values
k100_s_over_n = k100_swp_avg / k100_cmps_avg

k50_c_over_n = k50_cmps_avg / k50_n_values
k50_s_over_n = k50_swp_avg / k50_cmps_avg


fig, axes = plt.subplots(2, 2, figsize=(12, 12))

ax = axes[0, 0]

# Plot the raw data (lines without markers because you're drawing estimated functions)
# ax.plot(n_values, cmps_avg, label='k = 3', color='blue')
# ax.plot(qs_n_values, qs_cmps_avg, label='k = 3', color='orange')
# ax.plot(k50_n_values, k50_cmps_avg, label='k = 3', color='black')
# ax.plot(k100_n_values, k100_cmps_avg, label='k = 3', color='red')

# Now add best-fit lines for each dataset using np.polyfit
# For k = 16 (first dataset)
coeffs = np.polyfit(n_values, cmps_avg,3)  # degree=1 => linear fit
poly = np.poly1d(coeffs)
ax.plot(n_values, poly(n_values), color='blue', linestyle='-', label='near the beggining ', linewidth=2)

# For k = 32 (qs dataset)
coeffs = np.polyfit(qs_n_values, qs_cmps_avg,3)
poly = np.poly1d(coeffs)
ax.plot(qs_n_values, poly(qs_n_values), color='orange', linestyle='-', label='near the middle', linewidth=2)

# For k = 50
coeffs = np.polyfit(k50_n_values, k50_cmps_avg,3)
poly = np.poly1d(coeffs)
ax.plot(k50_n_values, poly(k50_n_values), color='black', linestyle='-', label='near the end', linewidth=2)

# For k = 75 (from k100 dataset)
coeffs = np.polyfit(k100_n_values, k100_cmps_avg,3)
poly = np.poly1d(coeffs)
ax.plot(k100_n_values, poly(k100_n_values), color='red', linestyle='-', label='out of scope', linewidth=2)

ax.set_title(name + ': Average Number of Comparisons (c) vs n')
ax.set_xlabel('Size of Array (n)')
ax.set_ylabel('Average Comparisons (c)')
ax.legend()

# --- Subplot 2: Average Time vs n ---
ax = axes[0, 1]
# ax.plot(n_values, swps_avg, label='k = 16', color='blue')
# ax.plot(qs_n_values, qs_swp_avg, label='k = 32', color='orange')
# ax.plot(k50_n_values, k50_swp_avg, label='k = 50', color='black')
# ax.plot(k100_n_values, k100_swp_avg, label='k = 75', color='red')

# Best-fit lines for Time
coeffs = np.polyfit(n_values, swps_avg,3)
poly = np.poly1d(coeffs)
ax.plot(n_values, poly(n_values), color='blue', linestyle='-', label='near the beggining ', linewidth=2)

coeffs = np.polyfit(qs_n_values, qs_swp_avg,3)
poly = np.poly1d(coeffs)
ax.plot(qs_n_values, poly(qs_n_values), color='orange', linestyle='-', label='near the middle', linewidth=2)

coeffs = np.polyfit(k50_n_values, k50_swp_avg,3)
poly = np.poly1d(coeffs)
ax.plot(k50_n_values, poly(k50_n_values), color='black', linestyle='-', label='near the end', linewidth=2)

coeffs = np.polyfit(k100_n_values, k100_swp_avg,3)
poly = np.poly1d(coeffs)
ax.plot(k100_n_values, poly(k100_n_values), color='red', linestyle='-', label='out of scope', linewidth=2)

ax.set_title(name + ': Time  vs n')
ax.set_xlabel('Size of Array (n)')
ax.set_ylabel('Average Time (s)')
ax.legend()

# --- Subplot 3: c/n Ratio vs n ---
ax = axes[1, 0]
# ax.plot(n_values, c_over_n, label='k = 16', color='blue')
# ax.plot(qs_n_values, qs_c_over_n, label='k = 32', color='orange')
# ax.plot(k50_n_values, k50_c_over_n, label='k = 50', color='black')
# ax.plot(k100_n_values, k100_c_over_n, label='k = 75', color='red')

coeffs = np.polyfit(n_values, c_over_n,3)
poly = np.poly1d(coeffs)
ax.plot(n_values, poly(n_values), color='blue', linestyle='-', label='near the beggining ', linewidth=2)

coeffs = np.polyfit(qs_n_values, qs_c_over_n,3)
poly = np.poly1d(coeffs)
ax.plot(qs_n_values, poly(qs_n_values), color='orange', linestyle='-', label='near the middle', linewidth=2)

coeffs = np.polyfit(k50_n_values, k50_c_over_n,3)
poly = np.poly1d(coeffs)
ax.plot(k50_n_values, poly(k50_n_values), color='black', linestyle='-', label='near the end', linewidth=2)

coeffs = np.polyfit(k100_n_values, k100_c_over_n,3)
poly = np.poly1d(coeffs)
ax.plot(k100_n_values, poly(k100_n_values), color='red', linestyle='-', label='out of scope', linewidth=2)

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
ax.plot(n_values, poly(n_values), color='blue', linestyle='-', label='near the beggining ', linewidth=2)

coeffs = np.polyfit(qs_n_values, qs_s_over_n,3)
poly = np.poly1d(coeffs)
ax.plot(qs_n_values, poly(qs_n_values), color='orange', linestyle='-', label='near the middle', linewidth=2)

coeffs = np.polyfit(k50_n_values, k50_s_over_n,3)
poly = np.poly1d(coeffs)
ax.plot(k50_n_values, poly(k50_n_values), color='black', linestyle='-', label='near the end', linewidth=2)

coeffs = np.polyfit(k100_n_values, k100_s_over_n,3)
poly = np.poly1d(coeffs)
ax.plot(k100_n_values, poly(k100_n_values), color='red', linestyle='-', label='out of scope', linewidth=2)

ax.set_title(name + ': Time Ratio vs n')
ax.set_xlabel('Size of Array (n)')
ax.set_ylabel('s/n')
ax.legend()

plt.tight_layout()
plt.show()