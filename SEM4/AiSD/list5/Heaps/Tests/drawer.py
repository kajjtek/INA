import matplotlib.pyplot as plt
import pandas as pd
import os

def plot_comparisons_history(num_trials=5, folder='.'):
    for i in range(num_trials):
        filename = os.path.join(folder, f"experiment_{i}.csv")
        if not os.path.exists(filename):
            print(f"File not found: {filename}")
            continue
        
        df = pd.read_csv(filename, header=None, names=['operation', 'comparisons'])
        
        n = 500  # liczba insertów w każdym heapie
        
        plt.figure(figsize=(12, 6))
        
        # heap-insert h1 (indeksy 1..500)
        plt.plot(df['operation'][0:n], df['comparisons'][0:n], label='heap-insert h1', color='blue')
        
        # heap-insert h2 (indeksy 501..1000)
        plt.plot(df['operation'][n:2*n], df['comparisons'][n:2*n], label='heap-insert h2', color='orange')
        
        # heap-union (indeks 1001)
        plt.plot(df['operation'][2*n], df['comparisons'][2*n], 'ro', label='heap-union', markersize=6)
        
        # extract-min (indeksy 1002..2001)
        plt.plot(df['operation'][2*n+1:], df['comparisons'][2*n+1:], label='extract-min', color='green')
        
        plt.xlabel("Operation index (i)")
        plt.ylabel("Number of comparisons")
        plt.title(f"Comparisons per Operation (Trial {i})")
        plt.legend()
        plt.grid(True)
        plt.tight_layout()
        plt.savefig(f"plot_comparisons_trial_{i}.png")
        plt.close()
        print(f"Saved plot_comparisons_trial_{i}.png")

def plot_average_cost_vs_n(filename='average_cost.csv'):
    if not os.path.exists(filename):
        print(f"File not found: {filename}")
        return
    df = pd.read_csv('average_cost.csv', header=None, names=['n', 'average_cost'])
    plt.figure(figsize=(10, 6))
    plt.plot(df['n'], df['average_cost'], marker='o', linestyle='-', color='green')
    plt.xlabel("n")
    plt.ylabel("Average cost per Extract-Min operation")
    plt.title("Average cost vs. n")
    plt.grid(True)
    plt.tight_layout()
    plt.savefig("plot_average_cost_vs_n.png")
    plt.close()
    print("Saved plot_average_cost_vs_n.png")

if __name__ == "__main__":
    plot_comparisons_history()
    plot_average_cost_vs_n()
