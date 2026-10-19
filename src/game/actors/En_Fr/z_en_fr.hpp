#ifndef _Z_EN_FR_H_
#define _Z_EN_FR_H_

#include "global.hpp"

struct EnFr;

typedef struct EnFr {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0xA48 - 0x228];
    /* 0x0A48 */ SkelAnime skelAnimeButterfly;
    /* 0x0ACC */ char unk_ACC[0xC04 - 0xACC];
    /* 0x0C04 */ void (*actionFunc)(struct EnFr*, PlayState*);
    /* 0x0C08 */ void* lightNode; // LightNode*
    /* 0x0C0C */ char unk_C0C[0xC28 - 0xC0C];
    /* 0x0C28 */ s32 unk_C28;
    /* 0x0C2C */ s32 blinkTimer;
    /* 0x0C30 */ void (*blinkFunc)(struct EnFr*);
    /* 0x0C34 */ u8 eyeTexIndex;
    /* 0x0C35 */ u8 unk_C35;
    /* 0x0C36 */ char unk_C36[0xC38 - 0xC36];
    /* 0x0C38 */ u8 isDeactivating;
    /* 0x0C39 */ u8 isActive;
    /* 0x0C3A */ u8 unk_C3A;
    /* 0x0C3B */ char unk_C3B[0xC3C - 0xC3B];
    /* 0x0C3C */ u8 unk_C3C;
    /* 0x0C3D */ u8 unk_C3D;
    /* 0x0C3E */ u8 songIndex;
    /* 0x0C3F */ u8 unk_C3F;
    /* 0x0C40 */ char unk_C40[0xC41 - 0xC40];
    /* 0x0C41 */ u8 unk_C41;
    /* 0x0C42 */ s16 unk_C42;
    /* 0x0C44 */ s32 getItemId;
    /* 0x0C48 */ f32 posLogSpotX;
    /* 0x0C4C */ f32 unk_C4C; // log spot Y (EnFr posLogSpot.y)
    /* 0x0C50 */ f32 posLogSpotZ;
    /* 0x0C54 */ f32 unk_C54; // xz distance to log spot
    /* 0x0C58 */ f32 unk_C58; // unusedFloat
    /* 0x0C5C */ char unk_C5C[0xC60 - 0xC5C];
    /* 0x0C60 */ Actor* unusedButterflyActor;
    /* 0x0C64 */ u8 isButterflyDrawn;
    /* 0x0C65 */ char unk_C65[0xE4C - 0xC65];
    /* 0x0E4C */ s8 unk_E4C;
    /* 0x0E4D */ char unk_E4D[0xE50 - 0xE4D];
} EnFr; // size = 0xE50

extern ActorInit En_Fr_InitVars;

#endif
