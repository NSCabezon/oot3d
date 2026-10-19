#ifndef _Z_BG_HEAVY_BLOCK_H_
#define _Z_BG_HEAVY_BLOCK_H_

#include "global.hpp"

struct BgHeavyBlock;

typedef void (*BgHeavyBlockActionFunc)(struct BgHeavyBlock*, PlayState*);

typedef struct BgHeavyBlock {
    /* 0x0000 */ DynaPolyActor dyna; // dyna.bgId at +0x1A4
    /* 0x01BC */ Vec3f unk_1BC;
    /* 0x01C8 */ s16 timer;
    /* 0x01CA */ u16 pieceFlags;
    /* 0x01CC */ BgHeavyBlockActionFunc actionFunc;
    /* 0x01D0 */ char unk_1D0[0x4];
    /* 0x01D4 */ SkeletonAnimationModel* pieceModel[3];
} BgHeavyBlock; // size = 0x1E0

extern ActorInit Bg_Heavy_Block_InitVars;

#endif
