import pandas as pd # type: ignore
import matplotlib.pyplot as plt
import os

def plot_experiment_results_separate(file_path="experiment_results.csv"):
    """
    Loads experiment results from a CSV and generates separate plots for each
    metric (Comparisons, Pointer Ops, Height), broken down by scenario and average/max.

    Args:
        file_path (str): The path to the CSV file containing experiment results.
    """
    if not os.path.exists(file_path):
        print(f"Error: The file '{file_path}' was not found.")
        return

    df = pd.read_csv(file_path)

    scenarios = df['Scenario'].unique()

    for scenario in scenarios:
        scenario_df = df[df['Scenario'] == scenario]
        print(f"Generating separate plots for scenario: {scenario}")

        # Define metrics to plot for Insert operations
        insert_metrics = {
            'Avg_Insert_Comparisons': 'Average Insert Comparisons',
            'Max_Insert_Comparisons': 'Maximum Insert Comparisons',
            'Avg_Insert_Pointer_Ops': 'Average Insert Pointer Operations',
            'Max_Insert_Pointer_Ops': 'Maximum Insert Pointer Operations'
        }

        # Define metrics to plot for Delete operations
        delete_metrics = {
            'Avg_Delete_Comparisons': 'Average Delete Comparisons',
            'Max_Delete_Comparisons': 'Maximum Delete Comparisons',
            'Avg_Delete_Pointer_Ops': 'Average Delete Pointer Operations',
            'Max_Delete_Pointer_Ops': 'Maximum Delete Pointer Operations'
        }

        # Define metrics to plot for Height
        height_metrics = {
            'Avg_Insert_Height': 'Average Insert Height',
            'Max_Insert_Height': 'Maximum Insert Height',
            'Avg_Delete_Height': 'Average Delete Height',
            'Max_Delete_Height': 'Maximum Delete Height'
        }

        # --- Generate separate plots for Insert Metrics ---
        for col, title_label in insert_metrics.items():
            plt.figure(figsize=(10, 5))
            plt.title(f'{scenario} - {title_label}')
            plt.plot(scenario_df['N'], scenario_df[col], label=title_label, marker='o')
            plt.xlabel('N (Number of Elements)')
            plt.ylabel('Count')
            plt.legend()
            plt.grid(True)
            plt.tight_layout()
            plt.savefig(f'{scenario.replace(" ", "_")}_{col}.png')
            plt.show()

        # --- Generate separate plots for Delete Metrics ---
        for col, title_label in delete_metrics.items():
            plt.figure(figsize=(10, 5))
            plt.title(f'{scenario} - {title_label}')
            plt.plot(scenario_df['N'], scenario_df[col], label=title_label, marker='o')
            plt.xlabel('N (Number of Elements)')
            plt.ylabel('Count')
            plt.legend()
            plt.grid(True)
            plt.tight_layout()
            plt.savefig(f'{scenario.replace(" ", "_")}_{col}.png')
            plt.show()

        # --- Generate separate plots for Height Metrics ---
        for col, title_label in height_metrics.items():
            plt.figure(figsize=(10, 5))
            plt.title(f'{scenario} - {title_label}')
            plt.plot(scenario_df['N'], scenario_df[col], label=title_label, marker='o')
            plt.xlabel('N (Number of Elements)')
            plt.ylabel('Height')
            plt.legend()
            plt.grid(True)
            plt.tight_layout()
            plt.savefig(f'{scenario.replace(" ", "_")}_{col}.png')
            plt.show()

# Example usage:
plot_experiment_results_separate("experiment_results.csv")