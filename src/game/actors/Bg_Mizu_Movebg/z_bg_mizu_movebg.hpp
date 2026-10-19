#ifndef _Z_BG_MIZU_MOVEBG_H_
#define _Z_BG_MIZU_MOVEBG_H_

#include "global.hpp"
#include "mtx34_identity.hpp"

struct BgMizuMovebg;

typedef void (*BgMizuMovebgActionFunc)(struct BgMizuMovebg*, struct PlayState*);

typedef struct BgMizuMovebg {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgMizuMovebgActionFunc actionFunc;
    /* 0x01C0 */ f32 homeY;
    /* 0x01C4 */ char unk_1C4[0x10];
    /* 0x01D4 */ u8 sfxFlags;
    /* 0x01D5 */ char unk_1D5[0x7];
    /* 0x01DC */ void* dList;
    /* 0x01E0 */ f32 unk_1E0;
    /* 0x01E4 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01E8 */ SkeletonAnimationModel* skelAnimModel2;
} BgMizuMovebg; // size = 0x1EC

extern ActorInit Bg_Mizu_Movebg_InitVars;

#endif
