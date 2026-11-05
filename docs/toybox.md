# SuperChargers' Toy Box: vehicles, seats, badges

> **Purpose:** what the in-game Toy Box and the collection show for SuperChargers (its vehicles, seats and pictures), how it is proved, what is still open · **Status:** current (2026-11-04) · **Tags:** [Alchemy] [SuperChargers] [V] [?]

The same screens and code as the other games (`runtime/host/toybox/`, [toybox_ux.md](https://github.com/arkeyan-forge/forge/blob/main/docs/runtime/host/toybox_ux.md));
`runtime/` has no SuperChargers case. What makes it SuperChargers' is data: its catalogue entry (seats, badges) and the vehicle
fields of the tag ([figures.md](https://github.com/arkeyan-forge/forge/blob/main/docs/runtime/host/figures.md#superchargers-vehicles-alchemy-superchargers)).

| Part | What [V] |
|---|---|
| Seats | Player 1, **Vehicle 1**, Player 2, **Vehicle 2**, Magic item / trophy (the catalogue's 5 seats); a vehicle seat takes a vehicle (category `vehicle`) and nothing else |
| Vehicles | 20 types (3220-3241), 13 decos from the owner's dumps (Dark, Legendary, Power Blue, Nitro, Golden, E3, holiday, chase); a blank one made by the Toy Box or `new:<type>` is read and driven (`undeadcar.pak` opens when the game reads Crypt Crusher, 181 s into a new game) |
| A vehicle's sheet | "Gearbits: N · driven <time>" once the game used it, "New: never driven" before; gearbits change through `collection::SetGearbits` (the launcher's Edit) |
| Trophies | 3500 Sky, 3501 Land, 3502 Sea, 3503 Kaos (the dumps' tags; the public list had two swapped); the item seat |
| Pictures | the disc's 469 round badges (88 px, `archives/toycollectionmaterials.pak`, [art.md](art.md)): 363 toys and variants mapped in the catalogue's `portraits` (SuperChargers' 44 and their decos, every older toy Swap Force and Trap Team name the same way). Drawn icons left: Smash Hit (no badge of its own on this disc) and the UFO Hat (only its variant's). `SUPERCHARGERS_ISO=<disc> SSAArtTest` checks them |

**Internal names** (the badges' `collection_<name>_<deco>`): Fiesta `souljam`, High Volt `lightbulb`, Splat `inkwell`, Stormblade
`peacock`, Spitfire `nitro`, Dive-Clops `airlock`, Astroblast `lightdriver`, Nightfall `darkdriver`, Thrillipede `lifebug`, the six
reposed cores `driver<name>`; vehicles `vehicle<name>`: Jet Stream `airplane`, Tomb Buggy `doombuggy`, Reef Ripper `watersub`,
Burn-Cycle `firebike`, Hot Streak `firecar`, Shark Tank `earthtank`, Thump Truck `earthtruck`, Crypt Crusher `undeadcar`, Stealth
Stinger `lifecopter` (its plain badge is named `plane`), Dive Bomber `watertorpedosub`, Sky Slicer `sparhawk`, Clown Cruiser
`clownplane`, Gold Rusher `techbike`, Shield Striker `techtruck`, Sun Runner `lightplane`, Sea Shadow `darkmantasub`, Splatter
Splasher `magicsub`, Soda Skimmer `magicfloat`, Barrel Blaster `dkcart`, Buzz Wing `lifeplane`; trophies `racepack_<sea, land, air,
kaos>`. Matched by looking at each badge and by the decos the dumps hold (a Power Blue badge for the toy that has a Power Blue
figure); Gold Rusher and Shield Striker by that rule only [?].

## Proof
- `tools/regress/superchargers_toybox.sh` (`SUPERCHARGERS_ISO`, `build-superchargers/`, about 4 min, through `../gamelock.sh`): a
  scratch collection of Fiesta, Crypt Crusher and the Sky Trophy, the game on an empty portal; the Toy Box (F6) seats Fiesta on
  Player 1, Crypt Crusher on Vehicle 1 and the trophy, then the menus' presses run to the intro: the log must show both seated and
  taken by the game; seven pictures.
- The tag fields: [figures.md](https://github.com/arkeyan-forge/forge/blob/main/docs/runtime/host/figures.md#superchargers-vehicles-alchemy-superchargers).

## The portal prompt (2026-11-04) [V]
The Toy Box opens by itself when the game asks for a figure, **and for a vehicle** ("Please place a LAND VEHICLE on the Portal of
Power"), on that seat, and closes when the prompt is gone. Code: `games/superchargers/superchargers_portal_prompt.*` and the
adapter's `ReadPortalPrompt` (four guest reads a frame); `runtime/` only learnt a `vehicle` flag on the prompt (seat `vehicle<N>`).
- **The object** is Swap Force's `CMagicMoment` singleton. Found by its class: the string `CMagicMoment` (`0x820B8374`) is
  registered by `sub_82B8CA78` with its descriptor `0x83765EA0`; the 3-instruction getter of that descriptor, `sub_82B39BC8` (Swap
  Force's `sub_8285FEA8`), is slot 1 of the class's vtable (`0x8209EF14`, found by its address in the image); slots 6 and 7,
  `sub_82B49390` and `sub_82B493A0`, store and clear the instance at **`0x83764ED0`** (Swap Force's slots 6 and 7 do the same with
  `0x83131BE0`).
- **The fields**, measured in runs `mmB` and `mmC` (a dump of the object at each state change, empty portal, A every 4 s):
  the state at **`+0x2C`** (the enum's names at `0x820976A0` are in value order: Swap Force's with `FirstVehicleSequence` added at 7,
  so Idle 5, IdleOutro 8, Dead 14, Error 17), the reason at **`+0x168`** (values listed at `0x820974B0`: 13 VehicleRequired as the
  vehicle prompt comes up; 12, VehicleAdded by the names' order, both after a vehicle came and while the prompt stays up with
  none, run `scP3`), the player asked for a Skylander at **`+0x1FC`** (0 for player 1; -1 while the vehicle is asked for). So a
  vehicle prompt is Idle with nobody named and the reason 13 or 12. A new game asks for the vehicle first (states 1, 2, 3, 18, 5 with reason 13), then, after the vehicle's
  intro (7, 8, 9, 11), for the Skylander (5 with player 0).
- **The run** `scP1` (empty portal, a scratch collection of Fiesta and Crypt Crusher, A every 4 s to 160 s, then Return every 5 s):
  `[toybox] prompt for player 1's vehicle`, `V1: Crypt Crusher on slot 2`, `the prompt is gone`, `V1: the game took Crypt Crusher`
  (`undeadcar.pak` at 182 s), then `prompt for player 1`, `P1: Fiesta on slot 0`, `P1: the game took Fiesta` (`souljam.pak` at
  201 s). `SSAPortalTest` checks the classification (`ClassifySuperChargers`).
- **A prompt counts after 2.5 s.** With the Skylander already on the portal, the vehicle's intro still passes through Idle for
  its driver (state 5, player 0) and the game reads the figure about a second later: the first build opened the Toy Box there
  and marked Fiesta out of health ("Fiesta is out of health: pick another one.", run `scV1`). The reader now reports a prompt
  once it has held for 2.5 s: run `scV2` (both figures placed from the start) goes through the two Idle passes with no prompt,
  `undeadcar.pak` at 182 s and `souljam.pak` at 196 s.
- **After both fixes** (runs `scP4`, `scP5`, 2026-11-04): on an empty portal the Toy Box opens on "Choose for Vehicle 1" by itself
  while the game shows "Please place a LAND VEHICLE" (shot `private/runs/scP4-shots_prompt.png`), Return seats Crypt Crusher, the
  game takes it, the Toy Box closes, `undeadcar.pak` opens, then it opens by itself on Player 1; with the vehicle placed from the
  start only the Skylander prompt opens it.

## Open [?]
- Player 2's prompts, the Dead state (14) and the error screens (17) were not run; while a vehicle is asked for no player field
  was seen, so a vehicle prompt goes to player 1 unless `+0x1FC` names one.
- A vehicle's mods (performance, specialty, cosmetic: bought in the Academy's garage) are not mapped. The Academy is reached
  (`SKY_LEVEL=academy`, or a continue), but the garage is a place to walk to: the pause menu (run `scM1`) has the quests, the
  controls (the 360's diagram, not replaced) and the Skylander's stats, hats and upgrades, no vehicle mods. Player 2's vehicle seat
  is untried in the game.
