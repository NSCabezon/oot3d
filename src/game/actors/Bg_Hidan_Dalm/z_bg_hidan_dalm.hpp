#ifndef _Z_BG_HIDAN_DALM_H_
#define _Z_BG_HIDAN_DALM_H_

#include "global.hpp"

struct BgHidanDalm;

typedef void (*BgHidanDalmActionFunc)(struct BgHidanDalm* self, PlayState* play);

// Collision types not yet in the shared headers (3DS layout), same as Bg_Hidan_Hamstep.
typedef struct {
    /* 0x00 */ Vec3f vtx[3];
} ColliderTrisElementDimInit; // size = 0x24

typedef struct {
    /* 0x00 */ ColliderInfoInit info;
    /* 0x18 */ ColliderTrisElementDimInit dim;
} ColliderTrisElementInit; // size = 0x3C

typedef struct {
    /* 0x00 */ ColliderInit base;
    /* 0x08 */ s32 count;
    /* 0x0C */ ColliderTrisElementInit* elements;
} ColliderTrisInit; // size = 0x10

typedef struct {
    /* 0x00 */ ColliderInfo base;
    /* 0x28 */ Vec3f vtx[3];
    /* 0x4C */ u8 unk_4C[0x10];
} ColliderTrisElement; // size = 0x5C

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ ColliderTrisElement* elements;
} ColliderTris; // size = 0x20

typedef struct BgHidanDalm {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgHidanDalmActionFunc actionFunc;
    /* 0x01C0 */ u8 switchFlag;
    /* 0x01C4 */ ColliderTris collider;
    /* 0x01E4 */ ColliderTrisElement colliderElements[4];
    /* 0x0354 */ SkeletonAnimationModel* skelAnimModel[2];
} BgHidanDalm; // size = 0x35C

extern ActorInit Bg_Hidan_Dalm_InitVars;

#endif
