#ifndef _Z_BG_GND_FIREMEIRO_H_
#define _Z_BG_GND_FIREMEIRO_H_

#include "global.hpp"

struct BgGndFiremeiro;

typedef void (*BgGndFiremeiroActionFunc)(struct BgGndFiremeiro*, PlayState*);

typedef struct BgGndFiremeiro {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ Vec3f initPos;
    /* 0x01C8 */ u16 timer;
    /* 0x01CA */ char unk_1CA[0x2];
    /* 0x01CC */ BgGndFiremeiroActionFunc actionFunc;
    /* 0x01D0 */ SkeletonAnimationModel* skelAnimModel;
} BgGndFiremeiro; // size = 0x1D4

extern ActorInit Bg_Gnd_Firemeiro_InitVars;

#endif
