## P02 - Comparing Lookup Type Data Structures Part 1
#### Jadyn Dangerfield

## Overview:
In this program, I added Counter code to each data structure so I could save teh counts for a single run. Next, I generated each workload and ran each data structure with each workload saving the results to JSON files. Finally, I created heatmaps and bargraphs using my results in order to perform and analysis.

## Files
#### Only including files relevant to this part of the assignment

|   #   | File                            | Description                                                                                                                       |
| :---: | ------------------------------- | --------------------------------------------------------------------------------------------------------------------------------- |
|   1   | `bst.hpp`                       | A binary search tree implementation with counters for operations and comparisons.                                                 |
|   2   | `hashTable.hpp`                 | A hash table implementation with counters for operations and comparisons.                                                         |
|   3   | `linkedList.hpp`                | A linked list implementation with counters for operations and comparisons.                                                        |
|   4   | `sortedArraySet.hpp`            | A sorted array set implementation with counters for operations and comparisons.                                                   |
|   5   | `counters.hpp`                  | A struct to hold various operation counts for data structures, with JSON serialization support.                                   |
|   6   | `workload_generator_v2.cpp`     | A standalone executable to generate workloads for testing.                                                                        |
|   7   | `drive_load_bst.cpp`            | A driver for testing binary search tree implementation with a given workload.                                                     |
|   8   | `drive_load_HashTable.cpp`      | A driver for testing hash table implementation with a given workload.                                                             |
|   9   | `drive_load_LinkedList.cpp`     | A driver for testing linked list implementation with a given workload.                                                            |
|   10  | `drive_load_SortedArraySet.cpp` | A driver for testing sorted array implementation with a given workload.                                                           |
|   11  | `generate_all_workloads.sh`     | Generates all workloads for the experiments and saves them to the `work_files` directory.                                          |
|   12  | `run_all.sh`                    | Runs all the drivers with all the workloads and sizes, and save the results in a timestamped directory.                           |
|   13  | `combine_results.py`            | Combines the results from all the runs into a single DataFrame, and then create various charts and tables to compare performance. |

## Changes
#### bst.hpp, hashTable.hpp, linkedList.hpp, sortedArraySet.hpp
- added Counter code to each data structure to I could save the counts for each run.
- defined a function in each file called `runJobFile` that reads in a workload JSON file and replays every operation in order.

#### drive_load_HashTable.cpp, drive_load_LinkedList.cpp, drive_load_SortedArraySet.cpp
- created drivers for data structures: Hash Table, Linked List, and Sorted Array Set.

#### generate_all_workloads.sh
- created a script that generates all workloads needed for this assignment.

#### run_all.sh
- created a script that runs each data structure with each workload, and saves the results to JSON files.

#### combine_results.py 
- Created a python that combines the results from all the runs into a single DataFrame, and then create various charts and tables to compare performance.
## Instructions
### 1. Compile all drivers and workload generator
from the `src/` directory run:
- `g++ -std=c++20 -Iinclude drivers/drive_load_Bst.cpp -o drivers/driver_bst`
- `g++ -std=c++20 -Iinclude drivers/drive_load_HashTable.cpp -o drivers/driver_ht`
- `g++ -std=c++20 -Iinclude drivers/drive_load_LinkedList.cpp -o drivers/driver_ll`
- `g++ -std=c++20 -Iinclude drivers/drive_load_SortedArraySet.cpp -o drivers/driver_sas`
- `g++ -std=c++20 -Iinclude workload_generator_v2.cpp -o v2`

### 3. Generate workloads
If you don't already have the workload files in *work_files/*, generate them:
- `./generate_all_workloads.sh`

### 4. Run all benchmarks
This will:
- Create a timestamped results folder (e.g., *results/run_2026-03-27_14-30-45/*)
- Run al drivers on all workloads
- Save all output JSONs in the new results folder
- `./run_all.sh`

### 5. Perform an analysis on the data
This will create heatmaps and bar graphs based on the results of the benchmarks, by running:
- `python3 combine_results.py`

### 6. Where to find results and analysis
- All output JSON files will be in a new subfolder under *results/* for each run.
- Each file is named like:
  `results_bst_A_1000.json`. etc.

## Analysis
### Where Each Structure Exelled/Failed
Sorted Array Set excelled most on comparison overall.
- Workload A: 234,881.75 (best)
- Workload B: 236,514.25 (best)
- Workload C: 198,992.50 (2nd)
- Workload D: 750,245.75 (best)
However, the Sorted Array Set failed on structural work in multiple workloads.
- Structural ops in Workload A: 4,870,987.50 (vs. 8,582.25 for BST/HT/LL)
- Structural ops in Workload C: 1,324,880.00 (vs. 8,937.75 for BST/HT/LL)
- Structural ops in Workload D: 4,870,987.50 (vs. 8,582.25 for BST/HT/LL)

Hash Table excelled hardest in workload C.
- Workload C comparisons: 12,052.25 (best by large margin)
Hash Table was weaker than Sorted Array Set in workloads A, B, and D comparisons.
- Workload A: 1,231,059.25
- Workload B: 1,248,675.25
- Workload D: 5,959,727.50

Binary Search Tree excelled as a middle ground in workloads A and D compared to Linked List.
- Workload A: 786,087.50 (better than LL's 125,427,961.50)
- Workload D: 2,684,252.75 (better than LL's 603,019,252.25)
Binary Search Tree failed badly on workload N
- Workload B comparisons: 376,099,311.00 (much worse than HT/SAS, though still better than LL)

Linked List failed in every workload on comparison count.
- Workload A: 125,427,961.50
- Workload B: 122,576,475.50
- Workload C: 589,226.75
- Workload D: 603,019,252.25
Linked List also showed higher lookup counts than others.
- Workload A lookup mean: 18,000.00 (others 9,000.00)
- Workload D lookup mean: 54,000.00 (others 45,000.00)

Workload Winners (Comparisons, lower is better)
- Workload A: Sorted Array Set (234,881.75)
- Workload B: Sorted Array Set (236,514.25)
- Workload C: Hash Table (12,052,25)
- Workload D: Sorted Array Set (750,245.75)

Tradeoffs
- Sorted Array Set: very low comparisons, but potentially huge structural maintenance cost depending on workload pattern.
- Hash Table: strong and stable comparisons, especially best on C; not always the best on workloads A, B, and D.
- Binary Search Tree: can be good when shape stays favorable, but can degrade drastically (workload B indicates a likely imbalance/adversarial order effect).
- Linked List: simple structure, but poor scaling for search-heavy workloads; consistently the highest comparison cost.
