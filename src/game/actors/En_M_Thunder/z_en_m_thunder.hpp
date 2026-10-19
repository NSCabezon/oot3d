#ifndef _Z_EN_M_THUNDER_H_
#define _Z_EN_M_THUNDER_H_

#include "global.hpp"

struct EnMThunder;

typedef void (*EnMThunderActionFunc)(struct EnMThunder*, PlayState*);

typedef struct EnMThunder {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ColliderCylinder collider;
    /* 0x01FC */ void* lightNode;
    /* 0x0200 */ char unk_200[0x18]; // LightInfo
    /* 0x0218 */ f32 spinAttackTimer;
    /* 0x021C */ f32 spinAttackAlpha;
    /* 0x0220 */ f32 spinTrailTexScroll;
    /* 0x0224 */ f32 spinChargePercent;
    /* 0x0228 */ f32 dimmingIntensity;
    /* 0x022C */ EnMThunderActionFunc actionFunc;
    /* 0x0230 */ u16 followPlayerTimer;
    /* 0x0232 */ u8 attackStrength;
    /* 0x0233 */ u8 swordType;
    /* 0x0234 */ u8 chargeAlpha;
    /* 0x0235 */ u8 targetScale;
    /* 0x0236 */ u8 isUsingMagic;
    /* 0x0237 */ char unk_237[0x1];
    /* 0x0238 */ struct EnMThunderFreer* freers[3];
    /* 0x0244 */ u16 unk_244;
    /* 0x0246 */ char unk_246[0x2];
    /* 0x0248 */ void* unk_248;
    /* 0x024C */ void* unk_24C;
    /* 0x0250 */ s32 unk_250;
} EnMThunder; // size = 0x254

extern ActorInit En_M_Thunder_InitVars;

#endif
