# Flappy Byte

> A Game Boy Advance (GBA) homebrew clone of Flappy Bird with an added "byte logic" mechanic.

## Overview

`Flappy Byte` is a small GBA game written in C that mimics the classic Flappy Bird gameplay and adds a twist: a pair of 8-bit values (bytes) and a randomly chosen logic gate (AND, OR, XOR, NOT). When the pipes pass a threshold the current gate (and an optional random byte) is applied to the player's byte, altering gameplay and score progression.

## Features
- Classic Flappy-style jumping and pipe obstacles
- 8-bit byte display rendered with OBJ sprites
- Randomly chosen logic gate (`AND`, `OR`, `XOR`, `NOT`) applied to the player's byte
- Pixel art assets in `graphics/` (Piskel source included)

## Technology
- Language: C
- Platform: Game Boy Advance (ARM7TDMI)
- Libraries/tooling: TONC (`libtonc`) and the devkitPro/devkitARM toolchain
- Build system: GNU Make with the provided `Makefile`

## How the program works (process)

- The program initializes GBA display, palettes, background tiles and OBJ (sprite) memory.
- A main loop lives in `source/main.c`:
  - Waits for VBlank (`vid_vsync()`), polls input, and advances frame counters.
  - Shows home/restart screens until `A` is pressed to start.
  - Updates the ball physics (`ball_update`) and moves pipes (`pipes_update`).
  - When pipes cross the threshold, `apply_byte_logic()` is called to mutate the player's `game_byte` according to the currently selected gate and (when applicable) a random byte.
  - Score is incremented when pipes are successfully passed.
  - Collisions or out-of-bounds reset the game state and return to the homescreen/restart loop.

Key source responsibilities:
- `source/main.c` — Game loop, state management, loading backgrounds and screens.
- `source/bytes.c` / `source/bytes.h` — Byte generation, gate selection, drawing bytes and gates, and applying logic.
- `source/ball.c` / `source/ball.h` — Player (ball) sprite and physics.
- `source/pipes.c` / `source/pipes.h` — Pipe generation/movement and collision detection.
- `graphics/` — Tile maps, tiles and Piskel source files used to generate GBA assets (grit output exists as `.c`/`.h`).

## Build & Run

Prerequisites
- Install devkitPro (includes devkitARM and libtonc). Make sure `DEVKITARM` and `DEVKITPRO` environment variables are set.
- `make` and `grit` (the `Makefile` uses grit for processing PNG -> GBA tiles).
- A GBA emulator such as mGBA or VisualBoyAdvance-M to run the generated `.gba` file.

Typical build (from project root):

```powershell
# If you installed devkitPro via the installer it usually sets the env vars for you.
cd path\to\flappy-byte
make
# Output: flappy-byte.gba
```

Clean build artifacts:

```powershell
make clean
```

Run the ROM with an emulator (replace with your emulator executable/path):

```powershell
mGBA.exe flappy-byte.gba
# or open flappy-byte.gba in VisualBoyAdvance-M
```

## Project Structure

- `Makefile` — Build rules (requires DEVKITARM / devkitPro and grit)
- `flappy-byte.gba` — Built ROM (if present)
- `source/` — C source files and headers
  - `main.c`, `bytes.c`, `ball.c`, `pipes.c`, etc.
- `graphics/` — Piskel source files and generated GBA graphics (`.c` / `.h`)
- `build/` — Build object files and intermediate artifacts

## Modifying graphics
The `graphics/` folder contains `.piskel` files (source art). Edit them in Piskel and re-run the build so `grit` regenerates the GBA tile/map C files.

## Notes & Contributing
- No license file is provided. If you plan to reuse or publish this code, consider adding a `LICENSE`.
- Contributions: open an issue or PR with small, focused changes (bug fixes, asset updates, or improvements to build scripts).

## Contact
If you want me to help with building, packaging, or adding a license, tell me which OS and toolchain you use and I can add more precise instructions.
