# Upstream-UI-Basis

Dieses Verzeichnis wird von `tools/fetch-upstream-ui.sh` gefuellt.

Quellen (Stand 22.11.2026):

- `skbenni/Lichii_with_my_change`, Branch `ui-modernization-clean`,
  Tree-SHA `66dde1b7439c4d8f98e1ce30142f6a2098b07c39`
  - `dist/qt_themes/lichii/style.qss`
  - `dist/qt_themes/lichii/style.qrc`
- `mihawk/PS5_Lichii`, Commit `a2c1edb8820cda12a6a3e4242ef509100d26c95a`
  - `ps5/menu/ui/lichii.rcss`
  - `ps5/menu/ui/lichii.rml`

`ui/lichii-modern.rcss` und `ui/menu.rml` dieses Projekts sind eine eigenstaendige
RmlUi-Uebersetzung dieser Tokens: `#17181b`, `#131416`, `#141518`, `#202125`,
`#2a2c31`, `#34363c`, `#2a2b30`, `#ececef`, `#c4c5ca`, `#9b9da4`, `#6f7178`,
Akzent `#f0386b` / Hover `#f45a85`, Gefahr `#c95d5d`, 1920x1080, Montserrat.
Die QSS-Datei bleibt Referenz, damit Aenderungen am Upstream-Theme nachgezogen
werden koennen.
