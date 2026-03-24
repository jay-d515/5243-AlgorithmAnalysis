# Lightweight Automatic Operation Counters
<sup>Author Chat GPT</sup>

Clean way to implement counters in each data structure. The basic idea is:

- put all counters in one struct
- make tiny helper methods/macros for incrementing them
- each data structure owns a `Counters` member
- expose `resetCounters()` and `getCounters()`

That way your benchmark harness can query each structure the same way.

---

# Step 1: Counters struct

Modern version:

```cpp
#pragma once

struct Counters {
    int comparisons{};
    int structural_ops{};
    int inserts{};
    int deletes{};
    int lookups{};
    int resize_events{};
};
```

This is nice because everything starts at zero automatically.

---

# Step 2: a tiny mixin/helper base class

This reduces repeated code across BST / linked list / array / hash / heap.

```cpp
#pragma once

#include "counters.hpp"

class Instrumented {
protected:
    Counters stats{};

    void countComparison(int n = 1)   { stats.comparisons += n; }
    void countStructuralOp(int n = 1) { stats.structural_ops += n; }
    void countInsert(int n = 1)       { stats.inserts += n; }
    void countDelete(int n = 1)       { stats.deletes += n; }
    void countLookup(int n = 1)       { stats.lookups += n; }
    void countResize(int n = 1)       { stats.resize_events += n; }

public:
    void resetCounters() {
        stats = {};
    }

    Counters getCounters() const {
        return stats;
    }
};
```

Now every structure can inherit from `Instrumented`.

---

# Step 3: use it in a data structure

Example with a linked list:

```cpp
#pragma once

#include "instrumented.hpp"

class LinkedList : public Instrumented {
private:
    struct Node {
        int data;
        Node* next;
        Node(int v) : data(v), next(nullptr) {}
    };

    Node* head = nullptr;

public:
    ~LinkedList() {
        Node* curr = head;
        while (curr) {
            Node* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }

    bool insert(int value) {
        countInsert();

        Node* n = new Node(value);
        n->next = head;
        head = n;

        countStructuralOp();   // new node linked in
        return true;
    }

    bool contains(int value) {
        countLookup();

        Node* curr = head;
        while (curr) {
            countComparison();
            if (curr->data == value) {
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    bool erase(int value) {
        countDelete();

        Node* curr = head;
        Node* prev = nullptr;

        while (curr) {
            countComparison();
            if (curr->data == value) {
                if (prev) {
                    prev->next = curr->next;
                } else {
                    head = curr->next;
                }

                delete curr;
                countStructuralOp();   // pointer rewiring
                return true;
            }

            prev = curr;
            curr = curr->next;
        }

        return false;
    }
};
```

That’s pretty low clutter.

---

# Step 4: same pattern for BST

Example idea for the contains method:

```cpp
bool contains(Node* node, int value) {
    countLookup();

    if (!node) return false;

    countComparison();
    if (value == node->data) return true;

    countComparison();
    if (value < node->data) return contains(node->left, value);

    return contains(node->right, value);
}
```

For BSTs, `comparisons` is especially meaningful.

For arrays, `structural_ops` might count shifts.

For heaps, `structural_ops` might count swaps.

For hash tables, `structural_ops` might count bucket insertions or rehash work.

---

# Even cleaner: macros if you want very low syntax noise

I don't usually like macros, but it's also my job to show you different techniques. Having said that, macros can make instrumentation very compact.

```cpp
#define CMP()   (++stats.comparisons)
#define SOP()   (++stats.structural_ops)
#define INS()   (++stats.inserts)
#define DEL()   (++stats.deletes)
#define LOOK()  (++stats.lookups)
#define RESZ()  (++stats.resize_events)
```

Then usage becomes:

```cpp
LOOK();
CMP();
if (curr->data == value) { ... }
```

