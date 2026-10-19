#ifndef _Z_BG_JYA_ZURERUKABE_H_
#define _Z_BG_JYA_ZURERUKABE_H_

#include "global.hpp"

struct BgJyaZurerukabe;

typedef void (*BgJyaZurerukabeActionFunc)(struct BgJyaZurerukabe*, PlayState*);

typedef struct BgJyaZurerukabe {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgJyaZurerukabeActionFunc actionFunc;
    /* 0x01C0 */ s16 unk_1C0; // N64: unk_168, index of this wall (0..3)
    /* 0x01C2 */ s16 unk_1C2; // N64: unk_16A, wait timer
    /* 0x01C4 */ s16 unk_1C4; // N64: unk_16C, slide position counter
    /* 0x01C6 */ s16 unk_1C6; // N64: unk_16E, slide direction
    /* 0x01C8 */ SkeletonAnimationModel* skelAnimModel;
} BgJyaZurerukabe; // size = 0x1CC

extern ActorInit Bg_Jya_Zurerukabe_InitVars;

#endif
