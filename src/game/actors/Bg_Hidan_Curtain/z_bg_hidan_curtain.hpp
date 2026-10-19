#ifndef _Z_BG_HIDAN_CURTAIN_H_
#define _Z_BG_HIDAN_CURTAIN_H_

#include "global.hpp"

struct BgHidanCurtain;

typedef void (*BgHidanCurtainActionFunc)(struct BgHidanCurtain*, PlayState*);

typedef struct BgHidanCurtain {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ BgHidanCurtainActionFunc actionFunc;
    /* 0x01A8 */ u8 type;
    /* 0x01A9 */ u8 treasureFlag;
    /* 0x01AA */ u8 size;
    /* 0x01AB */ u8 alpha;
    /* 0x01AC */ s16 timer;
    /* 0x01AE */ s16 texScroll;
    /* 0x01B0 */ ColliderCylinder collider;
    /* 0x0208 */ SkeletonAnimationModel* model;
} BgHidanCurtain; // size = 0x20C

extern ActorInit Bg_Hidan_Curtain_InitVars;

#endif
