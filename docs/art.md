# SuperChargers' pictures and menu theme from the disc: logo, background, portraits, music

> **Purpose:** where SuperChargers keeps its logo, background, toy portraits and menu theme, and what the launcher reads from the player's disc · **Status:** current (2026-11-04) · **Tags:** [Alchemy] [SuperChargers] [V] [?]

The provider and the rule are the other games' ([game_art.md](https://github.com/arkeyan-forge/forge/blob/main/docs/runtime/host/game_art.md), [game_music.md](https://github.com/arkeyan-forge/forge/blob/main/docs/runtime/host/game_music.md));
this page is what is SuperChargers'. Read from the player's own disc and cached in `<data>/SuperChargers/`; nothing is shipped.
Manifest: [art.json](../art.json). The archives are IGA v11 big-endian, their entries IGZ v9 ([shaders.md](shaders.md)); the entry names are
build paths (`Temporary/BuildServer/xenon/Output/textures/<shader>,textures@!ui!<folder>!<name>\`tga,<flags>.igz`, `/` separators).

## Where they are [V]
| What | Archive | Entry | Result |
|---|---|---|---|
| Logo | `archives/legal.pak` | `textures@!ui!do_not_compress!Skylanders!ToyFair_Title_NOTFINAL` | 688x316, A8R8G8B8: the full logo (exhausts, the element medallions, ™); `LevelLoad.pak`'s `Logo_Loading` (672x336) is a plainer copy, `ProtoTitle.pak`'s `levelTitle` (860x484) an earlier logo on clouds |
| Background | `archives/levelload.pak` | `textures@!ui!Loading!BG_Full` | 1024x724: the blueprint paper of the loading screens. The main menu is a 3D scene (`MainMenuBackground.pak`: the start screen's logo models and a skybox); no 2D menu painting was found on the disc [?] |
| Portraits | `archives/toycollectionmaterials.pak` | `materialInstances/ToyCollection/Collection_<Toy>_<Variant>.igz`, each naming its texture (`textures@!ui!Collections!ToyInventoryIcons!<year>!<Toy>`) | **469**, 88x88, round badges: every toy of the five games (year folders 2011 to 2015: characters, traps, vehicles, the race packs), e.g. `collection_souljam_normal` (Fiesta), `collection_vehicleundeadcar_normal` (Crypt Crusher) |
| Menu theme | `archives/inter_streamed_bank.pak` | `INTER_Main_Theme.wav,0,48000,kDefaultFormat,40,1,0,1` | FMOD FSB5/XMA, 48 kHz stereo, 80.6 s; music throughout (RMS 2500 to 8400 per 10 s, 0.011 % of samples clipped) |

- The theme: the streamed bank's events (`permanent.pak`, `sounds/banks/inter_streamed_bank.igz`) have `INTER_Menu_Screen` without a sample of
  its own, as in Swap Force, next to `INTER_Main_Theme` (and `INTER_Main_Theme_Fin2`); the only theme sample is `INTER_Main_Theme.wav`. The link from the
  menu event to that sub-sound was not followed object by object as for Swap Force [?]; the other candidate, `INTER_ActionPack_Academy_Menu_01`,
  is the race packs' menu.
- Proof: `SSAArtTest`'s disc extraction run once on this disc with a temporary call (logo 1, background 1, title icon 1, 469 portraits, 0.85 s,
  0.00 s cached); `Arkeyan-Builder --iso <disc> --menu-music <file.wav> --kit <scratch kit>` (`@done`). Shots in `private/superchargers/art/`.

## For the launcher (not done here)
- The catalogue's SuperChargers toys need portrait keys (the badge names above, `collection_<internal name>_<variant>`) and SuperChargers in
  `SSAArtTest`'s disc list (`TestDisc`, with the drawn-icon count it then has); the internal names are the game's (Fiesta is `SoulJam`, Crypt
  Crusher `VehicleUndeadCar`).
- `tools/sync_skylauncher.sh` carries the reader (IGA v11 big-endian, IGZ v9, the separators) and the manifest.

## Button pictures (glyphs, 2026-11-04) [Alchemy] [SuperChargers] [V]
Swap Force's way ([input.md](https://github.com/arkeyan-forge/forge/blob/main/docs/runtime/xbox360/input.md), "Glyphs"): the game composes its strings on the CPU, so the
buttons inside every text (the pause menu's "Select", "Back", "Quests" [V]; the tunnel ride's "Hold RT to Accelerate!", not run with a
style yet [?]) come from two pictures named in the fonts, which
`glyphs::MemoryImage` (adapter `GlyphImages`) redraws in guest memory for the device in use.

| Picture | Where | Status |
|---|---|---|
| Info font's buttons, 256x168, 32-bit tiled ARGB | `fonts/ProximaNova-Info.igz` (`languagestartup.pak`), `igImage2` 1 | **Swap Force's own** (the same XXH3 of the first 4096 bytes, `7360B650D459A892`; components measured again: the same boxes): Swap Force's cells, without its added Start cell (SuperChargers has no START hook) [V] |
| Body font's buttons, **128x209**, 32-bit tiled ARGB | `fonts/ProximaNova-Body.igz` (`essentialui.pak`), `igImage2` 1, `402F2F6915FA96F8` | new layout, two buttons a row: A 1,2-38,39 · B 44,1-80,39 · Y 85,1-123,39 · X 2,42-39,80 · LB 44,49-105,76 · LT 4,83-40,121 · RT 49,83-86,121 · RB 3,131-65,158 · LS 73,125-110,161 · RS 5,166-42,202; a D-pad at 47,164-88,202 has no glyph and stays [V] |
| `Turbo_A`, `_X`, `_Y` (88x88 DXT5) | `permanent_xenon.pak`, `textures@!ui!ButtonPrompt!XENON!turbo!` | not listed: the renderer hashes a tiled level padded to 32x32 blocks (16 KB) and the file stores 11 KB, so the hash cannot be taken from the disc; no run reached a turbo prompt [?] |
| World prompts `models/UI/ButtonPrompt/XENON/{A,B,X,Y,LT,RB,Mash_*,RS_*,...}` | `permanent_xenon.pak` | meshes coloured by 4x4 colour maps, as Swap Force's: stay the 360's (stream G) |
| Controller pictures (`ui!Controllers!Xbox_Controller`...) | | not replaced |

How (about 1 h): the two archives extracted from the disc (`tools/shaders/xdvdfs.py`) into `private/superchargers/disc/`, their pictures
listed and their stored pixels written by a scratch tool on `runtime/host/art/alchemy_archive.cpp` (`private/superchargers/scratch/fonts/`),
untiled, the opaque components' boxes printed and a picture of the body image looked at to name them.
Proof (run `scU2`, a continue into the Academy, 2560x1080, `SKY_GLYPHS_SCRIPT=ps5:122,switch:130,key:138,xbox360:146`, Start at 118 s):
`body font buttons 128x209 found in guest memory at AC6D6000`, `info font buttons 256x168 found ... at AC57A000`, each `drawn as`
keyboard, playstation, nintendo, keyboard, xbox360; the pause menu's hints in the shots: cross, circle, triangle; B, A, X (Nintendo, by
position); Space, X, V keycaps; the 360's A, B, Y.
