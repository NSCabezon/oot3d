#ifndef _Z_BG_SPOT00_HANEBASI_H_
#define _Z_BG_SPOT00_HANEBASI_H_

#include "global.hpp"

struct BgSpot00Hanebasi;

typedef void (*BgSpot00HanebasiActionFunc)(struct BgSpot00Hanebasi*, PlayState*);

// Model virtual class (vtable slot 1 = Destroy). _ZTI21BgSpot00HanebasiModel is an EQU in shared_syms.s (VFE key).
extern "C++" {
struct BgSpot00HanebasiModel {
    virtual void unk_vf0();
    virtual void Destroy();
};
}

typedef struct BgSpot00HanebasiTorch {
    /* 0x00 */ SkeletonAnimationModel* model;
    /* 0x04 */ void* unk_04;
    /* 0x08 */ SkeletonAnimationModel_unk_0C* anim;
} BgSpot00HanebasiTorch; // size = 0x0C

typedef struct BgSpot00Hanebasi {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgSpot00HanebasiActionFunc actionFunc;
    /* 0x01C0 */ void* lightNode;
    /* 0x01C4 */ u8 lightInfo[0x18];
    /* 0x01DC */ BgSpot00HanebasiModel* model;
    /* 0x01E0 */ BgSpot00HanebasiTorch unk_1E0[2];
    /* 0x01F8 */ s16 destAngle;
} BgSpot00Hanebasi; // size = 0x01FC

extern ActorInit Bg_Spot00_Hanebasi_InitVars;

#endif
