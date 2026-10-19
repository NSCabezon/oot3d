#ifndef _Z_BG_YDAN_MARUTA_H_
#define _Z_BG_YDAN_MARUTA_H_

#include "global.hpp"

struct BgYdanMaruta;

typedef void (*BgYdanMarutaActionFunc)(struct BgYdanMaruta*, PlayState*);

// Collision types not yet in the shared headers (3DS layout), same as Bg_Haka_Sgami.
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

typedef struct BgYdanMaruta {
    /* 0x0000 */ DynaPolyActor dyna; // dyna.actor = Actor, dyna.bgId at 0x1A4
    /* 0x01BC */ BgYdanMarutaActionFunc actionFunc;
    /* 0x01C0 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01C4 */ ColliderTris collider;
    /* 0x01E4 */ ColliderTrisElement colliderElements[2];
    /* 0x029C */ s16 unk_29C; // shake/timer
    /* 0x029E */ u8 switchFlag;
} BgYdanMaruta; // size = 0x2A0

extern ActorInit Bg_Ydan_Maruta_InitVars;

#endif
