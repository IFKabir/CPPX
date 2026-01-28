# CPPX Documentation

CPPX is a Linux-only C++ shared library that implements utility functions complementing the STL for GNU toolchains. It features a comprehensive automated test suite powered by Google Test.

## Requirements

- Linux distribution
- GNU-compatible C/C++ compiler (GCC) supporting C++14 or later
- CMake (3.15+)
- git
- **clang-format** (for code styling)
- **doxygen** (optional, for generating documentation)
- Internet connection (required for CMake to fetch Google Test automatically)

## Install Dependencies

### Ubuntu/Debian

```text
sudo apt update
sudo apt install build-essential cmake git clang-format doxygen graphviz
```

### Fedora/RHEL

```text
sudo dnf install cmake git clang-format doxygen graphviz
```

### Arch Linux

```text
sudo pacman -S cmake git clang-format doxygen graphviz
```

## Building & Testing (Developer Workflow)

### Clone

```text
git clone git@github.com:IFKabir/CPPX.git
cd CPPX
```

### Configure & Build

```text
cd build
cmake ..
make
```

### Run Tests

```text
cd build
ctest
```

> **Note:** Auto-detects new `.cpp` files in `src/` and `test_suite/`.

## Coding Style

This project enforces Microsoft Style (Allman braces, 4-space indent).

- Do not format manually. The build system (via `make`) applies the `.clang-format` rules to your source files automatically before compilation begins.

- Ensure you use `using namespace std;` in implementation files (`.cpp`) if desired, but avoid it in header files (`.h`) to prevent namespace pollution.

## Documentation

### Generate Docs

```text
cmake --build build --target doc
```

> **Note:** If Doxygen is installed, documentation is built automatically alongside the library.

### View Docs

```text
xdg-open build/docs/html/index.html
```

## Using the Library

- **Option A (Fastest):** Use precompiled `libcppx.so` + `cppx.h` from release.
- **Option B:** Build from source (artifacts: `build/bin/libcppx.so`, `include/cppx.h`)

## Integration

In your C++ file:

```cpp
#include "cppx.h"
```

### Compile

```text
g++ -std=c++17 main.cpp ./libcppx.so -Wl,-rpath=. -o app
```

### Run

```text
./app
```

## Usage

* **API Reference:** Generate and view the [local documentation](#documentation) for full class and function details.

* **Version Info:** Check the [Releases page](https://github.com/IFKabir/CPPX/releases) for version-specific notes and changes.
