Lichii Modern PS5
SPDX-License-Identifier: GPL-3.0-or-later

Dieses Projekt ist ein unabhaengiges Homebrew-Frontend. Es enthaelt keine
SPIEL-, Key-, Firmware- oder Sony-Dateien.

Herkunft und Nachnutzung:

- Lichii (https://github.com/zortos293/Lichii), GPL-3.0-or-later.
- skbenni/Lichii_with_my_change, Branch ui-modernization-clean
  (https://github.com/skbenni/Lichii_with_my_change), GPL-3.0.
  Grundlage fuer die UI-Ausrichtung und das Dark-Theme.
- PS5_Lichii (https://codeberg.org/mihawk/PS5_Lichii), GPL-3.0-or-later.
  Referenz fuer ps5/menu (RmlUi-Frontend, radio_input, scePad-Layout,
  Lichii-Dark-Farben aus ps5/menu/ui/lichii.rcss und dist/qt_themes).
- PS5 Payload SDK, John Törnblom (ps5-payload-dev), und die PS5-Vulkan/Mesa-
  Arbeiten von mihawk werden nur als Bauvoraussetzung referenced, nicht
  weitergegeben.

Die scePad-Datenstrukturen in src/ps5_pad_backend.cpp sind unabhaengig
nachgebaute ABI-Deklarationen; es worden keine Vendor-Header kopiert.
