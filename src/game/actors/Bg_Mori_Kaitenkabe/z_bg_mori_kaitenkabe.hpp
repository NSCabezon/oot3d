#ifndef _Z_BG_MORI_KAITENKABE_H_
#define _Z_BG_MORI_KAITENKABE_H_

#include "global.hpp"

struct BgMoriKaitenkabe;

typedef void (*BgMoriKaitenkabeActionFunc)(struct BgMoriKaitenkabe*, PlayState*);

typedef struct BgMoriKaitenkabe {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgMoriKaitenkabeActionFunc actionFunc;
    /* 0x01C0 */ s32 timer;
    /* 0x01C4 */ f32 rotDirection;
    /* 0x01C8 */ f32 rotSpeed;
    /* 0x01CC */ f32 rotYdeg;
    /* 0x01D0 */ Vec3f lockedPlayerPos;
    /* 0x01DC */ s8 moriTexObjectSlot;
    /* 0x01E0 */ SkeletonAnimationModel* skelAnimModel;
} BgMoriKaitenkabe; // size = 0x1E4

extern ActorInit Bg_Mori_Kaitenkabe_InitVars;

#endif
