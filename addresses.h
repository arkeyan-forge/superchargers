// Skylanders: SuperChargers: the guest functions the runtime hooks and the guest data it reads. Same names as the other
// games' addresses.h. A hook the game does not define is compiled out. A data global is a constexpr in namespace game, 0
// while the address is not known. Never reuse another game's address: every one here was re-found in SuperChargers' own
// binary (tools/sigmatch against Trap Team's and Swap Force's inventories, tools/xmatch.py shape as the second witness;
// games/superchargers/docs/research.md has the numbers).
#pragma once

// D3D (runtime/d3d/*.cpp): XDK D3D9 2.0.21256, Trap Team's. Ten are strict matches (1.0) of Trap Team's functions, and
// tools/xmatch.py offsets finds no device offset that moved (the few displacements that differ are global addresses);
// Resolve is sigmatch 0.62, shape 0.87 (1060 instructions against 1048), the only candidate above 0.2 [?].
#define GAME_HOOK_D3DDevice_NextRingSegment sub_82873EC0
#define GAME_HOOK_D3DDevice_KickOff sub_82874D18
#define GAME_HOOK_D3DDevice_BlockOnFence sub_82874E18
#define GAME_HOOK_D3DDevice_InsertCallback sub_82874F10
#define GAME_HOOK_D3DDevice_DrawVertices sub_82881158
#define GAME_HOOK_D3DDevice_DrawIndexedVertices sub_82881570
#define GAME_HOOK_D3DDevice_Resolve sub_8287FA48
#define GAME_HOOK_D3DDevice_ClearF_Worker sub_8287EC80
#define GAME_HOOK_D3DDevice_Swap sub_828786C0
#define GAME_HOOK_D3DVertexShader_Init sub_8287D530
#define GAME_HOOK_D3DPixelShader_Init sub_8287D338

// XMA (runtime/kernel/xma.cpp): the same two engines. Xma1_Init, Xma1_VoiceKick, Xma2_VoiceLock, Xma2_VoiceKick and
// Xma1_LockAll are strict matches of Trap Team's (and of Swap Force's); Xma2_Init, Xma1_VoiceClear and Xma2_VoiceClear are
// loose (0.9) [?].
#define GAME_HOOK_Xma1_Init sub_835967D8
#define GAME_HOOK_Xma2_Init sub_827EFD18
#define GAME_HOOK_Xma1_VoiceClear sub_83596838
#define GAME_HOOK_Xma1_VoiceKick sub_835971D0
#define GAME_HOOK_Xma2_VoiceClear sub_8286DE98
#define GAME_HOOK_Xma2_VoiceLock sub_8286E040
#define GAME_HOOK_Xma2_VoiceKick sub_8286E2F0
#define GAME_HOOK_Xma1_LockAll sub_83597060

// LZMA (engines/alchemy/lzma.cpp): the LZMA SDK's LzmaDec_DecodeReal, strict against Trap Team's and Swap Force's.
#define GAME_HOOK_LzmaDec_DecodeReal sub_82506808

// Bink 2 (runtime/d3d/bink_display.cpp): BinkOpen sigmatch 0.58, shape 0.74 (38 instructions; the next candidate 0.31);
// BinkClose sigmatch 0.74, shape 0.91 (296 instructions) [?].
#define GAME_HOOK_BinkOpen sub_831C9218
#define GAME_HOOK_BinkClose sub_831C9F38

#include <cstdint>

namespace game
{
    // D3D: the offsets the runtime reads, Trap Team's (Giants'), carried by the strict matches above [?].
    constexpr uint32_t DeviceVertexDeclaration = 0x2FD0;
    constexpr bool DrawIndexedBaseVertex = false; // DrawIndexedVertices: (device, primitive, start, count)
    constexpr bool ExpandTiledTargets = true;
    constexpr bool ResolveHasLevel = false; // Resolve: slice r8, color r9
    constexpr uint32_t DeviceStateShift = 0x80;
    constexpr uint32_t LanguageIndex = 0;
    constexpr uint32_t LanguageCodes = 0;
    constexpr uint32_t NglCurrentScene = 0;
    constexpr uint32_t Engine1CriticalSection =
        0x8370CE70; // sigmatch 1.0 (Trap Team's and Swap Force's), via Xma1_LockAll
    constexpr uint32_t DeviceGlobal = 0;
    constexpr uint32_t UltrawideDisplayAspect = 0;
    constexpr uint32_t UltrawideSceneWidth = 0, UltrawideSceneHeight = 0;
    constexpr bool UltrawideSceneKeepsArea = false;
    constexpr uint32_t UltrawideCompositeVertices = 0;
    constexpr int UltrawideCompositeSlot = 0;
    constexpr bool UltrawideCompositeVpos = false;
    constexpr uint64_t HostKernelPixelShader = 0;
    constexpr uint32_t HostKernelThreshold = 0;
}
