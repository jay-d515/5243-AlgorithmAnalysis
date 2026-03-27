#include "sortedArraySet.hpp"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Usage: ./driver_sas work_files/workload_A_1000.json" << endl;
        return 1;
    }

    SortedArraySet arr;
    arr.runJobFile(argv[1]);
    
    cout << arr.getCounters() << endl;
    
    // Extract workload name from filename (e.g., "A_1000" from "workload_A_1000.json")
        string output_dir = "results";
        if (argc >= 3) {
            output_dir = argv[2];
            if (!output_dir.empty() && (output_dir.back() == '/' || output_dir.back() == '\\')) {
                output_dir.pop_back();
            }
        }

        string fullpath = argv[1];
        string filename = fullpath.substr(fullpath.find_last_of('/') + 1);
        filename = filename.substr(9); // remove "workload_"
        size_t dot = filename.find_last_of('.');
        if (dot != string::npos) {
            filename = filename.substr(0, dot); // remove ".json"
        }

        string outpath = output_dir + "/results_sas_" + filename + ".json";
        arr.save(outpath, true);
    
    return 0;
}
