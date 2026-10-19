#ifndef _Z_EN_DNT_NOMAL_H_
#define _Z_EN_DNT_NOMAL_H_

#include "global.hpp"

struct EnDntNomal;

typedef void (*EnDntNomalActionFunc)(struct EnDntNomal*, PlayState*);

typedef struct EnDntNomalQuad {
    char data[0x80];
} EnDntNomalQuad;

typedef struct EnDntNomal {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnDntNomalActionFunc actionFunc;
    /* 0x01A8 */ u8 isSolid;
    /* 0x01AC */ Vec3f flowerPos;
    /* 0x01B8 */ Vec3f targetVtx[4];
    /* 0x01E8 */ s16 timer1;
    /* 0x01EA */ s16 timer2;
    /* 0x01EC */ s16 timer4;
    /* 0x01EE */ s16 timer5;
    /* 0x01F0 */ s16 blinkTimer;
    /* 0x01F2 */ s16 unkCounter;
    /* 0x01F4 */ s16 timer3;
    /* 0x01F6 */ s16 objectId;
    /* 0x01F8 */ s16 eyeState;
    /* 0x01FA */ s16 type;
    /* 0x01FC */ s16 hitCounter;
    /* 0x01FE */ s16 endFrame;
    /* 0x0200 */ s16 stageSignal;
    /* 0x0202 */ s16 rotDirection;
    /* 0x0204 */ s16 action;
    /* 0x0206 */ u8 ignore;
    /* 0x0207 */ u8 spawnedItem;
    /* 0x0208 */ u8 stagePrize;
    /* 0x0209 */ s8 requiredObjectSlot;
    /* 0x020C */ Vec3f mouthPos;
    /* 0x0218 */ Vec3f targetPos;
    /* 0x0224 */ EnDntNomalQuad targetColliderQuad;
    /* 0x02A4 */ ColliderCylinder bodyColliderCylinder;
    /* 0x02FC */ SkelAnime skelAnime;
    /* 0x0380 */ char faceAnims[0x1CC]; // FaceAnimations
    /* 0x054C */ char jointTable[0x1D4];
    /* 0x0720 */ char morphTable[0x1D4];
    /* 0x08F4 */ SkeletonAnimationModel* model;
} EnDntNomal; // size = 0x8F8

extern ActorInit En_Dnt_Nomal_InitVars;

#endif
