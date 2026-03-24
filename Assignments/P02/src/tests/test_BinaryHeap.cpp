#include "binaryHeap.hpp"
#include <iostream>

int main() {
    BinaryHeap h;

    h.push(10);
    h.push(4);
    h.push(15);
    h.push(2);

    std::cout << "top: " << h.top() << "\n";



    h.pop();
    std::cout << "top after pop: " << h.top() << "\n";

    std::vector<int> vals = {9, 7, 3, 12, 1, 8};
    BinaryHeap h2(vals);

    std::cout << "bulk-built top: " << h2.top() << "\n";
}
