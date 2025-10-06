# stl_ext BinaryTree Library Guide

This document explains the functions provided by the `stl_ext::Node` and `stl_ext::BinaryTree` classes and demonstrates how to use them.

## Classes and Functions

### 1. Node

**Declaration** (in `header.h`):
```cpp
class Node {
public:
  int data;
  Node* left;
  Node* right;
  explicit Node(int &val);
};
```

- **Node(int &val)**: Constructor that initializes a node with value `val`. Sets `left` and `right` pointers to `nullptr`.

**Usage Example**:
```cpp
int value = 10;
stl_ext::Node* root = new stl_ext::Node(value);
```

### 2. BinaryTree

**Declaration** (in `header.h`):
```cpp
class BinaryTree {
private:
  Node* head;
  void preorder(Node* node);
  void inorder(Node* node);
  void postorder(Node* node);
  void levelorder(Node* node);

public:
  BinaryTree();
  BinaryTree(Node* root);
  void setLeft(Node* parent, Node* leftChild);
  void setRight(Node* parent, Node* rightChild);
  Node* getRoot() const;
  void printPreorder(Node* node);
  void printInorder(Node* node);
  void printPostorder(Node* node);
  void printLevelorder(Node* node);
};
```

#### Constructors

- **BinaryTree()**: Default constructor. Initializes an empty tree (`head = nullptr`).
- **BinaryTree(Node* root)**: Initializes the tree with `root` as the root node.

**Usage Example**:
```cpp
stl_ext::BinaryTree tree;
stl_ext::BinaryTree treeWithRoot(root);
```

#### setLeft

**Signature**:
```cpp
void setLeft(Node* parent, Node* leftChild);
```

- Sets the `left` pointer of `parent` to `leftChild`.
- If `parent` is `nullptr`, no action is taken.

**Usage Example**:
```cpp
stl_ext::Node* child = new stl_ext::Node(childValue);
tree.setLeft(root, child);
```

#### setRight

**Signature**:
```cpp
void setRight(Node* parent, Node* rightChild);
```

- Sets the `right` pointer of `parent` to `rightChild`.
- If `parent` is `nullptr`, no action is taken.

**Usage Example**:
```cpp
stl_ext::Node* rightNode = new stl_ext::Node(otherValue);
tree.setRight(root, rightNode);
```

#### getRoot

**Signature**:
```cpp
Node* getRoot() const;
```

- Returns the root node (`head`) of the tree.

**Usage Example**:
```cpp
stl_ext::Node* rootNode = tree.getRoot();
```

#### printPreorder

**Signature**:
```cpp
void printPreorder(Node* node);
```

- Performs a **preorder traversal** (root, left, right) starting from `node`. Prints node data to `std::cout`, separated by spaces, and ends with newline.

**Usage Example**:
```cpp
tree.printPreorder(tree.getRoot());
```

#### printInorder

**Signature**:
```cpp
void printInorder(Node* node);
```

- Performs an **inorder traversal** (left, root, right) starting from `node`. Prints node data and ends with newline.

**Usage Example**:
```cpp
tree.printInorder(tree.getRoot());
```

#### printPostorder

**Signature**:
```cpp
void printPostorder(Node* node);
```

- Performs a **postorder traversal** (left, right, root) starting from `node`. Prints node data and ends with newline.

**Usage Example**:
```cpp
tree.printPostorder(tree.getRoot());
```

#### printLevelorder

**Signature**:
```cpp
void printLevelorder(Node* node);
```

- Performs a **level-order traversal** (breadth-first) starting from `node`. Uses a queue internally to print nodes level by level. Ends with newline.

**Usage Example**:
```cpp
tree.printLevelorder(tree.getRoot());
```

## Putting It All Together

Below is a complete usage example showing how to build a simple tree and print its traversals:

```cpp
#include "header.h"

int main() {
  int val1 = 1, val2 = 2, val3 = 3;
  stl_ext::Node* root = new stl_ext::Node(val1);
  stl_ext::Node* left = new stl_ext::Node(val2);
  stl_ext::Node* right = new stl_ext::Node(val3);

  stl_ext::BinaryTree tree(root);
  tree.setLeft(root, left);
  tree.setRight(root, right);

  std::cout << "Preorder: ";
  tree.printPreorder(tree.getRoot());

  std::cout << "Inorder: ";
  tree.printInorder(tree.getRoot());

  std::cout << "Postorder: ";
  tree.printPostorder(tree.getRoot());

  std::cout << "Levelorder: ";
  tree.printLevelorder(tree.getRoot());

  // Clean up
  delete left;
  delete right;
  delete root;

  return 0;
}
```