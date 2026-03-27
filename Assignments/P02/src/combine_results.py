import json
import os
from datetime import datetime
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from pathlib import Path

# Set up the plotting style
sns.set_style("whitegrid")
plt.rcParams['figure.figsize'] = (12, 8)

def load_results(results_dir):
    """Load all result JSON files and combine into a DataFrame."""
    data = []

    for file_path in Path(results_dir).glob("results_*.json"):
        filename = file_path.name
        # Parse filename: results_{ds}_{workload}_{size}.json
        parts = filename.replace('results_', '').replace('.json', '').split('_')
        ds, workload, size = parts[0], parts[1], int(parts[2])

        with open(file_path, 'r') as f:
            metrics = json.load(f)

        row = {
            'data_structure': ds,
            'workload': workload,
            'size': size,
            **metrics
        }
        data.append(row)

    return pd.DataFrame(data)


def find_latest_results_dir(base_results_dir='results'):
    """Return the newest timestamped run directory, or the base directory if none exist."""
    base_path = Path(base_results_dir)
    run_dirs = [p for p in base_path.glob('run_*') if p.is_dir()]

    if not run_dirs:
        return base_path

    # Directory names use run_YYYY-MM-DD_HH-MM-SS, so lexical max is the newest run.
    return max(run_dirs, key=lambda p: p.name)

def create_comparison_charts(df, output_dir, file_timestamp):
    """Create various comparison charts."""

    # Map data structures to readable names
    ds_names = {
        'bst': 'Binary Search Tree',
        'ht': 'Hash Table',
        'll': 'Linked List',
        'sas': 'Sorted Array Set'
    }
    df['data_structure_name'] = df['data_structure'].map(ds_names)

    # Metrics to plot
    metrics = ['comparisons', 'inserts', 'lookups', 'deletes', 'structural_ops']

    # 1. Bar chart: Average performance by data structure and workload
    fig, axes = plt.subplots(2, 3, figsize=(18, 12))
    axes = axes.flatten()

    for i, metric in enumerate(metrics):
        if i < len(axes):
            ax = axes[i]
            avg_data = df.groupby(['data_structure_name', 'workload'])[metric].mean().reset_index()
            sns.barplot(data=avg_data, x='workload', y=metric, hue='data_structure_name', ax=ax)
            ax.set_title(f'Average {metric.title()} by Workload')
            ax.set_ylabel(metric.title())
            ax.legend(title='Data Structure', bbox_to_anchor=(1.05, 1), loc='upper left')

    # Remove empty subplot
    if len(metrics) < len(axes):
        fig.delaxes(axes[-1])

    plt.tight_layout()
    plt.savefig(output_dir / f'performance_comparison_{file_timestamp}.png', dpi=300, bbox_inches='tight')
    plt.show()

    # 2. Line chart: Performance scaling with size
    fig, axes = plt.subplots(2, 3, figsize=(18, 12))
    axes = axes.flatten()

    for i, metric in enumerate(metrics):
        if i < len(axes):
            ax = axes[i]
            sns.lineplot(data=df, x='size', y=metric, hue='data_structure_name',
                        style='workload', markers=True, ax=ax)
            ax.set_title(f'{metric.title()} vs Size')
            ax.set_ylabel(metric.title())
            ax.set_xlabel('Size')
            ax.legend(title='Data Structure/Workload', bbox_to_anchor=(1.05, 1), loc='upper left')

    if len(metrics) < len(axes):
        fig.delaxes(axes[-1])

    plt.tight_layout()
    plt.savefig(output_dir / f'scaling_comparison_{file_timestamp}.png', dpi=300, bbox_inches='tight')
    plt.show()

    # 3. Heatmap: Performance matrix
    for metric in metrics:
        pivot_data = df.pivot_table(values=metric, index='data_structure_name',
                                   columns=['workload', 'size'], aggfunc='mean')

        plt.figure(figsize=(12, 6))
        sns.heatmap(pivot_data, annot=True, fmt='.0f', cmap='YlOrRd')
        plt.title(f'{metric.title()} Heatmap (Data Structure vs Workload/Size)')
        plt.ylabel('Data Structure')
        plt.xlabel('Workload and Size')
        plt.tight_layout()
        plt.savefig(output_dir / f'{metric}_heatmap_{file_timestamp}.png', dpi=300, bbox_inches='tight')
        plt.show()

def create_summary_table(df, output_dir):
    """Create a summary table of results. """
    summary = df.groupby(['data_structure', 'workload']).agg({
        'comparisons': ['mean', 'std'],
        'inserts': ['mean', 'std'],
        'lookups': ['mean', 'std'],
        'deletes': ['mean', 'std'],
        'structural_ops': ['mean', 'std']
    }).round(2)

    # Save to CSV
    summary.to_csv(output_dir / 'results_summary.csv')

    # Create a simpler JSON structure
    summary_dict = {}
    for (ds, workload), group in df.groupby(['data_structure', 'workload']):
        if ds not in summary_dict:
            summary_dict[ds] = {}
        summary_dict[ds][workload] = {
            'comparisons_mean': float(group['comparisons'].mean()),
            'comparisons_std': float(group['comparisons'].std()),
            'inserts_mean': float(group['inserts'].mean()),
            'inserts_std': float(group['inserts'].std()),
            'lookups_mean': float(group['lookups'].mean()),
            'lookups_std': float(group['lookups'].std()),
            'deletes_mean': float(group['deletes'].mean()),
            'deletes_std': float(group['deletes'].std()),
            'structural_ops_mean': float(group['structural_ops'].mean()),
            'structural_ops_std': float(group['structural_ops'].std()),
        }

    with open(output_dir / 'results_summary.json', 'w') as f:
        json.dump(summary_dict, f, indent=2)

    return summary

def main():
    results_dir = 'results'
    if not os.path.exists(results_dir):
        print(f"Results directory '{results_dir}' not found!")
        return

    analysis_dir = find_latest_results_dir(results_dir)
    file_timestamp = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
    print(f"Using results from: {analysis_dir}")

    print("Loading results...")
    df = load_results(analysis_dir)
    if df.empty:
        print(f"No result files found in '{analysis_dir}'")
        return
    print(f"Loaded {len(df)} result files")

    print("Creating summary table...")
    summary = create_summary_table(df, analysis_dir)
    print(f"Summary saved to {analysis_dir / 'results_summary.csv'} and {analysis_dir / 'results_summary.json'}")

    print("Creating charts...")
    create_comparison_charts(df, analysis_dir, file_timestamp)
    print("Charts saved as PNG files")

    print("\nAnalysis complete!")
    print("Generated files:")
    print("- results_summary.csv: Summary statistics")
    print("- results_summary.json: Summary in JSON format")
    print(f"- performance_comparison_{file_timestamp}.png: Bar charts comparing performance")
    print(f"- scaling_comparison_{file_timestamp}.png: Line charts showing scaling")
    print(f"- [metric]_heatmap_{file_timestamp}.png: Heatmaps for each metric")

if __name__ == "__main__":
    main()
