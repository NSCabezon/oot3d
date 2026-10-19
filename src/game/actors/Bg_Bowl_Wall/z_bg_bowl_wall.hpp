#ifndef _Z_BG_BOWL_WALL_H_
#define _Z_BG_BOWL_WALL_H_

#include "global.hpp"

struct BgBowlWall;

typedef void (*BgBowlWallActionFunc)(struct BgBowlWall*, PlayState*);

typedef struct BgBowlWall {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ s32 bgId;
    /* 0x01A8 */ char unk_1A8[0x14];
    /* 0x01BC */ BgBowlWallActionFunc actionFunc;
    /* 0x01C0 */ Vec3f homePos;
    /* 0x01CC */ Vec3f effCenterPos;
    /* 0x01D8 */ u16 targetHit;
    /* 0x01DA */ s16 timer;
    /* 0x01DC */ Actor* bowlingGirl; // EnBomBowlMan*
    /* 0x01E0 */ SkeletonAnimationModel* skelAnimModel;
} BgBowlWall; // size = 0x1E4

extern ActorInit Bg_Bowl_Wall_InitVars;

#endif
