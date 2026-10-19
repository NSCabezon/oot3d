#ifndef _Z_EN_WALL_TUBO_H_
#define _Z_EN_WALL_TUBO_H_

#include "global.hpp"

struct EnWallTubo;

typedef void (*EnWallTuboActionFunc)(struct EnWallTubo*, PlayState*);

typedef struct EnWallTubo {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnWallTuboActionFunc actionFunc;
    /* 0x01A8 */ s16 timer;
    /* 0x01AA */ char unk_1AA[0x0E];
    /* 0x01B8 */ Actor* bowlingGirl; // EnBomBowlMan*
    /* 0x01BC */ Vec3f homePos;
} EnWallTubo; // size = 0x1C8

extern ActorInit En_Wall_Tubo_InitVars;

#endif
