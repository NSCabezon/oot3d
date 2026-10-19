#ifndef _Z_BG_SPOT16_BOMBSTONE_H_
#define _Z_BG_SPOT16_BOMBSTONE_H_

#include "global.hpp"

struct BgSpot16Bombstone;

typedef void (*BgSpot16BombstoneActionFunc)(struct BgSpot16Bombstone* self, PlayState* play);

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct BgSpot16BombstoneColliderElement {
    /* 0x00 */ u8 unk_00[0x38];
    /* 0x38 */ Vec3f worldCenter;
    /* 0x44 */ f32 worldRadius;
    /* 0x48 */ u8 unk_48[0x8];
} BgSpot16BombstoneColliderElement; // size = 0x50

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ BgSpot16BombstoneColliderElement* elements;
} BgSpot16BombstoneColliderJntSph; // size = 0x20

typedef struct BgSpot16Bombstone {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ BgSpot16BombstoneActionFunc actionFunc;
    /* 0x01A8 */ u16 unk_154; // frame counter, incremented every Update
    /* 0x01AA */ s16 switchFlag; // params bits 8..13
    /* 0x01AC */ u16 unk_158; // index into the spawned-fragment table
    /* 0x01B0 */ f32 sinRotation;
    /* 0x01B4 */ f32 cosRotation;
    /* 0x01B8 */ BgSpot16BombstoneColliderJntSph colliderJntSph;
    /* 0x01D8 */ BgSpot16BombstoneColliderElement colliderJntSphElements[1];
    /* 0x0228 */ ColliderCylinder colliderCylinder;
    /* 0x0280 */ s16 unk_210; // rotation step X (debris)
    /* 0x0282 */ s16 unk_212; // rotation step Z (debris)
    /* 0x0284 */ s8 requiredObjectSlot;
    /* 0x0288 */ SkeletonAnimationModel* skelAnimModel;
} BgSpot16Bombstone; // size = 0x28C

extern ActorInit Bg_Spot16_Bombstone_InitVars;

#endif
