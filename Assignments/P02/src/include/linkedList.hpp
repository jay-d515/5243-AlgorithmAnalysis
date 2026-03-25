#pragma once

#include <iostream>
#include "counters.hpp"
#include "json.hpp"
#include <fstream>
#include <string>

using json = nlohmann::json;
using namespace std;

class LinkedList {
private:
    struct Node {
        int data;
        Node *next;

        Node(int v) : data(v), next(nullptr) {}
    };

    Node *head;
    mutable Counters c{};

public:
    LinkedList() : head(nullptr) {}

    ~LinkedList() {
        Node *curr = head;

        while (curr) {
            Node *temp = curr;
            curr = curr->next;
            c.structural_ops++;  // Track each deletion
            delete temp;
        }
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

    bool insert(int value) {
        c.inserts++;  // Track insert attempt

        if (contains(value))
            return false;

        c.structural_ops++;  // Track node allocation
        Node *n = new Node(value);

        n->next = head;
        head = n;

        return true;
    }

    bool contains(int value) const {
        c.lookups++;  // Track lookup attempt
        Node *curr = head;

        while (curr) {
            c.comparisons++;  // Count each comparison
            if (curr->data == value)
                return true;

            curr = curr->next;
        }

        return false;
    }

    bool erase(int value) {
        c.deletes++;  // Track delete attempt
        Node *curr = head;
        Node *prev = nullptr;

        while (curr) {
            c.comparisons++;  // Count each comparison
            if (curr->data == value) {

                if (prev)
                    prev->next = curr->next;
                else
                    head = curr->next;

                c.structural_ops++;  // Track deletion
                delete curr;
                return true;
            }

            prev = curr;
            curr = curr->next;
        }

        return false;
    }

    void print() const {
        Node *curr = head;

        while (curr) {
            std::cout << curr->data << " ";
            curr = curr->next;
        }

        std::cout << "\n";
    }
};