# Minicraft - Nintendo Wii Port 🎮

A native **Nintendo Wii** port of *Minicraft*, the 2D top-down survival crafting game originally created by Markus "Notch" Persson for Ludum Dare 22.

Based on the C/SDL reimplementation by [TheBigEye](https://github.com/thebigeye/minicraft-sdl), ported to Nintendo Wii with native controller support and devkitPPC/libogc integration.

---

## ✨ Features

- **Native Nintendo Wii support**: Runs at 60 FPS in 640x480 resolution.
- **Wiimote Support**:
  - **Horizontal (Sideways)**: Play NES/Game Boy style with D-Pad, Button 2 (Attack), Button 1 (Inventory/Menu).
  - **Vertical / Classic Controller**: Full support for D-Pad, Buttons A/B, and Plus.
- **GameCube Controller Support**: Analog stick and D-Pad movement, Buttons A/B/X/Y.
- **Zero Assets Required**: Self-contained standalone binary with embedded spritesheets and procedural map generation.
- **FAT / SD Card Ready**: Plug and play through Homebrew Channel.

---

## 🕹️ Controls

| Action | Wiimote (Horizontal) | Wiimote (Vertical) / Classic | GameCube Pad |
|---|---|---|---|
| **Move** | D-Pad (Up/Down/Left/Right) | D-Pad | Control Stick / D-Pad |
| **Attack / Mine / Interact** | Button 2 | Button A / 2 | Button A / B |
| **Inventory / Crafting** | Button 1 / Plus (+) | Button 1 / B / Plus (+) | Button Start / X / Y |
| **Menu / Pause** | Plus (+) / Home | Plus (+) / Home | Start |

---

## 📥 Installation on Nintendo Wii

1. Download `boot.dol` and `meta.xml` from the apps/minicraft folder 
2. Create a folder on your SD card or USB drive:
   ```text
   SD:/apps/minicraft/
     ├── boot.dol
     └── meta.xml
   ```
3. Insert your SD card into your Nintendo Wii and launch the game via **Homebrew Channel**.

---

## 🛠️ Building from Source

### Requirements
- [devkitPro](https://devkitpro.org/) with `devkitPPC`, `libogc`, `wii-sdl`, `wii-portlibs` (or use Docker image `devkitpro/devkitppc:latest`).
- CMake 3.13+

### Build command (Linux / WSL / Docker):
```bash
cmake -B build-wii -DCMAKE_TOOLCHAIN_FILE=/opt/devkitpro/cmake/Wii.cmake
cmake --build build-wii
```
This generates `minicraft.dol` inside the `build-wii/` directory.

---

## 📜 Credits
- **Markus "Notch" Persson** – Original Minicraft game concept and design.
- **TheBigEye** – C & SDL reimplementation.
- **szablewskidex** – Nintendo Wii port, controller integration, and Wii GX/SDL adaptation.
