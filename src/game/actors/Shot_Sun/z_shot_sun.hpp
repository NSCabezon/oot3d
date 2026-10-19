#ifndef _Z_SHOT_SUN_H_
#define _Z_SHOT_SUN_H_

#include "global.hpp"

struct ShotSun;

typedef void (*ShotSunActionFunc)(struct ShotSun* self, PlayState* play);

typedef struct ShotSun {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ColliderCylinder collider;
    /* 0x01FC */ ShotSunActionFunc actionFunc;
    /* 0x0200 */ char unk_200[0x8];
    /* 0x0208 */ u8 fairySpawnerState;
    /* 0x0209 */ char unk_209[0x7];
} ShotSun; // size = 0x210

extern ActorInit Shot_Sun_InitVars;

#endif
