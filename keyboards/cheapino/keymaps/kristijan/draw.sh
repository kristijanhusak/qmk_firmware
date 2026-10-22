#!/usr/bin/env bash
# Draws all layers of this keymap to keymap.svg using keymap-drawer
# (pipx install keymap-drawer). Combos come from combos.yaml.
set -euo pipefail

here="$(cd "$(dirname "$0")" && pwd)"
qmk_root="$(cd "$here/../../../.." && pwd)"
config="$here/keymap-drawer.yaml"
tmp="$(mktemp -d)"
trap 'rm -rf "$tmp"' EXIT

cd "$qmk_root"

# Layer names must match the order of `enum layers` in keymap.c
qmk c2json -kb cheapino -km kristijan "$here/keymap.c" 2>/dev/null |
    sed 's/\b_BASE\b/0/g; s/\b_NAV\b/1/g; s/\b_SYM\b/2/g; s/\b_FUN\b/3/g' > "$tmp/keymap.json"

keymap -c "$config" parse -q "$tmp/keymap.json" -l BASE NAV SYM FUN -c 10 > "$tmp/keymap.yaml"

keymap -c "$config" draw \
    -j "$qmk_root/keyboards/cheapino/keyboard.json" -l LAYOUT_split_3x5_3 \
    "$tmp/keymap.yaml" "$here/combos.yaml" > "$here/keymap.svg"

echo "Wrote $here/keymap.svg"
