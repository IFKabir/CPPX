#!/usr/bin/env bash
# ──────────────────────────────────────────────────────────────
#  ppa-upload.sh — CPPX PPA Update Workflow
# ──────────────────────────────────────────────────────────────
#  Automates the full Launchpad PPA release cycle:
#    1. Validates the environment (tools + GPG key)
#    2. Optionally bumps the debian/changelog
#    3. Builds a signed source package
#    4. Uploads to the configured PPA via dput
#
#  Usage:
#    ./scripts/ppa-upload.sh                  # interactive bump
#    ./scripts/ppa-upload.sh --no-bump        # skip version bump
#    ./scripts/ppa-upload.sh --dry-run        # build only, no upload
#    ./scripts/ppa-upload.sh --help           # show help
#
#  Prerequisites:
#    sudo apt install devscripts debhelper dput dh-make lintian
# ──────────────────────────────────────────────────────────────
set -euo pipefail

# ── Colours ──────────────────────────────────────────────────
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m' # No Colour

# ── Configurable defaults ────────────────────────────────────
PPA="${CPPX_PPA:-ppa:ifkabir/cppx}"
DISTRO="${CPPX_DISTRO:-noble}"
GPG_KEY="${CPPX_GPG_KEY:-}"            # auto-detected if empty

# ── Flags ────────────────────────────────────────────────────
SKIP_BUMP=false
DRY_RUN=false

# ── Helpers ──────────────────────────────────────────────────
info()  { echo -e "${CYAN}[INFO]${NC}  $*"; }
ok()    { echo -e "${GREEN}[OK]${NC}    $*"; }
warn()  { echo -e "${YELLOW}[WARN]${NC}  $*"; }
die()   { echo -e "${RED}[ERROR]${NC} $*" >&2; exit 1; }

usage() {
    cat <<EOF
${BOLD}CPPX PPA Upload Script${NC}

${BOLD}Usage:${NC}
  $0 [OPTIONS]

${BOLD}Options:${NC}
  --no-bump     Skip debian/changelog version bump
  --dry-run     Build the source package but don't upload
  --help        Show this help message

${BOLD}Environment Variables:${NC}
  CPPX_PPA        Target PPA (default: ${PPA})
  CPPX_DISTRO     Target distro series (default: ${DISTRO})
  CPPX_GPG_KEY    GPG key ID for signing (auto-detected if unset)

${BOLD}Example:${NC}
  CPPX_DISTRO=jammy $0 --no-bump

EOF
    exit 0
}

# ── Argument parsing ─────────────────────────────────────────
while [[ $# -gt 0 ]]; do
    case "$1" in
        --no-bump)  SKIP_BUMP=true; shift ;;
        --dry-run)  DRY_RUN=true;   shift ;;
        --help|-h)  usage ;;
        *)          die "Unknown option: $1  (try --help)" ;;
    esac
done

# ── Navigate to repo root ───────────────────────────────────
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
cd "${REPO_ROOT}"
info "Working directory: ${REPO_ROOT}"

# ── Pre-flight checks ───────────────────────────────────────
info "Running pre-flight checks…"

REQUIRED_TOOLS=(debuild dput dpkg-parsechangelog lintian)
for tool in "${REQUIRED_TOOLS[@]}"; do
    command -v "${tool}" >/dev/null 2>&1 \
        || die "'${tool}' not found. Install with: sudo apt install devscripts debhelper dput lintian"
done
ok "All required tools found."

[[ -f debian/changelog ]] || die "debian/changelog not found — are you in the repo root?"
[[ -f debian/control   ]] || die "debian/control not found."
[[ -f debian/rules     ]] || die "debian/rules not found."
ok "Debian packaging files present."

# Auto-detect GPG key if not set
if [[ -z "${GPG_KEY}" ]]; then
    GPG_KEY="$(gpg --list-secret-keys --keyid-format LONG 2>/dev/null \
        | grep '^sec' | grep -oP '/\K[A-F0-9]+' | head -1 || true)"
    [[ -n "${GPG_KEY}" ]] || die "No GPG secret key found. Set CPPX_GPG_KEY or import a key."
    info "Auto-detected GPG key: ${GPG_KEY}"
fi

# ── Parse the current version ────────────────────────────────
CURRENT_VERSION="$(dpkg-parsechangelog -S Version)"
PACKAGE_NAME="$(dpkg-parsechangelog -S Source)"
info "Package: ${BOLD}${PACKAGE_NAME}${NC}  Version: ${BOLD}${CURRENT_VERSION}${NC}"

# ── Optional version bump ────────────────────────────────────
if [[ "${SKIP_BUMP}" == false ]]; then
    echo ""
    echo -e "${BOLD}Current version:${NC} ${CURRENT_VERSION}"
    read -rp "$(echo -e "${CYAN}Enter new version${NC} (e.g. 3.1.0, or leave blank to keep current): ")" NEW_VERSION

    if [[ -n "${NEW_VERSION}" ]]; then
        info "Bumping changelog → ${NEW_VERSION} for ${DISTRO}…"
        DEBEMAIL="$(dpkg-parsechangelog -S Maintainer)" \
        dch --newversion "${NEW_VERSION}" \
            --distribution "${DISTRO}" \
            --urgency medium \
            "New upstream release."
        ok "debian/changelog updated."
    else
        info "Keeping current version ${CURRENT_VERSION}."
    fi
else
    info "Skipping version bump (--no-bump)."
fi

# ── Ensure changelog targets the right distro ────────────────
CHANGELOG_DISTRO="$(dpkg-parsechangelog -S Distribution)"
if [[ "${CHANGELOG_DISTRO}" != "${DISTRO}" ]]; then
    warn "Changelog targets '${CHANGELOG_DISTRO}', but CPPX_DISTRO='${DISTRO}'."
    read -rp "$(echo -e "${YELLOW}Replace with '${DISTRO}'?${NC} [Y/n] ")" REPLY
    if [[ "${REPLY}" =~ ^[Yy]?$ ]]; then
        sed -i "1s/${CHANGELOG_DISTRO}/${DISTRO}/" debian/changelog
        ok "Distribution updated to '${DISTRO}'."
    fi
fi

# ── Clean previous builds ────────────────────────────────────
info "Cleaning previous build artefacts…"
rm -rf build/ obj-x86_64-linux-gnu/ .cache/
rm -rf debian/.debhelper/ debian/cppx-dev/
rm -f  debian/cppx-dev.debhelper.log debian/cppx-dev.substvars \
       debian/debhelper-build-stamp debian/files
rm -f  ../${PACKAGE_NAME}_*.dsc ../${PACKAGE_NAME}_*.tar.* \
       ../${PACKAGE_NAME}_*.changes ../${PACKAGE_NAME}_*.upload \
       ../${PACKAGE_NAME}_*.buildinfo ../${PACKAGE_NAME}_*.build
ok "Clean."

# ── Build the signed source package ──────────────────────────
info "Building signed source package with key ${GPG_KEY}…"
echo ""
debuild -S -sa -k"${GPG_KEY}"
echo ""
ok "Source package built successfully."

# ── Lint check ───────────────────────────────────────────────
CHANGES_FILE="$(ls -t ../${PACKAGE_NAME}_*_source.changes 2>/dev/null | head -1)"
if [[ -n "${CHANGES_FILE}" ]]; then
    info "Running lintian on ${CHANGES_FILE}…"
    lintian "${CHANGES_FILE}" || warn "Lintian reported warnings (non-fatal)."
else
    warn "Could not find .changes file for lintian."
fi

# ── Upload ───────────────────────────────────────────────────
if [[ "${DRY_RUN}" == true ]]; then
    warn "Dry-run mode — skipping upload."
    info "You can manually upload with:"
    echo "  dput ${PPA} ${CHANGES_FILE}"
else
    info "Uploading to ${BOLD}${PPA}${NC}…"
    dput "${PPA}" "${CHANGES_FILE}"
    echo ""
    ok "Upload complete! 🚀"
    info "Launchpad will build the package shortly."
    info "Check status: https://launchpad.net/~ifkabir/+archive/ubuntu/cppx"
fi

echo ""
echo -e "${GREEN}${BOLD}✓ PPA update workflow finished.${NC}"
