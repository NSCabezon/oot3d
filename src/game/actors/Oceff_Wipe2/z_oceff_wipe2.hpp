#ifndef _Z_OCEFF_WIPE2_H_
#define _Z_OCEFF_WIPE2_H_

#include "global.hpp"

struct OceffWipe2;

typedef struct OceffWipe2 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ s16 timer;
    /* 0x01A6 */ char unk_1A6[0x2];
    /* 0x01A8 */ SkeletonAnimationModel* model;
    /* 0x01AC */ void* cmb;
    /* 0x01B0 */ void* obj;
} OceffWipe2; // size = 0x1B4

extern ActorInit Oceff_Wipe2_InitVars;

#endif
