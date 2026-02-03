# CPPX Documentation

<p align="left">
  <img src="logo.svg" alt="CPPX Logo" width="150" height="150">
</p>


CPPX is a cross-platform C++ template library providing extended data structures. It features a comprehensive automated test suite powered by Google Test.

## Requirements

- **C++17 compatible compiler**: GCC, Clang, or MSVC.
- **CMake (3.14+)**: Required for project configuration.
- **clang-format**: Automates code styling during the build process.
- **doxygen** **(Optional)**: Used for generating API documentation.
- **Internet connection**: Required for CMake to fetch Google Test.

## Install Dependencies

### Ubuntu/Debian

```bash
sudo apt update
sudo apt install build-essential cmake clang-format doxygen graphviz
```

### Fedora/RHEL

```bash
sudo dnf install cmake git clang-format doxygen graphviz
```

### Arch Linux

```bash
sudo pacman -S cmake git clang-format doxygen graphviz
```

### Windows

1. **Compiler**: Install [Visual Studio](https://visualstudio.microsoft.com/) (with "Desktop development with C++") or [MinGW-w64](https://www.mingw-w64.org/).

2. **CMake**: Download and install from [cmake.org](https://cmake.org/download/).

3. **Tools**: Install `clang-format` and `doxygen`(often included with LLVM or via [Chocolatey](https://chocolatey.org/)).

## Developer Workflow

#### Build & Test

1. **Linux / macOS**:

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

2. **Windows(PowerShell)**:

```bash
mkdir build; cd build
cmake ..
cmake --build .
```

> **Note:** If the build finishes without error, it means all tests passed. If a test fails, the build command will exit with an error and display the failure details.

## Integration

To use this library in any of your existing projects:

1. **Copy Folders**: Move the `src` and `include` folders from this repository into your project directory.

2. **Include Header**: In your C++ file, include the main header:

```cpp
#include "include/cppx.h"
```

3. **Compile**: Ensure your compiler knows where to look for the template files. You may need to add the src and include paths to your compilation command, otherwise run as usual.

## Coding Style

- **Automatic Formatting**: The build system applies `clang-format` rules to all `.cpp`, `.tpp`, and `.h` files automatically.

- **Namespaces**: All components reside within the stl_ext namespace.

## Usage

- **API Reference:** **[Online Documentation](https://ifkabir.github.io/CPPX/)** (Recommended)

- **Version Info:** Check the [Releases page](https://github.com/IFKabir/CPPX/releases) for version-specific notes and changes.
