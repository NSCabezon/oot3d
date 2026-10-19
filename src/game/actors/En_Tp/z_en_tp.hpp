#ifndef _Z_EN_TP_H_
#define _Z_EN_TP_H_

#include "global.hpp"

struct EnTp;

typedef void (*EnTpActionFunc)(struct EnTp* self, PlayState* play);

// Fragment/tail sub-state at +0x400 of EnTp.
typedef struct EnTpSub {
    /* 0x000 */ char unk_000[0xA4];
    /* 0x0A4 */ u8 dmgReaction;
    /* 0x0A5 */ char unk_0A5;
    /* 0x0A6 */ s16 timer;
    /* 0x0A8 */ s16 timer2;
    /* 0x0AA */ s16 fadeAlpha;
    /* 0x0AC */ s16 redShineAmount;
    /* 0x0AE */ s16 sparkTimer;
    /* 0x0B0 */ s16 unk_164;
    /* 0x0B2 */ char unk_0B2[0x2];
    /* 0x0B4 */ f32 unk_168;
    /* 0x0B8 */ f32 bodyElemDist;
    /* 0x0BC */ f32 unk_170;
} EnTpSub; // size = 0xC0

typedef struct EnTpElem {
    /* 0x00 */ char unk_00[0x34];
    /* 0x34 */ f32 modelRadius;
    /* 0x38 */ char unk_38[0xC];
    /* 0x44 */ f32 worldRadius;
} EnTpElem;

// Collider at +0x4C0 (layout partly known).
typedef struct EnTpCollider {
    /* 0x00 */ char unk_00[0x4];
    /* 0x04 */ Actor* at;
    /* 0x08 */ char unk_08[0x8];
    /* 0x10 */ u8 atFlags;
    /* 0x11 */ u8 acFlags;
    /* 0x12 */ char unk_12[0xA];
    /* 0x1C */ EnTpElem* elements;
    /* 0x20 */ char unk_20[0x50];
} EnTpCollider; // size = 0x70

typedef struct EnTp {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ s32 action;
    /* 0x01A8 */ s32 modeVar;
    /* 0x01AC */ EnTpActionFunc actionFunc;
    /* 0x01B0 */ SkelAnime skelAnime;
    /* 0x0234 */ char jointTable[0x138];
    /* 0x036C */ char morphTable[0x94];
    /* 0x0400 */ EnTpSub sub;
    /* 0x04C0 */ EnTpCollider collider;
    /* 0x0530 */ struct EnTp* headInstance;
    /* 0x0534 */ SkeletonAnimationModel* model;
    /* 0x0538 */ s8 scaleTimer;
    /* 0x0539 */ char unk_539[0x3];
    /* 0x053C */ SkeletonAnimationModel_unk_0C anim; // object freed by FUN_003685a0
} EnTp; // size = 0x5D4

extern ActorInit En_Tp_InitVars;

#endif
