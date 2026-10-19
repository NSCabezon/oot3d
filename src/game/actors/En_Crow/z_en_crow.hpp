#ifndef _Z_EN_CROW_H_
#define _Z_EN_CROW_H_

#include "global.hpp"

struct EnCrow;

typedef void (*EnCrowActionFunc)(struct EnCrow*, PlayState*);

typedef struct EnCrowElemInit {
    /* 0x00 */ char unk_00[0x28];
    /* 0x28 */ f32 modelRadius;
    /* 0x2C */ char unk_2C[0x4];
} EnCrowElemInit;

typedef struct EnCrowJntSphInit {
    /* 0x00 */ char unk_00[0xC];
    /* 0x0C */ EnCrowElemInit* elements;
} EnCrowJntSphInit;

typedef struct EnCrowElem {
    /* 0x00 */ char unk_00[0x38];
    /* 0x38 */ Vec3f worldCenter;
    /* 0x44 */ f32 worldRadius;
    /* 0x48 */ char unk_48[0x8];
} EnCrowElem; // size = 0x50

typedef struct EnCrow {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ Vec3f bodyPartsPos[4];
    /* 0x01D4 */ SkelAnime skelAnime;
    /* 0x0258 */ char unk_258[0x340];
    /* 0x0598 */ EnCrowActionFunc actionFunc;
    /* 0x059C */ s16 timer;
    /* 0x059E */ s16 aimRotX;
    /* 0x05A0 */ s16 aimRotY;
    /* 0x05A2 */ char unk_5A2[0x2];
    /* 0x05A4 */ char unk_5A4[0x10];
    /* 0x05B4 */ u8 colliderAtFlags;
    /* 0x05B5 */ u8 colliderAcFlags;
    /* 0x05B6 */ char unk_5B6[0xA];
    /* 0x05C0 */ struct EnCrowElem* colliderElements;
    /* 0x05C4 */ struct EnCrowElem colliderElemStorage;
    /* 0x0614 */ u8 unk_614;
    /* 0x0615 */ char unk_615[0x3];
} EnCrow; // size = 0x0618

extern ActorInit En_Crow_InitVars;

#endif
