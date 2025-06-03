import pandas as pd
import matplotlib.pyplot as plt
import os

def plot_splay_tree_results_aggregated(file_path="experiment_results.txt"):
    """
    Loads Splay Tree experiment results from a single TXT file, aggregates
    duplicate N entries (averaging averages, taking max of maxes), and
    generates separate plots for each metric vs N.
    """
    if not os.path.exists(file_path):
        print(f"Error: The file '{file_path}' was not found.")
        return

    # Define column names based on your printf format, explicitly naming them as 'Total' or 'Max'
    column_names = [
        'Scenario_Type', 'Operation_Type', 'N',
        'Total_Comparisons', 'Max_Comparisons',  # max_c from C
        'Total_Reads', 'Max_Reads',            # total_r, max_r from C
        'Total_Writes', 'Max_Writes',          # total_w, max_w from C
        'Total_Height', 'Max_Height'           # total_h, max_h from C
    ]

    print(f"Loading data from: {file_path}")
    # Read the text file into a DataFrame, using spaces as delimiters
    df = pd.read_csv(file_path, sep=r'\s+', names=column_names, header=None)

    # --- Initial Data Preprocessing ---
    # Combine 'Scenario_Type' and 'Operation_Type' into a single 'Full_Operation'
    df['Full_Operation'] = df['Scenario_Type'] + ' ' + df['Operation_Type']

    # Calculate Average values by dividing Total values by N
    # Handle potential division by zero for N if N can be 0 in your data
    # (though for N=0, these values typically wouldn't be logged)
    df['Avg_Comparisons'] = df['Total_Comparisons'] / df['N']
    df['Avg_Reads'] = df['Total_Reads'] / df['N']
    df['Avg_Writes'] = df['Total_Writes'] / df['N']
    df['Avg_Height'] = df['Total_Height'] / df['N']

    # Combine Reads and Writes into Pointer Operations (both average and max)
    df['Avg_Pointer_Ops'] = df['Avg_Reads'] + df['Avg_Writes']
    df['Max_Pointer_Ops'] = df['Max_Reads'] + df['Max_Writes']

    # --- Aggregation Step ---
    # Group by Full_Operation and N, then aggregate the metrics
    aggregated_df = df.groupby(['Full_Operation', 'N']).agg(
        # For average-like metrics, take the mean of the calculated averages
        Avg_Comparisons=('Avg_Comparisons', 'mean'),
        Avg_Pointer_Ops=('Avg_Pointer_Ops', 'mean'),
        Avg_Height=('Avg_Height', 'mean'),
        # For max-like metrics, take the maximum of the observed maxes
        Max_Comparisons=('Max_Comparisons', 'max'),
        Max_Pointer_Ops=('Max_Pointer_Ops', 'max'),
        Max_Height=('Max_Height', 'max')
    ).reset_index() # Reset index to turn 'Full_Operation' and 'N' back into columns

    print(f"Aggregated DataFrame head:\n{aggregated_df.head()}")

    # Identify unique full operations (e.g., 'asc insert', 'rand delete')
    full_operations = aggregated_df['Full_Operation'].unique()

    # Define metrics to plot, now explicitly with 'Avg_' and 'Max_'
    metrics_to_plot = {
        'Avg_Comparisons': {'label': 'Average Comparisons (per element)', 'ylabel': 'Count'},
        'Max_Comparisons': {'label': 'Maximum Comparisons (single op)', 'ylabel': 'Count'},
        'Avg_Pointer_Ops': {'label': 'Average Pointer Operations (per element)', 'ylabel': 'Count'},
        'Max_Pointer_Ops': {'label': 'Maximum Pointer Operations (single op)', 'ylabel': 'Count'},
        'Avg_Height': {'label': 'Average Height (per element)', 'ylabel': 'Height'},
        'Max_Height': {'label': 'Maximum Height (single op)', 'ylabel': 'Height'}
    }

    # --- Generate separate plots for each Full Operation and Metric ---
    for full_op in full_operations:
        # Filter data for the current full operation (e.g., 'asc insert')
        # This dataframe now has only one row per N due to aggregation
        op_df = aggregated_df[aggregated_df['Full_Operation'] == full_op].sort_values(by='N')
        print(f"Generating plots for: {full_op}")

        for col, props in metrics_to_plot.items():
            plt.figure(figsize=(10, 5))
            plt.title(f'{full_op} - {props["label"]}')
            plt.plot(op_df['N'], op_df[col], label=props['label'], marker='o')
            plt.xlabel('N (Number of Elements)')
            plt.ylabel(props['ylabel'])
            plt.legend()
            plt.grid(True)
            plt.tight_layout()

            # Sanitize filename for saving (replace spaces and problematic chars with underscores)
            clean_filename = f'{full_op.replace(" ", "_").replace("/", "_")}_{col}.png'
            plt.savefig(clean_filename)
            plt.show()

# Example usage:
# Assuming your data is in a single file named "experiment_results.txt"
plot_splay_tree_results_aggregated("experiment_results.txt")