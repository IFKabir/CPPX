# {#mainpage}

**CPPX** — a C++17 template library for memory-safe data structures.

## Available Structures

| Module | Description | Class |
|---|---|---|
| Binary Search Tree | Standard unbalanced BST | stl_ext::BST |
| AVL Tree | Self-balancing BST (strict balance) | stl_ext::AVLTree |
| Red-Black Tree | Self-balancing BST (red-black properties) | stl_ext::RBTree |
| Binary Tree | Base class with traversals | stl_ext::BinaryTree |
| Node | Building block using smart pointers | stl_ext::Node |

## Example

```cpp
#include "cppx.h"

int main() {
    stl_ext::AVLTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    // Root is 20 (auto-balanced)
    tree.get_root()->get_data(); // 20
}
```

## Links

- [GitHub](https://github.com/IFKabir/CPPX)
- [Issues](https://github.com/IFKabir/CPPX/issues)
- [Contributing](CONTRIBUTING.md)
