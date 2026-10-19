#ifndef _Z_EN_SKJNEEDLE_H_
#define _Z_EN_SKJNEEDLE_H_

#include "global.hpp"

struct EnSkjneedle;

typedef struct EnSkjneedle {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01A8 */ char unk_1A8[0x64];
} EnSkjneedle; // size = 0x20C

extern ActorInit En_Skjneedle_InitVars;

#endif
