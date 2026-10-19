#ifndef _Z_BG_HAKA_ZOU_H_
#define _Z_BG_HAKA_ZOU_H_

#include "global.hpp"

struct BgHakaZou;

typedef enum BgHakaZouType {
    /* 0x0 */ BGHAKAZOU_TYPE_GIANT_BIRD_STATUE,
    /* 0x1 */ BGHAKAZOU_TYPE_BOMBABLE_SKULL_WALL,
    /* 0x2 */ BGHAKAZOU_TYPE_BOMBABLE_RUBBLE,
    /* 0x3 */ BGHAKAZOU_TYPE_SKULL_WALL_RUBBLE
} BgHakaZouType;

typedef void (*BgHakaZouActionFunc)(struct BgHakaZou*, PlayState*);

typedef struct BgHakaZou {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgHakaZouActionFunc actionFunc;
    /* 0x01C0 */ s8 requiredObjectSlot;
    /* 0x01C1 */ u8 switchFlag;
    /* 0x01C2 */ s16 timer;
    /* 0x01C4 */ ColliderCylinder collider;
    /* 0x021C */ SkeletonAnimationModel* skelAnimModel;
} BgHakaZou; // size = 0x0220

extern ActorInit Bg_Haka_Zou_InitVars;

#endif
