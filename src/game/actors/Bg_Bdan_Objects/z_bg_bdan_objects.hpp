#ifndef _Z_BG_BDAN_OBJECTS_H_
#define _Z_BG_BDAN_OBJECTS_H_

#include "global.hpp"

struct BgBdanObjects;

typedef void (*BgBdanObjectsActionFunc)(struct BgBdanObjects* self, PlayState* play);

typedef struct BgBdanObjects {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgBdanObjectsActionFunc actionFunc;
    /* 0x01C0 */ u8 switchFlag;
    /* 0x01C1 */ char unk_1C1[0x1];
    /* 0x01C2 */ s16 timer;
    /* 0x01C4 */ ColliderCylinder collider;
    /* 0x021C */ s32 unk_21C;
    /* 0x0220 */ struct SkeletonAnimationModel* skelAnimModel[4];
    /* 0x0230 */ s32 unk_230;
} BgBdanObjects; // size = 0x234

extern ActorInit Bg_Bdan_Objects_InitVars;

#endif
