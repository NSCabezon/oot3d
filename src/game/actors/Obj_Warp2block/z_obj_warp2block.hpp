#ifndef _Z_OBJ_WARP2BLOCK_H_
#define _Z_OBJ_WARP2BLOCK_H_

#include "global.hpp"

struct ObjWarp2block;

typedef void (*ObjWarp2blockActionFunc)(struct ObjWarp2block*, PlayState*);
typedef s32 (*ObjWarp2blockFunc168)(struct ObjWarp2block*, PlayState*);

typedef struct ObjWarp2block {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ ObjWarp2blockActionFunc actionFunc;
    /* 0x01C0 */ ObjWarp2blockFunc168 func_168;
    /* 0x01C4 */ s16 unk_16C;
    /* 0x01C6 */ s16 unk_16E;
    /* 0x01C8 */ s16 unk_170;
    /* 0x01CA */ u16 unk_172;
    /* 0x01CC */ s16 unk_174;
    /* 0x01CE */ u8 unk_1CE;
    /* 0x01D0 */ SkeletonAnimationModel* skelAnimModel;
} ObjWarp2block; // size = 0x01D4

extern ActorInit Obj_Warp2block_InitVars;

#endif
