#ifndef _Z_EN_GELDB_H_
#define _Z_EN_GELDB_H_

#include "global.hpp"

struct EnGeldB;

typedef void (*EnGeldBActionFunc)(struct EnGeldB*, PlayState*);

typedef struct EnGeldB {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ Vec3s bodyPartsPos[10];
    /* 0x01E0 */ SkelAnime skelAnime;
    /* 0x0264 */ char unk_264[0x7B8];
    /* 0x0A1C */ char faceAnimations[0x1CC]; // FaceAnimations
    /* 0x0BE8 */ s32 action;
    /* 0x0BEC */ char unk_BEC[0x4];
    /* 0x0BF0 */ EnGeldBActionFunc actionFunc;
    /* 0x0BF4 */ s16 unkTimer;
    /* 0x0BF6 */ s16 lookTimer;
    /* 0x0BF8 */ s16 iceTimer;
    /* 0x0BFA */ u8 damageReaction;
    /* 0x0BFB */ char unk_BFB[0x1];
    /* 0x0BFC */ s32 timer;
    /* 0x0C00 */ f32 approachRate;
    /* 0x0C04 */ char unk_C04[0x4];
    /* 0x0C08 */ f32 unkFloat;
    /* 0x0C0C */ s16 swordState;
    /* 0x0C0E */ s16 spinAttackState;
    /* 0x0C10 */ s16 keyFlag;
    /* 0x0C12 */ char unk_C12[0x2];
    /* 0x0C14 */ s16 invisible;
    /* 0x0C16 */ u8 blinkState;
    /* 0x0C17 */ u8 unk_C17;
    /* 0x0C18 */ s32 blureIndex;
    /* 0x0C1C */ ColliderCylinder bodyCollider;
    /* 0x0C74 */ char swordCollider[0x80]; // ColliderQuad
    /* 0x0CF4 */ char blockCollider[0x20]; // ColliderTris
    /* 0x0D14 */ char blockElements[0xB8]; // ColliderTrisElement[2]
    /* 0x0DCC */ Vec3f rightFootPos;
    /* 0x0DD8 */ Vec3f leftFootPos;
    /* 0x0DE4 */ Vec3s headRot;
} EnGeldB; // size = 0x0DEC

extern ActorInit En_GeldB_InitVars;

#endif
