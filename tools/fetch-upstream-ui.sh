#!/usr/bin/env bash
set -euo pipefail

BASE="https://raw.githubusercontent.com/skbenni/Lichii_with_my_change/ui-modernization-clean"
CODEBERG="https://codeberg.org/mihawk/PS5_Lichii/raw/commit/a2c1edb8820cda12a6a3e4242ef509100d26c95a"

mkdir -p ui/upstream

curl -fsSL "$BASE/dist/qt_themes/lichii/style.qss" -o ui/upstream/lichii-style.qss
curl -fsSL "$BASE/dist/qt_themes/lichii/style.qrc" -o ui/upstream/lichii-style.qrc
curl -fsSL "$CODEBERG/ps5/menu/ui/lichii.rcss" -o ui/upstream/lichii-menu.rcss
curl -fsSL "$CODEBERG/ps5/menu/ui/lichii.rml" -o ui/upstream/lichii-menu.rml

printf 'Upstream UI assets actulisiert nach ui/upstream/\n'
