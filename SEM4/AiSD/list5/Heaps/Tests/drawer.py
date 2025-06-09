import matplotlib.pyplot as plt
import pandas as pd
import os

def plot_comparisons_history(num_trials=5, folder='.'):
    for i in range(1, num_trials + 1):
        filename = os.path.join(folder, f"experiment_{i}.csv")
        if not os.path.exists(filename):
            print(f"File not found: {filename}")
            continue
        
        df = pd.read_csv(filename, header=None, names=['operation', 'comparisons'])
        
        plt.figure(figsize=(10, 5))
        plt.plot(df['operation'], df['comparisons'], label=f'Trial {i}', color='blue')
        plt.xlabel("Operation index (i)")
        plt.ylabel("Number of comparisons")
        plt.title(f"Comparisons per Extract-Min operation (Trial {i})")
        plt.grid(True)
        plt.tight_layout()
        plt.savefig(f"plot_comparisons_trial_{i}.png")
        plt.close()
        print(f"Saved plot_comparisons_trial_{i}.png")

def plot_average_cost_vs_n(filename='average_costs.csv'):
    if not os.path.exists(filename):
        print(f"File not found: {filename}")
        return
    df = pd.read_csv(filename)
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
