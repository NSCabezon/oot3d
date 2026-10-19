#ifndef _Z_BG_JYA_BOMBCHUIWA_H_
#define _Z_BG_JYA_BOMBCHUIWA_H_

#include "global.hpp"

struct BgJyaBombchuiwa;

typedef void (*BgJyaBombchuiwaActionFunc)(struct BgJyaBombchuiwa*, PlayState*);

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ Vec3f center;
    /* 0x0C */ f32 radius;
} BgJyaBombchuiwaSphere; // size = 0x10

typedef struct {
    /* 0x00 */ u8 unk_00[0x28];
    /* 0x28 */ BgJyaBombchuiwaSphere modelSphere;
    /* 0x38 */ BgJyaBombchuiwaSphere worldSphere;
    /* 0x48 */ u8 unk_48[0x08];
} BgJyaBombchuiwaColliderElement; // size = 0x50

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ BgJyaBombchuiwaColliderElement* elements;
} BgJyaBombchuiwaColliderJntSph; // size = 0x20

typedef struct BgJyaBombchuiwa {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ BgJyaBombchuiwaActionFunc actionFunc;
    /* 0x01A8 */ BgJyaBombchuiwaColliderJntSph collider;
    /* 0x01C8 */ char unk_1C8[0x50];
    /* 0x0218 */ f32 lightRayIntensity;
    /* 0x021C */ s16 timer;
    /* 0x021E */ u8 drawFlags; // Used to determine how the actor is drawn.
    /* 0x021F */ char unk_21F[0x1];
    /* 0x0220 */ SkeletonAnimationModel* skelAnimModel0;
    /* 0x0224 */ SkeletonAnimationModel* skelAnimModel1;
    /* 0x0228 */ SkeletonAnimationModel* skelAnimModel2;
    /* 0x022C */ SkeletonAnimationModel* skelAnimModel3;
    /* 0x0230 */ void* unk_230;
} BgJyaBombchuiwa; // size = 0x234

extern ActorInit Bg_Jya_Bombchuiwa_InitVars;

#endif
