#ifndef _Z_DEMO_KANKYO_H_
#define _Z_DEMO_KANKYO_H_

#include "global.hpp"
#include <stddef.h>

struct DemoKankyo;

// The 3DS actor has 150 effect entries; fields verified against
// UpdateClouds, UpdateRock and Destroy. The N64 entry shape is preserved.
typedef struct DemoKankyoEffect {
    Vec3f velocity;
    Vec3f offset;
    f32 translation;
    f32 unk_1C;
    s16 angle;
    u8 mode;
    u8 unk_23;
    s16 alpha;
} DemoKankyoEffect;

typedef struct DemoKankyo {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ u8 requiredObjectSlot;
    /* 0x01A5 */ u8 sparkleCounter;
    /* 0x01A6 */ u8 warpTimer;
    /* 0x01A7 */ u8 unk_1A7;
    /* 0x01A8 */ DemoKankyoEffect effects[150];
    /* 0x1918 */ void (*actionFunc)(DemoKankyo*, PlayState*);
    /* 0x191C */ void* effectResource;
    /* 0x1920 */ SkeletonAnimationModel* models[150];
    /* 0x1B78 */ s32 resourcesLoaded;
} DemoKankyo; // size = 0x1B7C

typedef char DemoKankyoSizeCheck[(sizeof(DemoKankyo) == 0x1B7C) ? 1 : -1];

typedef char DemoKankyoEffectSizeCheck[(sizeof(DemoKankyoEffect) == 0x28) ? 1 : -1];
typedef char DemoKankyoActionOffsetCheck[(offsetof(DemoKankyo, actionFunc) == 0x1918) ? 1 : -1];

extern ActorInit Demo_Kankyo_InitVars;

#endif
