#ifndef _Z_BG_MENKURI_EYE_H_
#define _Z_BG_MENKURI_EYE_H_

#include "global.hpp"

struct BgMenkuriEye;

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ u8 unk_00[0x34];
    /* 0x34 */ f32 modelSphereRadius;
    /* 0x38 */ Vec3f worldSphereCenter;
    /* 0x44 */ f32 worldSphereRadius;
    /* 0x48 */ u8 unk_48[0x8];
} BgMenkuriEyeColliderElement; // size = 0x50

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ BgMenkuriEyeColliderElement* elements;
} BgMenkuriEyeColliderJntSph; // size = 0x20

typedef struct BgMenkuriEye {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ s16 framesUntilDisable;
    /* 0x01A8 */ BgMenkuriEyeColliderJntSph collider;
    /* 0x01C8 */ BgMenkuriEyeColliderElement colliderElements[1];
    /* 0x0218 */ SkeletonAnimationModel* skelAnimModel;
} BgMenkuriEye; // size = 0x21C

extern ActorInit Bg_Menkuri_Eye_InitVars;

#endif
