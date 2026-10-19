#ifndef _Z_BG_HAKA_GATE_H_
#define _Z_BG_HAKA_GATE_H_

#include "global.hpp"

struct BgHakaGate;

typedef void (*BgHakaGateActionFunc)(struct BgHakaGate* self, PlayState* play);

typedef struct BgHakaGate {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgHakaGateActionFunc actionFunc;
    /* 0x01C0 */ u8 switchFlag;
    /* 0x01C2 */ s16 actionVar1;
    /* 0x01C4 */ s16 actionVar2;
    /* 0x01C6 */ s16 actionVar3;
    /* 0x01C8 */ s16 actionVar4;
    /* 0x01CA */ s16 actionVar5;
    /* 0x01CC */ s32 unk_1CC;
    /* 0x01D0 */ SkeletonAnimationModel* skelAnimModel[6];
    /* 0x01E8 */ char unk_1E8[0x4];
    /* 0x01EC */ void* unk_1EC;
} BgHakaGate; // size = 0x1F0

extern ActorInit Bg_Haka_Gate_InitVars;

#endif
