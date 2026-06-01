#!/usr/bin/env bash
# ──────────────────────────────────────────────────────────────
#  bump-version.sh — CPPX Version Management
# ──────────────────────────────────────────────────────────────
#  Single-command version bumper. Updates the version across
#  all project files from a single source of truth (CMakeLists.txt).
#
#  Usage:
#    ./scripts/bump-version.sh <new-version>   # e.g. 3.2.0
#    ./scripts/bump-version.sh --sync          # sync all files to CMakeLists.txt version
#    ./scripts/bump-version.sh --show          # print the current version
#    ./scripts/bump-version.sh --help
#
#  Files updated:
#    - CMakeLists.txt        (project VERSION)
#    - docs/SECURITY.md      (supported version table)
#    - debian/changelog      (new changelog entry prepended)
# ──────────────────────────────────────────────────────────────
set -euo pipefail

# ── Colours ──────────────────────────────────────────────────
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m'

info()  { echo -e "${CYAN}[INFO]${NC}  $*"; }
ok()    { echo -e "${GREEN}[OK]${NC}    $*"; }
warn()  { echo -e "${YELLOW}[WARN]${NC}  $*"; }
die()   { echo -e "${RED}[ERROR]${NC} $*" >&2; exit 1; }

usage() {
    cat <<EOF
${BOLD}CPPX Version Bump Script${NC}

${BOLD}Usage:${NC}
  $0 <new-version>     Set a new version (e.g. 3.2.0)
  $0 --sync            Sync all files to the version in CMakeLists.txt
  $0 --show            Print the current version and exit
  $0 --help            Show this help message

${BOLD}Files updated:${NC}
  • CMakeLists.txt          project(CPPX VERSION x.y.z ...)
  • docs/SECURITY.md        Supported version table
  • debian/changelog        Prepends a new changelog entry

${BOLD}Examples:${NC}
  $0 3.3.0              # bump to 3.3.0 everywhere
  $0 --sync             # after manually editing CMakeLists.txt

EOF
    exit 0
}

# ── Navigate to repo root ───────────────────────────────────
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
cd "${REPO_ROOT}"

# ── Read current version from CMakeLists.txt ────────────────
get_cmake_version() {
    grep -oP 'project\(CPPX\s+VERSION\s+\K[0-9]+\.[0-9]+\.[0-9]+' CMakeLists.txt
}

CURRENT_VERSION="$(get_cmake_version)" || die "Could not parse version from CMakeLists.txt"

# ── Argument parsing ─────────────────────────────────────────
if [[ $# -eq 0 ]]; then
    usage
fi

case "$1" in
    --help|-h)  usage ;;
    --show)
        echo "${CURRENT_VERSION}"
        exit 0
        ;;
    --sync)
        NEW_VERSION="${CURRENT_VERSION}"
        info "Syncing all files to CMakeLists.txt version: ${BOLD}${NEW_VERSION}${NC}"
        ;;
    -*)
        die "Unknown option: $1  (try --help)"
        ;;
    *)
        NEW_VERSION="$1"
        # Validate semver format
        if ! [[ "${NEW_VERSION}" =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
            die "Invalid version format '${NEW_VERSION}'. Expected: X.Y.Z (e.g. 3.2.0)"
        fi
        ;;
esac

OLD_VERSION="${CURRENT_VERSION}"
info "Current version: ${BOLD}${OLD_VERSION}${NC}"
info "Target version:  ${BOLD}${NEW_VERSION}${NC}"

# ── 1. Update CMakeLists.txt ────────────────────────────────
if [[ "${OLD_VERSION}" != "${NEW_VERSION}" ]]; then
    sed -i "s/project(CPPX VERSION ${OLD_VERSION}/project(CPPX VERSION ${NEW_VERSION}/" CMakeLists.txt
    ok "CMakeLists.txt → ${NEW_VERSION}"
else
    ok "CMakeLists.txt already at ${NEW_VERSION}"
fi

# ── 2. Update docs/SECURITY.md ──────────────────────────────
if [[ -f docs/SECURITY.md ]]; then
    cat > docs/SECURITY.md <<SECEOF
# Security Policy

## Supported Versions

| Version | Supported |
| ------- | --------- |
| ${NEW_VERSION}   | ✅        |
| < ${NEW_VERSION} | ❌        |

## Reporting a Vulnerability

**Do not** open a public issue. Email **ifk.ishanfalak@gmail.com** with:

- Type of issue (buffer overflow, memory leak, etc.)
- Steps to reproduce
- Proof-of-concept if possible

**Response**: within 48 hours. Fix and coordinated disclosure to follow.
SECEOF
    ok "docs/SECURITY.md → ${NEW_VERSION}"
fi

# ── 3. Update debian/changelog ──────────────────────────────
if [[ -f debian/changelog ]]; then
    CHANGELOG_TOP_VERSION="$(head -1 debian/changelog | grep -oP '\(\K[^)]+' || true)"
    if [[ "${CHANGELOG_TOP_VERSION}" != "${NEW_VERSION}" ]]; then
        MAINTAINER="Ishan Falak Kabir <ifk.ishanfalak@gmail.com>"
        TIMESTAMP="$(date -R)"
        DISTRO="noble"

        # Prepend new entry
        NEW_ENTRY="cppx (${NEW_VERSION}) ${DISTRO}; urgency=medium

  * New upstream release.

 -- ${MAINTAINER}  ${TIMESTAMP}
"
        echo "${NEW_ENTRY}" | cat - debian/changelog > debian/changelog.tmp
        mv debian/changelog.tmp debian/changelog
        ok "debian/changelog → ${NEW_VERSION}"
    else
        ok "debian/changelog already at ${NEW_VERSION}"
    fi
fi

# ── Summary ─────────────────────────────────────────────────
echo ""
echo -e "${GREEN}${BOLD}✓ Version bumped: ${OLD_VERSION} → ${NEW_VERSION}${NC}"
echo ""
echo -e "Updated files:"
echo -e "  • CMakeLists.txt"
echo -e "  • docs/SECURITY.md"
echo -e "  • debian/changelog"
echo ""
echo -e "${CYAN}Tip:${NC} Rebuild with: cd build && cmake .. && cmake --build ."
