import pandas as pd
import matplotlib.pyplot as plt
import os

def compare_experiments(csv_file):
    """
    Reads a CSV, identifies numerical columns, compares their statistics 
    between experiments, generates a comparative bar chart, and saves it as an image.
    """
    if not os.path.exists(csv_file):
        print(f"Error: File not found at {csv_file}")
        return

    try:
        df = pd.read_csv(csv_file)
    except Exception as e:
        print(f"Error reading CSV file: {e}")
        return

    # Identify all numerical columns in the DataFrame
    numerical_cols = df.select_dtypes(include=['number']).columns.tolist()
    
    if not numerical_cols:
        print("No numerical columns found in the CSV to compare.")
        return

    # For simplicity, we will compare the first three numerical columns found
    # Adjust this if you only want to compare specific parameters.
    parameters_to_compare = numerical_cols[:3] 
    
    if len(parameters_to_compare) < 2:
        print("Need at least two numerical columns to perform a comparison.")
        return

    print(f"Comparing the following parameters: {parameters_to_compare}")

    # --- Data Preparation ---
    # Calculate the mean and standard deviation for the selected parameters
    comparison_data = df[parameters_to_compare].describe().loc[['mean', 'std']]
    
    # Prepare data for plotting (using mean as a representative value)
    plot_df = comparison_data.T.reset_index()
    plot_df.columns = ['Parameter', 'Mean Value', 'Std Dev']

    # --- Diagram Generation ---
    plt.figure(figsize=(10, 6))
    
    bar_width = 0.35
    index = range(len(plot_df))

    # Plotting the mean values
    plt.bar([i - bar_width/2 for i in index], plot_df['Mean Value'], bar_width, label='Media')

    # Adding error bars (using standard deviation)
    plt.errorbar(
        [i - bar_width/2 for i in index], 
        plot_df['Mean Value'], 
        yerr=plot_df['Std Dev'], 
        fmt='none', 
        capsize=5, 
        color='black'
    )

    plt.xlabel('Parametro Numerico')
    plt.ylabel('Valore')
    plt.title('Confronto dei Parametri Numerici tra Esperimenti')
    plt.xticks(index, plot_df['Parameter'])
    plt.legend()
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    plt.tight_layout()

    # --- Salvataggio del Diagramma ---
    output_filename = 'parameter_comparison_diagram.png'
    plt.savefig(output_filename)
    print(f"Diagramma generato e salvato in {output_filename}")


    if __name__ == "__main__":
        csv_file_path = '/data/esperimenti.csv'
        compare_experiments(csv_file_path)