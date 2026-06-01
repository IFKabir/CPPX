#!/usr/bin/env bash
# ──────────────────────────────────────────────────────────────
#  cppx.sh — CPPX Project Manager
# ──────────────────────────────────────────────────────────────
#  A single entry point for every project workflow.
#
#  Usage:
#    ./scripts/cppx.sh <command> [options]
#
#  Commands:
#    build       Configure & compile the project
#    test        Run the full test suite via CTest
#    bench       Run the benchmark suite
#    docs        Generate Doxygen API documentation
#    format      Run clang-format on all source files
#    clean       Remove the build directory entirely
#    rebuild     Clean + build from scratch
#    version     Show, bump, or sync the project version
#    release     Run the PPA upload workflow
#    all         Build + test + bench + docs (full pipeline)
#    help        Show this help message
# ──────────────────────────────────────────────────────────────
set -euo pipefail

# ── Colours ──────────────────────────────────────────────────
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
BOLD='\033[1m'
DIM='\033[2m'
NC='\033[0m'

# ── Helpers ──────────────────────────────────────────────────
info()    { echo -e "${CYAN}[INFO]${NC}    $*"; }
ok()      { echo -e "${GREEN}[OK]${NC}      $*"; }
warn()    { echo -e "${YELLOW}[WARN]${NC}    $*"; }
err()     { echo -e "${RED}[ERROR]${NC}   $*" >&2; }
die()     { err "$*"; exit 1; }
section() { echo -e "\n${MAGENTA}${BOLD}━━━ $* ━━━${NC}\n"; }

# ── Navigate to repo root ───────────────────────────────────
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
cd "${REPO_ROOT}"

BUILD_DIR="${REPO_ROOT}/build"
JOBS="$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)"

# ── Timing helper ───────────────────────────────────────────
timer_start() { TIMER_START="$(date +%s)"; }
timer_end() {
    local elapsed=$(( $(date +%s) - TIMER_START ))
    local mins=$(( elapsed / 60 ))
    local secs=$(( elapsed % 60 ))
    if [[ ${mins} -gt 0 ]]; then
        echo -e "${DIM}⏱  ${mins}m ${secs}s${NC}"
    else
        echo -e "${DIM}⏱  ${secs}s${NC}"
    fi
}

# ── Get current version ─────────────────────────────────────
get_version() {
    grep -oP 'project\(CPPX\s+VERSION\s+\K[0-9]+\.[0-9]+\.[0-9]+' CMakeLists.txt
}

# ══════════════════════════════════════════════════════════════
#  COMMANDS
# ══════════════════════════════════════════════════════════════

cmd_build() {
    local build_type="${1:-Debug}"
    section "Building CPPX (${build_type})"
    timer_start

    mkdir -p "${BUILD_DIR}"
    info "Configuring with CMake..."
    cmake -S . -B "${BUILD_DIR}" \
        -DCMAKE_BUILD_TYPE="${build_type}" \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        2>&1

    info "Compiling with ${JOBS} parallel jobs..."
    cmake --build "${BUILD_DIR}" --parallel "${JOBS}" 2>&1

    ok "Build complete."
    timer_end
}

cmd_test() {
    local filter="${1:-}"
    section "Running Tests"
    timer_start

    if [[ ! -f "${BUILD_DIR}/unit_tests" ]]; then
        warn "Build not found. Building first..."
        cmd_build "Debug"
    fi

    if [[ -n "${filter}" ]]; then
        info "Filter: ${filter}"
        cd "${BUILD_DIR}" && ./unit_tests --gtest_filter="${filter}"
    else
        cd "${BUILD_DIR}" && ctest --output-on-failure --parallel "${JOBS}"
    fi

    local exit_code=$?
    cd "${REPO_ROOT}"

    if [[ ${exit_code} -eq 0 ]]; then
        ok "All tests passed."
    else
        err "Some tests failed."
    fi
    timer_end
    return ${exit_code}
}

cmd_bench() {
    section "Running Benchmarks"
    timer_start

    if [[ ! -f "${BUILD_DIR}/benchmark" ]]; then
        warn "Benchmark binary not found. Building first..."
        cmd_build "Release"
    fi

    info "Executing benchmark suite..."
    cmake --build "${BUILD_DIR}" --target run_benchmark 2>&1

    ok "Benchmarks complete."
    if [[ -f docs/benchmark_results.csv ]]; then
        info "Results: docs/benchmark_results.csv"
        info "Chart:   docs/benchmark_chart.svg"
    fi
    timer_end
}

cmd_docs() {
    section "Generating Documentation"
    timer_start

    if [[ ! -d "${BUILD_DIR}" ]]; then
        warn "Build directory not found. Configuring first..."
        mkdir -p "${BUILD_DIR}"
        cmake -S . -B "${BUILD_DIR}" -DCPPX_BUILD_DOCS=ON 2>&1
    fi

    info "Building Doxygen docs..."
    cmake --build "${BUILD_DIR}" --target doc_doxygen 2>&1

    ok "Documentation generated."
    if [[ -f "${BUILD_DIR}/html/index.html" ]]; then
        info "Open: ${BUILD_DIR}/html/index.html"
    fi
    timer_end
}

cmd_format() {
    section "Formatting Code"
    timer_start

    if [[ ! -d "${BUILD_DIR}" ]]; then
        warn "Build directory not found. Configuring first..."
        mkdir -p "${BUILD_DIR}"
        cmake -S . -B "${BUILD_DIR}" 2>&1
    fi

    cmake --build "${BUILD_DIR}" --target format_code 2>&1

    ok "Code formatted."
    timer_end
}

cmd_clean() {
    section "Cleaning Build"

    if [[ -d "${BUILD_DIR}" ]]; then
        rm -rf "${BUILD_DIR}"
        ok "Removed ${BUILD_DIR}"
    else
        info "Build directory doesn't exist. Nothing to clean."
    fi
}

cmd_rebuild() {
    cmd_clean
    cmd_build "${1:-Debug}"
}

cmd_version() {
    local subcmd="${1:-show}"
    shift 2>/dev/null || true

    case "${subcmd}" in
        show)
            echo "$(get_version)"
            ;;
        bump)
            local new_ver="${1:-}"
            if [[ -z "${new_ver}" ]]; then
                die "Usage: $0 version bump <X.Y.Z>"
            fi
            ./scripts/bump-version.sh "${new_ver}"
            ;;
        sync)
            ./scripts/bump-version.sh --sync
            ;;
        *)
            die "Unknown version subcommand: ${subcmd}. Use: show | bump <X.Y.Z> | sync"
            ;;
    esac
}

cmd_release() {
    section "PPA Release"

    if [[ ! -x scripts/ppa-upload.sh ]]; then
        die "scripts/ppa-upload.sh not found or not executable."
    fi

    ./scripts/ppa-upload.sh "$@"
}

cmd_all() {
    local start_all="$(date +%s)"

    section "CPPX Full Pipeline"
    info "Version: $(get_version)"
    echo ""

    # 1. Clean + Build
    cmd_rebuild "Release"

    # 2. Test
    cmd_test

    # 3. Benchmark
    cmd_bench

    # 4. Docs
    cmd_docs

    # 5. Summary
    local total=$(( $(date +%s) - start_all ))
    local mins=$(( total / 60 ))
    local secs=$(( total % 60 ))

    echo ""
    section "Pipeline Complete"
    echo -e "  ${GREEN}✔${NC} Build     ${GREEN}passed${NC}"
    echo -e "  ${GREEN}✔${NC} Tests     ${GREEN}passed${NC}"
    echo -e "  ${GREEN}✔${NC} Benchmark ${GREEN}complete${NC}"
    echo -e "  ${GREEN}✔${NC} Docs      ${GREEN}generated${NC}"
    echo ""
    echo -e "  ${DIM}Total time: ${mins}m ${secs}s${NC}"
    echo ""
}

cmd_help() {
    local ver
    ver="$(get_version 2>/dev/null || echo '?')"

    cat <<EOF

${BOLD}${CYAN}  ╔══════════════════════════════════╗
  ║     CPPX Project Manager v${ver}    ║
  ╚══════════════════════════════════╝${NC}

${BOLD}Usage:${NC}
  ./scripts/cppx.sh <command> [options]

${BOLD}Commands:${NC}
  ${GREEN}build${NC}   [Release|Debug]     Configure & compile (default: Debug)
  ${GREEN}test${NC}    [filter]            Run tests (optional gtest filter)
  ${GREEN}bench${NC}                       Run the benchmark suite
  ${GREEN}docs${NC}                        Generate Doxygen HTML documentation
  ${GREEN}format${NC}                      Run clang-format on all sources
  ${GREEN}clean${NC}                       Remove the build directory
  ${GREEN}rebuild${NC} [Release|Debug]     Clean + build from scratch

  ${GREEN}version${NC} show               Print current version
  ${GREEN}version${NC} bump <X.Y.Z>       Bump version across all files
  ${GREEN}version${NC} sync               Sync files to CMakeLists.txt version

  ${GREEN}release${NC} [options]           Run PPA upload (passes args to ppa-upload.sh)
  ${GREEN}all${NC}                         Full pipeline: rebuild + test + bench + docs

${BOLD}Examples:${NC}
  ./scripts/cppx.sh build Release     # optimized build
  ./scripts/cppx.sh test "AVL*"       # run only AVL tests
  ./scripts/cppx.sh version bump 3.3.0
  ./scripts/cppx.sh all               # the everything button
  ./scripts/cppx.sh release --dry-run # test PPA workflow

EOF
}

# ══════════════════════════════════════════════════════════════
#  DISPATCH
# ══════════════════════════════════════════════════════════════

COMMAND="${1:-help}"
shift 2>/dev/null || true

case "${COMMAND}" in
    build)    cmd_build "$@" ;;
    test)     cmd_test "$@" ;;
    bench)    cmd_bench "$@" ;;
    docs)     cmd_docs "$@" ;;
    format)   cmd_format "$@" ;;
    clean)    cmd_clean ;;
    rebuild)  cmd_rebuild "$@" ;;
    version)  cmd_version "$@" ;;
    release)  cmd_release "$@" ;;
    all)      cmd_all ;;
    help|-h|--help) cmd_help ;;
    *)        err "Unknown command: ${COMMAND}"; cmd_help; exit 1 ;;
esac
