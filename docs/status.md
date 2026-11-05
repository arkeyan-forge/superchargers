# Skylanders: SuperChargers (superchargers)

> **Purpose:** which disc SuperChargers needs, where the port stands, and where to start · **Status:** current (2026-11-04) · **Tags:** [Xbox] [XDK] [Alchemy] [SuperChargers] [V] [?]

## Disc
| | |
|---|---|
| Release | 2015, Vicarious Visions; Redump "Skylanders - SuperChargers (Europe)", 8738846720 bytes (the only disc tried) |
| Title ID, media ID, version | `4156091F`, `74144F53`, `0x00000007` [V] (the XEX execution info) |
| `default.xex` SHA-256 | `02ff6672...` (full value in `game.txt` and `info.cpp`), 24985600 bytes |
| Engine | **Alchemy**, Swap Force's line on the game side, Trap Team's XDK **2.0.21256** [V] ([research.md](research.md)) |

The disc stays outside git; everything derived from it is in `private/superchargers/` (day 1: `private/superchargers/day1/`).

## Status (day 2, 2026-11-04)
Codegen, build and boot work; the picture is right; **the first level plays with a figure and a vehicle**: the rift tunnel ride, Crypt
Crusher driven by Fiesta, at 30 fps with no fault over 335 s [V] ([boot.md](boot.md)).

| Step | Result |
|---|---|
| Codegen | clean after three recompiler additions (`patches/xenonrecomp.patch`): `vadduhs`, `vpkuwum128`, `vpkd3d128` FLOAT16_2; 102087 functions, 0 trap left in the generated code |
| Build | `build-superchargers` links (`Skylanders SuperChargers`, bundle `SSCRecomp`); 0 warnings in `games/superchargers/` |
| Picture | right since two renderer fixes (the scissor with the test off, 16-bit pair positions; [boot.md](boot.md) "The black frame") |
| Movies | `bumper_vvisions.bk2` and `fmv01.bk2` play (Bink 2) |
| Menus | the title, the play style, the saves `SUPERCHARGERS_OPTIONS` and `SUPERCHARGERS_0` |
| First level | the intro cutscene, then the rift tunnel ride with a vehicle; RT accelerates, gearbits are collected |
| Figures | Fiesta (`new:3400`) and Crypt Crusher (`new:3227`), generated blank tags, are read and played; the instant vehicle also works |
| Shaders | from the disc: IGA v11 big-endian, IGZ v9, 1207 containers ([shaders.md](shaders.md)) |

## Where to start
- Build: `cmake -S . -B build-superchargers -G Ninja -DCMAKE_BUILD_TYPE=Release -DSSA_GAME=superchargers`, `nice ninja -C build-superchargers -j4 ArkeyanRuntime`
  (the generated code: [research.md](research.md) "XenonRecomp"; the shader cache: `SKY_GAME=superchargers tools/shaders/build_cache.sh <disc> <dump dir>`
  from a `SKY_DUMP_SHADERS` run: 1214 of 1215 translate, `203D384226E0FD37.ps` is rejected by the Metal compiler).
- Run: the isolation block of [testing.md](https://github.com/arkeyan-forge/forge/blob/main/docs/getting-started/testing.md) plus `SKY_PORTAL=emulated SKY_PORTAL_PLACE=new:3400`
  and an A every 4 s from 12 s: the menus to the intro in ~42 s; add `new:3227` (a vehicle) and RT for the tunnel ride
  ([boot.md](boot.md) "To the first level").

## Open problems
1. Past the tunnel ride: the next areas, on foot, not reached yet.
2. The engine hooks of Swap Force's line (zones for `SKY_LEVEL`, language, save task, ultrawide, the portal prompt `CMagicMoment`): re-find by hand.
3. Vehicles: the tags are blank ones of our generator (no real dump); the vehicle's own data (mods, gear) is unknown ([research.md](research.md)).
4. Audio not checked (runs use the disk driver); 60 FPS, ultrawide, glyphs, language: not started.
