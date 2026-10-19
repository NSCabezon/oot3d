#ifndef _Z_BG_RELAY_OBJECTS_H_
#define _Z_BG_RELAY_OBJECTS_H_

#include "global.hpp"

struct BgRelayObjects;

typedef void (*BgRelayObjectsActionFunc)(struct BgRelayObjects*, PlayState*);

typedef struct BgRelayObjects {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgRelayObjectsActionFunc actionFunc;
    /* 0x01C0 */ u8 switchFlag;
    /* 0x01C1 */ s8 unk_1C1; // a room id
    /* 0x01C2 */ s16 timer;
    /* 0x01C4 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01C8 */ SkeletonAnimationModel* skelAnimModel2;
} BgRelayObjects; // size = 0x1CC

extern ActorInit Bg_Relay_Objects_InitVars;

#endif
