# SuperChargers: research and day 1's numbers

> **Purpose:** what day 1 on the disc measured for SuperChargers (identity, XDK, sigmatch, codegen, imports), and the leads for vehicles and the new portal · **Status:** current (2026-11-03) · **Tags:** [Xbox] [XDK] [Alchemy] [SuperChargers] [V] [?]

The public facts from before the disc (release, toys, sources) are in [the older page](https://github.com/arkeyan-forge/forge/blob/main/docs/porting/future_games/superchargers/research.md);
every line there is [?] unless this page says [V]. Nothing here is copied from another project.

## Day 1 (2026-11-03): the Europe disc [V]
What `tools/day1.sh`, `tools/sigmatch`, `tools/xmatch.py`, `tools/find_switch_tables.py` and XenonRecomp gave. All derived files are in
`private/superchargers/` (day 1: `private/superchargers/day1/`).

| Question | Result [V] |
|---|---|
| Identity | title ID **`4156091F`** (the older page's guess from a Marketplace GUID was right), media ID `74144F53`, version `0x7`, disc 1 of 1; XEX SHA-256 `02ff6672...`, 24985600 bytes; image `0x01890000` bytes at `0x82000000` (Trap Team `0x00980000`), entry `0x83560158`, `.text` `0x82450000` + `0x129CDBC`; 291 imports |
| XDK | every library 2.0.**21256** (D3D9LTCG, XAUDIO2, XMP, XAPILIB...): **Trap Team's** (Swap Force 21250, Giants 21076); LIBCMT/LINK 11886 |
| Engine | **Alchemy**. SSA's inventory (297 entries): **177 found, 52 strict** (Trap Team 177 / 52): the XDK and CRT only, no Lux hook |
| Sigmatch, Trap Team's inventory (43, `tools/sigmatch/game_inventory.py trapteam`) | **42 found, 29 strict**: D3D 10 of 11 strict (`Resolve` 0.62), XMA 5 strict + 3 at 0.9, LZMA 1.0, `Console_Language` 1.0, `Scene_Tiles` 1.0, `Scene_SurfaceTiles` / `Gfx_ConfigInit` / `Ultrawide_Projection` 0.9, the ten helpers 1.0; `BinkOpen` 0.58, `BinkClose` 0.74, `Language_Get` 0.61, `Movie_Open` 0.76, `Ultrawide_ScenePass` 0.53, `LanguageAppObject` 0.04; not found: `Language_LoadPack` |
| Sigmatch, Swap Force's inventory (50) | **40 found, 24 strict**: D3D only 5 strict (`NextRingSegment`, `KickOff`, `BlockOnFence`, `DrawIndexedVertices`, `Resolve`, `Swap` fuzzy: Swap Force's D3D build differs), the same XMA, LZMA, Bink and helpers; engine hooks weak (`SaveTask_Poll` 0.77, `Font_CharMetrics` 0.61, `Language_Get` 0.62, `FallCheck` 0.57, `NightmareUnlocked` 0.49) or none (`Zone_IndexByName`, `SwapWait_Status`, `Language_SoundPak`, `Movie_Open`, `String_Intern`, both ultrawide hooks); `SaveTaskDone` 1.0 via `SaveTask_Poll`'s candidate |
| Reading | the XDK side (D3D, XMA, Bink, CRT) is Trap Team's build; the high hit rate is that, not the engine. The game side is **Swap Force's line** (Vicarious Visions): the disc is `/archives/*.pak` (2661) with `/archives/shaders_xenon.pak` and Bink 2 `.bk2` movies like Swap Force, not Trap Team's `.arc`/`.bld`; `CMagicMoment` is back (139 strings; Trap Team 2, Swap Force 138). Its engine hooks moved: re-find them by hand |
| D3D device | `tools/xmatch.py .:trapteam superchargers offsets` over the ten strict D3D functions: the only displacements that differ are global addresses (lo halves), no device field moved; `Resolve` (shape 0.87, 1060 instructions against 1048) has 22, from the misalignment. Trap Team's device constants are kept [?] |
| XenonRecomp | **two new instructions**: `vpkuwum128` (17 sites) and `vadduhs` (2), added to `patches/xenonrecomp.patch` with their samples in `tools/xenonrecomp/` (`SSAXenonRecompTest`: 72 checks, 0 failures). Then a clean log: **485 jump tables** (12 `bctr` unresolved at `0x82458DAC`-`0x82458E88`, one stride of 0x14: the index register is overwritten, a table of tail calls [?]), 36216 explicit functions (pass 2 brings one, `0x8342CBB8`, which overlaps pass 1's `0x8342CBE0` and is already in the mapping: not added), **102087 functions** in the mapping (Swap Force 82017, Trap Team 29961), 404 files, 328 MB, 3 s |
| C++ EH | 955 `__CxxFrameHandler` sites (`0x835F34B8`, sigmatch 1.0), 62 `__C_specific_handler` (import thunk `0x836EBE9C`; sigmatch's 0.4 candidate was another import), a custom handler `0x83605F80` before `0x83605F5C` (1.0) |
| Imports | **10** beyond our runtime's: `NetDll_getsockname`, `NetDll_XNetQosListen`, `NetDll_XNetQosLookup`, `XamQueryLiveHiveA`, `XamShowGameInviteUI`, `XAudioEnableDucker`, `XAudioGetDucker{AttackTime,HoldTime,ReleaseTime,Threshold}` (`games/superchargers/imports.cpp`: offline answers, outputs written, first call logged) |
| Shaders | `shaders_xenon.pak` (681983 bytes) is **IGA v11 big-endian** (Trap Team's v11 is little-endian, Swap Force's v10 big-endian) holding **IGZ v9**: read since day 2, 1207 containers from the disc, all in a run-time dump ([shaders.md](shaders.md)) |

## Vehicles and the new portal (leads, 2026-11-03) [?]
From the image's strings only (names, no code read yet):
- **Vehicles** are toys of their own: `_vehicleToyId`, `_vehicleTag`, `_vehicleTagList`, `_vehicleVariant`, `_vehicleMods`, `_vehicleUpgrade`,
  `_vehicleTypeAir` / `_vehicleTypeSea` (land the default), `_vehiclesOnPortal`, `eNC_VehiclePortalTags` (a list apart from the characters'),
  `_vehiclePortalDelay`, `_magicMomentDataStoredForVehicle`, `_magicMomentDataStoredForRacingPack`. So a vehicle tag is read and kept beside the
  character's, with its own data (mods, upgrades) [?].
- **Racing**: `CRacer`, `CRaceNodes`, `Race Mode`, `hERaceType`, trophies (`_trophyId`, `HTrophyBlueprints`, per-terrain trophy materials).
- **The portal**: `CPortalDriver`, `CVirtualPortal`, `EPortalHardwareYear`, `_portalVersion`, `CPortalLEDColor`, the `[TFB Portal] %s: ` log
  format (the portal library is Toys for Bob's, shared with Trap Team [?]), `_trapsOnPortal` (traps still read). The emulated portal already
  answers SuperChargers' four-byte id (`runtime/host/portal_identity.cpp`, `02 0A 03 02`; checked by `SSAPortalTest` only) [V].
- **Next**: the toy and variant tables from the disc (method in [collection.md](https://github.com/arkeyan-forge/forge/blob/main/docs/runtime/host/collection.md)) to check the 44
  catalogue rows (20 vehicles, 20 SuperChargers, 4 trophies; public lists so far), then `SKY_PORTAL_LOG=1` with a vehicle on the emulated portal.
