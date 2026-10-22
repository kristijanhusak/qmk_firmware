#!/usr/bin/env bash
# Draws all layers of this keymap to keymap.svg using keymap-drawer
# (pipx install keymap-drawer). Combos come from combos.yaml, encoder labels
# from the ENC_* entries in keymap-drawer.yaml.
set -euo pipefail

here="$(cd "$(dirname "$0")" && pwd)"
qmk_root="$(cd "$here/../../../.." && pwd)"
config="$here/keymap-drawer.yaml"
tmp="$(mktemp -d)"
trap 'rm -rf "$tmp"' EXIT

# Layer names must match the order of `enum layers` in keymap.c
layers=(BASE NAV SYM FUN)

cd "$qmk_root"

qmk c2json -kb cheapino -km kristijan "$here/keymap.c" 2>/dev/null |
    sed 's/\b_BASE\b/0/g; s/\b_NAV\b/1/g; s/\b_SYM\b/2/g; s/\b_FUN\b/3/g' > "$tmp/keymap.json"

# Append an ENC_<layer> placeholder key to every layer, and an encoder key
# between the halves to the physical layout.
python3 - "$tmp" "$qmk_root/keyboards/cheapino/keyboard.json" "${layers[@]}" <<'EOF'
import json, sys

tmp, info_path, *layers = sys.argv[1:]

with open(f"{tmp}/keymap.json") as f:
    keymap = json.load(f)
for name, layer in zip(layers, keymap["layers"]):
    layer.append(f"ENC_{name}")
with open(f"{tmp}/keymap.json", "w") as f:
    json.dump(keymap, f)

with open(info_path) as f:
    info = json.load(f)
layout = info["layouts"]["LAYOUT_split_3x5_3"]["layout"]
layout.append({"matrix": [3, 0], "x": 5.2, "y": 0.25, "w": 1.6, "h": 1.5})
with open(f"{tmp}/info.json", "w") as f:
    json.dump({"layouts": {"LAYOUT_split_3x5_3": {"layout": layout}}}, f)
EOF

keymap -c "$config" parse -q "$tmp/keymap.json" -l "${layers[@]}" -c 10 > "$tmp/keymap.yaml"

keymap -c "$config" draw \
    -j "$tmp/info.json" -l LAYOUT_split_3x5_3 \
    "$tmp/keymap.yaml" "$here/combos.yaml" > "$here/keymap.svg"

echo "Wrote $here/keymap.svg"
