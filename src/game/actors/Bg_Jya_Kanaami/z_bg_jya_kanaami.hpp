#ifndef _Z_BG_JYA_KANAAMI_H_
#define _Z_BG_JYA_KANAAMI_H_

#include "global.hpp"

struct BgJyaKanaami;

typedef void (*BgJyaKanaamiActionFunc)(struct BgJyaKanaami*, PlayState*);

typedef struct BgJyaKanaami {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgJyaKanaamiActionFunc actionFunc;
    /* 0x01C0 */ u16 unk_168;
    /* 0x01C2 */ s16 unk_16A;
    /* 0x01C4 */ SkeletonAnimationModel* skelAnimModel;
} BgJyaKanaami; // size = 0x1C8

extern ActorInit Bg_Jya_Kanaami_InitVars;

#endif
