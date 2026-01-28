# Contributing to stl_ext

Thank you for your interest in contributing to `stl_ext`!

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
3.  **Commit** your changes.
4.  **Push** to the branch (`git push origin feature/amazing-feature`).
5.  **Open a Pull Request** targeting the `main` branch.

Please keep your PRs focused on a single issue or feature.

## Development & Coding Guidelines

To keep the codebase consistent, please follow these guidelines:

### C++ Standards

- The project uses **Modern C++** (smart pointers, `nullptr`, etc.).
- Please ensure your code compiles without warnings.

### Template Instantiation (Important!)

This library separates template declarations (`header.h`) from implementations (`.cpp` files).

- If you add a new template class or function, you must write the implementation in a `.cpp` file.
- **Explicit Instantiation**: You must explicitly instantiate the template for supported types (e.g., `int`, `double`, `float`) at the bottom of the `.cpp` file.
  - _Example:_ `template class stl_ext::BST<int>;`

### Namespaces

- All code must be contained within the `namespace stl_ext`.
- Do not use `using namespace std;` in header files.

## Style Guide

To keep the codebase clean and consistent, please adhere to the following naming conventions:

### Naming Conventions

| Entity                | Style                  | Example                                             |
| :-------------------- | :--------------------- | :-------------------------------------------------- |
| **Classes**           | PascalCase             | `BinaryTree`, `Node`, `BST`                         |
| **Files**             | snake_case             | `binary_tree.cpp`, `node.cpp`, `cppx.h`             |
| **Functions**         | snake_case             | `get_min()`, `compute_size()`, `insert_iterative()` |
| **Variables (Local)** | snake_case             | `current_node`, `succ_parent`                       |
| **Member Variables**  | Hungarian-style prefix | `m_data` (member), `p_head` (pointer)               |

### C++ Specifics

- **Namespaces:** All code must be wrapped in `namespace stl_ext`.
- **No `using namespace std`:** Do not use `using namespace std;` in header files (`.h`). It is acceptable in implementation files (`.cpp`) if it improves readability, but explicit qualification (`std::`) is preferred.
- **Smart Pointers:** Use `std::unique_ptr` for ownership and memory management. Avoid raw `new`/`delete`.
- **Template Instantiation:** Because implementation is separated into `.cpp` files, you **must** explicitly instantiate templates at the bottom of the `.cpp` file for all supported types (int, double, float, char, long).

### Important Concepts for Contributors

When working on data structures or database-related components in `stl_ext`, keep these principles in mind:

#### Data Management

- **Data vs. Information:** Ensure your implementations treat raw data correctly so it can be processed into meaningful information.
- **Data Integrity:** Avoid data redundancy and inconsistency in your structures. Ensure methods maintain data integrity and atomicity where applicable.

#### Architecture & Design

- **Abstraction:** Aim for a clear separation between the logical level (how users interact with the class) and the physical/internal level (how data is stored/managed internally).
- **Schema Independence:** Internal changes to data storage (physical schema) should not break the public API (logical schema).

#### Entity Modeling

- **Entities:** Treat objects as distinct entities.
- **Attributes:** Handle single-valued, composite, and multi-valued attributes appropriately within your classes.
- **Constraints:** Enforce domain constraints (valid data types/values) and entity constraints (e.g., valid keys) in your implementations.

## Questions?

If you have questions, feel free to open an issue or contact me via the method listed in the Code of Conduct.

---

_Thank you for helping improve stl_ext!_
