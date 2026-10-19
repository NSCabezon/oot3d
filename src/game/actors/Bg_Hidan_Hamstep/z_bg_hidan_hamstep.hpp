#ifndef _Z_BG_HIDAN_HAMSTEP_H_
#define _Z_BG_HIDAN_HAMSTEP_H_

#include "global.hpp"

struct BgHidanHamstep;

typedef void (*BgHidanHamstepActionFunc)(struct BgHidanHamstep* self, PlayState* play);

// Collision types not yet in the shared headers (3DS layout), same as Bg_Haka_Sgami / Bg_Ydan_Maruta.
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
    /* 0x00 */ u8 bytes[0x5C];
} ColliderTrisElement;

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ ColliderTrisElement* elements;
} ColliderTris; // size = 0x20

typedef struct BgHidanHamstep {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ ColliderTris collider;
    /* 0x01DC */ ColliderTrisElement colliderElements[2];
    /* 0x0294 */ BgHidanHamstepActionFunc actionFunc;
    /* 0x0298 */ s32 action;
    /* 0x029C */ s32 unk_29C;
    /* 0x02A0 */ SkeletonAnimationModel* skelAnimModel[2];
} BgHidanHamstep; // size = 0x2A8

extern ActorInit Bg_Hidan_Hamstep_InitVars;

#endif
