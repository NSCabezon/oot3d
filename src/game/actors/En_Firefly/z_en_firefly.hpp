#ifndef _Z_EN_FIREFLY_H_
#define _Z_EN_FIREFLY_H_

#include "global.hpp"

struct EnFirefly;

typedef struct EnFirefly {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ u8 unk_1A4[0x24];
    /* 0x01C8 */ SkelAnime skelAnime;
    /* 0x024C */ ActorFunc actionFunc;
    /* 0x0250 */ u8 auraType;
    /* 0x0251 */ u8 flameType;
    /* 0x0252 */ s16 timer;
    /* 0x0254 */ s16 targetPitch;
    /* 0x0256 */ u8 unk_256[2];
    /* 0x0258 */ u8 jointTable[12 * 0x34];
    /* 0x04C8 */ u8 morphTable[12 * 0x34];
    /* 0x0738 */ f32 homeY;
    /* 0x073C */ u8 collider[0x20];
    /* 0x075C */ u8 colliderElement[0x50];
    /* 0x07AC */ void* model;
    /* 0x07B0 */ u8 drawEnabled;
    /* 0x07B1 */ u8 unk_7B1[3];
} EnFirefly; // size = 0x7B4

typedef char EnFirefly_SizeCheck[(sizeof(EnFirefly) == 0x7B4) ? 1 : -1];

extern ActorInit En_Firefly_InitVars;

#endif
