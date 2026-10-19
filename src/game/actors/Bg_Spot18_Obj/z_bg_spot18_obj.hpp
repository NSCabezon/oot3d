#ifndef _Z_BG_SPOT18_OBJ_H_
#define _Z_BG_SPOT18_OBJ_H_

#include "global.hpp"

struct BgSpot18Obj;

typedef void (*BgSpot18ObjActionFunc)(struct BgSpot18Obj*, PlayState*);
typedef s32 (*BgSpot18ObjInitFunc)(struct BgSpot18Obj*, PlayState*);

typedef struct BgSpot18Obj {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgSpot18ObjActionFunc actionFunc;
    /* 0x01C0 */ s16 unk_1C0; // Timer
    /* 0x01C2 */ char unk_1C2[0x2];
    /* 0x01C4 */ SkeletonAnimationModel* skelAnimModel;
} BgSpot18Obj; // size = 0x1C8

extern ActorInit Bg_Spot18_Obj_InitVars;

#endif
