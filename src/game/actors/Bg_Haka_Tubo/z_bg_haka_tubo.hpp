#ifndef _Z_BG_HAKA_TUBO_H_
#define _Z_BG_HAKA_TUBO_H_

#include "global.hpp"

struct BgHakaTubo;

typedef void (*BgHakaTuboActionFunc)(struct BgHakaTubo*, PlayState*);

typedef struct BgHakaTubo {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgHakaTuboActionFunc actionFunc;
    /* 0x01C0 */ s16 dropTimer;
    /* 0x01C2 */ u16 fireScroll;
    /* 0x01C4 */ ColliderCylinder potCollider;
    /* 0x021C */ ColliderCylinder flamesCollider;
    /* 0x0274 */ void* unk_274; // freed by FUN_00357248 (virtual Destroy on the pointee)
    /* 0x0278 */ SkeletonAnimationModel* skelAnimModel;
} BgHakaTubo; // size = 0x27C

extern ActorInit Bg_Haka_Tubo_InitVars;

#endif
