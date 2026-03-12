# CPPX

<p align="left">
  <img src="logo.svg" alt="CPPX Logo" width="120" height="120">
</p>

A cross-platform C++17 template library providing extended data structures with automated testing via Google Test.

**[API Reference](https://ifkabir.github.io/CPPX/)** · **[Releases](https://github.com/IFKabir/CPPX/releases)**

---

## Requirements

- C++17 compiler (GCC / Clang / MSVC)
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

Benchmarks compare `stl_ext::AVLTree`, `stl_ext::BST`, and `std::set` (Red-Black Tree).

| Structure | N | Insert (ms) | Lookup (ms) | Delete (ms) |
|---|---:|---:|---:|---:|
| `std::set` | 10K | 0.63 | 0.29 | 0.09 |
| `stl_ext::AVLTree` | 10K | 1.53 | 0.29 | 0.16 |
| `stl_ext::BST` | 10K | 1.86 | 0.33 | 0.12 |
| `std::set` | 100K | 11.68 | 7.10 | 1.61 |
| `stl_ext::AVLTree` | 100K | 33.55 | 7.61 | 3.68 |
| `stl_ext::BST` | 100K | 33.18 | 7.49 | 2.90 |
| `std::set` | 1M | 477.55 | 260.82 | 61.08 |
| `stl_ext::AVLTree` | 1M | 860.29 | 241.79 | 92.53 |

> `stl_ext::BST` skipped at 1M — unbalanced tree causes deep recursion.

![Benchmark Chart](benchmark_chart.svg)

### Key Takeaways

- **`std::set`** wins on insert/delete due to stdlib optimisations.
- **`AVLTree` beats `std::set` on lookups** at 1M (~7% faster) — stricter balancing = shorter tree.
- **`BST`** matches `AVLTree` on random data but degrades to O(n) on sorted input.
