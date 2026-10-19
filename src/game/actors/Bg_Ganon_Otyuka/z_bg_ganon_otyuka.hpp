#ifndef _Z_BG_GANON_OTYUKA_H_
#define _Z_BG_GANON_OTYUKA_H_

#include "global.hpp"

struct BgGanonOtyuka;

typedef void (*BgGanonOtyukaActionFunc)(struct BgGanonOtyuka*, PlayState*);

typedef struct BgGanonOtyuka {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgGanonOtyukaActionFunc actionFunc;
    /* 0x01C0 */ s16 dropTimer;
    /* 0x01C2 */ u8 isFalling;
    /* 0x01C3 */ u8 unwalledSides;
    /* 0x01C4 */ u8 visibleSides;
    /* 0x01C5 */ u8 flashTimer;
    /* 0x01C6 */ u8 flashState;
    /* 0x01C8 */ f32 flashYScale;
    /* 0x01CC */ f32 flashPrimColorR;
    /* 0x01D0 */ f32 flashPrimColorG;
    /* 0x01D4 */ f32 flashPrimColorB;
    /* 0x01D8 */ f32 flashEnvColorR;
    /* 0x01DC */ f32 flashEnvColorG;
    /* 0x01E0 */ f32 flashEnvColorB;
    /* 0x01E4 */ SkeletonAnimationModel* skelAnimModel[5];
} BgGanonOtyuka; // size = 0x01F8

extern ActorInit Bg_Ganon_Otyuka_InitVars;

#endif
