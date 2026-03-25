#pragma once

#include <cstddef>
#include <iostream>
#include "counters.hpp"
#include "json.hpp"
#include <fstream>
#include <string>

using json = nlohmann::json;
using namespace std;

class SortedArraySet {
private:
    int *data;
    std::size_t count;
    std::size_t capacity;
    mutable Counters c{};

    void resize(std::size_t newCapacity) {
        c.resize_events++;  // Track resize
        c.structural_ops++;  // Track memory operation
        int *newData = new int[newCapacity];

        for (std::size_t i = 0; i < count; i++) {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

    std::size_t lowerBound(int value) const {
        std::size_t left = 0;
        std::size_t right = count;

        while (left < right) {
            c.comparisons++;  // Count each binary search comparison
            std::size_t mid = left + (right - left) / 2;

            if (data[mid] < value) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }

        return left;
    }

public:
    explicit SortedArraySet(std::size_t initialCapacity = 8)
        : data(new int[initialCapacity]), count(0), capacity(initialCapacity) {}

    ~SortedArraySet() {
        delete[] data;
    }

    std::size_t size() const {
        return count;
    }

    bool empty() const {
        return count == 0;
    }

    void reset() {
        c = {};
    }

    Counters getCounters() const {
        return c;
    }

    void save(std::string filename, bool dict = true) {
        c.saveCounters(filename, dict);
    }
    

    // Process JSON workload file
    void runJobFile(std::string fname) {
        std::ifstream f(fname);
        json j = json::parse(f);

        for (auto &element : j) {
            std::string op = element["op"];
            if (op == "insert") {
                insert(element["value"]);
            } else if (op == "contains") {
                contains(element["value"]);
            } else if (op == "delete") {
                erase(element["value"]);
            }
        }
    }

    bool contains(int value) const {
        c.lookups++;  // Track lookup
        if (count == 0) {
            return false;
        }

        std::size_t pos = lowerBound(value);
        c.comparisons++;  // Final check
        return pos < count && data[pos] == value;
    }

    bool insert(int value) {
        c.inserts++;  // Track insert attempt
        std::size_t pos = lowerBound(value);

        if (pos < count && data[pos] == value) {
            c.comparisons++;  // Duplicate check
            return false;
        }

        if (count == capacity) {
            resize(capacity * 2);
        }

        // Shift elements
        for (std::size_t i = count; i > pos; i--) {
            c.structural_ops++;  // Count each element move
        }

        data[pos] = value;
        count++;

        return true;
    }

    bool erase(int value) {
        c.deletes++;  // Track delete attempt
        if (count == 0) {
            return false;
        }

        std::size_t pos = lowerBound(value);
        c.comparisons++;  // Value check

        if (pos >= count || data[pos] != value) {
            return false;
        }

        // Shift elements left
        for (std::size_t i = pos; i + 1 < count; i++) {
            c.structural_ops++;  // Count each element move
        }

        count--;
        return true;
    }

    void print() const {
        std::cout << "[";
        for (std::size_t i = 0; i < count; i++) {
            std::cout << data[i];
            if (i + 1 < count) {
                std::cout << ", ";
            }
        }
        std::cout << "]\n";
    }
};