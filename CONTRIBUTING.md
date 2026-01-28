# Contributing to stl_ext

Thank you for your interest in contributing to `stl_ext`!

As a solo maintainer, I appreciate any help—whether it's fixing bugs, improving documentation, or suggesting new features. Please take a moment to review this document to make the contribution process easy and effective for everyone.

## Code of Conduct

This project and everyone participating in it is governed by the [Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code.

## How Can I Contribute?

### 1. Reporting Bugs
If you find a bug, please create a GitHub Issue.
* **Check existing issues** to see if it has already been reported.
* **Be specific**: Include the code that caused the error, the expected behavior, and the actual behavior.
* **Security**: If you find a security vulnerability, please do **not** open a public issue. Refer to [SECURITY.md](SECURITY.md) for instructions.

### 2. Suggesting Enhancements
Have an idea for a new data structure or algorithm?
* Open an issue with the tag **enhancement**.
* Explain why this feature would be useful to the library.

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
* The project uses **Modern C++** (smart pointers, `nullptr`, etc.).
* Please ensure your code compiles without warnings.

### Template Instantiation (Important!)
This library separates template declarations (`header.h`) from implementations (`.cpp` files).
* If you add a new template class or function, you must write the implementation in a `.cpp` file.
* **Explicit Instantiation**: You must explicitly instantiate the template for supported types (e.g., `int`, `double`, `float`) at the bottom of the `.cpp` file.
    * *Example:* `template class stl_ext::BST<int>;`

### Namespaces
* All code must be contained within the `namespace stl_ext`.
* Do not use `using namespace std;` in header files.

## Style Guide
* **Indentation**: Follow the existing indentation style (4 spaces).
* **Naming**:
    * Classes: `PascalCase` (e.g., `BinaryTree`)
    * Functions/Methods: `snake_case` (e.g., `get_min`, `insert`)
    * Member variables: `m_prefix` or `snake_case` (e.g., `m_data`, `p_head`).

## Questions?
If you have questions, feel free to open an issue or contact me via the method listed in the Code of Conduct.

---
*Thank you for helping improve stl_ext!*
