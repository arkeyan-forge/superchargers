# SuperChargers boot: what broke on the way to the first level

> **Purpose:** what stood between SuperChargers' disc and its first level, the causes and the fixes · **Status:** current (2026-11-04) · **Tags:** [Xbox] [XDK] [Alchemy] [SuperChargers] [V] [?]

## Fixed on day 1 (2026-11-03) [V]
- **Two instructions XenonRecomp did not know**: `vpkuwum128` (17 sites; a modulo pack, the low halves of the words) and `vadduhs` (2;
  unsigned saturating halfword add). Added to `patches/xenonrecomp.patch`, with samples in `tools/xenonrecomp/sample_gen.cpp` checked by
  `SSAXenonRecompTest`. No earlier game has them (their XenonRecomp logs are clean), so their generated code does not change.
- **The portal thread aborted at start** (`No code padding left for a host thunk`, signal 6 in `PortalThread`): the title asks for
  `XamInputNonControllerGetRaw` through `XexGetProcedureAddress`, and the runtime places a host thunk in a run of 16 zero words between
  code sections. SuperChargers' code is one section (`0x82450000` + `0x129CDBC`) with no such run: 55124 lone zero words between
  functions, 7 runs of 2 and 7 of 3. `CreateHostThunk` now takes a lone zero word where no function starts when no run of 16 exists
  (`runtime/thunk_slot.h`, `SSAThunkSlotTest`); a thunk is only an entry of the function table, and such a word is never a call target.
  The other games still find their run of 16 first, as before.
- **A trap on a job worker at 13 s** (signal 5 in `sub_82691C70`): `vpkd3d128 v63,v62,3,1,3`, the FLOAT16_2 pack (4 sites), was emitted
  as `__builtin_debugtrap`. Written as the inverse of our `vupkd3d128` FLOAT16_2 (x to the high half of the word that `shift` names,
  y to the low half, the D3DCOLOR form's word convention; the float to half conversion of the FLOAT16_4 form), with a round-trip sample.
  The regenerated code differs only at the 4 sites; the 167 FLOAT16_4 sites are byte for byte the same.
- **Ten imports** (`games/superchargers/imports.cpp`): network and Live calls answer as offline, the audio ducker is off. Seen called
  so far: none of them logged a first call in the runs to 210 s [V].

## The black frame (fixed 2026-11-04) [V]
Every image presented was black (23 shots, menus, movies, intro) while the game drew 110 to 346 draws a frame, none skipped. Two
causes, one after the other, both in our renderer; neither is SuperChargers-specific.

1. **An empty scissor applied with the scissor test off.** The frame dump printed `sc 0 1440 0 1440` (left, top, right, bottom) for every
   draw into the 2560x1440 frame target and for 42 scene draws: an empty rectangle. The game leaves an empty rectangle in the device
   (`D3DDevice_SetScissorRect`, `sub_82871118`, stores it as given at `+0x32FC`) with `D3DRS_SCISSORTESTENABLE` off (`+0x3000`, the dump
   now prints it: `(test 0)`), and that function only folds the rectangle into the window scissor register when the test is on. The
   renderer applied the device's rectangle to every draw (the clear path already checked the flag). Fix: `d3d::DrawScissor`
   (`runtime/d3d/render_scale.h`, tested in `SSARenderScaleTest`): the whole target when the test is off. The movies, the title, the
   menus' text and the HUD appeared (run `sc7`).
2. **A SHORT4N position read as (y, x, w, z).** The menus' 3D background and the whole world stayed black, and moving HUD elements left
   trails: the frame target was never covered. Its first draw, the composite of the scene (`vs 263a001ff7d5a917`, `ps 72f716a3a7453b07`,
   an indexed quad, stride 20), feeds the position as SHORT4N (`R16G16B16A16_SNORM`). Vertex data is byte-swapped in 32-bit words, which
   swaps the halves of each 16-bit pair; only texture coordinates were swapped back (in the translated shader). The quad got w = z = 0
   and never rasterized. Fix: any other input in a 16-bit pair format has its stream copied on the CPU and its halves swapped back
   (`d3d::SwapHalfPairs`, `runtime/d3d/vertex_pairs.h`, `SSAVertexPairsTest`). No earlier game fed a 16-bit position.

The other games with both fixes [V] (2026-11-04): `tools/test_all.sh` ALL PASS (400 checks; the frames of SSA's level, Giants' three runs and
Swap Force's tutorial within their PSNR bounds: 22.6, 31.5, 28.6, 24.9, 19.5 dB); Trap Team, whose harness section has no game run, to
its tutorial with Gusto (`new:450`) in an isolated run: the whole scene and the HUD drawn, no fault.

The leads of day 1, each checked [V]:
| Lead | Verdict | Evidence |
|---|---|---|
| Device offsets taken from Trap Team | not the cause | the two D3D functions that write the scissor field (`sub_8286E830`, `sub_82871118`) are Trap Team's at shape 1.00 with no displacement moved (`tools/xmatch.py offsets`); the picture is right with Trap Team's offsets |
| The `Resolve` hook (fuzzy match) | not the cause | the scene's resolves hold the picture in every dump (menu, tunnel); the frame is right with the same hook |
| The frame target's depth/stencil state | not the cause | the composite runs with z off (`depthctl 00708730`) and was still empty; the scissor and the position explain all of it |
| The frame target's format, EDRAM base, the gamma/present pass | not the cause | unchanged by the two fixes, which give the full picture (runs `sc7`, `sc12`) |

## To the first level with a figure and a vehicle (2026-11-04) [V]
Run `sc12`: the isolation block, `SKY_PORTAL=emulated SKY_PORTAL_PLACE=new:3400,new:3227` (Fiesta and Crypt Crusher, her own land vehicle,
both blank tags made by our generator), A every 4 s to the play style, Y to skip, A for the dialogues, then RT held (`rt:205:30`, the
tutorial says "Hold RT to Accelerate!") with some steering. 30 fps, no fault in 335 s.
- 14 s `bumper_vvisions.bk2`; the title ("press start"); 32 s `SUPERCHARGERS_OPTIONS`; the play style menu ("Choose your play style", the
  difficulty) over the 3D menu background; 40 s `SUPERCHARGERS_0`; 42 s `fmv01.bk2` and the intro (`intro.pak`): Master Eon's cutscene.
- 181 s the vehicle is read (`undeadcar.pak` and its sound banks), then Fiesta's (`souljam.pak`); without a vehicle on the portal the game
  asks "Please place a LAND VEHICLE on the Portal of Power" and offers an instant vehicle (A takes Hot Streak, `firecar.pak`; run `sc8`).
- From 205 s the rift tunnel ride is played: Crypt Crusher driven by Fiesta, the HUD, the gearbit counter rising (31 at 240 s, 52 at
  300 s), the tutorial's hints ("Use the stick to change direction").
- Portal traffic: `R`, `A`, `S`, `Q` (the figure reads), `W` (writes, answered), `J`, `L`, `V` (lights, ignored).
