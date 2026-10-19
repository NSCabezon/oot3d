#ifndef _Z_BG_BOM_GUARD_H_
#define _Z_BG_BOM_GUARD_H_

#include "global.hpp"

struct BgBomGuard;

typedef void (*BgBomGuardActionFunc)(struct BgBomGuard*, PlayState*);

typedef struct BgBomGuard {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgBomGuardActionFunc actionFunc;
    /* 0x01C0 */ u8 isActive;
    /* 0x01C4 */ Vec3f homePos;
} BgBomGuard; // size = 0x1D0

extern ActorInit Bg_Bom_Guard_InitVars;

#endif
