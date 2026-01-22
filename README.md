## CPPX

CPPX is currently a Linux-only C++ shared library that implements utility functions complementing the STL for GNU toolchains.

## Requirements

- Linux distribution
- GNU-compatible C/C++ compiler (GCC)
- CMake (3.10+)
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
### Option A (Fastest)
#### Use precompiled .so & .h

1. Export the ```libcppx.so``` file and the ```cppx.h``` file from a release build.

2. Move them to the folder where you plan to compile your C++ source file.

```
CPPX/
├── build/
│   └── bin/
│       └── libcppx.so
└── include/
    └── cppx.h
```
### Option B
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
│       └── libcppx.so
└── include/
    └── cppx.h
```

### Integration & Running

1. Copy both `libcppx.so` and `cppx.h` into the same directory as your source file (e.g., `main.cpp`).

2. Include the header in your C++ file using #include ```"cppx.h"```.

3. Compile your application. You must link the .so file and set the runtime path (rpath) so the executable can find the library.

```bash
g++ -std=c++17 main.cpp ./libcppx.so -Wl,-rpath=. -o app
```

4. Run the generated executable

```bash
./app
```

## Usage

- See the corresponding release that was published.
