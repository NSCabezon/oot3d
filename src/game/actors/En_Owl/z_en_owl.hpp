#ifndef _Z_EN_OWL_H_
#define _Z_EN_OWL_H_

#include "global.hpp"
#include "z3Danimation.hpp"

struct EnOwl;

typedef struct EnOwl {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x0238 - 0x01A4];
    /* 0x0238 */ f32 unk_238;
    /* 0x023C */ char unk_23C[0x1334 - 0x023C];
    /* 0x1334 */ SkelAnime* drawSkelAnime;
    /* 0x1338 */ FaceAnimations* faceAnimations;
    /* 0x133C */ u8 drawFlag;
    /* 0x133D */ u8 bgCheckInitialized;
    /* 0x133E */ u8 hasMoved;
    /* 0x133F */ char unk_133F[0x134C - 0x133F];
    /* 0x134C */ s16 unk_134C;
    /* 0x134E */ s16 unk_134E;
    /* 0x1350 */ s16 unk_1350;
    /* 0x1352 */ s16 unk_1352;
    /* 0x1354 */ s16 eyeTexIndex;
    /* 0x1356 */ s16 unk_1356;
    /* 0x1358 */ f32 unk_1358;
    /* 0x135C */ u16 actionFlags;
    /* 0x135E */ u16 unk_135E;
    /* 0x1360 */ s16 targetYaw;
    /* 0x1362 */ s16 subCamId;
    /* 0x1364 */ u8 unk_1364[2];
    /* 0x1366 */ s16 unk_1366;
    /* 0x1368 */ u8 unk_1368[2];
    /* 0x136A */ s16 unk_136A;
    /* 0x136C */ u8 cueId;
    /* 0x136D */ char unk_136D[3];
    /* 0x1370 */ void* actionFunc;
    /* 0x1374 */ void* updateFunc;
    /* 0x1378 */ s32 unk_1378;
} EnOwl; // size = 0x137C

typedef char EnOwl_SizeCheck[sizeof(EnOwl) == 0x137C ? 1 : -1];

extern ActorInit En_Owl_InitVars;

#endif
