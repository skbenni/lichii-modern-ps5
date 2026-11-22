# Installation ueber ShadowMountPlus (empfohlen)

ShadowMountPlus 1.7beta4 oder neuer montiert das Homebrew direkt aus dem Ordner, in den
du es kopierst, und haengt zusaetzlich `/data`, USB und erweiterte Speicher in die
Sandbox ein. Damit kommt `Lichii Modern PS5` an Spiele und Einstellungen, ohne dass ein
eigenes Elevation-Helper-ELF geladen werden muss.

## Schritte

1. `dist/PPSA99011/` mit FTP auf die PS5 kopieren. Zulässige Ziele sind unter anderem
   `/data/homebrew/PPSA99011/` oder `/mnt/usb0/homebrew/PPSA99011/`.
2. Sicherstellen, dass `dist/PPSA99011/eboot.bin` direkt im Titelordner liegt.
3. ShadowMountPlus starten und den Titelordner scannen/mounten lassen. Bei aelteren
   Versionen muss der Ordner in den von ShadowMountPlus durchsuchten Homebrew-Pfaden
   liegen.
4. Eigene, selbst erstellte Spiel-Dumps nach `/data/lichii/roms/` legen
   (`.nup`, `.nsp`, `.xci`). Keys nach `/data/lichii/keys/`, Firmware nach
   `/data/lichii/firmware/`.
5. App aus dem Homebrew-Menue starten. Die Spieleliste wird beim Start eingelesen.

## Alternative ohne ShadowMountPlus

- etaHEN: `PPSA99011` in die App-Jailbreak-Liste aufnehmen.
- OnionHEN: `PPSA99011` am Ende von `exact_title_ids` in `config.ini` eintragen
  (kein Komma danach).
- Sonst: lokaler ELF-Loader auf Port 9021 muss erreichbar sein.

Das Projekt enthaelt keinen Self-Updater; installierte Dateien werden nicht automatisch
ersetzt. Beim Update den gesamten Ordner austauschen, `/data/lichii/` bleibt erhalten.

## Hinweise

- ShadowMountPlus muss vor dem Start des Titels geladen sein.
- Ohne gemountetes `/data` findet die App keine Spiele und zeigt den Leerzustand.
- Keine Keys, Firmware oder Spiele aus dem Netz verwenden. Nur selbst gedumpte Daten.
