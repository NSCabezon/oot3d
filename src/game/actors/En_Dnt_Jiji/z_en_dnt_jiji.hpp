#ifndef _Z_EN_DNT_JIJI_H_
#define _Z_EN_DNT_JIJI_H_

#include "global.hpp"

struct EnDntJiji;
struct EnDntDemo;

typedef void (*EnDntJijiActionFunc)(struct EnDntJiji*, PlayState*);

typedef struct EnDntJiji {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnDntJijiActionFunc actionFunc;
    /* 0x01A8 */ Vec3f flowerPos;
    /* 0x01B4 */ u8 isSolid;
    /* 0x01B5 */ char unk_1B5;
    /* 0x01B6 */ s16 action;
    /* 0x01B8 */ s16 timer;
    /* 0x01BA */ s16 sfxTimer;
    /* 0x01BC */ s16 blinkTimer;
    /* 0x01BE */ s16 unkTimer;
    /* 0x01C0 */ s16 endFrame;
    /* 0x01C2 */ s16 unburrow;
    /* 0x01C4 */ s16 eyeState;
    /* 0x01C6 */ s16 stageSignal;
    /* 0x01C8 */ s16 unused; // always set to 5
    /* 0x01CA */ s16 attackFlag; // Is never set
    /* 0x01CC */ s32 getItemId;
    /* 0x01D0 */ struct EnDntDemo* stage;
    /* 0x01D4 */ ColliderCylinder collider;
    /* 0x022C */ SkelAnime skelAnime;
    /* 0x02B0 */ char faceAnims[0x1CC]; // FaceAnimations
    /* 0x047C */ char jointTable[0x23C];
    /* 0x06B8 */ char morphTable[0x23C];
    /* 0x08F4 */ SkeletonAnimationModel* model;
} EnDntJiji; // size = 0x08F8

extern ActorInit En_Dnt_Jiji_InitVars;

#endif
