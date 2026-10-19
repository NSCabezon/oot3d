#ifndef _Z_BG_HIDAN_FWBIG_H_
#define _Z_BG_HIDAN_FWBIG_H_

#include "global.hpp"

struct BgHidanFwbig;

typedef void (*BgHidanFwbigActionFunc)(struct BgHidanFwbig*, PlayState*);

typedef struct BgHidanFwbig {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ BgHidanFwbigActionFunc actionFunc;
    /* 0x01A8 */ s8 direction;
    /* 0x01A9 */ u8 moveState;
    /* 0x01AA */ s16 timer;
    /* 0x01AC */ ColliderCylinder collider;
    /* 0x0204 */ SkeletonAnimationModel* skelAnimModel;
} BgHidanFwbig; // size = 0x208

extern ActorInit Bg_Hidan_Fwbig_InitVars;

#endif
