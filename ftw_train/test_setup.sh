#!/bin/bash
# ======================================
# Test setup for OPS Lab – Royal Archive
# ======================================

ROOT_DIR="archive_test"
REAL_DIR="$ROOT_DIR/real_archive"

echo "Creating test environment in $ROOT_DIR..."

# Clean previous test dirs
rm -rf "$ROOT_DIR"
mkdir -p "$REAL_DIR"

# Create directories according to historical map (some missing, some extra)
mkdir -p "$REAL_DIR/Hall/Treasury/Vault"
mkdir -p "$REAL_DIR/Hall/Library/Scriptorium"
mkdir -p "$REAL_DIR/Hall/Library/Annex/HiddenDrawer"

# Create example files
echo "Golden coins inventory" > "$REAL_DIR/Hall/Treasury/Coins.txt"
echo "Ancient scroll of wisdom" > "$REAL_DIR/Hall/Library/Scriptorium/OldScroll.txt"
echo "Confidential notes" > "$REAL_DIR/Hall/Treasury/Vault/ExtraNotes.txt"
echo "Visitor list" > "$REAL_DIR/Hall/Entrance.txt"
echo "Secret documents (empty for now)" > "$REAL_DIR/Hall/Library/Annex/HiddenDrawer/.keep"

# Create manuscript
cat > "$ROOT_DIR/manuscript.txt" <<EOF
# The Royal Archive Map (Historical Manuscript)
Hall Treasury
Hall Library
Library Scriptorium
Treasury Vault
Vault SecretRoom
EOF

echo "✅ Archive test environment ready!"
echo "Try scanning: ./scan_archive $REAL_DIR 4"
echo "Or comparing: ./compare_archive $ROOT_DIR/manuscript.txt $REAL_DIR 4"
