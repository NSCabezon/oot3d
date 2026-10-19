#ifndef _Z_BG_JYA_BOMBIWA_H_
#define _Z_BG_JYA_BOMBIWA_H_

#include "global.hpp"

struct BgJyaBombiwa;

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ Vec3f center;
    /* 0x0C */ f32 radius;
} BgJyaBombiwaSphere; // size = 0x10

typedef struct {
    /* 0x00 */ u8 unk_00[0x38];
    /* 0x38 */ BgJyaBombiwaSphere worldSphere;
    /* 0x48 */ u8 unk_48[0x08];
} BgJyaBombiwaColliderElement; // size = 0x50

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ BgJyaBombiwaColliderElement* elements;
} BgJyaBombiwaColliderJntSph; // size = 0x20

typedef struct {
    /* 0x00 */ ColliderInit base;
    /* 0x08 */ s32 count;
    /* 0x0C */ void* elements;
} BgJyaBombiwaJntSphInit;

typedef struct BgJyaBombiwa {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ s32 unk_1BC;
    /* 0x01C0 */ BgJyaBombiwaColliderJntSph collider;
    /* 0x01E0 */ BgJyaBombiwaColliderElement colliderElements[1];
    /* 0x0230 */ SkeletonAnimationModel* skelAnimModel;
} BgJyaBombiwa; // size = 0x234

extern ActorInit Bg_Jya_Bombiwa_InitVars;

#endif
