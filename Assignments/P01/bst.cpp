/* bst.cpp
*  Implementation of a Binary Search Tree (BST) with deletion functionality
*  and Graphiz DOT file generation for visualization.
*/
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Node {
    int data;
    Node *left;
    Node *right;

    Node(int x) {
        data = x;
        left = right = nullptr;
    }
};

class GraphvizBST {
public:
    static void saveDotFile(const string &filename, const string &dotContent) {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            outFile << dotContent;
            outFile.close();
            cout << "DOT file saved: " << filename << endl << endl;
        } else {
            cerr << "Error: Could not open file " << filename << endl;
        }
    }

    static string generateDot(const Node *root) {
        string dot = "digraph BST {\n";
        dot += "    node [fontname=\"Arial\"];\n";
        dot += generateDotHelper(root);
        dot += "}\n";
        return dot;
    }

private:
    static string generateDotHelper(const Node *node) {
        if (!node)
            return "";
        string result;
        if (node->left) {
            result += "    " + to_string(node->data) + " -> " + to_string(node->left->data) + " [label=\"L\"];\n";
            result += generateDotHelper(node->left);
        } else {
            string nullNode = "nullL" + to_string(node->data);
            result += "    " + nullNode + " [shape=point];\n";
            result += "    " + to_string(node->data) + " -> " + nullNode + ";\n";
        }
        if (node->right) {
            result += "    " + to_string(node->data) + " -> " + to_string(node->right->data) + " [label=\"R\"];\n";
            result += generateDotHelper(node->right);
        } else {
            string nullNode = "nullR" + to_string(node->data);
            result += "    " + nullNode + " [shape=point];\n";
            result += "    " + to_string(node->data) + " -> " + nullNode + ";\n";
        }
        return result;
    }
};

class Bst {
    Node *root;

    void _printSideways(Node *node, int depth){
        if (!node) return;

        _printSideways(node->right, depth + 1);

        for (int i =0; i < depth; i++){
            cout << "    ";
        }
        cout << node->data << endl;

        _printSideways(node->left, depth + 1);
    }
    void _insert(Node *&subroot, int x) {
        if (!subroot) { // if(root == nullptr)
            subroot = new Node(x);
        } else {
            if (x < subroot->data) {
                _insert(subroot->left, x);
            } else {
                _insert(subroot->right, x);
            }
        }
    }
    int _ipl(Node *root, int depth = 0) {
        if (!root)
            return 0; // Base case: Empty subtree contributes 0 to IPL
        return depth + _ipl(root->left, depth + 1) + _ipl(root->right, depth + 1);
    }

    Node* _findMin(Node *subroot){
        while (subroot && subroot->left){
            subroot = subroot->left;
        }
        return subroot;
    }

    void _delete(Node *&subroot, int x){
        // Case 1: Empty tree or value not found
        if (!subroot){
            return;
        }

        // Traverse the tree to find the node to delete
        if (x < subroot->data){
            _delete(subroot->left, x);
        }
        else if (x > subroot->data){
            _delete(subroot->right, x);
        }
        else {
            // Node found

            // Case 2: Leaf node
            if (!subroot->left && !subroot->right){
                delete subroot;
                subroot = nullptr;
            }

            // Case 3: One child on the right
            else if (!subroot->left){
                Node *temp = subroot;
                subroot = subroot->right;
                delete temp;
            }

            // Case 4: One child on the left
            else if (!subroot->right){
                Node *temp = subroot;
                subroot = subroot->left;
                delete temp;
            }

            // Case 5: Two children
            else {
                // Find the inorder successor (minimum in the right subtree)
                Node *successor = _findMin(subroot->right);

                // Copy the successor's value to the current node
                subroot->data = successor->data;

                // Delete the successor node
                _delete(subroot->right, successor->data);
            }
        }

    }

public:
    Bst() { root = nullptr; }
    void insert(int x) { _insert(root, x); }
    bool search(int key) { return 0; }
    void printTree(){ _printSideways(root, 0); }
    void saveDotFile(const string &filename) {
        string dotContent = GraphvizBST::generateDot(root);
        GraphvizBST::saveDotFile(filename, dotContent);
    }
    void remove(int x) { _delete(root, x); }

    /**
     * Computes the Internal Path Length (IPL) of a Binary Search Tree (BST).
     *
     * Definition:
     * The Internal Path Length (IPL) of a BST is the sum of the depths of all nodes in the tree.
     * The depth of a node is the number of edges from the root to that node.
     *
     * Example:
     *        10
     *       /  \
     *      5    15
     *     / \     \
     *    2   7    20
     *
     * IPL = (depth of 10) + (depth of 5) + (depth of 15) + (depth of 2) + (depth of 7) + (depth of 20)
     *     = 0 + 1 + 1 + 2 + 2 + 2 = 8
     *
     * @param root Pointer to the root node of the BST.
     * @param depth Current depth of the node (default is 0 for the root call).
     * @return The sum of depths of all nodes (Internal Path Length).
     */
    int ipl() {
        return _ipl(root);
    }
};

bool unique_value(int *arr, int n, int x) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == x) {
            return false;
        }
    }
    return true;
}

int main() {
    // Case 1: Delete from an empty tree 
    Bst tree1;
    tree1.remove(10);
    cout << "Tree 1 after deletion (should be empty): \n";
    tree1.printTree();
    tree1.saveDotFile("trees/bst_case1.dot");

    // Case 2: Tree with one node, delete that node
    Bst tree2;
    // Insert a single node
    tree2.insert(10);
    cout << "Tree 2 before deletion: \n";
    tree2.printTree();
    // Snapshot of tree 2 before deletion
    
    tree2.saveDotFile("trees/bst_case2_before.dot");
    // Delete the only node
    tree2.remove(10);
    cout << "\nTree 2 after deletion: \n";
    tree2.printTree();
    
    // Snapshot of tree 2 after deletion
    tree2.saveDotFile("trees/bst_case2_after.dot");

    // Case 3: Delete a leaf node
    Bst tree3;
    // Insert nodes
    tree3.insert(10);
    tree3.insert(5);
    tree3.insert(15);
    cout << "Tree 3 before deletion: \n";
    tree3.printTree();
    // Snapshot of tree 3 before deletion
    
    tree3.saveDotFile("trees/bst_case3_before.dot");
    // Delete a leaf node
    tree3.remove(5);
    cout << "Tree 3 after deletion: \n";
    tree3.printTree();
    // Snapshot of tree 3 after deletion
    
    tree3.saveDotFile("trees/bst_case3_after.dot");

    // Case 4: Delete a node with one left child
    Bst tree4;
    // Insert nodes
    tree4.insert(10);
    tree4.insert(5);
    tree4.insert(2);
    cout << "Tree 4 before deletion: \n";
    tree4.printTree();
    // Snapshot of tree 4 before deletion
    
    tree4.saveDotFile("trees/bst_case4_before.dot");
    // Delete a node with one left child
    tree4.remove(5);
    cout << "Tree 4 after deletion: ";
    tree4.printTree();
    // Snapshot of tree 4 after deletion
    
    tree4.saveDotFile("trees/bst_case4_after.dot");

    // Case 5: Delete a node with one right child
    Bst tree5;
    // Insert nodes
    tree5.insert(10);
    tree5.insert(15);
    tree5.insert(20);
    cout << "Tree 5 before deletion: \n";
    tree5.printTree();
    // Snapshot of tree 5 before deletion
    
    tree5.saveDotFile("trees/bst_case5_before.dot");
    // Delete a node with one right child
    tree5.remove(15);
    cout << "Tree 5 after deletion: \n";
    tree5.printTree();
    // Snapshot of tree 5 after deletion
    
    tree5.saveDotFile("trees/bst_case5_after.dot");

    // Case 6: Delete a node with two children
    Bst tree6;
    // Insert nodes
    tree6.insert(10);
    tree6.insert(5);
    tree6.insert(15);
    tree6.insert(2);
    tree6.insert(7);
    cout << "Tree 6 before deletion: \n";
    tree6.printTree();
    // Snapshot of tree 6 before deletion
    
    tree6.saveDotFile("trees/bst_case6_before.dot");
    // Delete a node with two children
    tree6.remove(5);
    cout << "Tree 6 after deletion: ";
    tree6.printTree();
    // Snapshot of tree 6 after deletion
    
    tree6.saveDotFile("trees/bst_case6_after.dot");

    // Case 7: Delete the root node with two children
    Bst tree7;
    // Insert nodes
    tree7.insert(10);
    tree7.insert(5);
    tree7.insert(15);
    tree7.insert(2);
    tree7.insert(7);
    tree7.insert(20);
    cout << "Tree 7 before deletion: \n";
    tree7.printTree();
    // Snapshot of tree 7 before deletion
    
    tree7.saveDotFile("trees/bst_case7_before.dot");
    // Delete the root node
    tree7.remove(10);
    cout << "Tree 7 after deletion: \n";
    tree7.printTree();
    // Snapshot of tree 7 after deletion
    
    tree7.saveDotFile("trees/bst_case7_after.dot");

    // Case 8: Sequential values (linked list BST)
    Bst tree8;
    // Insert sequential nodes
    for (int i = 1; i <= 7; i++) {
        tree8.insert(i);
    }
    cout << "Tree 8 before deletion: \n";
    tree8.printTree();
    // Snapshot of tree 8 before deletion
    
    tree8.saveDotFile("trees/bst_case8_before.dot");
    // Delete a middle node
    tree8.remove(4);
    cout << "Tree 8 after deletion: \n";
    tree8.printTree();
    // Snapshot of tree 8 after deletion
    
    tree8.saveDotFile("trees/bst_case8_after.dot");

    // Case 9: Delete non-existent value
    Bst tree9;
    // Insert nodes
    tree9.insert(10);
    tree9.insert(5);
    tree9.insert(15);
    cout << "Tree 9 before deletion: \n";
    tree9.printTree();
    // Snapshot of tree 9 before deletion
    
    tree9.saveDotFile("trees/bst_case9_before.dot");
    // Attempt to delete a non-existent value
    tree9.remove(20);
    cout << "Tree 9 after deletion: \n";
    tree9.printTree();
    // Snapshot of tree 9 after deletion
    
    tree9.saveDotFile("trees/bst_case9_after.dot");

    // Case 10: Random values
    Bst tree10;
    int root = pow(2, 15) / 2;
    int max = pow(2, 15) - 1;
    vector<int> arr;
    arr.push_back(root);
    tree10.insert(root);
    for (int i = 1; i < 5000; i++){
        // Generate unique random value
        int r = rand() % max;
        while (!unique_value(arr.data(), arr.size(), r)){
            // Regenerate if not unique
            r = rand() % max;
        }
        // Insert unique random value
        tree10.insert(r);
        // Keep track of inserted values
        arr.push_back(r); 
    }
    cout << "Inserted " << arr.size() << " unique random values\n";
    //cout << "Tree 10 before deletion: \n";
    //tree10.print();
    // Snapshot of tree 10 before deletion
    
    tree10.saveDotFile("trees/bst_case10_before.dot");

    for (int x : arr){
        tree10.remove(x);
    }
    //cout << "\nAll values deleted.\n";
    //tree10.print();
    // Snapshot of tree 10 after all deletions
    
    tree10.saveDotFile("trees/bst_case10_after.dot");
}