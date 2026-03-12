# Contributing to CPPX

## Reporting Issues

- Check existing issues first.
- Include: code that caused the error, expected vs actual behavior.
- Security issues → see [SECURITY.md](SECURITY.md), **not** a public issue.

## Pull Requests

1. Fork → create branch (`feature/my-feature`)
2. Add tests in `test_suite/`
3. Build (`cmake --build build`) — this auto-formats and runs tests
4. Push → open PR to `main`

Keep PRs focused on one thing.

## Code Guidelines

### Structure

| File Type | Purpose |
|---|---|
| `.h` (in `include/`) | Class declarations |
| `.tpp` (in `src/`) | Template implementations (included at bottom of `.h`) |
| `.cpp` (in `test_suite/`) | Unit tests (Google Test) |

### Naming

| Entity | Style | Example |
|---|---|---|
| Classes | PascalCase | `BinaryTree`, `AVLTree` |
| Files | snake_case | `binary_tree.tpp` |
| Functions | snake_case | `get_min()`, `insert_iterative()` |
| Members | Prefixed | `m_data`, `p_head` |

### Rules

- All code in `namespace stl_ext`
- Use `std::unique_ptr` for ownership — no raw `new`/`delete`
- Use `std::move` for node transfers
- No `using namespace std;` in headers
- Template definitions go in `.tpp` files, not `.cpp`

## Questions?

Open an issue or email via [Code of Conduct](CODE_OF_CONDUCT.md).
