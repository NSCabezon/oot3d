#ifndef _Z_DEMO_GEFF_H_
#define _Z_DEMO_GEFF_H_

#include "global.hpp"

struct DemoGeff;

typedef void (*DemoGeffFunc)(struct DemoGeff*, struct PlayState*);

typedef struct DemoGeff {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ s32 action;
    /* 0x01A8 */ s32 drawConfig;
    /* 0x01AC */ s32 objectSlot;
    /* 0x01B0 */ Actor* demoGt;
    /* 0x01B4 */ f32 deltaPosX;
    /* 0x01B8 */ f32 deltaPosY;
    /* 0x01BC */ f32 deltaPosZ;
    /* 0x01C0 */ SkeletonAnimationModel* skelAnimModel;
} DemoGeff; // size = 0x1C4

extern ActorInit Demo_Geff_InitVars;

#endif
