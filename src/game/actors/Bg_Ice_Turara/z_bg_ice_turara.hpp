#ifndef _Z_BG_ICE_TURARA_H_
#define _Z_BG_ICE_TURARA_H_

#include "global.hpp"

struct BgIceTurara;

typedef void (*BgIceTurara_ActionFunc)(struct BgIceTurara* self, PlayState* play);

typedef struct BgIceTurara {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgIceTurara_ActionFunc actionFunc;
    /* 0x01C0 */ s16 shiverTimer;
    /* 0x01C2 */ char unk_1C2[0x2];
    /* 0x01C4 */ ColliderCylinder collider;
    /* 0x021C */ SkeletonAnimationModel* model;
} BgIceTurara; // size = 0x220

extern ActorInit Bg_Ice_Turara_InitVars;

#endif
