# Minicraft PSP Edition

Port gry Minicraft na konsolę Sony PlayStation Portable (PSP).

## Sterowanie PSP:
- **D-Pad / Gałka analogowa**: Poruszanie postacią / nawigacja w menu
- **Krzyżyk (X)**: Atak / Akcja / Zatwierdź w menu
- **Kółko (O)**: Atak / Akcja alternatywna / Powrót
- **Kwadrat ([]) / Trójkąt (/_\) / Select**: Ekwipunek (Plecak) / Menu craftingu
- **R-Trigger (R)**: Szybki wybór następnego przedmiotu / narzędzia
- **L-Trigger (L)**: Szybki wybór poprzedniego przedmiotu / narzędzia
- **Start**: Pauza / Menu gry
- **Home Button**: Czyste wyjście do XMB

## Instalacja na karcie pamięci PSP:
1. Skopiuj cały folder `minicraft-psp` (lub utwórz folder `minicraft`) na kartę pamięci PSP do ścieżki:
   `ms0:/PSP/GAME/minicraft/`
2. Upewnij się, że wewnątrz znajduje się plik `EBOOT.PBP`.
3. Uruchom grę z menu Gry (Memory Stick) w XMB.

## Cechy portu:
- Natywna rozdzielczość PSP 480x272 (skalowanie 2x retro pixel art)
- Taktowanie procesora PSP podbite do 333 MHz (stałe 60 FPS)
- Pełna obsługa dźwięku i muzyki otoczenia w tle (SDL_mixer)
- Zapisy gry (3 niezależne sloty zapisu)
- Wbudowana ikona (ICON0.PNG) oraz tło XMB (PIC1.PNG)
