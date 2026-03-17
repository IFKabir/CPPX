# CPPX — C++23 Extended Data Structures Library {#mainpage}

> A high-performance, header-only C++23 template library providing self-balancing trees
> and an arena-based memory pooling system — all living in `namespace stl_ext`.

---

## Overview

**CPPX** is a cross-platform C++23 template library that extends the C++ standard library
with efficient tree data structures backed by an **arena allocator** (`stl_ext::NodePool`).
All structures rely on **raw pointers** and contiguous block allocation to maximize
cache locality and eliminate per-node heap overhead.

Key highlights:

- 🌳 **Four tree types** — BST, AVL, Red-Black, and a generic Binary Tree base
- ⚡ **Arena allocation** — nodes live in contiguous 4096-node blocks; no per-node `new`
- 🔄 **Iterative algorithms** — AVL insert/remove and RBTree rotations avoid deep stacks
- 🎨 **Graphviz export** — `dump_to_dot()` produces `.dot` files for visual debugging
- 📐 **C++23** — uses modern language features throughout

---

## Quick Start

Copy the `src/` and `include/` folders into your project and include the umbrella header:

```cpp
#include "include/cppx.h"
using namespace stl_ext;

int main() {
    AVLTree<int> tree;
    for (int v : {10, 20, 30, 40, 50})
        tree.insert(v);

    tree.contains(30);  // true
    tree.get_min();     // 10
    tree.print_tree();  // sideways ASCII tree to stdout
}
```

---

## Module Reference

| Module | Class | Description |
|:---|:---|:---|
| **Node** | `stl_ext::Node<T>` | Generic tree node with data, left/right/parent pointers, height (`int8_t`), and color (`uint8_t`) |
| **Node Pool** | `stl_ext::NodePool<T>` | Arena allocator — allocates nodes from 4096-node contiguous blocks with a free-list for recycled slots |
| **Binary Tree** | `stl_ext::BinaryTree<T>` | Base class providing traversals (pre/in/post/level-order), `print_tree()`, `dump_to_dot()`, copy/move semantics |
| **BST** | `stl_ext::BST<T>` | Standard unbalanced Binary Search Tree with insert, remove, contains, min/max, successor/predecessor |
| **AVL Tree** | `stl_ext::AVLTree<T>` | Self-balancing BST maintaining strict balance (|BF| ≤ 1) via iterative rotations |
| **Red-Black Tree** | `stl_ext::RBTree<T>` | Self-balancing BST enforcing red-black properties; includes `validate_rb_properties()` and `to_sorted_vector()` |

---

## Architecture

```
stl_ext::BinaryTree<T>   (base: traversals, DOT export, copy/move)
       │
       ├── stl_ext::BST<T>          (insert, remove, search, min/max)
       │       │
       │       ├── stl_ext::AVLTree<T>    (height-balanced, iterative)
       │       │
       │       └── stl_ext::RBTree<T>    (red-black balanced)
       │
       └─── uses ──► stl_ext::NodePool<T>   (arena allocator)
                              │
                              └── stl_ext::Node<T>  (raw-pointer node)
```

Every tree instance owns a `NodePool<T>` member. Rotations and pointer surgery are
simple 3-pointer swaps with **no allocator involvement**.

### Design Choices

| Choice | Rationale |
|:---|:---|
| `int8_t` height field | AVL heights never exceed 45 for any practical input |
| `uint8_t` Color enum | Saves padding bytes in the node struct layout |
| Raw pointers (no `unique_ptr`) | Rotations are simple 3-pointer swaps |
| Iterative AVL insert/remove | Avoids deep recursive stack frames |
| Parent pointers on all nodes | Enables efficient iterative RBTree rotations |
| 4096-node arena blocks | Keeps a full block inside CPU cache |

---

## Tree Visualization

### Console (`print_tree()`)

Renders a sideways tree to `stdout`:

```
                ┌── 50
            ┌── 40
        ┌── 30
        │   └── 25
    ┌── 20
10
```

### Graphviz (`dump_to_dot()`)

```cpp
tree.dump_to_dot("tree.dot");
```

Then render with:

```bash
dot -Tpng tree.dot -o tree.png
```

---

## Performance

Benchmarks run with `-O3 -march=native` (median of 3 trials, 1 warmup).

| Structure | N | Insert (ms) | Lookup (ms) | Delete (ms) |
|:---|---:|---:|---:|---:|
| `std::map` | 10K | 0.85 | 0.33 | 0.09 |
| `std::set` | 10K | 0.79 | 0.32 | 0.09 |
| `std::unordered_set` | 10K | 0.20 | 0.04 | 0.02 |
| `stl_ext::BST` | 10K | 0.64 | 0.29 | 0.07 |
| `stl_ext::AVLTree` | 10K | 1.58 | 0.26 | 0.18 |
| `stl_ext::RBTree` | 10K | 0.71 | 0.26 | 0.08 |
| `std::map` | 100K | 14.96 | 6.63 | 1.70 |
| `std::set` | 100K | 14.65 | 6.56 | 1.62 |
| `std::unordered_set` | 100K | 3.09 | 0.54 | 0.37 |
| `stl_ext::BST` | 100K | 10.94 | 6.19 | 1.36 |
| `stl_ext::AVLTree` | 100K | 21.06 | 5.22 | 2.48 |
| `stl_ext::RBTree` | 100K | 10.91 | 5.25 | 1.34 |
| `std::map` | 1M | 495.52 | 240.18 | 56.05 |
| `std::set` | 1M | 482.13 | 244.48 | 55.52 |
| `std::unordered_set` | 1M | 137.60 | 12.50 | 16.35 |
| `stl_ext::AVLTree` | 1M | 459.19 | 163.96 | 72.13 |
| `stl_ext::RBTree` | 1M | 284.37 | 162.31 | 39.44 |

> `stl_ext::BST` is skipped at 1M — unbalanced insertion causes excessive recursion depth.

### Key Takeaways

- **`stl_ext::RBTree` outperforms `std::set`** at 1M — insert is **1.7× faster** (284ms vs 482ms),
  delete is **1.4× faster** (39ms vs 56ms), thanks to arena allocation and raw-pointer rotations.
- **`stl_ext::AVLTree` beats `std::set` on lookups** — 164ms vs 244ms at 1M due to stricter balance.
- **`stl_ext::BST` fastest at small N** — no rebalancing overhead dominates at 10K–100K on random data.
- **`std::unordered_set`** remains unbeatable for pure lookup workloads (O(1) amortized).

---

## Requirements

| Dependency | Version | Notes |
|:---|:---|:---|
| C++ Compiler | C++23 | GCC / Clang / MSVC |
| CMake | 3.14+ | Fetches Google Test automatically |
| clang-format | any | Auto-applied on every build |
| Doxygen + Graphviz | any | Optional — for generating this documentation |

---

## Build & Test

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

> A successful build means **all tests pass**. Failures halt the build with details.

### Run Benchmarks

```bash
cmake --build build --target run_benchmark
```

Outputs `docs/benchmark_results.csv` and `docs/benchmark_chart.svg`.

---

## Links

- 📦 [GitHub Repository](https://github.com/IFKabir/CPPX)
- 🐛 [Issue Tracker](https://github.com/IFKabir/CPPX/issues)
- 📋 [Releases](https://github.com/IFKabir/CPPX/releases)
- 🤝 [Contributing Guide](CONTRIBUTING.md)
