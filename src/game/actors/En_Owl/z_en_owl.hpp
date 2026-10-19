#ifndef _Z_EN_OWL_H_
#define _Z_EN_OWL_H_

#include "global.hpp"
#include "z3Danimation.hpp"

struct EnOwl;

typedef struct EnOwl {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x1334 - 0x1A4];
    /* 0x1334 */ SkelAnime* drawSkelAnime;
    /* 0x1338 */ FaceAnimations* faceAnimations;
    /* 0x133C */ u8 drawFlag;
    /* 0x133D */ char unk_133D[0x1352 - 0x133D];
    /* 0x1352 */ s16 unk_1352;
    /* 0x1354 */ s16 eyeTexIndex;
    /* 0x1356 */ s16 unk_1356;
    /* 0x1358 */ f32 unk_1358;
    /* 0x135C */ u16 actionFlags;
    /* 0x135E */ s16 unk_135E;
    /* 0x1360 */ char unk_1360[0x1364 - 0x1360];
    /* 0x1364 */ u8 unk_1364[2];
    /* 0x1366 */ s16 unk_1366;
    /* 0x1368 */ u8 unk_1368[2];
    /* 0x136A */ char unk_136A[0x1370 - 0x136A];
    /* 0x1370 */ void* actionFunc;
    /* 0x1374 */ void* updateFunc;
    /* 0x1378 */ char unk_1378[0x137C - 0x1378];
} EnOwl; // size = 0x137C

typedef char EnOwl_SizeCheck[sizeof(EnOwl) == 0x137C ? 1 : -1];

extern ActorInit En_Owl_InitVars;

#endif
