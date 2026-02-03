## P01 - Implementing Delete in a Binary Search Tree (BST)
#### Jadyn Dangerfield
#### School Email: jadangerfield0515@my.msutexas.edu
#### Personal Email: jadyndangerfield12@gmail.com
## Overview:
 In this program, I extended the provided Binary Search Tree implementation by implementing a `delete` method.
## Requirements:
- Create a directory called `trees` before running the code, as this is where the Graphviz .dot files will save.
## Explanation:
### The cases I handled for deletion
#### For my deletion method, I implemented five different deletion cases in the Bst class provided:
- An empty tree, or a non-existent value
- A leaf node (a node with no children)
- A node with one child on the right
- A node with one child on the left
- A node with two children
#### Then, in my main function, I tested ten possible cases:
- Deleting from an empty tree
- Deleting a leaf node
- Deleting a child leaf node
- Deleting a node with one left child
- Deleting a node with one right child
- Deleting a node with two children
- Deleting the root node
- Deleting a node from a tree with sequential values
- Deleting a non-existent value
- Deleting an entire tree of random values
### The logic behind choosing a replacement value for nodes with two children
 I decided to use the inorder successor as the replacement value becasue it maintains the Binary Search Tree (BST) properties. A BST must satisfy:
- Left subtree: all values < node's values
- Right subtree: all values > node's values
 The inorder successor is the smallest node in the right subtree of the node being deleted, so replacing the deleted node's value with its inorder successor ensures the ordering of the BST remains valid. Additionally, using the inorder successor also simplified the deletion logic, and ensured I would not have to restructure large parts of the tree.
### Any challenges I faced while implementing deletion
I would say my biggest challenge was figuring out what cases I wanted to test in my main function. It was hard to come up with ideas once I tested the cases you provided in the assignment description, so I utilized Google to help me come up with some additional test cases.
### Additional Comments
There were some changes I made to the original code outside of the deletion implementation. The first change I made was to the print function. I wanted to include a quick visual in the terminal as well, so I edited the original method to print the tree sideways. This provides a more readable tree in the terminal. The second change I made was adding a findMin method in the Bst class. This just made it easier to find the successor in the case of deleting a node with two children.
