#ifndef _Z_BG_JYA_HAHENIRON_H_
#define _Z_BG_JYA_HAHENIRON_H_

#include "global.hpp"

struct BgJyaHaheniron;

typedef void (*BgJyaHahenironActionFunc)(struct BgJyaHaheniron* self, PlayState* play);

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ Vec3f center;
    /* 0x0C */ f32 radius;
} BgJyaHahenironSphere; // size = 0x10

typedef struct {
    /* 0x00 */ u8 unk_00[0x38];
    /* 0x38 */ BgJyaHahenironSphere worldSphere;
    /* 0x48 */ u8 unk_48[0x08];
} BgJyaHahenironColliderElement; // size = 0x50

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ BgJyaHahenironColliderElement* elements;
} BgJyaHahenironColliderJntSph; // size = 0x20

typedef struct {
    /* 0x00 */ ColliderInit base;
    /* 0x08 */ s32 count;
    /* 0x0C */ void* elements;
} BgJyaHahenironJntSphInit;

typedef struct BgJyaHaheniron {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ BgJyaHahenironActionFunc actionFunc;
    /* 0x01A8 */ BgJyaHahenironColliderJntSph collider;
    /* 0x01C8 */ BgJyaHahenironColliderElement colliderElements[1];
    /* 0x0218 */ s16 timer;
    /* 0x021C */ SkeletonAnimationModel* skelAnimModel;
} BgJyaHaheniron; // size = 0x220

extern ActorInit Bg_Jya_Haheniron_InitVars;

#endif
