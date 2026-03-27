#pragma once

#include "counters.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <string>

using json = nlohmann::json;


using namespace std;

class Bst
{
protected:
    struct Node
    {
        int data;
        Node *left;
        Node *right;
        int height;

        explicit Node(int value)
            : data(value), left(nullptr), right(nullptr), height(1) {}
    };

    Node *root;
    mutable Counters c{};

    // Recursive insert helper
    bool insert(Node *&node, int value)
    {
        c.comparisons++;
        if (!node)
        {
            c.structural_ops++;
            node = new Node(value);
            return true;
        }

        c.comparisons++;
        if (value < node->data)
        {

            return insert(node->left, value);
        }
        
        c.comparisons++;
        if (value > node->data)
        {
            return insert(node->right, value);
        }

        // duplicate value: do nothing
        return false;
    }

    // Recursive search helper
    bool contains(Node *node, int value) const
    {

        c.comparisons++;
        if (!node)
        {

            return false;
        }

        c.comparisons++;
        if (value == node->data)
        {

            return true;
        }
        c.comparisons++;
        if (value < node->data)
        {

            return contains(node->left, value);
        }

        return contains(node->right, value);
    }

    // Find smallest node in subtree
    Node *findMin(Node *node) const
    {
        c.lookups++;
        while (node && node->left)
        {
            c.comparisons++;
            node = node->left;
        }
        return node;
    }

    // Recursive erase helper
    bool erase(Node *&node, int value)
    {

        c.comparisons++;
        if (!node)
        {
            return false;
        }

        c.comparisons++;
        if (value < node->data)
        {
            return erase(node->left, value);
        }

        c.comparisons++;
        if (value > node->data)
        {
            return erase(node->right, value);
        }

        // Found node to delete

        // Case 1: leaf node
        if (!node->left && !node->right)
        {
            c.structural_ops++;
            delete node;
            node = nullptr;
            return true;
        }

        // Case 2: only right child
        if (!node->left)
        {
            Node *temp = node;
            node = node->right;
            c.structural_ops++;
            delete temp;
            return true;
        }

        // Case 3: only left child
        if (!node->right)
        {
            Node *temp = node;
            node = node->left;
            c.structural_ops++;
            delete temp;
            return true;
        }

        // Case 4: two children
        Node *successor = findMin(node->right);
        node->data = successor->data;
        return erase(node->right, successor->data);
    }

    // Postorder cleanup helper
    void clear(Node *node)
    {
        c.comparisons++;
        if (!node)
        {
            return;
        }

        clear(node->left);
        clear(node->right);
        c.structural_ops++;
        delete node;
    }

public:
    Bst() : root(nullptr)
    {

    }

    void reset(){
         c = {}; 
    }
    void save(string filename,bool dict=true){
        c.saveCounters(filename,dict);
    }

    virtual ~Bst()
    {
        clear(root);
    }

    Counters getCounters(){
        return c;
    }

    void runJobFile(std::string fname)
    {
        std::ifstream f(fname);
        json j = json::parse(f);

        // iterate over json object and process each operation
        for (auto &element : j)
        {
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

    bool insert(int value)
    {
        // insert++
        c.inserts++;
        return insert(root, value);
        
    }

    bool contains(int value) const
    {
        c.lookups++;
        return contains(root, value);
    }

    bool erase(int value)
    {
        c.deletes++;
        return erase(root, value);
    }

    virtual const char *name() const
    {
        return "BST";
    }
};