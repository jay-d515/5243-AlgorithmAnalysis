WORKLOAD_DIR="work_files"
SIZES=(1000 5000 10000 20000)
WORKLOADS=(A B C D)

mkdir -p $WORKLOAD_DIR

echo "Generating workloads... to $WORKLOAD_DIR/"

for size in "${SIZES[@]}"; do
    for workload in "${WORKLOADS[@]}"; do
        filename="workload_${workload}_${size}.json"
        filepath="$WORKLOAD_DIR/$filename"
        echo "Generating $filename..."
        ./v2 --save "$filepath" -n "$size" -w "$workload"
    done
done

echo "Done! All workloads saved to $WORKLOAD_DIR/"