import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("../csvfiles/koptimal.csv")
df = df.iloc[:, [0, 1]]
df.columns = ["k", "time_ms"]

avg_times = df.groupby("k")["time_ms"].mean()

plt.figure(figsize=(10, 6))
plt.plot(avg_times.index, avg_times.values, marker='o')
plt.title("Average time vs k (Hybrid Sort)")
plt.xlabel("k (threshold for switching to Insertion Sort)")
plt.ylabel("Average time [ms]")
plt.grid(True)
plt.tight_layout()
plt.show()