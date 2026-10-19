#ifndef _Z_BG_MORI_HASHIRA4_H_
#define _Z_BG_MORI_HASHIRA4_H_

#include "global.hpp"

struct BgMoriHashira4;

typedef void (*BgMoriHashira4ActionFunc)(struct BgMoriHashira4*, PlayState*);

typedef struct BgMoriHashira4 {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgMoriHashira4ActionFunc actionFunc;
    /* 0x01C0 */ s8 moriTexObjectSlot;
    /* 0x01C1 */ s8 switchFlag;
    /* 0x01C2 */ s16 gateTimer;
    /* 0x01C4 */ SkeletonAnimationModel* skelAnimModel[2]; // [params] = model used by Draw; [0]/[1] also loaded/freed in Init/Destroy
} BgMoriHashira4; // size = 0x1CC

extern ActorInit Bg_Mori_Hashira4_InitVars;

#endif
