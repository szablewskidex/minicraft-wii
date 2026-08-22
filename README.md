# Minicraft - Nintendo Wii Port (Enhanced Edition) 🎮

An enhanced native **Nintendo Wii** port of *Minicraft*, the 2D top-down survival crafting game originally created by Markus "Notch" Persson for Ludum Dare 22.

Based on the C/SDL reimplementation by [TheBigEye](https://github.com/thebigeye/minicraft-sdl), ported and significantly expanded for Nintendo Wii with native controller & Wiimote rumble support, bow & arrow combat, fishing rod & fish, farming & animal breeding, subterranean dungeons & treasure chests, HUD minimap radar, 4-tier armor system, doors, boats, leveling with glowing EXP orbs, smooth day/night cycle, multi-slot save system, and multi-language support.

---

## ✨ Features & Enhancements

### 🏹 Bow & Arrow Ranged Combat
- Craftable **Bow** (`Wood` × 5 + `Cloth` × 3) and **Arrows** (`Wood` × 3 + `Stone` × 2 + `Cloth` × 1).
- Fire arrows across distance with directional physics, hit detection against monsters/animals, tile collisions, and smash particles.

### 🎣 Fishing Rod & Marine Loot
- Craftable **Fishing Rod** (`Wood` × 5 + `Cloth` × 3).
- Cast into water/oceans to catch **Raw Fish**, and rarely reel up precious treasures (**Gems, Iron Ingots, Hide**).
- Cook raw fish in the **Oven** with coal to produce delicious **Cooked Fish** (restores 4 full hearts!).

### 🌾 Agriculture, Animal Breeding & Shears
- **Animal Breeding:** Feed Wheat to Cows/Sheep, Seeds to Chickens, and Carrots/Apples to Pigs to display love hearts (`<3`) and gain bonus EXP!
- **Shears Tool:** Craftable with 2 Iron Ingots at the Anvil to shear live sheep for wool/cloth without hurting them.
- **New Crops:** Plantable and edible **Carrots** and **Potatoes**.

### 📯 Wiimote Rumble Feedback
- Realistic haptic feedback on the Wiimote controller:
  - Subtle tap when mining rocks, chopping trees, or shooting arrows.
  - Medium rumble when hitting monsters or catching fish.
  - Strong rumble on taking damage or creeper explosions!

### 🗺️ HUD Minimap Radar
- Integrated live radar in the top-right corner of the screen:
  - Real-time terrain colors (Oceans, Beaches, Forests, Caves, Lava, Dungeons).
  - Center player tracking and direction indicator.

### 🏰 Subterranean Dungeons & Treasure Chests
- Procedurally generated underground dungeon rooms built with reinforced hard rock.
- Guarded treasure chests filled with valuable minerals, gems, weapons, and ammunition.

### 🐾 Animated Animals & Biomes
- **Chicken (Kura), Pig (Świnka), Sheep (Owca)** roaming grassy biomes.
- **Crab (Krab)** scuttling along beaches and coastlines.
- **Frog (Żaba)** hopping through ponds and forests.
- 4-frame smooth animations and resource drops (feathers, hide, wool, meat).

### ⚔️ Upgraded Monsters & Combat
- 4-frame walking and attack animations for **Zombies, Skeletons, Slimes, Creepers, and Knights**.
- Boss battle with the **Air Wizard**.

### ✨ EXP Orbs & Leveling System
- Defeated monsters and animals drop **bouncing, glowing EXP Orbs** that pulse with vivid colors.
- **Magnetic attraction:** Orbs float toward the player when nearby.
- **Leveling progression:** HUD displays your level (`LV: X`) and EXP progress bar.
- **Stamina reduction:** Higher levels reduce the stamina consumed when mining and chopping with tools!
- **Level Up:** Restores full health and stamina with a golden notification.

### 🛡️ 4-Tier Armor System & Defense
- Craftable armors with authentic Minicraft Plus icons:
  - 🟫 **Leather Armor** (`Hide` × 5) — +1 Defense
  - ⬜ **Iron Armor** (`Iron Ingot` × 5) — +2 Defense
  - 🟨 **Gold Armor** (`Gold Ingot` × 5) — +3 Defense
  - 🟦 **Gem Armor** (`Gem` × 5) — +4 Defense
- Equip directly from hand with a confirmation sound.
- Visual character tinting matching your equipped armor.
- HUD armor icon displaying your active defense points.
- Damage reduction from all monster attacks.

### 🚪 Interactive Doors & Building
- Craftable **Wooden Doors** (`Wood` × 5 at Workbench).
- Place anywhere; press Attack (**2** / **A** / **C**) to smoothly open or close.
- Transparent doorway allowing free passage when opened, solid collision when closed.

### 🚣 Boat & Aquatic Exploration
- Craftable **Boat** (`Wood` × 5).
- Use on water to hop in: renders a wooden boat hull under your character.
- Move across oceans and rivers at full walking speed with **zero stamina exhaustion**.

### 🌅 Smooth Progressive Day/Night Cycle
- **Realistic Dusk & Dawn:** Daylight smoothly fades into night with circular dithered shadows creeping in from the horizon towards light sources.
- Torches, lanterns, and lava pierce through the dark.
- Beds allow fast-forwarding through the night.

### 💾 Multi-Slot Save System & Languages
- Multiple save slots with SD card persistence.
- Built-in **Polish (Polski)** and **English** language options.

---

## 🕹️ Controls

| Action | Wiimote + Nunchuk | Wiimote (Horizontal) | Classic Controller | GameCube Pad |
|---|---|---|---|---|
| **Move** | Nunchuk Analog Stick / D-Pad | D-Pad (Up/Down/Left/Right) | Left Stick / D-Pad | Control Stick / D-Pad |
| **Attack / Mine / Shoot / Open Door** | Button A / Nunchuk C | Button 2 | Button A / B | Button A / B |
| **Inventory / Crafting Menu** | Button 1 / Minus (-) | Button 1 | Button X / Y | Button X / Y |
| **Cycle Held Item (Next)** | **Nunchuk Z** / Button B (Trigger) | Button A | **Button R / ZR** | **Trigger R / Z** |
| **Cycle Held Item (Prev)** | Button Minus (-) | Button Minus (-) | **Button L / ZL** | **Trigger L** |
| **Pause / Options** | Plus (+) / Home | Plus (+) / Home | Plus (+) / Home | Start |

---

## 📥 Installation on Nintendo Wii

1. Copy the `apps` folder to the root of your SD card or USB drive:
   ```text
   SD:/apps/minicraft/
     ├── boot.dol
     ├── icon.png
     └── meta.xml
   ```
2. Insert your SD card/USB into your Nintendo Wii.
3. Launch **Minicraft** from the **Homebrew Channel**.

---

## 🛠️ Building from Source

### Requirements
- [devkitPro](https://devkitpro.org/) with `devkitPPC`, `libogc`, `wii-sdl`, `wii-portlibs` (or use Docker image `devkitpro/devkitppc:latest`).
- CMake 3.13+

### Build command (Linux / WSL / Docker):
```bash
source /opt/devkitpro/wiivars.sh
cmake -B build-wii -DCMAKE_TOOLCHAIN_FILE=/opt/devkitpro/cmake/Wii.cmake
cmake --build build-wii
```
This produces `minicraft.dol` inside the `build-wii/` directory.

---

## 📜 Credits
- **Markus "Notch" Persson** – Original Minicraft game concept and design.
- **TheBigEye** – C & SDL reimplementation.
- **Minicraft Plus Revived Team** – Textures and mechanics reference.
- **szablewskidex** – Nintendo Wii port, enhanced systems (Bow/Arrow, Fishing, Animals, Dungeons, Minimap Radar, Rumble, Armors, Doors, Boat, Audio, Controllers, Day/Night).
