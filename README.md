## CPPX

CPPX is currently a Linux-only C++ shared library that implements utility functions complementing the STL for GNU toolchains.

## Requirements

- Linux distribution
- GNU-compatible C/C++ compiler
- CMake
- git

## Install dependencies

- Ubuntu/Debian

```bash
sudo apt update
sudo apt install build-essential cmake git
```

- Fedora/RHEL

```bash
sudo dnf install cmake git
```

- Arch Linux

```bash
sudo pacman -S cmake git
```

## Building CPPX
### Option A (Recommended for Minimal Hassle)
#### Use precompiled .so & .h

1. Download the `lib-cppx.so` file from the `/build/bin` directory and the `cppx.h` file from the `/include` directory, then move them to the folder where you plan to compile your C++ source file.

```
CPPX/
├── build/
│   └── bin/
│       └── lib-cppx.so
├── include/
    └── cppx.h
```
### Option B (You are own your own now)
#### Build from source .so & .h

1. Clone the repository and enter the project root.

```bash
git clone git@github.com:IFKabir/CPPX.git
cd CPPX
```

2. Create a build directory and configure with CMake.

```bash
mkdir -p build && cd build
cmake ..
```

3. Build the shared library.

```bash
make
```

4. You now can find .so file and .h file in

```
CPPX/
├── build/
│   └── bin/
│       └── lib-cppx.so
├── include/
    └── cppx.h
```

### Run

1. Copy both `lib-cppx.so` and `header.h` into the same directory as your `yourFile.cpp`.

2. Include `header.h` file in your cpp file using `#include "header.h"`.

3. With your file named yourFile.cpp run the following command making sure that `lib-cppx.so` is in the same directory as the executable when you run it.

```bash
g++ -std=c++17 yourFile.cpp \
    ./lib-cppx.so \
    -Wl,-rpath=. \
    -o app
```

2. Run the generated executable

```bash
./app
```

## Usage

- See the markdown file in the include directory for a list of available functions and usage examples.
