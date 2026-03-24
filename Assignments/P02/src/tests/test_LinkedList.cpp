#include "linkedList.hpp"
#include <iostream>

int main() {
    LinkedList list;

    list.insert(10);
    list.insert(20);
    list.insert(30);

    std::cout << std::boolalpha;

    std::cout << "contains 20: " << list.contains(20) << "\n";
    std::cout << "contains 99: " << list.contains(99) << "\n";

    list.erase(20);

    std::cout << "contains 20: " << list.contains(20) << "\n";

    list.print();
}