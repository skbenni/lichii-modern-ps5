#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
UPSTREAM="${LICHII_UPSTREAM_DIR:-$ROOT/third_party/PS5_Lichii}"
VULKAN="${PS5_VULKAN_DIR:-$ROOT/third_party/PS5_Vulkan}"
SDK_FORK="${PS5_PAYLOAD_SDK_FORK:-$ROOT/third_party/PS5_PayloadSDK}"
REVISION="a2c1edb8820cda12a6a3e4242ef509100d26c95a"
TITLE_ID="PPSA99011"

die() {
    printf '%s\n' "$*" >&2
    exit 2
}

command -v git >/dev/null || die "git fehlt"
command -v python3 >/dev/null || die "python3 fehlt"
command -v ninja >/dev/null || die "ninja fehlt"
[[ -d $VULKAN ]] || die "PS5_Vulkan fehlt: $VULKAN (PS5_VULKAN_DIR setzen)"
[[ -d $SDK_FORK ]] || die "PS5_PayloadSDK fehlt: $SDK_FORK (PS5_PAYLOAD_SDK_FORK setzen)"

if [[ ! -d $UPSTREAM/.git ]]; then
    mkdir -p "$(dirname "$UPSTREAM")"
    git clone https://codeberg.org/mihawk/PS5_Lichii.git "$UPSTREAM"
fi

git -C "$UPSTREAM" fetch --force origin "$REVISION" 2>/dev/null || true
git -C "$UPSTREAM" checkout --detach "$REVISION"
git -C "$UPSTREAM" checkout -- ps5/menu/storage_paths.h ps5/menu/ui/lichii.rcss ps5/sce_sys/param.json

cat "$ROOT/ui/lichii-modern.rcss" >> "$UPSTREAM/ps5/menu/ui/lichii.rcss"

python3 - "$UPSTREAM/ps5/menu/storage_paths.h" "$TITLE_ID" <<'PY'
import pathlib
import sys

path = pathlib.Path(sys.argv[1])
path.write_text(path.read_text(encoding="utf-8").replace("PPSA99010", sys.argv[2]), encoding="utf-8")
PY

python3 - "$UPSTREAM/ps5/sce_sys/param.json" "$TITLE_ID" <<'PY'
import json
import sys

path = sys.argv[1]
title_id = sys.argv[2]
with open(path, "r", encoding="utf-8") as handle:
    data = json.load(handle)
data["titleId"] = title_id
if "contentId" in data:
    data["contentId"] = title_id + "_00-LICHIIMODERN001"
with open(path, "w", encoding="utf-8") as handle:
    json.dump(data, handle, indent=2, ensure_ascii=False)
    handle.write("\n")
PY

export PS5_PAYLOAD_SDK_FORK="$SDK_FORK"
bash "$UPSTREAM/ps5/tools/build.sh" lichii-ps5

SOURCE="$UPSTREAM/dist/$TITLE_ID"
DEST="$ROOT/dist/$TITLE_ID"
[[ -f $SOURCE/eboot.bin ]] || die "Build hat keine eboot.bin erzeugt"
rm -rf "$DEST"
mkdir -p "$(dirname "$DEST")"
cp -r "$SOURCE" "$DEST"
printf 'Fertig: %s/eboot.bin\n' "$DEST"
