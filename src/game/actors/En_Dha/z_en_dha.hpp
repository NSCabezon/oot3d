#ifndef _Z_EN_DHA_H_
#define _Z_EN_DHA_H_

#include "global.hpp"

struct EnDha;

typedef void (*EnDhaActionFunc)(struct EnDha*, PlayState*);

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ u8 unk_00[0x50];
} EnDhaColliderElement; // size = 0x50

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ EnDhaColliderElement* elements;
} EnDhaColliderJntSph; // size = 0x20

typedef struct EnDha {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ Vec3s jointTable[26];
    /* 0x02C4 */ Vec3s morphTable[26];
    /* 0x0360 */ u8 unk_1C0;
    /* 0x0361 */ char unk_361[0x3];
    /* 0x0364 */ EnDhaActionFunc actionFunc;
    /* 0x0368 */ s16 actionTimer;
    /* 0x036A */ char unk_36A[0x4];
    /* 0x036E */ s16 limbAngleX[2];
    /* 0x0372 */ s16 limbAngleY;
    /* 0x0374 */ Vec3s handAngle;
    /* 0x037A */ char unk_37A[0x2];
    /* 0x037C */ Vec3f armPos;
    /* 0x0388 */ Vec3f handPos[2];
    /* 0x03A0 */ EnDhaColliderJntSph collider;
    /* 0x03C0 */ EnDhaColliderElement colliderElements[5];
} EnDha; // size = 0x550

extern ActorInit En_Dha_InitVars;

#endif
