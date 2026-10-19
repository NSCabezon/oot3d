#ifndef _Z_BG_MORI_BIGST_H_
#define _Z_BG_MORI_BIGST_H_

#include "global.hpp"

struct BgMoriBigst;

typedef void (*BgMoriBigstActionFunc)(struct BgMoriBigst*, PlayState*);

typedef struct BgMoriBigst {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgMoriBigstActionFunc actionFunc;
    /* 0x01C0 */ s16 waitTimer;
    /* 0x01C2 */ s8 moriTexObjectSlot;
    /* 0x01C3 */ char unk_1C3;
    /* 0x01C4 */ SkeletonAnimationModel* skelAnimModel;
} BgMoriBigst; // size = 0x1C8

extern ActorInit Bg_Mori_Bigst_InitVars;

#endif
