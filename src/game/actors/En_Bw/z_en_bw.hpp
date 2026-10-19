#ifndef _Z_EN_BW_H_
#define _Z_EN_BW_H_

#include "global.hpp"

struct EnBw;
typedef void (*EnBwActionFunc)(struct EnBw*, PlayState*);

typedef struct EnBw {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char jointTable[0x104];
    /* 0x032C */ char morphTable[0x104];
    /* 0x0430 */ SkeletonAnimationModel_unk_0C* unk_430;
    /* 0x0434 */ SkeletonAnimationModel* fireModel;
    /* 0x0438 */ void* unk_438;
    /* 0x043C */ SkeletonAnimationModel_unk_0C* fireAnim;
    /* 0x0440 */ s32 unk_440;
    /* 0x0444 */ u8 unk_444;
    /* 0x0445 */ u8 unk_445;
    /* 0x0446 */ s16 unk_446;
    /* 0x0448 */ s16 unk_448;
    /* 0x044A */ char unk_44A[0x2];
    /* 0x044C */ EnBwActionFunc actionFunc;
    /* 0x0450 */ u8 unk_450;
    /* 0x0451 */ u8 unk_451;
    /* 0x0452 */ u8 unk_452;
    /* 0x0453 */ u8 unk_453;
    /* 0x0454 */ u8 unk_454;
    /* 0x0455 */ char unk_455;
    /* 0x0456 */ u8 unk_456;
    /* 0x0457 */ char unk_457[0x3];
    /* 0x045A */ s16 unk_45A;
    /* 0x045C */ char unk_45C[0x3];
    /* 0x045F */ u8 iceTimer;
    /* 0x0460 */ u8 unk_460;
    /* 0x0461 */ char unk_461[0xB];
    /* 0x046C */ f32 unk_46C;
    /* 0x0470 */ char unk_470[0x4];
    /* 0x0474 */ f32 unk_474;
    /* 0x0478 */ char unk_478[0x4];
    /* 0x047C */ f32 unk_47C;
    /* 0x0480 */ f32 unk_480;
    /* 0x0484 */ f32 unk_484;
    /* 0x0488 */ Vec3f unk_488;
    /* 0x0494 */ Vec3f unk_494;
    /* 0x04A0 */ Vec3f unk_4A0;
    /* 0x04AC */ Vec3f unk_4AC;
    /* 0x04B8 */ ColliderCylinder collider1;
    /* 0x0510 */ ColliderCylinder collider2;
    /* 0x0568 */ char unk_568[0x4];
} EnBw; // size = 0x56C

extern ActorInit En_Bw_InitVars;

#endif
