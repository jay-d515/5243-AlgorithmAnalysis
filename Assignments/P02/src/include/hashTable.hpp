#pragma once

#include <cstddef>
#include <vector>
#include "counters.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <string>

using json = nlohmann::json;
using namespace std;

class HashTable {
private:
    std::vector<std::vector<int>> table;
    std::size_t capacity;
    mutable Counters c{};

    // Integer hash function
    std::size_t hash(int key) const {
        return static_cast<std::size_t>(key) * 2654435761u;
    }

    // Compress hash value into a valid bucket index
    std::size_t indexFor(int key) const {
        return hash(key) % capacity;
    }

public:
    explicit HashTable(std::size_t cap = 101)
        : table(cap), capacity(cap) {}

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

    bool insert(int key) {
        // track each insert attempt
        c.inserts++;
        std::size_t idx = indexFor(key);
        auto &bucket = table[idx];

        // Ignore duplicates
        for (int value : bucket) {
            // count each comparison
            c.comparisons++;
            if (value == key) {
                return false;
            }
        }

        // track memory operations
        c.structural_ops++;
        bucket.push_back(key);
        return true;
    }

    bool contains(int key) const {
        // track each lookup attempt
        c.lookups++;
        std::size_t idx = indexFor(key);
        const auto &bucket = table[idx];

        for (int value : bucket) {
            // count each comparison
            c.comparisons++;
            if (value == key) {
                return true;
            }
        }

        return false;
    }

    bool erase(int key) {
        // track each delete attempt
        c.deletes++;
        std::size_t idx = indexFor(key);
        auto &bucket = table[idx];

        for (std::size_t i = 0; i < bucket.size(); i++) {
            // count each comparison
            c.comparisons++;
            if (bucket[i] == key) {
                // track memory operations
                c.structural_ops++;
                // swap-pop delete: fast, order not preserved
                bucket[i] = bucket.back();
                bucket.pop_back();
                return true;
            }
        }
        return false;
    }
};