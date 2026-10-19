#ifndef _Z_BG_SPOT03_TAKI_H_
#define _Z_BG_SPOT03_TAKI_H_

#include "global.hpp"

struct BgSpot03Taki;

typedef enum BgSpot03TakiState {
    WATERFALL_CLOSED,
    WATERFALL_OPENING_IDLE,
    WATERFALL_OPENING_ANIMATED,
    WATERFALL_OPENED,
    WATERFALL_CLOSING
} BgSpot03TakiState;

typedef void (*BgSpot03TakiActionFunc)(struct BgSpot03Taki*, PlayState*);

typedef struct BgSpot03Taki {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgSpot03TakiActionFunc actionFunc;
    /* 0x01C0 */ s16 timer;
    /* 0x01C2 */ u8 state;
    /* 0x01C4 */ u16 switchFlag;
    /* 0x01C8 */ f32 openingAlpha;
    /* 0x01CC */ u8 bufferIndex;
    /* 0x01D0 */ SkeletonAnimationModel* skelAnimModel1;
    /* 0x01D4 */ char unk_1D4[0x4];
    /* 0x01D8 */ SkeletonAnimationModel* skelAnimModel0;
} BgSpot03Taki; // size = 0x01DC

extern ActorInit Bg_Spot03_Taki_InitVars;

#endif
