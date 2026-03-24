#include "avl.hpp"
#include <iostream>

int main() {
    Avl tree;

    // tree.insert(30);
    // tree.insert(20);
    // tree.insert(10); // triggers rotation

    // tree.insert(40);
    // tree.insert(50); // triggers rotation

    for (int i = 0; i < 15; i++) {
        tree.insert(i);
    }

    std::cout << std::boolalpha;
    std::cout << "contains 20: " << tree.contains(20) << "\n";
    std::cout << "contains 99: " << tree.contains(99) << "\n";

    tree.erase(20);
    std::cout << "contains 20 after erase: " << tree.contains(20) << "\n";

    std::cout << "height: " << tree.height() << std::endl;

    return 0;
}