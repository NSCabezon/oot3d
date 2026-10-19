#ifndef _Z_EN_PO_RELAY_H_
#define _Z_EN_PO_RELAY_H_

#include "global.hpp"

struct EnPoRelay;

typedef void (*EnPoRelayActionFunc)(struct EnPoRelay*, PlayState*);

typedef struct EnPoRelay {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x8D0];
    /* 0x0AF8 */ char unk_AF8[0x20];
    /* 0x0B18 */ EnPoRelayActionFunc actionFunc;
    /* 0x0B1C */ u8 hookshotSlotFull;
    /* 0x0B1D */ u8 bobTimer; // used for the up & down bobbing
    /* 0x0B1E */ s16 actionTimer;
    /* 0x0B20 */ s16 pathPoint; // current (upcoming) path point
    /* 0x0B22 */ s16 yawTowardsPathPoint; // yaw towards the current (upcoming) path point
    /* 0x0B24 */ u16 textId; // mirrors actor.textId
    /* 0x0B26 */ u16 eyeTextureIdx;
    /* 0x0B28 */ u8 lightColorR;
    /* 0x0B29 */ u8 lightColorG;
    /* 0x0B2A */ u8 lightColorB;
    /* 0x0B2B */ u8 lightColorA;
    /* 0x0B2C */ void* lightNode; // LightNode*
    /* 0x0B30 */ char unk_B30[0x18];
    /* 0x0B48 */ ColliderCylinder collider;
    /* 0x0BA0 */ void* unk_BA0;
    /* 0x0BA4 */ char unk_BA4[0x1CC]; // FaceAnimations at +0xBA4 (C++ class, not in repo headers)
} EnPoRelay; // size = 0xD70

extern ActorInit En_Po_Relay_InitVars;

#endif
