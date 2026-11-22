# Lichii Modern PS5

Eigenes PS5-Homebrew-Grundgeruest mit DualSense-Steuerung und dem Lichii-Dark-Design aus
`skbenni/Lichii_with_my_change` (`ui-modernization-clean`). Stand: 22.11.2026.

## Was hier drin ist

- `src/ps5_pad_backend.cpp`: nativer DualSense-Zugriff ueber `sceUserService`/`scePad`,
  inklusive Rumble und Lightbar. Die Struktur-Layouts sind unabhaengig nachgebaut.
- `src/pad_input.cpp`: Digitale Und Kanal-Abstaende, Edge-Detection, Tastenwiederholung,
  Analogstick-To-D-Pad mit Deadzone und Zuordnung auf UI-Aktionen.
- `src/menu_model.cpp`: Spiele-/Einstellungs-Navigation, Tabs, Einstellungswechsel.
- `ui/menu.rml`, `ui/lichii-modern.rcss`: 1920x1080-Menue im Lichii-Dark-Farbschema
  (`#17181b`, Akzent `#f0386b`, Montserrat), abgeleitet aus dem Branch
  `ui-modernization-clean`. `tools/fetch-upstream-ui.sh` lädt die Original-QSS/RML
  nach `ui/upstream/`; Herkunft und Tree-SHA stehen in `ui/UPSTREAM.md`.
- `tests/test_menu.cpp`: Host-Tests fuer Navigation, Einstellungen und Pad-Logik.

## Wichtig

Dieses Repository enthaelt zwei Wege:

- `tools/build-full.sh` baut den **vollstaendigen** Titel: Lichii-Kern, Vulkan/RADV,
  SDL2/RmlUi-Menue, DualSense-Input und Signierung. Das ist der Weg fuer "alles geht".
- `src/` und `tools/build-ps5.sh` sind ein host-testbares Frontend-Grundgeruest und
  dienen als Referenz/Tests; allein haben sie keinen Emulator-Kern.
- Der Vollbuild holt `mihawk/PS5_Lichii` an Commit `a2c1edb8820cda12a6a3e4242ef509100d26c95a`,
  haengt unser `ui/lichii-modern.rcss` an das Upstream-Menue an und setzt `PPSA99011`
  auch in `ps5/menu/storage_paths.h`.
- Pfadpruefung fuer Keys, Firmware und Spiele steht in `VERIFY.md`.
- Keine Keys, Firmware oder Spiele enthalten; nur selbst gedumpte Daten verwenden.

## Vollbuild (echter Emulator)

Voraussetzungen: Linux, Ninja, Python3, git sowie Checkouts unter `third_party/`:
`PS5_PayloadSDK` (Fork mit Revision `88d6e41ec7ee7e76d871ff14cd78b5cc4c8e5989`),
`PS5_Vulkan` (mit gebautem RADV-Release) und `PS5_Mesa`. Pfade lassen sich ueber
`PS5_PAYLOAD_SDK_FORK`, `PS5_VULKAN_DIR` und `LICHII_UPSTREAM_DIR` ueberschreiben.

```sh
./tools/build-full.sh
```

Das Skript baut das Upstream-Target `lichii-ps5` mit dem PS5-Toolchain, signiert die
`eboot.bin` und legt das Ergebnis in `dist/PPSA99011/` ab. Danach wie unten installieren.

## Host-Tests / Grundgeruest

```sh
cmake -S . -B build-host
cmake --build build-host --parallel
ctest --test-dir build-host --output-on-failure
./build-host/host_demo
```

Nur das Frontend-Grundgeruest (ohne Emulator):

```sh
export PS5_PAYLOAD_SDK=/pfad/zu/ps5-payload-sdk
export PS5_TOOLCHAIN=/pfad/zu/ps5-toolchain.cmake
./tools/build-ps5.sh
```

Danach `dist/PPSA99011/` per FTP nach `/data/homebrew/PPSA99011/` (oder
`/mnt/usb0/homebrew/PPSA99011/`) kopieren (`eboot.bin` muss direkt im Ordner liegen).
Bevorzugter Weg: ShadowMountPlus 1.7beta4+ mountet den Titel und haengt `/data`, USB
und extended Storage in die Sandbox ein. Details und die Alternative ueber etaHEN/
OnionHEN stehen in `SHADOWMOUNT.md`. Die Titel-ID `PPSA99011` ist im Vollbuild konsistent
in `param.json` gesetzt; beim reinen Scaffold ist sie ein Platzhalter.

## Rollen der Dateien

Der Vollbuild benutzt das Upstream-Frontend (`ps5/menu/frontend.cpp`, `radio_input.c`,
`lichii_menu.cpp`) und damit Kern, Renderer und Controller. `Draw(const ViewModel&)` in
`src/main.cpp` bleibt die textuelle Host-Vorschau unseres Grundgeruests; es wird vom
Vollbuild nicht gelinkt.

## Steuerung

| Eingabe | Aktion |
| --- | --- |
| Linke Sticks / Kreuz | Navigation, mit Wiederholung |
| X | Start / Einstellung bestaetigen |
| O | Zurueck zum Spiele-Tab |
| L1 / R1 | Tab wechseln |
| Optionen | Beenden |
| Touchpad | reserviert |

## Lizenz

GPL-3.0-or-later. Siehe `NOTICE.md`. Keine Spiele, Keys oder Firmware enthalten.
Nur selbst erstellte Backups und selbst gedumpte Systemdateien verwenden.
