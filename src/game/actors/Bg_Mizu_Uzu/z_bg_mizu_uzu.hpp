#ifndef _Z_BG_MIZU_UZU_H_
#define _Z_BG_MIZU_UZU_H_

#include "global.hpp"

struct BgMizuUzu;

typedef void (*BgMizuUzuActionFunc)(struct BgMizuUzu*, PlayState*);

typedef struct BgMizuUzu {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgMizuUzuActionFunc actionFunc;
} BgMizuUzu; // size = 0x1C0

extern ActorInit Bg_Mizu_Uzu_InitVars;

#endif
