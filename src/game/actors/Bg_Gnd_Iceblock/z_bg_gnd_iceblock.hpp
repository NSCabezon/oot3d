#ifndef _Z_BG_GND_ICEBLOCK_H_
#define _Z_BG_GND_ICEBLOCK_H_

#include "global.hpp"

struct BgGndIceblock;

typedef void (*BgGndIceblockActionFunc)(struct BgGndIceblock* self, PlayState* play);

typedef struct BgGndIceblock {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgGndIceblockActionFunc actionFunc;
    /* 0x01C0 */ Vec3f targetPos;
    /* 0x01CC */ SkeletonAnimationModel* model;
} BgGndIceblock; // size = 0x1D0

extern ActorInit Bg_Gnd_Iceblock_InitVars;

#endif
