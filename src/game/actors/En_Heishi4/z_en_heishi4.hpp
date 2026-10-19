#ifndef _Z_EN_HEISHI4_H_
#define _Z_EN_HEISHI4_H_

#include "global.hpp"

struct EnHeishi4;

typedef enum Heishi4Type {
    /* 0x00 */ HEISHI4_AT_KAKARIKO_ENTRANCE,
    /* 0x04 */ HEISHI4_AT_IMPAS_HOUSE = 4,
    /* 0x07 */ HEISHI4_AT_MARKET_DYING = 7,
    /* 0x08 */ HEISHI4_AT_MARKET_NIGHT
} Heishi4Type;

typedef void (*EnHeishi4ActionFunc)(struct EnHeishi4*, PlayState*);

typedef struct EnHeishi4InteractInfo {
    /* 0x00 */ char unk_00[0x8];
    /* 0x08 */ Vec3s headRot;
    /* 0x0E */ Vec3s torsoRot;
    /* 0x14 */ f32 yOffset;
    /* 0x18 */ Vec3f trackPos;
    /* 0x24 */ char unk_24[0x4];
} EnHeishi4InteractInfo; // size = 0x28

typedef struct EnHeishi4 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ Vec3s jointTable[17];
    /* 0x028E */ char unk_28E[0x2DA];
    /* 0x0568 */ Vec3s morphTable[17];
    /* 0x05CE */ char unk_5CE[0x2DA];
    /* 0x08A8 */ EnHeishi4ActionFunc actionFunc;
    /* 0x08AC */ Vec3s headRot;
    /* 0x08B2 */ Vec3s torsoRot;
    /* 0x08B8 */ Vec3f pos;
    /* 0x08C4 */ f32 height;
    /* 0x08C8 */ s16 unk_27C;
    /* 0x08CA */ s16 unk_27E;
    /* 0x08CC */ s16 type;
    /* 0x08CE */ s16 unk_282;
    /* 0x08D0 */ s16 unk_284;
    /* 0x08D2 */ char unk_8D2[0x2];
    /* 0x08D4 */ f32 unk_288;
    /* 0x08D8 */ EnHeishi4InteractInfo interactInfo;
    /* 0x0900 */ u8 unk_2B4;
    /* 0x0901 */ char unk_901[0x7];
    /* 0x0908 */ ColliderCylinder collider;
    /* 0x0960 */ SkeletonAnimationModel* skelAnimModel;
} EnHeishi4; // size = 0x0964

extern ActorInit En_Heishi4_InitVars;

#endif
