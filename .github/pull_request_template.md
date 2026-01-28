# Description

Please include a summary of the changes and the problem you are solving. If this adds a new feature to the **CPPX** library, explain how it is intended to be used.

**Related Issue:** # (Optional - e.g., Fixes #123)

## Type of Change

- [ ] 🐛 Bug fix (non-breaking change which fixes an issue)
- [ ] ✨ New feature (non-breaking change which adds functionality to the library)
- [ ] 💥 Breaking change (fix or feature that would cause existing clients to crash or fail to compile)
- [ ] 📝 Documentation update (README.md or comments)
- [ ] 🔧 Build/Configuration change (Makefiles, CMake, etc.)

# 🧪 Testing Information

Please verify that you have run the automated build script, which handles formatting and testing.

- [ ] **Automated Check**: I ran `./check_code.sh` (or `make`) and it passed without errors.
- [ ] **Manual Testing**: Describe any additional manual checks you performed.

**Test Logs / Screenshots** (Optional):
**Test Environment**:

- **OS:** (e.g., Ubuntu 22.04)
- **Compiler:** (e.g., g++ 11.4 / clang++)
- **GTest Version:** (Managed via CMake)

# ✅ Checklist

- [ ] I have run `./check_code.sh` to **automatically format** my code (Microsoft Style).
- [ ] I have added/updated the necessary **header files (`.h`)**.
- [ ] I have added new Google Tests to cover my changes.
- [ ] All new and existing tests passed locally.
- [ ] I have verified that there are no obvious memory leaks (e.g., pointers cleaned up).
- [ ] _(If applicable)_ I have updated the `README.md` or documentation with usage examples.
