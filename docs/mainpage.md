# {#mainpage}

**CPPX** is a modern C++17 library providing robust, memory-safe implementations of complex data structures.

This documentation provides detailed API references for all classes, namespaces, and functions available in the library.

---

## Quick Navigation

If you are looking for specific data structures, start here:

| Module                 | Description                                               | Key Class           |
| :--------------------- | :-------------------------------------------------------- | :------------------ |
| **Binary Search Tree** | Standard unbalanced BST implementation.                   | stl_ext::BST        |
| **AVL Tree**           | Self-balancing binary search tree (strictly balanced).    | stl_ext::AVLTree    |
| **Binary Tree**        | Base class providing traversals and node logic.           | stl_ext::BinaryTree |
| **Node**               | The fundamental building block managed by smart pointers. | stl_ext::Node       |

---

## Example Usage

A simple example of using the **AVL Tree** to ensure O(log n) lookups:

```cpp
#include "cppx.h"
#include <iostream>

int main() {
    stl_ext::AVLTree<int> tree;

    // These insertions would normally create a linked list in a standard BST
    // But AVLTree will rotate and balance them automatically.
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    std::cout << "Root is: " << tree.get_root()->get_data() << std::endl; // Output: 20
    return 0;
}
```

## Useful Links

- [GitHub Repository](https://github.com/IFKabir/CPPX)

- [Issue Tracker](https://github.com/IFKabir/CPPX/issues)

- [Contributor Guidelines](CODE_OF_CONDUCT.md)
