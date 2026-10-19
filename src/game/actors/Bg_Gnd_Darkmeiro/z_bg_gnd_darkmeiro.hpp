#ifndef _Z_BG_GND_DARKMEIRO_H_
#define _Z_BG_GND_DARKMEIRO_H_

#include "global.hpp"

struct BgGndDarkmeiro;

typedef void (*BgGndDarkmeiroActionFunc)(struct BgGndDarkmeiro* self, PlayState* play);

typedef struct BgGndDarkmeiro {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ u16 actionFlags;
    /* 0x01BE */ s16 timer1;
    /* 0x01C0 */ s16 timer2;
    /* 0x01C2 */ s16 timer3;
    /* 0x01C4 */ BgGndDarkmeiroActionFunc updateFunc;
    /* 0x01C8 */ SkeletonAnimationModel* skelAnimModel1;
    /* 0x01CC */ SkeletonAnimationModel* skelAnimModel2;
    /* 0x01D0 */ char unk_1D0[0xC];
} BgGndDarkmeiro; // size = 0x1DC

extern ActorInit Bg_Gnd_Darkmeiro_InitVars;

#endif
