#ifndef _Z_BG_JYA_GOROIWA_H_
#define _Z_BG_JYA_GOROIWA_H_

#include "global.hpp"

struct BgJyaGoroiwa;

typedef void (*BgJyaGoroiwaActionFunc)(struct BgJyaGoroiwa* self, PlayState* play);

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ Vec3f center;
    /* 0x0C */ f32 radius;
} BgJyaGoroiwaSphere; // size = 0x10

typedef struct {
    /* 0x00 */ u8 unk_00[0x38];
    /* 0x38 */ BgJyaGoroiwaSphere worldSphere;
    /* 0x48 */ u8 unk_48[0x08];
} BgJyaGoroiwaColliderElement; // size = 0x50

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ BgJyaGoroiwaColliderElement* elements;
} BgJyaGoroiwaColliderJntSph; // size = 0x20

typedef struct BgJyaGoroiwa {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ BgJyaGoroiwaActionFunc actionFunc;
    /* 0x01A8 */ BgJyaGoroiwaColliderJntSph collider;
    /* 0x01C8 */ BgJyaGoroiwaColliderElement colliderElements[1];
    /* 0x0218 */ f32 speedFactor;
    /* 0x021C */ u16 hasHit;
    /* 0x021E */ s16 waitTimer;
    /* 0x0220 */ f32 yOffsetSpeed;
    /* 0x0224 */ SkeletonAnimationModel* skelAnimModel;
} BgJyaGoroiwa; // size = 0x228

extern ActorInit Bg_Jya_Goroiwa_InitVars;

#endif
