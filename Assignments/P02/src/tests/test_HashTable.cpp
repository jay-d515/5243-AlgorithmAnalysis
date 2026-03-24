#include "hashTable.hpp"
#include <iostream>

int main() {
    HashTable ht(17);

    ht.insert(10);
    ht.insert(20);
    ht.insert(30);
    ht.insert(27);

    std::cout << std::boolalpha;
    std::cout << "contains 20: " << ht.contains(20) << "\n";
    std::cout << "contains 99: " << ht.contains(99) << "\n";

    ht.erase(20);

    std::cout << "contains 20: " << ht.contains(20) << "\n";
}