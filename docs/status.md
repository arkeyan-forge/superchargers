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

## Status (day 4, 2026-11-04)
Codegen, build and boot work; the picture is right; **a new game reaches the Academy (the hub)**: the intro and the rift tunnel ride,
then `SKY_LEVEL=academy` (or, without it, `intro1` on foot), and **a continue from the save goes straight to the Academy**; 30 fps,
no fault. The automatic test is `tools/regress/superchargers_lobby.sh` ([boot.md](boot.md) "SKY_LEVEL and the test").

| Step | Result |
|---|---|
| Codegen | clean after three recompiler additions (`patches/xenonrecomp.patch`): `vadduhs`, `vpkuwum128`, `vpkd3d128` FLOAT16_2; 102087 functions, 0 trap left in the generated code |
| Build | `build-superchargers` links (`Skylanders SuperChargers`, bundle `SSCRecomp`); 0 warnings in `games/superchargers/` |
| Picture | right since two renderer fixes (the scissor with the test off, 16-bit pair positions; [boot.md](boot.md) "The black frame") |
| Story | the menus, `fmv01`, the intro, the tunnel ride with a vehicle, `intro1` on foot (1245 draws a frame, 30 fps), the Academy |
| Figures | Fiesta (`new:3400`) and Crypt Crusher (`new:3227`), generated blank tags, are read and played; the instant vehicle also works |
| Shaders | from the disc: IGA v11 big-endian, IGZ v9, 1207 containers ([shaders.md](shaders.md)) |
| The launcher's look | logo, background, 469 portraits, the menu theme, from the disc ([art.md](art.md)) |

## The details the earlier ports got wrong first (2026-11-04)
Each checked in SuperChargers with a log line or a shot (runs under `private/runs/`); [V] seen, [?] not checked.

| Detail | Result |
|---|---|
| Movies | the logos (`bumper_atvi`, `bumper_vvisions`; the adapter lists them for `skip_intro_movies`) and `fmv01` (3905 frames, its file read to the end) play [V]; at 21:9 with black bars [V]. `fmv01` is not skipped by Y, X, B or A; Start pauses it [V]; the game's skip waits for a "ready to skip" state not found ([boot.md](boot.md), "Movies: the skip") [?] |
| Text and voices | this disc has English and German only (1146 `.lng` and 347 voice banks each, no French). `--language de`: every voice and text bank opened is `_de` (118), none `_en` [V]. **Voices in another language** (`voice_language`): Swap Force's hooks re-addressed, `voices en (6), text 9` [V]; a movie's voice track (5 English, 6 German) switched [?] ([boot.md](boot.md), "Text and voice language") |
| Sound | not silent: 20898 non-silent 1024-sample blocks in a 455 s run, 0.00 % repeated (`analyze.py audio`) [V] |
| HUD and prompts | the HUD (portrait, gearbits, health), the game's own Xbox 360 button prompts ("Hold RT to Accelerate!") [V]. **Glyphs**: the fonts' two button pictures redrawn in memory (Swap Force's way): the pause menu's hints follow PS5, Switch, the keyboard's keycaps and the 360 [V] ([art.md](art.md), "Button pictures"); the world's 3D prompts are meshes and stay the 360's |
| Save and continue | `SUPERCHARGERS_OPTIONS` and `SUPERCHARGERS_0` written; Continue reads them and loads the Academy [V]; Swap Force's save-task race is not in this game's code (no hook) [V] |
| Portal prompt | **the Toy Box opens by itself** on Vehicle 1 at "Please place a LAND VEHICLE", then on Player 1 for the Skylander; both seated and taken (run `scP1`) [V] ([toybox.md](toybox.md), "The portal prompt") |
| Toy Box (F6) | opens over the Academy: 469 portraits read from the disc, the seats Player 1, Vehicle 1, Player 2, Vehicle 2, Magic item, Fiesta and Crypt Crusher seated [V]; their cards show drawn icons until the catalogue has SuperChargers' portrait keys |
| Outlines at scale 2 | the FXAA pass (`732669B14C8E3386`) is `HostKernelPixelShader`, so its kernel stays in guest pixels at scale 2, as Trap Team's. **Side by side** (the Academy from the same save and camera, the full-resolution scene, scale 1 run `scS4` against scale 2 run `scS1`, crops enlarged twice: `private/runs/outlines_scale1_vs_2.png`): the same outlines on Fiesta and the scenery, sharper at 2, none doubled or thinned [V] |
| Water and lava above 30 FPS | not measured: no water simulation pass found yet (`stepPassShaders` empty); the image names `_applyWaterSimulationNormals` and ripple settings, so one may exist in a water level [?] |
| Ultrawide and the scene | the 3D at full resolution (1040x620 -> 1280x720) and **ultrawide**: 21:9 (2560x1080: scene 1696x720, Hor+ 80 -> 96.4 degrees) and 32:9 (3840x1080: scene bounded to 2048x576, 118.4 degrees; a 2560x720 scene stopped the game in the Academy), HUD and menus at 16:9 [V] ([ultrawide.md](https://github.com/arkeyan-forge/forge/blob/main/docs/runtime/gpu/ultrawide.md), "SuperChargers") |
| 60 FPS | the game asks for interval 2 and paces on it (as Swap Force): `presentIntervalPacesGame`; at `--fps-limit 60` the menus and the intro run at 60.0 fps, `intro1` at 58.4-60 (1330 draws), the story at its pace (`intro1` at 321 s against 329 s at 30) [V]. The Academy (1280 draws) held 58.3 fps with the renderer at 11.2 ms a frame; with the 16-bit pair streams in the buffer cache and the swap spin made a wait (2026-11-04, [renderer_threads.md](https://github.com/arkeyan-forge/forge/blob/main/docs/runtime/gpu/renderer_threads.md) "SuperChargers and Imaginators at 60"): renderer 6.0 ms, **59.9 fps held**, gap p99 19 ms, process CPU 38.6 -> 31.6 ms a frame; with the drop to 30 on, no drop: the Academy 60.0 fps, `intro1` 59.2, `sky1` 59.1 [V]. The other zones (sky2-3, nat1-3, quest1-3, mag1, dark1, final1-2: `MAP_*_bank` on the disc) are not measured yet: `tools/perf/game_perf.sh superchargers <disc> <map> 60 <out>` with `SECONDS_TO_RUN=470` |

## Where to start
- Build: `cmake -S . -B build-superchargers -G Ninja -DCMAKE_BUILD_TYPE=Release -DSSA_GAME=superchargers`, `nice ninja -C build-superchargers -j4 ArkeyanRuntime`
  (the generated code: [research.md](research.md) "XenonRecomp"; the shader cache: `SKY_GAME=superchargers tools/shaders/build_cache.sh <disc> <dump dir>`
  from a `SKY_DUMP_SHADERS` run: 1214 of 1215 translate, `203D384226E0FD37.ps` is rejected by the Metal compiler).
- Run: the isolation block of [testing.md](https://github.com/arkeyan-forge/forge/blob/main/docs/getting-started/testing.md) plus `SKY_PORTAL=emulated SKY_PORTAL_PLACE=new:3400`
  and an A every 4 s from 12 s: the menus to the intro in ~42 s; add `new:3227` (a vehicle) and RT for the tunnel ride
  ([boot.md](boot.md) "To the first level").

## Open problems
1. The movies' skip (`fmv01`): the game's "ready to skip" state is not found.
2. Vehicles: the tags are blank ones of our generator (no real dump); a vehicle's mods (the Academy's garage) are not mapped
   ([research.md](research.md)); player 2's prompts, the Dead and error states not run.
3. Glyphs: the turbo pictures (`Turbo_A/X/Y`, not met) and the world's 3D prompts (meshes, stream G).
4. Two stubs on the boot path: `XamUserReadProfileSettings` and `XMsgCancelIORequest` (answered 0; nothing seen wrong).
