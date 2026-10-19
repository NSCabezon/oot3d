#ifndef _Z_EN_CS_H_
#define _Z_EN_CS_H_

#include "global.hpp"

struct EnCs;

typedef void (*EnCsActionFunc)(struct EnCs*, struct PlayState*);

typedef struct EnCs {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnCsActionFunc actionFunc;
    /* 0x01A8 */ char collider[0x58]; // ColliderCylinder (3DS)
    /* 0x0200 */ s32 talkState;
    /* 0x0204 */ s32 eyeIndex;
    /* 0x0208 */ s32 eyeBlinkTimer;
    /* 0x020C */ s32 unk_20C;
    /* 0x0210 */ char unk_210[0x4];
    /* 0x0214 */ s32 path;
    /* 0x0218 */ char unk_218[0x4];
    /* 0x021C */ s32 waypoint;
    /* 0x0220 */ s32 animLoopCount; // How many time the current animation should loop before changing
    /* 0x0224 */ f32 walkAngle;
    /* 0x0228 */ f32 walkDist;
    /* 0x022C */ f32 walkSpeed;
    /* 0x0230 */ s32 currentAnimIndex;
    /* 0x0234 */ char unk_234[0x4];
    /* 0x0238 */ MTX34 spookyMaskMtx;
    /* 0x0268 */ char unk_268[0x8]; // NpcInteractInfo
    /* 0x0270 */ Vec3s torsoRot;
    /* 0x0276 */ Vec3s headRot;
    /* 0x027C */ char unk_27C[0x4];
    /* 0x0280 */ Vec3f trackPos;
    /* 0x028C */ char unk_28C[0x4];
    /* 0x0290 */ s32 flag;
    /* 0x0294 */ SkelAnime skelAnime;
    /* 0x0318 */ SkeletonAnimationModel* model;
    /* 0x031C */ FaceAnimations face;
    /* 0x04E8 */ char jointTable[0x7F4 - 0x4E8];
    /* 0x07F4 */ char morphTable[0xB00 - 0x7F4];
    /* 0x0B00 */ s32 prevFrame;
} EnCs; // size = 0xB04

extern ActorInit En_Cs_InitVars;

#endif
