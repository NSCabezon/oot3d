#ifndef _Z_BG_HIDAN_SIMA_H_
#define _Z_BG_HIDAN_SIMA_H_

#include "global.hpp"

struct BgHidanSima;

typedef void (*BgHidanSimaActionFunc)(struct BgHidanSima*, PlayState*);

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ Vec3f center;
    /* 0x0C */ f32 radius;
} BgHidanSimaSphere; // size = 0x10

typedef struct {
    /* 0x00 */ u8 unk_00[0x28];
    /* 0x28 */ BgHidanSimaSphere modelSphere;
    /* 0x38 */ BgHidanSimaSphere worldSphere;
    /* 0x48 */ u8 unk_48[0x08];
} BgHidanSimaColliderElement; // size = 0x50

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ BgHidanSimaColliderElement* elements;
} BgHidanSimaColliderJntSph; // size = 0x20

typedef struct BgHidanSima {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgHidanSimaActionFunc actionFunc;
    /* 0x01C0 */ s16 timer;
    /* 0x01C2 */ char unk_1C2[0x2];
    /* 0x01C4 */ BgHidanSimaColliderJntSph collider;
    /* 0x01E4 */ char unk_1E4[0xA0];
    /* 0x0284 */ SkeletonAnimationModel* skelAnimModel[6];
    /* 0x029C */ s32 unk_29C;
} BgHidanSima; // size = 0x2A0

extern ActorInit Bg_Hidan_Sima_InitVars;

#endif
