#ifndef _Z_BG_BREAKWALL_H_
#define _Z_BG_BREAKWALL_H_

#include "global.hpp"

struct BgBreakwall;

typedef void (*BgBreakwallActionFunc)(struct BgBreakwall*, PlayState*);

// 3DS collider quad (N64 layout, 0x80 bytes); opaque here, only its address is used
typedef struct BgBreakwallQuad {
    /* 0x00 */ u8 unk_00[0x80];
} BgBreakwallQuad;

typedef struct BombableWallInfo {
    /* 0x00 */ s8 colIdx;
    /* 0x01 */ s8 dListIdx; // -1 = no display list
    /* 0x02 */ s8 colType;
} BombableWallInfo;

typedef struct BgBreakwall {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgBreakwallActionFunc actionFunc;
    /* 0x01C0 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01C4 */ BgBreakwallQuad collider;
    /* 0x0244 */ s8 colType;
    /* 0x0245 */ s8 requiredObjectSlot;
    /* 0x0246 */ u8 unk_246;
    /* 0x0247 */ u8 breakTimer; // 3DS: set in Wait (60 or 90) instead of Actor_Kill
} BgBreakwall; // size = 0x248

extern ActorInit Bg_Breakwall_InitVars;

#endif
