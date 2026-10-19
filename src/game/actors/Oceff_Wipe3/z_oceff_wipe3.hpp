#ifndef _Z_OCEFF_WIPE3_H_
#define _Z_OCEFF_WIPE3_H_

#include "global.hpp"

struct OceffWipe3;

typedef struct OceffWipe3 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ s16 counter;
    /* 0x01A6 */ char unk_1A6[2];
    /* 0x01A8 */ SkeletonAnimationModel* model;
    /* 0x01AC */ void* cmb;
    /* 0x01B0 */ void* camObj;
} OceffWipe3; // size = 0x1B4

extern ActorInit Oceff_Wipe3_InitVars;

#endif
