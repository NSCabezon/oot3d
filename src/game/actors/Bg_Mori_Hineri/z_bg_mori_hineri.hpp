#ifndef _Z_BG_MORI_HINERI_H_
#define _Z_BG_MORI_HINERI_H_

#include "global.hpp"

struct BgMoriHineri;

typedef void (*BgMoriHineriActionFunc)(struct BgMoriHineri*, PlayState*);

typedef struct BgMoriHineri {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgMoriHineriActionFunc actionFunc;
    /* 0x01C0 */ s8 moriHineriObjectSlot;
    /* 0x01C1 */ s8 altObjectSlot;
    /* 0x01C2 */ s8 boxObjectSlot;
    /* 0x01C3 */ s8 switchFlag;
    /* 0x01C4 */ SkelAnime skelAnime;
    /* 0x0248 */ SkeletonAnimationModel* skelAnimModel0;
    /* 0x024C */ SkeletonAnimationModel* skelAnimModel1;
    /* 0x0250 */ s32 unk_250;
    /* 0x0254 */ SkeletonAnimationModel* skelAnimModel2;
    /* 0x0258 */ char unk_258[0x8];
} BgMoriHineri; // size = 0x260

extern ActorInit Bg_Mori_Hineri_InitVars;

#endif
