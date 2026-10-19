#ifndef _Z_MAGIC_WIND_H_
#define _Z_MAGIC_WIND_H_

#include "global.hpp"
#include "graphics/skeleton_animation_model.hpp"

struct MagicWind;

typedef void (*MagicWindFunc)(struct MagicWind*, PlayState*);

typedef struct MagicWind {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char skelCurve[0x8];
    /* 0x01AC */ ZARInfo* zarInfo;
    /* 0x01B0 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01B4 */ char unk_1B4[0x128];
    /* 0x02DC */ f32 alpha;
    /* 0x02E0 */ s16 timer;
    /* 0x02E2 */ char unk_2E2[0x2];
    /* 0x02E4 */ MagicWindFunc actionFunc;
} MagicWind; // size = 0x2E8

extern ActorInit Magic_Wind_InitVars;

#endif
