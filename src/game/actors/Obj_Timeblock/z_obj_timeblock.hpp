#ifndef _Z_OBJ_TIMEBLOCK_H_
#define _Z_OBJ_TIMEBLOCK_H_

#include "global.hpp"

struct ObjTimeblock;

typedef s32 (*ObjTimeblockSongObserverFunc)(struct ObjTimeblock*, PlayState*);
typedef void (*ObjTimeblockActionFunc)(struct ObjTimeblock*, PlayState*);

typedef struct ObjTimeblock {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ ObjTimeblockActionFunc actionFunc;
    /* 0x01C0 */ ObjTimeblockSongObserverFunc songObserverFunc;
    /* 0x01C4 */ s16 demoEffectTimer;
    /* 0x01C6 */ s16 songEndTimer;
    /* 0x01C8 */ s16 demoEffectFirstPartTimer;
    /* 0x01CA */ u16 unk_172;
    /* 0x01CC */ u8 unk_174;
    /* 0x01CD */ u8 unk_175;
    /* 0x01CE */ u8 unk_176;
    /* 0x01CF */ u8 unk_177;
    /* 0x01D0 */ u8 isVisible;
    /* 0x01D4 */ SkeletonAnimationModel* skelAnimModel; // 3DS-only, freed in Destroy
} ObjTimeblock; // size = 0x1D8

extern ActorInit Obj_Timeblock_InitVars;

#endif
