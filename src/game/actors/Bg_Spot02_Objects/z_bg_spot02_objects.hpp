#ifndef _Z_BG_SPOT02_OBJECTS_H_
#define _Z_BG_SPOT02_OBJECTS_H_

#include "global.hpp"

struct BgSpot02Objects;

typedef void (*BgSpot02ObjectsActionFunc)(struct BgSpot02Objects*, PlayState*);

typedef struct BgSpot02Objects {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgSpot02ObjectsActionFunc actionFunc;
    /* 0x01C0 */ s16 timer;
    /* 0x01C2 */ u8 unk_1C2; // N64 unk_16A
    /* 0x01C3 */ u8 unk_1C3; // N64 unk_16B: event flag index (params >> 8)
    /* 0x01C4 */ char unk_1C4[0x4];
    /* 0x01C8 */ u16 unk_1C8; // N64 unk_170
    /* 0x01CA */ u16 unk_1CA; // N64 unk_172
    /* 0x01CC */ void* unk_1CC; // SystemArena object
    /* 0x01D0 */ struct SkeletonAnimationModel* skelAnimModel[4];
} BgSpot02Objects; // size = 0x1E0

extern ActorInit Bg_Spot02_Objects_InitVars;

#endif
