#ifndef _Z_EN_FLOORMAS_H_
#define _Z_EN_FLOORMAS_H_

#include "global.hpp"
#include <stddef.h>

struct EnFloormas;

typedef struct EnFloormas {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x634];
    /* 0x07D8 */ void (*actionFunc)(EnFloormas*, GameState*);
    /* 0x07DC */ s16 timer;
    /* 0x07DE */ s16 actionTarget;
    /* 0x07E0 */ s16 zOffset;
    /* 0x07E2 */ s16 smallTimer;
    /* 0x07E4 */ ColliderCylinder collider;
} EnFloormas; // size = 0x83C

typedef char EnFloormasLayoutCheck[(sizeof(EnFloormas) == 0x83C &&
                                     sizeof(ColliderCylinder) == 0x58 &&
                                     offsetof(EnFloormas, collider) == 0x7E4 &&
                                     offsetof(EnFloormas, actionFunc) == 0x7D8 &&
                                     offsetof(EnFloormas, timer) == 0x7DC &&
                                     offsetof(EnFloormas, actionTarget) == 0x7DE) ? 1 : -1];

extern ActorInit En_Floormas_InitVars;

#endif
