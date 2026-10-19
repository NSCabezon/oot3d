#ifndef _Z_BG_MORI_RAKKATENJO_H_
#define _Z_BG_MORI_RAKKATENJO_H_

#include "global.hpp"

struct BgMoriRakkatenjo;

typedef void (*BgMoriRakkatenjoActionFunc)(struct BgMoriRakkatenjo*, PlayState*);

typedef struct BgMoriRakkatenjo {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgMoriRakkatenjoActionFunc actionFunc;
    /* 0x01C0 */ s32 timer;
    /* 0x01C4 */ s32 bounceCount;
    /* 0x01C8 */ s32 fallCount;
    /* 0x01CC */ s8 moriTexObjectSlot;
    /* 0x01CD */ char unk_1CD[0x3];
    /* 0x01D0 */ SkeletonAnimationModel* skelAnimModel;
} BgMoriRakkatenjo; // size = 0x1D4

extern ActorInit Bg_Mori_Rakkatenjo_InitVars;

#endif
