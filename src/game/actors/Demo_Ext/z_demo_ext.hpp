#ifndef _Z_DEMO_EXT_H_
#define _Z_DEMO_EXT_H_

#include "global.hpp"

struct DemoExt;

typedef void (*DemoExtFunc)(Actor*, GameState*);

// N64 layout (0x0184 bytes) shifted by +0x58 on 3DS (Actor is 0x1A4 bytes)
typedef struct DemoExt {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ s32 action;
    /* 0x01A8 */ s32 drawMode;
    /* 0x01AC */ s32 cueId;
    /* 0x01B0 */ s16 scrollIncr[4];
    /* 0x01B8 */ s16 curScroll[4];
    /* 0x01C0 */ s16 rotationPitch;
    /* 0x01C4 */ f32 alphaTimer;
    /* 0x01C8 */ s32 primAlpha;
    /* 0x01CC */ s32 envAlpha;
    /* 0x01D0 */ Vec3f scale;
    /* 0x01DC */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01E0 */ SkeletonAnimationModel_unk_0C* skelAnimModelUnk0C;
} DemoExt; // size = 0x01E4

extern ActorInit Demo_Ext_InitVars;

#endif
