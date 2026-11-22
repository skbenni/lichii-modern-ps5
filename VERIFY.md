# Pfadpruefung (Upstream-Commit `a2c1edb8820cda12a6a3e4242ef509100d26c95a`)

Stand: 22.11.2026. Geprueft wurden die Pfade, die der User genannt hat.

| Thema | Erwarteter Pfad | Befund |
| --- | --- | --- |
| `prod.keys` | `/data/lichii/keys/prod.keys` | OK. `ps5/menu/lichii_menu.cpp` uebergibt beim Metadaten-Extrahieren hart `/data/lichii/keys`. `ps5/src/main.cpp` setzt zusaetzlich `EdenPath::KeysDir` auf `Eden::AssetsPath("keys")`. |
| Firmware | `/data/lichii/firmware/` | OK, aber fixiert. `ps5/menu/assets_dir.h` setzt `kDefaultAssetsDir` auf `/data/lichii`; es gibt in diesem Build **keinen Menu-Punkt zum Auswaehlen** des Assets-Ordners. Firmware muss vor dem Start in `/data/lichii/firmware/` liegen. |
| Spiele | `/data/lichii/roms/`, alternativ `/data/lichii-new/roms/` | OK. `ps5/menu/lichii_menu.cpp` definiert `kGameRoots[]` genau mit diesen beiden Ordnern; Unterordner werden rekursiv gescannt, `.nup`, `.nsp`, `.xci` werden beruecksichtigt. |
| App-Install | `/data/homebrew/PPSA99011/` | OK nach Patch. Upstream hatte `PPSA99010` hart in `ps5/menu/storage_paths.h` (`kInstallDir` und Sandbox-Pfad). `tools/build-full.sh` ersetzt beides jetzt auf `PPSA99011`, sonst waere das UI trotz `param.json` unter dem falschen Titel gesucht worden. |

## Wichtige Details

- `Eden::AssetsDir()` ist in `ps5/menu/assets_dir.h` eine Konstante. Der Hinweis im Upstream-Quelltext
  lautet wörtlich, dass dieser Build keine eigenen Einstellungen neben Lichii hat und der Ordner
  deshalb fixed ist. Ein „Firmware-Select“ wie in manchen anderen Ports existiert hier nicht.
- `prod.keys` liegt direkt in `/data/lichii/keys/`. Die Spiele-Metadaten nutzen `/data/lichii/keys`; die
  Kernel-Konfiguration nutzt `EdenPath::KeysDir` = `Eden::AssetsPath("keys")` = `/data/lichii/keys`.
- ROMs duerfen auch in `/data/lichii-new/roms/` liegen; dieser Pfad ist als zweiter Root eingebaut.
- Lichii-UI-Dateien muessen nach `/data/homebrew/PPSA99011/ui/` kopiert werden. Durch den
  `storage_paths.h`-Patch sucht `AppDir()` jetzt genau dort.

## ShadowMount+

ShadowMountPlus 1.7beta4+ haengt den Titel-Ordner sowie `/data`, USB und erweiterten Speicher in
die Sandbox ein. Ohne ShadowMount+ etaHEN/OnionHEN-Whitelist `PPSA99011` ergaenzen (bei OnionHEN
kein Komma nach der ID). Siehe `SHADOWMOUNT.md`.
