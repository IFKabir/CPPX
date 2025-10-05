### ---CPPX

CPPX is currently a Linux-only C++ shared library that implements utility functions complementing the STL for GNU toolchains.

### ---Requirements

- Linux with a GNU-compatible toolchain, CMake, git, C/C++ compiler.

### ---Install dependencies

- Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential cmake git
```

- Fedora/RHEL
```bash
sudo dnf install cmake
```

- Arch Linux
```bash
sudo pacman -S cmake
```

### ---Use precompiled .so & .h

If a distribution folder contains lib-cppx.so and cppx.h, the project can be used without building from source.
```
CPPX/
├── build/
│   └── bin/
│       └── lib-cppx.so
├── include/
    └── cppx.h
```

### ---Build from source .so & .h

1) Clone the repository and enter the project root.
```bash
git clone <git@github.com:IFKabir/CPPX.git>
cd CPPX
```

2) Create a build directory and configure with CMake.
```bash
mkdir -p build && cd build
cmake ..
```

3) Build the shared library.
```bash
make
```

4) You now can find .so file and .h file in
```
CPPX/
├── build/
│   └── bin/
│       └── lib-cppx.so
├── include/
    └── cppx.h
```

### ---Using the library

1) Compile your C++ source file
```bash
g++ -std=c++17 yourFile.cpp \
    -I/path/to/cppx.h \
    -L/path/to/lib-cppx.so \
    -lcppx \
    -Wl,-rpath=/path/to/CPPX \
    -o yourFile
```
2) Run the generated executable
```bash
./yourFile
```

### ---Functions available

The authoritative list of implemented functions is declared in include/cppx.h; review this header to know exactly what the library exposes in each release.

### ---Notes

- The library currently targets Linux-only environments with GNU toolchain assumptions for compilation and linking behavior.

- If not installing system-wide, embedding an rpath during linking avoids reliance on environment variables at runtime for shared library discovery.

- You can find the implementations of the functions in the /src folder.