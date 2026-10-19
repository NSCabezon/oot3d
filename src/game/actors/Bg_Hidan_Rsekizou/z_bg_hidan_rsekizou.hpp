#ifndef _Z_BG_HIDAN_RSEKIZOU_H_
#define _Z_BG_HIDAN_RSEKIZOU_H_

#include "global.hpp"

struct BgHidanRsekizou;

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ Vec3f center;
    /* 0x0C */ f32 radius;
} BgHidanRsekizouSphere; // size = 0x10

typedef struct {
    /* 0x00 */ u8 unk_00[0x28];
    /* 0x28 */ BgHidanRsekizouSphere modelSphere;
    /* 0x38 */ BgHidanRsekizouSphere worldSphere;
    /* 0x48 */ u8 unk_48[0x08];
} BgHidanRsekizouColliderElement; // size = 0x50

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ BgHidanRsekizouColliderElement* elements;
} BgHidanRsekizouColliderJntSph; // size = 0x20

typedef struct {
    /* 0x00 */ ColliderInit base;
    /* 0x08 */ s32 count;
    /* 0x0C */ void* elements;
} BgHidanRsekizouJntSphInit;

typedef struct BgHidanRsekizou {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ s16 bendFrame;
    /* 0x01BE */ s16 burnFrame;
    /* 0x01C0 */ BgHidanRsekizouColliderJntSph collider;
    /* 0x01E0 */ BgHidanRsekizouColliderElement colliderElements[6];
    /* 0x03C0 */ SkeletonAnimationModel* skelAnimModel[9];
} BgHidanRsekizou; // size = 0x3E4

extern ActorInit Bg_Hidan_Rsekizou_InitVars;

#endif
