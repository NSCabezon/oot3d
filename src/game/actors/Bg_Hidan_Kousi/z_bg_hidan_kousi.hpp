#ifndef _Z_BG_HIDAN_KOUSI_H_
#define _Z_BG_HIDAN_KOUSI_H_

#include "global.hpp"

struct BgHidanKousi;

typedef void (*BgHidanKousiActionFunc)(struct BgHidanKousi*, PlayState*);

typedef struct BgHidanKousi {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgHidanKousiActionFunc actionFunc;
    /* 0x01C0 */ s16 unk_168;
    /* 0x01C2 */ char unk_1C2[0x2];
    /* 0x01C4 */ SkeletonAnimationModel* skelAnimModel;
} BgHidanKousi; // size = 0x1C8

extern ActorInit Bg_Hidan_Kousi_InitVars;

#endif
