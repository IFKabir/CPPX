/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "CPPX", "index.html", [
    [ "CPPX — C++23 Extended Data Structures Library", "index.html", "index" ],
    [ "CPPX", "md_docs_2README.html", [
      [ "What's Inside", "md_docs_2README.html#autotoc_md2", null ],
      [ "Requirements", "md_docs_2README.html#autotoc_md4", null ],
      [ "Installation", "md_docs_2README.html#autotoc_md6", [
        [ "Option 1 — Header-only (copy into your project)", "md_docs_2README.html#autotoc_md7", null ],
        [ "Option 2 — Ubuntu PPA", "md_docs_2README.html#autotoc_md8", null ],
        [ "Option 3 — CMake FetchContent", "md_docs_2README.html#autotoc_md9", null ]
      ] ],
      [ "Quick Start", "md_docs_2README.html#autotoc_md11", [
        [ "Self-Balancing Trees", "md_docs_2README.html#autotoc_md12", null ],
        [ "Red-Black Tree", "md_docs_2README.html#autotoc_md13", null ],
        [ "Binary Search Tree", "md_docs_2README.html#autotoc_md14", null ],
        [ "Manual Tree Construction", "md_docs_2README.html#autotoc_md15", null ]
      ] ],
      [ "Tree Visualization", "md_docs_2README.html#autotoc_md17", null ],
      [ "For Developers", "md_docs_2README.html#autotoc_md19", [
        [ "Build & Test", "md_docs_2README.html#autotoc_md20", null ],
        [ "Run Benchmarks", "md_docs_2README.html#autotoc_md21", null ],
        [ "Code Style", "md_docs_2README.html#autotoc_md22", null ],
        [ "PPA Release Workflow", "md_docs_2README.html#autotoc_md23", null ]
      ] ],
      [ "Architecture", "md_docs_2README.html#autotoc_md25", null ],
      [ "Performance", "md_docs_2README.html#autotoc_md27", [
        [ "Key Takeaways", "md_docs_2README.html#autotoc_md28", null ]
      ] ]
    ] ],
    [ "Code of Conduct", "md_docs_2CODE__OF__CONDUCT.html", [
      [ "Summary", "md_docs_2CODE__OF__CONDUCT.html#autotoc_md30", null ],
      [ "Enforcement", "md_docs_2CODE__OF__CONDUCT.html#autotoc_md31", null ]
    ] ],
    [ "Contributing to CPPX", "md_docs_2CONTRIBUTING.html", [
      [ "Reporting Issues", "md_docs_2CONTRIBUTING.html#autotoc_md33", null ],
      [ "Pull Requests", "md_docs_2CONTRIBUTING.html#autotoc_md34", null ],
      [ "Adding a New Data Structure", "md_docs_2CONTRIBUTING.html#autotoc_md35", null ],
      [ "Code Guidelines", "md_docs_2CONTRIBUTING.html#autotoc_md36", [
        [ "Structure", "md_docs_2CONTRIBUTING.html#autotoc_md37", null ],
        [ "Naming", "md_docs_2CONTRIBUTING.html#autotoc_md38", null ],
        [ "Rules", "md_docs_2CONTRIBUTING.html#autotoc_md39", null ]
      ] ],
      [ "Questions?", "md_docs_2CONTRIBUTING.html#autotoc_md40", null ]
    ] ],
    [ "CPPX Developer Guide", "developer_guide.html", [
      [ "Table of Contents", "developer_guide.html#autotoc_md42", null ],
      [ "1. Prerequisites", "developer_guide.html#autotoc_md44", null ],
      [ "2. Repository Layout", "developer_guide.html#autotoc_md46", null ],
      [ "3. Building from Source", "developer_guide.html#autotoc_md48", [
        [ "Standard Build (tests + docs)", "developer_guide.html#autotoc_md49", null ],
        [ "Build Without Documentation", "developer_guide.html#autotoc_md50", null ],
        [ "Build Without Tests or Benchmarks", "developer_guide.html#autotoc_md51", null ],
        [ "Parallel Builds (faster)", "developer_guide.html#autotoc_md52", null ],
        [ "Clean Rebuild", "developer_guide.html#autotoc_md53", null ]
      ] ],
      [ "4. Running the Test Suite", "developer_guide.html#autotoc_md55", [
        [ "Run All Tests via CTest (recommended)", "developer_guide.html#autotoc_md56", null ],
        [ "Run the Test Binary Directly", "developer_guide.html#autotoc_md57", null ],
        [ "Run a Specific Test or Filter", "developer_guide.html#autotoc_md58", null ],
        [ "Test Coverage by File", "developer_guide.html#autotoc_md59", null ],
        [ "Writing a New Test", "developer_guide.html#autotoc_md60", null ]
      ] ],
      [ "5. Running Benchmarks", "developer_guide.html#autotoc_md62", [
        [ "Run via CMake Target (recommended)", "developer_guide.html#autotoc_md63", null ],
        [ "Run the Binary Directly", "developer_guide.html#autotoc_md64", null ],
        [ "Benchmark Output", "developer_guide.html#autotoc_md65", null ],
        [ "Interpreting Results", "developer_guide.html#autotoc_md66", null ],
        [ "Adding a Benchmark for a New Structure", "developer_guide.html#autotoc_md67", null ]
      ] ],
      [ "6. Generating API Documentation", "developer_guide.html#autotoc_md69", null ],
      [ "7. Code Style & Formatting", "developer_guide.html#autotoc_md71", [
        [ "Automatic Formatting", "developer_guide.html#autotoc_md72", null ],
        [ "Manual Formatting", "developer_guide.html#autotoc_md73", null ],
        [ "Style Rules at a Glance", "developer_guide.html#autotoc_md74", null ]
      ] ],
      [ "8. CMake Options Reference", "developer_guide.html#autotoc_md76", [
        [ "CMake Targets", "developer_guide.html#autotoc_md77", null ]
      ] ],
      [ "9. Adding a New Data Structure", "developer_guide.html#autotoc_md79", [
        [ "Step 1 — Declare the class in <tt>include/cppx.h</tt>", "developer_guide.html#autotoc_md80", null ],
        [ "Step 2 — Implement in <tt>src/my_structure.tpp</tt>", "developer_guide.html#autotoc_md81", null ],
        [ "Step 3 — Include the <tt>.tpp</tt> at the bottom of <tt>cppx.h</tt>", "developer_guide.html#autotoc_md82", null ],
        [ "Step 4 — Add tests in <tt>test_suite/my_structure_test_cases.cpp</tt>", "developer_guide.html#autotoc_md83", null ],
        [ "Step 5 — Add benchmarks in <tt>benchmarks/benchmark_main.cpp</tt>", "developer_guide.html#autotoc_md84", null ],
        [ "Step 6 — Rebuild and verify", "developer_guide.html#autotoc_md85", null ]
      ] ],
      [ "10. Architecture Deep-Dive", "developer_guide.html#autotoc_md87", [
        [ "Class Hierarchy", "developer_guide.html#autotoc_md88", null ],
        [ "Arena Allocator (<tt>NodePool<T></tt>)", "developer_guide.html#autotoc_md89", null ],
        [ "Node Layout", "developer_guide.html#autotoc_md90", null ],
        [ "Key Invariants", "developer_guide.html#autotoc_md91", null ]
      ] ],
      [ "11. PPA Release Workflow", "developer_guide.html#autotoc_md93", [
        [ "Prerequisites", "developer_guide.html#autotoc_md94", null ],
        [ "Full Release (interactive)", "developer_guide.html#autotoc_md95", null ],
        [ "Options", "developer_guide.html#autotoc_md96", null ],
        [ "Environment Variables", "developer_guide.html#autotoc_md97", null ],
        [ "After Upload", "developer_guide.html#autotoc_md98", null ]
      ] ],
      [ "12. Troubleshooting", "developer_guide.html#autotoc_md100", [
        [ "Google Test not fetching", "developer_guide.html#autotoc_md101", null ],
        [ "<tt>clang-format</tt> not found — format target skipped", "developer_guide.html#autotoc_md102", null ],
        [ "Doxygen warning: <tt>dot</tt> not found", "developer_guide.html#autotoc_md103", null ],
        [ "Running the <tt>benchmark</tt> binary directly produces wrong output paths", "developer_guide.html#autotoc_md104", null ],
        [ "Tests pass but <tt>ctest</tt> says \"no tests found\"", "developer_guide.html#autotoc_md105", null ],
        [ "Linker errors about missing template instantiations", "developer_guide.html#autotoc_md106", null ]
      ] ]
    ] ],
    [ "Security Policy", "md_docs_2SECURITY.html", [
      [ "Supported Versions", "md_docs_2SECURITY.html#autotoc_md142", null ],
      [ "Reporting a Vulnerability", "md_docs_2SECURITY.html#autotoc_md143", null ]
    ] ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", null ],
        [ "Enumerations", "namespacemembers_enum.html", null ]
      ] ]
    ] ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", null ],
        [ "Functions", "functions_func.html", null ],
        [ "Variables", "functions_vars.html", null ],
        [ "Typedefs", "functions_type.html", null ],
        [ "Related Symbols", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"annotated.html",
"functions_vars.html"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';