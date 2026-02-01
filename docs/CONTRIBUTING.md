# Contributing to CPPX (stl_ext)

Thank you for your interest in contributing to `CPPX`!

As a solo maintainer, I appreciate any help—whether it's fixing bugs, improving documentation, or suggesting new features. Please take a moment to review this document to make the contribution process easy and effective for everyone.

## Code of Conduct

This project and everyone participating in it is governed by the [Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code.

## How Can I Contribute?

### 1. Reporting Bugs

If you find a bug, please create a GitHub Issue.

- **Check existing issues** to see if it has already been reported.
- **Be specific**: Include the code that caused the error, the expected behavior, and the actual behavior.
- **Security**: If you find a security vulnerability, please do **not** open a public issue. Refer to [SECURITY.md](SECURITY.md) for instructions.

### 2. Suggesting Enhancements

Have an idea for a new data structure or algorithm?

- Open an issue with the tag **enhancement**.
- Explain why this feature would be useful to the library.

### 3. Pull Requests

I welcome Pull Requests (PRs)!

1.  **Fork** the repository.
2.  **Create a branch** for your feature or fix (`git checkout -b feature/amazing-feature`).
3.  **Write Tests**: Ensure your change is covered by unit tests in `test_suite/`.
4.  **Build & Format**: Run the build command locally (`cmake --build build`). This will automatically format your code using `clang-format`.
5.  **Run Tests**: Ensure all tests pass (`ctest` or ./`unit_tests`)
6.  **Commit** your changes.
7.  **Push** to the branch (`git push origin feature/amazing-feature`).
8.  **Open a Pull Request** targeting the `main` branch.

Please keep your PRs focused on a single issue or feature.

## Development & Coding Guidelines

To keep the codebase consistent, please follow these guidelines:

### Project Structure (Template Library)

This library uses a header-only style with separated implementation files to keep code clean.

- **Header Files** (`.h`): Contain class declarations and documentation. (e.g., `include/cppx.h`).
- **Implementation Files** (`.tpp`): Contain the actual template logic.
  - Do not use `.cpp` for template classes.
  - Implementations in `.tpp` files are automatically included at the bottom of the main header.

### Testing

We use **GoogleTest** for unit testing.

- Every new feature must be accompanied by a corresponding test file in the `test_suite/` directory.
- Run tests locally before submitting using `ctest` in your build directory.

### Namespaces

- All code must be contained within the `namespace stl_ext`.
- Do not use `using namespace std;` in header files.

## Style Guide

We enforce a consistent coding style automatically.

### Automated Formatting

**Do not format manually**. The build system uses `clang-format` to enforce Microsoft Style (Allman braces, 4-space indent). simply running the build command will clean up your code.

### Naming Conventions

| Entity                | Style                  | Example                                             |
| :-------------------- | :--------------------- | :-------------------------------------------------- |
| **Classes**           | PascalCase             | `BinaryTree`, `Node`, `BST`                         |
| **Files**             | snake_case             | `binary_tree.cpp`, `node.cpp`, `cppx.h`             |
| **Functions**         | snake_case             | `get_min()`, `compute_size()`, `insert_iterative()` |
| **Variables (Local)** | snake_case             | `current_node`, `succ_parent`                       |
| **Member Variables**  | Hungarian-style prefix | `m_data` (member), `p_head` (pointer)               |

### C++ Specifics

- **Smart Pointers:** Use `std::unique_ptr` for ownership and memory management. Avoid raw `new`/`delete`.
- **Move Semantics**: Utilize `std::move` when transferring ownership of nodes.
- **Template Instantiation:** Place all template definitions in `.tpp` files. Do not explicit instantiate templates for specific types (e.g., `int`, `double`) inside the library code; let the compiler handle instantiation based on user usage.

### Important Concepts for Contributors

When working on data structures or database-related components in `stl_ext`, keep these principles in mind:

#### Data Management & Integrity

- **Data vs. Information:** Ensure your implementations treat raw data correctly so it can be processed into meaningful information.
- **Data Integrity:** Avoid data redundancy. Ensure methods (like `insert` or `remove`) maintain the structural integrity of the tree (e.g., preserving BST properties).

#### Architecture

- **Abstraction:** Keep the internal node logic (`Node`class) distinct from the tree operations (`BST` class).
- **Schema Independence:** Internal changes to how nodes are linked should not break the public API of the `BST` or `BinaryTree` classes.

## Questions?

If you have questions, feel free to open an issue or contact me via the method listed in the [Code of Conduct](CODE_OF_CONDUCT.md).

---

_Thank you for helping improve CPPX!_
