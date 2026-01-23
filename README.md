# CPPX Documentation

CPPX is a Linux-only C++ shared library that implements utility functions complementing the STL for GNU toolchains. It features a comprehensive automated test suite powered by Google Test.

## Requirements

- Linux distribution
- GNU-compatible C/C++ compiler (GCC) supporting C++14 or later
- CMake (3.15+)
- git
- Internet connection (required for CMake to fetch Google Test automatically)

## Install Dependencies

### Ubuntu/Debian

```text
sudo apt update
sudo apt install build-essential cmake git
```

### Fedora/RHEL

```text
sudo dnf install cmake git
```

### Arch Linux

```text
sudo pacman -S cmake git
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

## Documentation

### Generate Docs

```text
cmake --build build --target doc
```

> **Note:** If you configured CMake to build docs automatically, this step happens during the normal build.

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
