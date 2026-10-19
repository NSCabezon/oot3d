#ifndef _Z_EN_EX_RUPPY_H_
#define _Z_EN_EX_RUPPY_H_

#include "global.hpp"

struct EnExRuppy;

typedef void (*EnExRuppyActionFunc)(EnExRuppy*, PlayState*);

typedef struct EnExRuppy {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnExRuppyActionFunc actionFunc;
    /* 0x01A8 */ s16 colorIdx;
    /* 0x01AA */ s16 type;
    /* 0x01AC */ s16 invisible;
    /* 0x01AE */ s16 timer;
    /* 0x01B0 */ s16 rupeeValue;
    /* 0x01B2 */ s16 throwDistance;
    /* 0x01B4 */ s16 galleryFlag;
    /* 0x01B6 */ char unk_1B6[2];
    /* 0x01B8 */ f32 scale;
    /* 0x01BC */ SkeletonAnimationModel* skelAnimModel;
} EnExRuppy; // size = 0x1C0

extern ActorInit En_Ex_Ruppy_InitVars;

#endif
