#ifndef _Z_BG_HIDAN_SYOKU_H_
#define _Z_BG_HIDAN_SYOKU_H_

#include "global.hpp"

struct BgHidanSyoku;

typedef void (*BgHidanSyokuActionFunc)(struct BgHidanSyoku*, PlayState*);

typedef struct BgHidanSyoku {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgHidanSyokuActionFunc actionFunc;
    /* 0x01C0 */ s16 unk_168;
    /* 0x01C2 */ s16 timer;
    /* 0x01C4 */ SkeletonAnimationModel* skelAnimModel;
} BgHidanSyoku; // size = 0x1C8

extern ActorInit Bg_Hidan_Syoku_InitVars;

#endif
