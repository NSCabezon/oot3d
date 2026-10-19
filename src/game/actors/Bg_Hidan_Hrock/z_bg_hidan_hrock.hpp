#ifndef _Z_BG_HIDAN_HROCK_H_
#define _Z_BG_HIDAN_HROCK_H_

#include "global.hpp"

struct BgHidanHrock;

typedef struct ColliderTrisPlaceholder {
    /* 0x00 */ char unk_00[0x20];
} ColliderTrisPlaceholder;

typedef void (*BgHidanHrockActionFunc)(struct BgHidanHrock*, PlayState*);

typedef struct BgHidanHrock {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgHidanHrockActionFunc actionFunc;
    /* 0x01C0 */ s16 timer;
    /* 0x01C2 */ u8 switchFlag;
    /* 0x01C4 */ ColliderTrisPlaceholder collider; // ColliderTris (no repo type yet)
    /* 0x01E4 */ char colliderElements[0xB8]; // 2x ColliderTrisElement
    /* 0x029C */ SkeletonAnimationModel* skelAnimModel;
} BgHidanHrock; // size = 0x02A0

extern ActorInit Bg_Hidan_Hrock_InitVars;

#endif
