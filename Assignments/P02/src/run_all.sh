#!/bin/bash

set -e

# Remove old outputs and ensure clean JSON results
rm -f results_*_workload_*.json 2>/dev/null || true
rm -rf results
mkdir -p results

# List of data structures
structures=("bst" "ht" "ll" "sas")

# Corresponding driver executables
drivers=("driver_bst" "driver_ht" "driver_ll" "driver_sas")

# Workload types
workloads=("A" "B" "C" "D")

# Sizes
sizes=("1000" "5000" "10000" "20000")

# Loop over each structure
for i in "${!structures[@]}"; do
    struct="${structures[$i]}"
    driver="${drivers[$i]}"
    
    # Loop over each workload
    for workload in "${workloads[@]}"; do
        # Loop over each size
        for size in "${sizes[@]}"; do
            workload_file="work_files/workload_${workload}_${size}.json"
            output_file="results/results_${struct}_${workload}_${size}.json"
            
            echo "Running $driver with $workload_file -> $output_file"
            ./$driver "$workload_file" >/dev/null
            if [ -f "results_${struct}_${workload}_${size}.json" ]; then
                mv "results_${struct}_${workload}_${size}.json" "$output_file"
            fi
        done
    done
done

echo "All runs completed. JSON files in results/"