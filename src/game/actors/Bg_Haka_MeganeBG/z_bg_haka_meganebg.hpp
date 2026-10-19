#ifndef _Z_BG_HAKA_MEGANEBG_H_
#define _Z_BG_HAKA_MEGANEBG_H_

#include "global.hpp"

struct BgHakaMeganeBG;

typedef void (*BgHakaMeganeBGActionFunc)(struct BgHakaMeganeBG*, PlayState*);

typedef struct BgHakaMeganeBG {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgHakaMeganeBGActionFunc actionFunc;
    /* 0x01C0 */ u8 switchFlag;
    /* 0x01C1 */ char unk_1C1[0x1];
    /* 0x01C2 */ s16 timer;
    /* 0x01C4 */ SkeletonAnimationModel* skelAnimModel[4]; // indexed by params
    /* 0x01D4 */ char unk_1D4[0x4];
} BgHakaMeganeBG; // size = 0x1D8

extern ActorInit Bg_Haka_MeganeBG_InitVars;

#endif
