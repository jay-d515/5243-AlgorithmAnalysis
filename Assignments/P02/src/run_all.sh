#!/bin/bash

set -e

set -e

# Create timestamped results directory
TIMESTAMP=$(date +%Y-%m-%d_%H-%M-%S)
RESULTS_DIR="results/run_${TIMESTAMP}"
mkdir -p "$RESULTS_DIR"

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
            echo "Running $driver with $workload_file -> $RESULTS_DIR"
            ./$driver "$workload_file" "$RESULTS_DIR" >/dev/null
        done
    done
done

echo "All runs completed. JSON files in $RESULTS_DIR/"
echo "Run timestamp: $TIMESTAMP"