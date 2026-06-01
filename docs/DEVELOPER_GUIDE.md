# CPPX Developer Guide {#developer_guide}

> **Audience:** Contributors, maintainers, and anyone building CPPX from source.
> This document covers everything you need to develop, test, benchmark, and release CPPX.

---

## Table of Contents

1. Prerequisites
2. Repository Layout
3. Building from Source
4. Running the Test Suite
5. Running Benchmarks
6. Generating API Documentation
7. Code Style & Formatting
8. CMake Options Reference
9. Adding a New Data Structure
10. Architecture Deep-Dive
11. PPA Release Workflow
12. Troubleshooting

---

## 1. Prerequisites

Install all required tools before building:

```bash
# Compiler & build tools (C++23 required)
sudo apt install build-essential cmake clang-format

# Testing & docs (fetched automatically by CMake, but Doxygen is local)
sudo apt install doxygen graphviz

# For PPA releases only
sudo apt install devscripts debhelper dput dh-make lintian
```

| Tool | Minimum Version | Notes |
|---|---|---|
| GCC / Clang / MSVC | C++23 support | GCC 13+, Clang 17+, MSVC 19.38+ |
| CMake | 3.14 | Fetches Google Test via `FetchContent` |
| clang-format | any | Applied automatically on build |
| Doxygen | any | Optional — for HTML API docs |
| Graphviz (`dot`) | any | Optional — for class hierarchy diagrams |
| Internet access | — | Required on first build to fetch Google Test |

---

## 2. Repository Layout

```
CPPX/
├── include/
│   └── cppx.h               # ← Single umbrella header (all declarations)
├── src/
│   ├── avl.tpp              # AVLTree<T> template implementation
│   ├── binary_tree.tpp      # BinaryTree<T> template implementation
│   ├── bst.tpp              # BST<T> template implementation
│   ├── node.tpp             # Node<T> template implementation
│   ├── node_pool.tpp        # NodePool<T> arena allocator implementation
│   └── rbt.tpp              # RBTree<T> template implementation
├── test_suite/
│   ├── avl_test_cases.cpp
│   ├── binary_tree_test_cases.cpp
│   ├── bst_test_cases.cpp
│   ├── node_test_cases.cpp
│   ├── rbt_test_cases.cpp
│   └── visualization_test_cases.cpp
├── benchmarks/
│   └── benchmark_main.cpp   # Standalone benchmark runner (no Google Benchmark)
├── docs/
│   ├── README.md            # Project README (rendered on GitHub)
│   ├── DEVELOPER_GUIDE.md   # ← This file
│   ├── CONTRIBUTING.md      # Short contribution reference
│   ├── CODE_OF_CONDUCT.md
│   ├── SECURITY.md
│   ├── mainpage.md          # Doxygen main page
│   ├── benchmark_results.csv
│   └── benchmark_chart.svg
├── scripts/
│   └── ppa-upload.sh        # Automated Launchpad PPA release script
├── debian/                  # Debian packaging files
├── CMakeLists.txt
├── Doxyfile.in              # Doxygen config template (processed by CMake)
└── .clang-format            # Formatting rules (Google-based style)
```

**Key design rule:** Template class *declarations* live in `include/cppx.h`.
Template *implementations* live in `src/*.tpp` files, which are `#include`-d at the
bottom of `cppx.h`. This keeps the header clean while satisfying the C++ requirement
that template definitions be visible at instantiation time.

---

## 3. Building from Source

### Standard Build (tests + docs)

```bash
# Clone
git clone https://github.com/IFKabir/CPPX.git
cd CPPX

# Configure (first time fetches Google Test — requires internet)
mkdir build && cd build
cmake ..

# Build everything: library, unit_tests, benchmark, and Doxygen HTML
cmake --build .
```

> **Note:** A clean successful build means **all unit tests passed**. Test failures
> halt the build and print the failing test names and assertions.

### Build Without Documentation

If Doxygen is not installed or you want a faster iteration loop:

```bash
cmake .. -DCPPX_BUILD_DOCS=OFF
cmake --build .
```

### Build Without Tests or Benchmarks

```bash
cmake .. -DCPPX_BUILD_TESTS=OFF
cmake --build .
```

### Parallel Builds (faster)

```bash
cmake --build . --parallel $(nproc)
```

### Clean Rebuild

```bash
cd ..
rm -rf build
mkdir build && cd build
cmake ..
cmake --build .
```

---

## 4. Running the Test Suite

Tests are compiled into the `unit_tests` binary by CMake and auto-discovered by CTest.

### Run All Tests via CTest (recommended)

```bash
cd build
ctest --output-on-failure
```

### Run the Test Binary Directly

```bash
./unit_tests
```

### Run a Specific Test or Filter

Google Test supports `--gtest_filter`:

```bash
# Run only AVL tests
./unit_tests --gtest_filter="AVL*"

# Run only insertion tests across all suites
./unit_tests --gtest_filter="*Insert*"

# List all available test names without running them
./unit_tests --gtest_list_tests
```

### Test Coverage by File

| File | Covers |
|---|---|
| `avl_test_cases.cpp` | `AVLTree<T>` — insert, remove, balance invariants, rotations |
| `bst_test_cases.cpp` | `BST<T>` — insert, remove, search, min/max, successor/predecessor |
| `rbt_test_cases.cpp` | `RBTree<T>` — insert, remove, red-black property validation |
| `binary_tree_test_cases.cpp` | `BinaryTree<T>` — manual construction, traversals, copy/move semantics |
| `node_test_cases.cpp` | `Node<T>` — accessors, constructors |
| `visualization_test_cases.cpp` | `print_tree()`, `dump_to_dot()` output correctness |

### Writing a New Test

Add a file to `test_suite/` — CMake picks up all `*.cpp` files via glob automatically:

```cpp
// test_suite/my_structure_test_cases.cpp
#include <gtest/gtest.h>
#include "cppx/cppx.h"

using namespace stl_ext;

TEST(MyStructure, InsertAndContains) {
    MyStructure<int> s;
    s.insert(42);
    EXPECT_TRUE(s.contains(42));
    EXPECT_FALSE(s.contains(99));
}

TEST(MyStructure, EmptyOnConstruction) {
    MyStructure<int> s;
    EXPECT_TRUE(s.is_empty());
    EXPECT_EQ(s.size(), 0);
}
```

Rebuild and run — no CMakeLists changes needed:

```bash
cmake --build build
cd build && ctest --output-on-failure
```

---

## 5. Running Benchmarks

The benchmark suite compares `stl_ext` structures against `std::set`, `std::map`,
and `std::unordered_set` at N = 10K, 100K, and 1M elements. It uses its own
`Benchmark` timing class (no external dependency), with **1 warmup run** and
**median of 3 timed runs** per operation category (insert / lookup / delete).

### Run via CMake Target (recommended)

```bash
# From the repo root:
cmake --build build --target run_benchmark
```

This compiles the `benchmark` binary with `-O3 -march=native` and runs it,
passing the repo root as the output directory.

### Run the Binary Directly

```bash
# From inside build/
./benchmark /path/to/CPPX   # writes CSV + SVG to docs/
./benchmark                  # writes to docs/ relative to CWD (may differ)
```

> **Always pass the repo root path** when running directly so output files land in `docs/`.

### Benchmark Output

| Output file | Description |
|---|---|
| `docs/benchmark_results.csv` | Raw timing data (structure, N, insert_ms, lookup_ms, delete_ms) |
| `docs/benchmark_chart.svg` | SVG bar chart comparing all structures at all sizes |

The console also prints a formatted table:

```
Structure               N     Insert (ms)     Lookup (ms)     Delete (ms)
────────────────────────────────────────────────────────────────────────────
std::set               10K            0.79            0.32            0.09
stl_ext::AVLTree       10K            1.58            0.26            0.18
...
```

### Interpreting Results

- **BST is skipped at 1M** — an unbalanced tree risks deep recursion on sorted/random data.
- **RBTree outperforms `std::set`** at large N due to arena allocation (fewer cache misses).
- **AVLTree has slower inserts** (more rotations) but **faster lookups** (stricter balance = shallower tree).
- **`std::unordered_set`** wins on pure lookup — it is the correct baseline for O(1) access.

### Adding a Benchmark for a New Structure

Open `benchmarks/benchmark_main.cpp` and follow the existing pattern:

```cpp
static BenchmarkResult bench_my_structure(std::size_t n,
    const std::vector<int>& data,
    const std::vector<int>& lookup_data,
    const std::vector<int>& delete_data)
{
    Benchmark bm;
    std::vector<double> ins_times, lkp_times, del_times;

    for (int i = 0; i < WARMUP_RUNS; ++i) {
        stl_ext::MyStructure<int> warmup;
        for (int v : data) warmup.insert(v);
    }

    for (int i = 0; i < BENCH_RUNS; ++i) {
        stl_ext::MyStructure<int> s;
        bm.start(); for (int v : data)        s.insert(v);   bm.stop(); ins_times.push_back(bm.elapsed_ms());
        bm.start(); volatile bool sink=false;
                    for (int v : lookup_data)  sink = s.contains(v); bm.stop(); lkp_times.push_back(bm.elapsed_ms()); (void)sink;
        bm.start(); for (int v : delete_data)  s.remove(v);  bm.stop(); del_times.push_back(bm.elapsed_ms());
    }

    std::sort(ins_times.begin(), ins_times.end());
    std::sort(lkp_times.begin(), lkp_times.end());
    std::sort(del_times.begin(), del_times.end());

    return {"stl_ext::MyStructure", n,
            ins_times[BENCH_RUNS/2], lkp_times[BENCH_RUNS/2], del_times[BENCH_RUNS/2]};
}
```

Then call it inside `main()` alongside the existing `bench_avl(...)` calls.

---

## 6. Generating API Documentation

Doxygen docs are built automatically when `CPPX_BUILD_DOCS=ON` (the default) and
Doxygen is found on the system.

```bash
# If already built, docs are at:
open build/html/index.html   # macOS
xdg-open build/html/index.html  # Linux

# Or explicitly rebuild just the docs target:
cmake --build build --target doc_doxygen
```

The Doxygen theme is [doxygen-awesome-css](https://github.com/jothepro/doxygen-awesome-css),
fetched automatically by CMake. Configuration is in `Doxyfile.in` (processed into
`build/Doxyfile` at configure time).

The hosted version is at **https://cppx.vercel.app/**.

---

## 7. Code Style & Formatting

CPPX uses **clang-format** with the rules defined in `.clang-format` at the repo root
(a Google-based style with project-specific overrides).

### Automatic Formatting

clang-format runs **automatically** during every `cmake --build` invocation via the
`format_code` CMake target, which is a build dependency of both `unit_tests` and `benchmark`.
You do not need to run it manually.

### Manual Formatting

```bash
cmake --build build --target format_code
```

Or format a single file:

```bash
clang-format -i -style=file src/my_structure.tpp
```

### Style Rules at a Glance

| Category | Rule |
|---|---|
| Namespace | All code inside `namespace stl_ext { }` |
| Classes | PascalCase — `BinaryTree`, `AVLTree`, `NodePool` |
| Files | snake_case — `binary_tree.tpp`, `avl_test_cases.cpp` |
| Functions | snake_case — `get_min()`, `insert_iterative()`, `rotate_left()` |
| Member variables | Prefixed — `m_data`, `m_height` (value), `p_left`, `p_head` (pointer) |
| Headers | No `using namespace std;` — always fully qualify in headers |
| Memory | Use `NodePool<T>` — **no raw `new`/`delete`** — the arena owns all nodes |
| Node links | Raw pointers (not `std::unique_ptr`) — enables simple rotation pointer surgery |
| Template split | Declarations in `.h`, implementations in `.tpp` (included at bottom of `.h`) |

---

## 8. CMake Options Reference

| Option | Default | Description |
|---|---|---|
| `CPPX_BUILD_TESTS` | `ON` | Build `unit_tests` and `benchmark` executables; fetch Google Test |
| `CPPX_BUILD_DOCS` | `ON` | Build Doxygen HTML docs; fetch doxygen-awesome-css theme |

Pass options at configure time:

```bash
cmake .. -DCPPX_BUILD_TESTS=OFF -DCPPX_BUILD_DOCS=OFF
```

### CMake Targets

| Target | Command | Description |
|---|---|---|
| `all` (default) | `cmake --build .` | Build library, tests, benchmark, docs |
| `unit_tests` | `cmake --build . --target unit_tests` | Build test executable only |
| `benchmark` | `cmake --build . --target benchmark` | Build benchmark binary only |
| `run_benchmark` | `cmake --build . --target run_benchmark` | Build and run the full benchmark suite |
| `doc_doxygen` | `cmake --build . --target doc_doxygen` | Regenerate Doxygen HTML |
| `format_code` | `cmake --build . --target format_code` | Run clang-format across all sources |

---

## 9. Adding a New Data Structure

CPPX is designed to grow. Follow these steps to add a new structure correctly:

### Step 1 — Declare the class in `include/cppx.h`

Forward-declare and then fully declare your class, inheriting from `BinaryTree<T>`
or `BST<T>` as appropriate:

```cpp
// Forward declaration (add near the top with other forwards)
template <typename T> class MyStructure;

// Full declaration (add after RBTree<T>)
template <typename T>
class MyStructure : public BST<T>
{
  public:
    using BST<T>::p_head;
    using BST<T>::m_pool;

    void insert(const T& val) override;
    void remove(const T& val) override;
    // ... your new methods
};
```

### Step 2 — Implement in `src/my_structure.tpp`

```cpp
// src/my_structure.tpp
namespace stl_ext {

template <typename T>
void MyStructure<T>::insert(const T& val)
{
    // Implementation using m_pool.allocate(...) — never raw new
}

} // namespace stl_ext
```

### Step 3 — Include the `.tpp` at the bottom of `cppx.h`

```cpp
// At the bottom of include/cppx.h, with the other .tpp includes:
#include "cppx/detail/my_structure.tpp"
```

> CMake automatically symlinks `src/*.tpp` into `build/staging_include/cppx/detail/`,
> so no CMakeLists changes are needed.

### Step 4 — Add tests in `test_suite/my_structure_test_cases.cpp`

Cover at minimum: construction, insert, remove, contains, edge cases (empty tree,
single node, duplicate values). See existing test files for patterns.

### Step 5 — Add benchmarks in `benchmarks/benchmark_main.cpp`

Follow the `bench_avl()` pattern (see **Section 5 — Running Benchmarks** above).

### Step 6 — Rebuild and verify

```bash
cmake --build build
cd build && ctest --output-on-failure
cmake --build . --target run_benchmark
```

CMake's `file(GLOB_RECURSE ...)` picks up the new `.tpp` and `_test_cases.cpp` files
automatically — **no CMakeLists edits required**.

---

## 10. Architecture Deep-Dive

### Class Hierarchy

```
stl_ext::BinaryTree<T>        (base: traversals, Graphviz export, copy/move)
       │   owns ──► stl_ext::NodePool<T>   (arena allocator)
       │                 └── stl_ext::Node<T>  (raw-pointer node)
       │
       └── stl_ext::BST<T>           (insert, remove, search, min/max, successor)
               │
               ├── stl_ext::AVLTree<T>    (height-balanced, iterative rotations)
               │
               └── stl_ext::RBTree<T>    (red-black balanced, parent pointers)
```

### Arena Allocator (`NodePool<T>`)

`NodePool<T>` allocates `Node<T>` objects from contiguous 4096-node blocks:

- **Allocation:** Bumps a `used` counter inside the current `Block`. If the block
  is full, a new `Block` is heap-allocated.
- **Deallocation:** Pushes the freed slot onto a free-list (`std::vector<void*>`).
  Next allocation checks the free-list first.
- **Destruction:** `~NodePool()` / `destroy_all()` deletes all blocks at once — O(num_blocks),
  not O(N). No per-node destructor calls on the tree nodes.

This design provides:

| Property | Benefit |
|---|---|
| Contiguous blocks | CPU cache lines hold multiple adjacent nodes — reduces cache misses on traversal |
| No per-node `new`/`delete` | Eliminates allocator lock contention and heap fragmentation |
| Free-list recycling | `remove()` can return nodes without block fragmentation |
| Move semantics on `NodePool` | Entire pools can be transferred (e.g. tree move constructor) in O(1) |

### Node Layout

```cpp
template <typename T>
class Node {
    T           m_data;            // user data
    Node<T>*    p_left   = nullptr;
    Node<T>*    p_right  = nullptr;
    Node<T>*    p_parent = nullptr; // required for iterative RBTree rotations
    int8_t      m_height = 1;      // AVL; fits in 1 byte — max height ~45
    Color       m_color  = RED;    // uint8_t enum — RBT; saves padding
};
```

Parent pointers allow AVL and RB rotations to be implemented **iteratively**,
avoiding deep recursion and making the stack trace flat under a profiler.

### Key Invariants

- **AVL:** `|balance_factor(node)| ≤ 1` for every node. Balance factor = `height(left) - height(right)`.
- **RBTree:** Root is BLACK; no two consecutive RED nodes; every path from root
  to a null leaf has the same number of BLACK nodes (black-height).
- **NodePool:** All nodes pointed to by a tree are owned by that tree's pool.
  Copying a tree (`BinaryTree` copy constructor) calls `clone_subtree()` which
  allocates new nodes in the destination's own pool.

---

## 11. PPA Release Workflow

When you have merged new features to `main` and want to publish to the Ubuntu PPA:

### Prerequisites

```bash
sudo apt install devscripts debhelper dput lintian
gpg --list-secret-keys   # ensure you have a GPG signing key
```

### Full Release (interactive)

```bash
./scripts/ppa-upload.sh
```

The script:

1. Validates required tools and `debian/` packaging files
2. Auto-detects your GPG key
3. Prompts for a new version number (or keeps current)
4. Updates `debian/changelog` via `dch`
5. Cleans previous build artifacts
6. Builds a signed source package with `debuild -S -sa`
7. Runs `lintian` for policy checks (non-fatal warnings are OK)
8. Uploads to `ppa:ifkabir/cppx` via `dput`

### Options

```bash
./scripts/ppa-upload.sh --no-bump    # skip version prompt, use current changelog version
./scripts/ppa-upload.sh --dry-run    # build + lint but do NOT upload (safe for testing)
./scripts/ppa-upload.sh --help       # full option reference
```

### Environment Variables

```bash
CPPX_PPA=ppa:ifkabir/cppx           # override target PPA
CPPX_DISTRO=jammy                   # target Ubuntu series (default: noble)
CPPX_GPG_KEY=ABCDEF1234567890       # override GPG key ID
```

Example — release to Jammy without bumping the version:

```bash
CPPX_DISTRO=jammy ./scripts/ppa-upload.sh --no-bump
```

### After Upload

Launchpad typically builds within 5–30 minutes. Check build status at:
https://launchpad.net/~ifkabir/+archive/ubuntu/cppx

---

## 12. Troubleshooting

### Google Test not fetching

`FetchContent` requires internet access on first configure. If you're behind a proxy:

```bash
export http_proxy=http://your-proxy:port
export https_proxy=http://your-proxy:port
cmake ..
```

Or pre-download GoogleTest v1.14.0 and point CMake to a local mirror.

### `clang-format` not found — format target skipped

Install it:

```bash
sudo apt install clang-format
```

Then reconfigure:

```bash
cmake ..   # re-runs find_program(CLANG_FORMAT ...)
```

### Doxygen warning: `dot` not found

Install Graphviz:

```bash
sudo apt install graphviz
```

### Running the `benchmark` binary directly produces wrong output paths

The benchmark binary accepts the repo root as its first argument:

```bash
./build/benchmark /path/to/CPPX
```

Without the argument it defaults to `./`, which will write `./docs/benchmark_results.csv`
relative to wherever you ran it from — potentially not the `docs/` folder in the repo.
Use the CMake target to avoid this:

```bash
cmake --build build --target run_benchmark
```

### Tests pass but `ctest` says "no tests found"

Make sure you ran `cmake ..` with `CPPX_BUILD_TESTS=ON` (the default) and that
`gtest_discover_tests(unit_tests)` had a chance to run. Re-run:

```bash
cmake .. && cmake --build . && ctest
```

### Linker errors about missing template instantiations

All `.tpp` files must be `#include`-d at the bottom of `cppx.h`. Check that your
new `.tpp` file has a corresponding `#include "cppx/detail/your_file.tpp"` line there.

---

*For usage questions or bug reports, open an issue at https://github.com/IFKabir/CPPX/issues.*
*For security vulnerabilities, see [SECURITY.md](SECURITY.md).*
