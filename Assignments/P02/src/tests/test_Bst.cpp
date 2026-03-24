#include "bst.hpp"
#include <iostream>

int main() {
    Bst tree;

    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    std::cout << std::boolalpha;
    std::cout << "contains 40: " << tree.contains(40) << "\n";
    std::cout << "contains 99: " << tree.contains(99) << "\n";

    tree.erase(20); // leaf
    tree.erase(30); // one child
    tree.erase(50); // two children

    std::cout << "contains 50: " << tree.contains(50) << "\n";
}