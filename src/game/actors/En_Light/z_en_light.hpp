#ifndef _Z_EN_LIGHT_H_
#define _Z_EN_LIGHT_H_

#include "global.hpp"

struct EnLight;

typedef struct EnLight {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ void* model; // SkeletonAnimationModel*
    /* 0x01A8 */ void* unk_1A8;
    /* 0x01AC */ u8 timer;
    /* 0x01AD */ char unk_1AD[0x3];
    /* 0x01B0 */ void* lightNode; // LightNode*
    /* 0x01B4 */ char lightInfo[0x18]; // LightInfo, 3DS layout (local)
} EnLight; // size = 0x1CC

extern ActorInit En_Light_InitVars;

#endif
