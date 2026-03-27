// driver_ll.cpp
// Driver program for testing the LinkedList implementation with a workload file.
#include "linkedList.hpp"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Usage: ./driver_ll <workload_file.json> [output_dir]" << endl;
        return 1;
    }
    // 
    string output_dir = "results";
    if (argc >= 3) {
        output_dir = argv[2];
        if (!output_dir.empty() && (output_dir.back() == '/' || output_dir.back() == '\\')) {
            output_dir.pop_back();
        }
    }

    LinkedList ll;
    ll.runJobFile(argv[1]);

    cout << ll.getCounters() << endl;

    // Extract workload name from filename (e.g., "A_1000" from "workload_A_1000.json")
    string fullpath = argv[1];
    string filename = fullpath.substr(fullpath.find_last_of('/') + 1);
    filename = filename.substr(9);
    size_t dot = filename.find_last_of('.');
    if (dot != string::npos) {
        filename = filename.substr(0, dot);
    }

    string outpath = output_dir + "/results_ll_" + filename + ".json";
    ll.save(outpath, true);

    return 0;
}
