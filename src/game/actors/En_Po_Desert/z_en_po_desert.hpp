#ifndef _Z_EN_PO_DESERT_H_
#define _Z_EN_PO_DESERT_H_

#include "global.hpp"

struct EnPoDesert;

typedef void (*EnPoDesertActionFunc)(struct EnPoDesert*, struct PlayState*);

typedef struct EnPoDesert {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char jointTable[0x340];
    /* 0x0568 */ char morphTable[0x340];
    /* 0x08A8 */ SkeletonAnimationModel* skelModel;
    /* 0x08AC */ MTX34 skelModelMtx;
    /* 0x08DC */ EnPoDesertActionFunc actionFunc;
    /* 0x08E0 */ s16 actionTimer;
    /* 0x08E2 */ s16 speedModifier;
    /* 0x08E4 */ s32 currentPathPoint;
    /* 0x08E8 */ f32 initDistToNextPoint;
    /* 0x08EC */ f32 yDiff;
    /* 0x08F0 */ f32 targetY;
    /* 0x08F4 */ Color_RGBA8 lightColor;
    /* 0x08F8 */ void* lightNode;
    /* 0x08FC */ char lightInfo[0x18];
    /* 0x0914 */ ColliderCylinder collider;
} EnPoDesert; // size = 0x096C

extern ActorInit En_Po_Desert_InitVars;

#endif
