#ifndef _Z_BG_HAKA_HUTA_H_
#define _Z_BG_HAKA_HUTA_H_

#include "global.hpp"

struct BgHakaHuta;

typedef void (*BgHakaHutaActionFunc)(struct BgHakaHuta*, PlayState*);

typedef struct BgHakaHuta {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgHakaHutaActionFunc actionFunc;
    /* 0x01C0 */ s16 counter;
    /* 0x01C2 */ s16 unk_1C2; // N64 unk_16A: spawn type from params bits 8..15
    /* 0x01C4 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01C8 */ SkeletonAnimationModel* unk_1C8; // always NULL
} BgHakaHuta; // size = 0x1CC

extern ActorInit Bg_Haka_Huta_InitVars;

#endif
