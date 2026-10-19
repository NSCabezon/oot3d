#ifndef _Z_BG_MIZU_SHUTTER_H_
#define _Z_BG_MIZU_SHUTTER_H_

#include "global.hpp"

struct BgMizuShutter;

typedef void (*BgMizuShutterActionFunc)(struct BgMizuShutter*, PlayState*);

typedef struct BgMizuShutter {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgMizuShutterActionFunc actionFunc;
    /* 0x01C0 */ s32 timer;
    /* 0x01C4 */ s32 timerMax;
    /* 0x01C8 */ f32 maxSpeed;
    /* 0x01CC */ Vec3f closedPos;
    /* 0x01D8 */ Vec3f openPos;
    /* 0x01E4 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01E8 */ u8 unk_1E8;
} BgMizuShutter; // size = 0x01EC

extern ActorInit Bg_Mizu_Shutter_InitVars;

#endif
