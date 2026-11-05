// SuperChargers' adapter: day 1, the minimum (runtime/game.h). No portal prompt, glyphs, intro list, language or first-level
// redirect yet; it presents at every blank like the other Alchemy games [?].
#include "game.h"

namespace
{
    const game::Adapter g_adapter = { g_gameInfo, nullptr, false, nullptr, {}, {}, true };
}

const game::Adapter& game::Current() { return g_adapter; }
