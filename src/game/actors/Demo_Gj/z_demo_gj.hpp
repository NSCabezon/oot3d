#ifndef _Z_DEMO_GJ_H_
#define _Z_DEMO_GJ_H_

#include "global.hpp"

struct DemoGj;

typedef struct DemoGj {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ s32 updateMode;
    /* 0x01C0 */ s32 drawConfig;
    /* 0x01C4 */ Vec3s rotationVec;
    /* 0x01CA */ Vec3s unk_1CA;
    /* 0x01D0 */ struct BossGanon2* ganon;
    /* 0x01D4 */ s32 isTransformedIntoGanon;
    /* 0x01D8 */ s32 isRotated;
    /* 0x01DC */ ColliderCylinder colliderCylinders[3];
    /* 0x02E4 */ s32 killFlag; // never set by this actor, read by the explosion update
    /* 0x02E8 */ Vec3f unk_2E8;
    /* 0x02F4 */ SkeletonAnimationModel* unk_2F4;
    /* 0x02F8 */ SkeletonAnimationModel* unk_2F8;
    /* 0x02FC */ SkeletonAnimationModel* unk_2FC;
    /* 0x0300 */ SkeletonAnimationModel* unk_300;
    /* 0x0304 */ SkeletonAnimationModel* unk_304;
    /* 0x0308 */ SkeletonAnimationModel* unk_308;
    /* 0x030C */ SkeletonAnimationModel* unk_30C;
    /* 0x0310 */ SkeletonAnimationModel* unk_310;
} DemoGj; // size = 0x314

extern ActorInit Demo_Gj_InitVars;

#endif
