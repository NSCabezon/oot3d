#ifndef _Z_EN_HONOTRAP_H_
#define _Z_EN_HONOTRAP_H_

#include "global.hpp"

struct EnHonotrap;

typedef void (*EnHonotrapActionFunc)(struct EnHonotrap*, struct PlayState*);

// Collision types not yet in the shared headers (3DS layout), same as Bg_Bombwall.
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

typedef union EnHonotrapCollider {
    /* 0x00 */ ColliderCylinder cyl;
    /* 0x00 */ struct {
        /* 0x00 */ Collider base;
        /* 0x18 */ s32 count;
        /* 0x1C */ ColliderTrisElement* elements;
        /* 0x20 */ ColliderTrisElement elems[2];
    } tris;
} EnHonotrapCollider;

typedef struct EnHonotrap {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnHonotrapActionFunc actionFunc;
    /* 0x01A8 */ EnHonotrapCollider collider;
    /* 0x0280 */ s16 timer;
    /* 0x0282 */ s16 eyeState;
    /* 0x0284 */ Vec3f targetPos;
    /* 0x0290 */ f32 speedMod;
    /* 0x0294 */ s16 bobPhase;
    /* 0x0296 */ s16 flameScroll;
    /* 0x0298 */ u8 colChkFlags;
    /* 0x0299 */ char unk_299[0x3];
    /* 0x029C */ SkeletonAnimationModel* eyeModel;
    /* 0x02A0 */ SkeletonAnimationModel_unk_0C* eyeAnim;
    /* 0x02A4 */ SkeletonAnimationModel* flameModel;
    /* 0x02A8 */ void* unk_2A8;
    /* 0x02AC */ SkeletonAnimationModel_unk_0C* flameAnim;
} EnHonotrap; // size = 0x2B0

extern ActorInit En_Honotrap_InitVars;

#endif
