#ifndef _Z_BG_MIZU_WATER_H_
#define _Z_BG_MIZU_WATER_H_

#include "global.hpp"
#include "graphics/skeleton_animation_model.hpp"

struct BgMizuWater;

typedef void (*BgMizuWaterActionFunc)(struct BgMizuWater*, PlayState*);

typedef struct BgMizuWater {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ BgMizuWaterActionFunc actionFunc;
    /* 0x01A8 */ s32 type;
    /* 0x01AC */ f32 targetY;
    /* 0x01B0 */ f32 baseY;
    /* 0x01B4 */ s32 switchFlag; // only used for types 2-4
    /* 0x01B8 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01BC */ f32 drawY;
} BgMizuWater; // size = 0x01C0

#define WATER_TEMPLE_WATER_F3_Y 765.0f
#define WATER_TEMPLE_WATER_F2_Y 445.0f
#define WATER_TEMPLE_WATER_F1_Y -15.0f
#define WATER_TEMPLE_WATER_B1_Y -835.0f

#define WATER_TEMPLE_WATER_F3_FLAG 0x1E
#define WATER_TEMPLE_WATER_F2_FLAG 0x1D
#define WATER_TEMPLE_WATER_F1_FLAG 0x1C

extern ActorInit Bg_Mizu_Water_InitVars;

#endif
