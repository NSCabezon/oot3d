#ifndef _Z_BG_TOKI_HIKARI_H_
#define _Z_BG_TOKI_HIKARI_H_

#include "global.hpp"

struct BgTokiHikari;

typedef void (*BgTokiHikariActionFunc)(struct BgTokiHikari*, PlayState*);

typedef struct BgTokiHikari {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ f32 unk_14C;
    /* 0x01A8 */ BgTokiHikariActionFunc actionFunc;
    /* 0x01AC */ SkeletonAnimationModel* skelAnimModel[6];
} BgTokiHikari; // size = 0x1C4

extern ActorInit Bg_Toki_Hikari_InitVars;

#endif
