#ifndef _Z_EN_HEISHI3_H_
#define _Z_EN_HEISHI3_H_

#include "global.hpp"

struct EnHeishi3;

typedef void (*EnHeishi3ActionFunc)(struct EnHeishi3*, PlayState*);

typedef struct EnHeishi3 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ Vec3s jointTable[16];
    /* 0x0288 */ char unk_288[0x568 - 0x288];
    /* 0x0568 */ Vec3s morphTable[16];
    /* 0x05C8 */ char unk_5C8[0x800 - 0x5C8];
    /* 0x0800 */ char unk_800[0xA0 - 0x00];
    /* 0x08A0 */ char unk_8A0[0x8A8 - 0x8A0];
    /* 0x08A8 */ EnHeishi3ActionFunc actionFunc;
    /* 0x08AC */ Vec3s limb15Rot;
    /* 0x08B2 */ char unk_8B2[0x6];
    /* 0x08B8 */ Vec3s limb8Rot;
    /* 0x08BE */ s16 caughtTimer;
    /* 0x08C0 */ s16 unk_274;
    /* 0x08C2 */ u8 respawnFlag;
    /* 0x08C3 */ char unk_8C3[0x1];
    /* 0x08C4 */ s16 unk_278;
    /* 0x08C6 */ char unk_8C6[0x2];
    /* 0x08C8 */ ColliderCylinder collider;
    /* 0x0920 */ SkeletonAnimationModel* skelAnimModel;
} EnHeishi3; // size = 0x924

extern ActorInit En_Heishi3_InitVars;

#endif
