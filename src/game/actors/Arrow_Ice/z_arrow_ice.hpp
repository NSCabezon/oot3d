#ifndef _Z_ARROW_ICE_H_
#define _Z_ARROW_ICE_H_

#include "global.hpp"

struct ArrowIce;

typedef void (*ArrowIceActionFunc)(struct ArrowIce*, PlayState*);

// Object at +0x1C4: real virtual call, slot 1 (Destroy). _ZTI13ArrowIceModel is an EQU in
// src/game/shared_syms.s (absolute placeholder) so armlink VFE knows the call class.
extern "C++" {
class ArrowIceModel {
public:
    virtual void vf00();
    virtual void Destroy();
};
}

// Parent En_Arrow, only the fields Arrow_Ice reads (timer at +0x304, hitFlags at +0x305).
typedef struct ArrowIceParentArrow {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x160];
    /* 0x0304 */ u8 timer;
    /* 0x0305 */ u8 hitFlags;
} ArrowIceParentArrow;

typedef struct ArrowIce {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ s16 radius;
    /* 0x01A6 */ u16 timer;
    /* 0x01A8 */ u8 alpha;
    /* 0x01AC */ Vec3f unkPos;
    /* 0x01B8 */ f32 unk_1B8;
    /* 0x01BC */ f32 unk_1BC;
    /* 0x01C0 */ ArrowIceActionFunc actionFunc;
    /* 0x01C4 */ ArrowIceModel* model;
    /* 0x01C8 */ SkeletonAnimationModel_unk_0C* animCtl;
} ArrowIce; // size = 0x1CC

extern ActorInit Arrow_Ice_InitVars;

#endif
