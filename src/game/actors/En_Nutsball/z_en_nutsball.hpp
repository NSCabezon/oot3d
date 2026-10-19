#ifndef _Z_EN_NUTSBALL_H_
#define _Z_EN_NUTSBALL_H_

#include "global.hpp"

struct EnNutsball;

typedef void (*EnNutsballActionFunc)(struct EnNutsball* self, PlayState* play);

typedef struct EnNutsball {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnNutsballActionFunc actionFunc;
    /* 0x01A8 */ s8 requiredObjectSlot;
    /* 0x01A9 */ char unk_1A9[0x3];
    /* 0x01AC */ ColliderCylinder collider;
    /* 0x0204 */ void* model;
} EnNutsball; // size = 0x208

extern ActorInit En_Nutsball_InitVars;

#endif
