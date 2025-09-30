# scripts/generate_charts.py
# Python script to analyze stress test logs and generate performance comparison charts.

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os
import platform

# --- Configuration ---
# Log file path (relative to the script's directory, assuming this script is also in scripts/)
LOG_DIR = '../results/'
CHART_DIR = os.path.join(LOG_DIR, 'performance_charts')

# CSV filenames
INSERT_NO_INDEX_FILE = os.path.join(LOG_DIR, 'no_index_insert_log.csv')
QUERY_NO_INDEX_FILE = os.path.join(LOG_DIR, 'no_index_query_log.csv')
INSERT_WITH_INDEX_FILE = os.path.join(LOG_DIR, 'with_index_insert_log.csv')
QUERY_WITH_INDEX_FILE = os.path.join(LOG_DIR, 'with_index_query_log.csv')

# Ensure the chart output directory exists
os.makedirs(CHART_DIR, exist_ok=True)

# Set plot style
sns.set_theme(style="whitegrid")
plt.rcParams['axes.unicode_minus'] = False # Ensure minus sign displays correctly

def load_and_prepare_data(file_path, numeric_column='duration_ms', x_col_check=None):
    """Reads a CSV file and ensures the specified column is numeric."""
    try:
        df = pd.read_csv(file_path)
        # Explicitly convert the duration_ms column to numeric, coercing errors to NaN
        if numeric_column in df.columns:
            df[numeric_column] = pd.to_numeric(df[numeric_column], errors='coerce')
            df.dropna(subset=[numeric_column], inplace=True) # Drop rows where conversion failed
        else:
            print(f"Warning: Numeric column '{numeric_column}' not found in file '{file_path}'.")
            return pd.DataFrame()

        if x_col_check and x_col_check not in df.columns:
            print(f"Warning: X-axis column '{x_col_check}' not found in file '{file_path}'.")
            return pd.DataFrame()
        if x_col_check: # Also ensure x-axis column is numeric if it's for plotting
             df[x_col_check] = pd.to_numeric(df[x_col_check], errors='coerce')
             df.dropna(subset=[x_col_check], inplace=True)

        return df
    except FileNotFoundError:
        print(f"Error: Log file '{file_path}' not found. Please run the respective scripts to generate logs first.")
        return pd.DataFrame() 
    except Exception as e:
        print(f"Error reading or processing file '{file_path}': {e}")
        return pd.DataFrame()


def plot_insert_performance():
    """Plots insert performance comparison chart."""
    df_insert_no_index = load_and_prepare_data(INSERT_NO_INDEX_FILE, x_col_check='total_rows_after_insert_approx')
    df_insert_with_index = load_and_prepare_data(INSERT_WITH_INDEX_FILE, x_col_check='total_rows_after_insert_approx')

    if df_insert_no_index.empty and df_insert_with_index.empty:
        print("No data available to plot insert performance.")
        return

    plt.figure(figsize=(12, 7))

    if not df_insert_no_index.empty:
        avg_insert_no_index = df_insert_no_index.groupby('total_rows_after_insert_approx')['duration_ms'].mean().reset_index()
        plt.plot(avg_insert_no_index['total_rows_after_insert_approx'], avg_insert_no_index['duration_ms'],
                 label='Insert Time (Passenger.name No Index)', marker='.', linestyle='-', markersize=5)
        print(f"Insert No Index - Data points for plot (after grouping): {len(avg_insert_no_index)}")
        if len(avg_insert_no_index) < 5 and len(avg_insert_no_index) > 0:
            print("Sample data for Insert No Index (after grouping):")
            print(avg_insert_no_index.head())


    if not df_insert_with_index.empty:
        avg_insert_with_index = df_insert_with_index.groupby('total_rows_after_insert_approx')['duration_ms'].mean().reset_index()
        plt.plot(avg_insert_with_index['total_rows_after_insert_approx'], avg_insert_with_index['duration_ms'],
                 label='Insert Time (Passenger.name With Index)', marker='x', linestyle='--', markersize=5)
        print(f"Insert With Index - Data points for plot (after grouping): {len(avg_insert_with_index)}")
        if len(avg_insert_with_index) < 5 and len(avg_insert_with_index) > 0:
            print("Sample data for Insert With Index (after grouping):")
            print(avg_insert_with_index.head())


    plt.xlabel('Total Rows in Passenger Table (Approx.)')
    plt.ylabel('Average Batch Insert Duration (ms)')
    plt.title('Insert Performance Comparison: With Index vs. No Index (on Passenger.name)')
    plt.legend()
    plt.grid(True)
    plt.tight_layout() 
    
    chart_path = os.path.join(CHART_DIR, 'insert_performance_comparison.png')
    plt.savefig(chart_path)
    print(f"Insert performance comparison chart saved to: {chart_path}")
    plt.show()

def plot_query_performance():
    """Plots query performance comparison chart."""
    df_query_no_index = load_and_prepare_data(QUERY_NO_INDEX_FILE, x_col_check='current_table_rows_approx')
    df_query_with_index = load_and_prepare_data(QUERY_WITH_INDEX_FILE, x_col_check='current_table_rows_approx')

    if df_query_no_index.empty and df_query_with_index.empty:
        print("No data available to plot query performance.")
        return

    plt.figure(figsize=(12, 7))

    bin_size = 50000 
    min_points_for_coarse_line = 5 # Prefer coarse binned line if it has at least this many points
    min_points_for_fine_line = 3   # Prefer fine grained line if it has at least this many points

    # --- Plotting for "No Index" ---
    if not df_query_no_index.empty:
        print(f"\nQuery No Index - Original data points: {len(df_query_no_index)}")
        
        # Coarse binning
        df_query_no_index['rows_bin_coarse'] = (df_query_no_index['current_table_rows_approx'] // bin_size) * bin_size
        avg_query_no_index_coarse = df_query_no_index.groupby('rows_bin_coarse')['duration_ms'].mean().reset_index()
        print(f"Query No Index - Data points after grouping by bins of {bin_size}: {len(avg_query_no_index_coarse)}")
        if len(avg_query_no_index_coarse) > 0 and len(avg_query_no_index_coarse) < 5 :
             print("Sample data for Query No Index (after coarse grouping):")
             print(avg_query_no_index_coarse.head())

        # Fine-grained grouping
        avg_query_no_index_fine = df_query_no_index.sort_values('current_table_rows_approx').groupby('current_table_rows_approx')['duration_ms'].mean().reset_index()
        print(f"Query No Index - Data points after fine-grained grouping (by unique row count): {len(avg_query_no_index_fine)}")
        if len(avg_query_no_index_fine) > 0 and len(avg_query_no_index_fine) < 10 :
            print("Sample data for Query No Index (fine grained):")
            print(avg_query_no_index_fine.head())

        if len(avg_query_no_index_coarse) >= min_points_for_coarse_line:
            print(f"Query No Index: Plotting coarse binned line (binsize {bin_size}).")
            plt.plot(avg_query_no_index_coarse['rows_bin_coarse'], avg_query_no_index_coarse['duration_ms'],
                     label='Query Time (Passenger.name No Index)', marker='.', linestyle='-', markersize=5)
        elif len(avg_query_no_index_fine) >= min_points_for_fine_line:
            print(f"Query No Index: Coarse binning resulted in < {min_points_for_coarse_line} points. Plotting fine-grained line.")
            plt.plot(avg_query_no_index_fine['current_table_rows_approx'], avg_query_no_index_fine['duration_ms'],
                     label='Query Time (Passenger.name No Index - Fine Grained)', marker='.', linestyle='-', markersize=4, alpha=0.8)
        elif len(df_query_no_index) > 0 : # If even fine-grained is too sparse, plot raw data as scatter
            print(f"Query No Index: Fine-grained grouping resulted in < {min_points_for_fine_line} points. Plotting raw data as scatter.")
            plt.scatter(df_query_no_index['current_table_rows_approx'], df_query_no_index['duration_ms'],
                        label='Query Time (Passenger.name No Index - Raw Data)', marker='.', alpha=0.5, s=10)
        else:
            print("Query No Index: No data to plot.")

    # --- Plotting for "With Index" ---
    if not df_query_with_index.empty:
        print(f"\nQuery With Index - Original data points: {len(df_query_with_index)}")

        # Coarse binning
        df_query_with_index['rows_bin_coarse'] = (df_query_with_index['current_table_rows_approx'] // bin_size) * bin_size
        avg_query_with_index_coarse = df_query_with_index.groupby('rows_bin_coarse')['duration_ms'].mean().reset_index()
        print(f"Query With Index - Data points after grouping by bins of {bin_size}: {len(avg_query_with_index_coarse)}")
        if len(avg_query_with_index_coarse) > 0 and len(avg_query_with_index_coarse) < 5:
            print("Sample data for Query With Index (after coarse grouping):")
            print(avg_query_with_index_coarse.head())

        # Fine-grained grouping
        avg_query_with_index_fine = df_query_with_index.sort_values('current_table_rows_approx').groupby('current_table_rows_approx')['duration_ms'].mean().reset_index()
        print(f"Query With Index - Data points after fine-grained grouping (by unique row count): {len(avg_query_with_index_fine)}")
        if len(avg_query_with_index_fine) > 0 and len(avg_query_with_index_fine) < 10 :
            print("Sample data for Query With Index (fine grained):")
            print(avg_query_with_index_fine.head())

        if len(avg_query_with_index_coarse) >= min_points_for_coarse_line:
            print(f"Query With Index: Plotting coarse binned line (binsize {bin_size}).")
            plt.plot(avg_query_with_index_coarse['rows_bin_coarse'], avg_query_with_index_coarse['duration_ms'],
                     label='Query Time (Passenger.name With Index)', marker='x', linestyle='--', markersize=5)
        elif len(avg_query_with_index_fine) >= min_points_for_fine_line:
            print(f"Query With Index: Coarse binning resulted in < {min_points_for_coarse_line} points. Plotting fine-grained line.")
            plt.plot(avg_query_with_index_fine['current_table_rows_approx'], avg_query_with_index_fine['duration_ms'],
                     label='Query Time (Passenger.name With Index - Fine Grained)', marker='x', linestyle='--', markersize=4, alpha=0.8)
        elif len(df_query_with_index) > 0: # If even fine-grained is too sparse, plot raw data as scatter
            print(f"Query With Index: Fine-grained grouping resulted in < {min_points_for_fine_line} points. Plotting raw data as scatter.")
            plt.scatter(df_query_with_index['current_table_rows_approx'], df_query_with_index['duration_ms'],
                        label='Query Time (Passenger.name With Index - Raw Data)', marker='x', alpha=0.5, s=10)
        else:
            print("Query With Index: No data to plot.")


    plt.xlabel('Total Rows in Passenger Table (Approx.)')
    plt.ylabel('Average Query Duration (ms)')
    plt.title('Query Performance Comparison: With Index vs. No Index (on Passenger.name)')
    plt.legend()
    plt.grid(True)
    # plt.xlim(0.99999e6, 1.00001e6)
    plt.tight_layout()

    chart_path = os.path.join(CHART_DIR, 'query_performance_comparison.png')
    plt.savefig(chart_path)
    print(f"Query performance comparison chart saved to: {chart_path}")
    plt.show()

if __name__ == '__main__':
    print("Starting to generate performance comparison charts...")
    plot_insert_performance()
    plot_query_performance()
    print("Chart generation complete.")

