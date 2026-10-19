#ifndef _Z_BG_GND_SOULMEIRO_H_
#define _Z_BG_GND_SOULMEIRO_H_

#include "global.hpp"

struct BgGndSoulmeiro;

typedef void (*BgGndSoulmeiroActionFunc)(struct BgGndSoulmeiro*, PlayState*);

typedef struct BgGndSoulmeiro {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ColliderCylinder collider;
    /* 0x01FC */ u16 timer;
    /* 0x01FE */ char unk_1FE[0x2];
    /* 0x0200 */ BgGndSoulmeiroActionFunc actionFunc;
    /* 0x0204 */ SkeletonAnimationModel* skelAnimModel;
} BgGndSoulmeiro; // size = 0x208

extern ActorInit Bg_Gnd_Soulmeiro_InitVars;

#endif
