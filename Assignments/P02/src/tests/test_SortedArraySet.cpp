#include "sortedArraySet.hpp"
#include <iostream>

int main() {
    SortedArraySet s;

    std::cout << std::boolalpha;

    std::cout << "Insert 40: " << s.insert(40) << "\n";
    std::cout << "Insert 10: " << s.insert(10) << "\n";
    std::cout << "Insert 30: " << s.insert(30) << "\n";
    std::cout << "Insert 20: " << s.insert(20) << "\n";
    std::cout << "Insert 20 again: " << s.insert(20) << "\n";

    std::cout << "Current contents: ";
    s.print();

    std::cout << "Contains 30: " << s.contains(30) << "\n";
    std::cout << "Contains 99: " << s.contains(99) << "\n";

    std::cout << "Erase 30: " << s.erase(30) << "\n";
    std::cout << "After erasing 30: ";
    s.print();

    std::cout << "Erase 99: " << s.erase(99) << "\n";
    std::cout << "After trying to erase 99: ";
    s.print();

    std::cout << "Size: " << s.size() << "\n";

    return 0;
}