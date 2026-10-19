#ifndef _Z_DEMO_EFFECT_H_
#define _Z_DEMO_EFFECT_H_

#include "global.hpp"

struct DemoEffect;

typedef void (*DemoEffectFunc)(struct DemoEffect*, struct PlayState*);

typedef struct DemoEffect {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0xEC]; // SkelCurve at 0x1A4
    /* 0x0290 */ u16 unk_290;
    /* 0x0292 */ s16 unk_292;
    /* 0x0294 */ char unk_294[0x10];
    /* 0x02A4 */ DemoEffectFunc updateFunc;
    /* 0x02A8 */ void* unk_2A8[3]; // SkeletonAnimationModel* (slot 1 = Destroy)
    /* 0x02B4 */ char unk_2B4[0xC];
    /* 0x02C0 */ void* unk_2C0[3];
    /* 0x02CC */ void* unk_2CC;
    /* 0x02D0 */ void* unk_2D0[2];
    /* 0x02D8 */ char unk_2D8[0x24];
} DemoEffect; // size = 0x2FC

extern ActorInit Demo_Effect_InitVars;

#endif
