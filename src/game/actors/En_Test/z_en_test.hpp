#ifndef _Z_EN_TEST_H_
#define _Z_EN_TEST_H_

#include "global.hpp"

struct EnTest;

typedef void (*EnTestActionFunc)(struct EnTest*, PlayState*);

typedef struct EnTest {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ Vec3s bodyPartsPos[10];
    /* 0x01E0 */ SkelAnime skelAnime;
    /* 0x0264 */ SkelAnime skelAnime2;
    /* 0x02E8 */ char unk_2E8[0x1C4C - 0x2E8];
    /* 0x1C4C */ u8 unk_1C4C;
    /* 0x1C4D */ char unk_1C4D[0x3];
    /* 0x1C50 */ EnTestActionFunc actionFunc;
    /* 0x1C54 */ Vec3s headRot;
    /* 0x1C5A */ char unk_1C5A[0x1C62 - 0x1C5A];
    /* 0x1C62 */ u8 unk_1C62;
    /* 0x1C63 */ char unk_1C63[0x1C64 - 0x1C63];
    /* 0x1C64 */ s16 iceTimer;
    /* 0x1C66 */ char unk_1C66[0x1C68 - 0x1C66];
    /* 0x1C68 */ s32 unk_1C68;
    /* 0x1C6C */ s32 timer;
    /* 0x1C70 */ char unk_1C70[0x1C74 - 0x1C70];
    /* 0x1C74 */ char bodyBreak[0x14];
    /* 0x1C88 */ s8 swordState;
    /* 0x1C89 */ char unk_1C89[0x1C8C - 0x1C89];
    /* 0x1C8C */ s32 effectIndex;
    /* 0x1C90 */ ColliderCylinder shieldCollider;
    /* 0x1CE8 */ ColliderCylinder bodyCollider;
    /* 0x1D40 */ char unk_1D40[0x1D68 - 0x1D40];
    /* 0x1D68 */ char swordCollider[0x1DC0 - 0x1D68];
    /* 0x1DC0 */ void* unk_1DC0;
    /* 0x1DC4 */ void* unk_1DC4;
    /* 0x1DC8 */ char unk_1DC8[0x1DCC - 0x1DC8];
} EnTest; // size = 0x1DCC

extern ActorInit En_Test_InitVars;

#endif
