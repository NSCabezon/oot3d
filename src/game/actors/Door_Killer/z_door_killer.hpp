#ifndef _Z_DOOR_KILLER_H_
#define _Z_DOOR_KILLER_H_

#include "global.hpp"

struct DoorKiller;

typedef void (*DoorKillerActionFunc)(struct DoorKiller*, PlayState*);

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ void* elements;
} DoorKillerColliderJntSph; // size = 0x20

typedef struct {
    MTX34 m;
    f32 pad;
} DoorKillerJoint; // size = 0x34

typedef struct DoorKiller {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ DoorKillerJoint joints[16];
    /* 0x0568 */ char unk_568[0x1CC];
    /* 0x0734 */ void* unk_734;
    /* 0x0738 */ char unk_738[0x1CD];
    /* 0x0905 */ u8 playerIsOpening;
    /* 0x0906 */ char unk_906[0x2];
    /* 0x0908 */ ColliderCylinder colliderCylinder;
    /* 0x0960 */ u16 hasHitPlayerOrGround;
    /* 0x0962 */ u16 timer;
    /* 0x0964 */ char unk_964[0x3];
    /* 0x0967 */ s8 dieCounter;
    /* 0x0968 */ u8 switchFlagSet;
    /* 0x0969 */ char unk_969[0x3];
    /* 0x096C */ DoorKillerColliderJntSph colliderJntSph;
    /* 0x098C */ char unk_98C[0x50];
    /* 0x09DC */ DoorKillerActionFunc actionFunc;
} DoorKiller; // size = 0x09E0

extern ActorInit Door_Killer_InitVars;

#endif
