# CPPX

<p align="left">
  <img src="logo.svg" alt="CPPX Logo" width="120" height="120">
</p>

A cross-platform C++23 template library providing extended data structures with automated testing via Google Test.

**[API Reference](https://ifkabir.github.io/CPPX/)** · **[Releases](https://github.com/IFKabir/CPPX/releases)**

---

## Requirements

- C++23 compiler (GCC / Clang / MSVC)
- CMake 3.14+
- clang-format
- Internet (CMake fetches Google Test)
- doxygen + graphviz *(optional, for API docs)*

---

## For Users

### Installation

Copy the `src/` and `include/` folders into your project, then:

```cpp
#include "include/cppx.h"
```

Add `src/` and `include/` to your compiler's include paths.

### Quick Start

```cpp
#include "cppx.h"
using namespace stl_ext;

int main() {
    AVLTree<int> tree;
    for (int v : {10, 20, 30, 40, 50})
        tree.insert(v);

    tree.contains(30);  // true
    tree.get_min();     // 10
    tree.print_tree();  // visual debug
}
```

### Tree Visualization

**Console** — `print_tree()` renders a sideways tree:
```
        ┌── 50
    ┌── 40
10
    └── 30
└── 20
```

**Graphviz** — `dump_to_dot("tree.dot")` exports a `.dot` file:
```bash
dot -Tpng tree.dot -o tree.png
```

---

## For Developers

### Build & Test

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

> Build succeeds = all tests pass. Failures stop the build with details.

### Run Benchmarks

```bash
cmake --build build --target run_benchmark
```

Compiles with `-O2`, runs the suite, and outputs `docs/benchmark_results.csv` + `docs/benchmark_chart.svg`. No scripts or external tools.

### Code Style

- `clang-format` runs automatically on every build.
- All code lives in `namespace stl_ext`.

---

## Performance

Benchmarks compare `stl_ext::AVLTree`, `stl_ext::BST`, `stl_ext::RBTree`, `std::map`, `std::set`, and `std::unordered_set`.

| Structure | N | Insert (ms) | Lookup (ms) | Delete (ms) |
|---|---:|---:|---:|---:|
| `std::map` | 10K | 0.82 | 0.31 | 0.09 |
| `std::set` | 10K | 0.64 | 0.28 | 0.09 |
| `std::unordered_set` | 10K | 0.20 | 0.05 | 0.03 |
| `stl_ext::AVLTree` | 10K | 1.43 | 0.28 | 0.15 |
| `stl_ext::BST` | 10K | 1.79 | 0.34 | 0.12 |
| `stl_ext::RBTree` | 10K | 0.78 | 0.29 | 0.09 |
| `std::map` | 100K | 15.87 | 6.63 | 1.87 |
| `std::set` | 100K | 10.76 | 7.11 | 1.68 |
| `std::unordered_set` | 100K | 2.35 | 0.56 | 0.35 |
| `stl_ext::AVLTree` | 100K | 22.05 | 12.42 | 3.61 |
| `stl_ext::BST` | 100K | 36.08 | 7.71 | 2.43 |
| `stl_ext::RBTree` | 100K | 17.34 | 7.04 | 1.84 |
| `std::map` | 1M | 704.51 | 359.32 | 114.35 |
| `std::set` | 1M | 370.67 | 247.93 | 65.54 |
| `std::unordered_set` | 1M | 92.19 | 15.18 | 15.01 |
| `stl_ext::AVLTree` | 1M | 635.48 | 227.72 | 88.59 |
| `stl_ext::RBTree` | 1M | 552.46 | 286.53 | 73.42 |

> `stl_ext::BST` skipped at 1M — unbalanced tree causes deep recursion.

![Benchmark Chart](benchmark_chart.svg)

### Key Takeaways

- **`std::set` and `std::unordered_set`** perform extremely well for generalized loads.
- **`stl_ext::RBTree`** demonstrates competitive `O(log n)` execution, scaling robustly up to 1M elements, significantly outperforming `AVLTree` on inserts.
- **`stl_ext::AVLTree` beats `std::set` on lookups** at 1M — stricter balancing yields a slightly shorter tree, though it pays for this with slower inserts.
- **`stl_ext::BST`** matches balanced trees on random data but degrades to O(n) on sorted input.
