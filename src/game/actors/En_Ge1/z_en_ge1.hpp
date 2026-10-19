#ifndef _Z_EN_GE1_H_
#define _Z_EN_GE1_H_

#include "global.hpp"

struct EnGe1;

typedef void (*EnGe1ActionFunc)(struct EnGe1*, PlayState*);
typedef void (*EnGe1AnimFunc)(struct EnGe1*);

typedef struct EnGe1 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x58];
    /* 0x01FC */ char unk_1FC[0x69C];
    /* 0x0898 */ Vec3s limbRot; // applied to limb 0xE in the override callback
    /* 0x089E */ char unk_89E[0xA];
    /* 0x08A8 */ u16 stateFlags;
    /* 0x08AA */ char unk_8AA[0x1];
    /* 0x08AB */ u8 cutsceneTimer;
    /* 0x08AC */ s32 animation;
    /* 0x08B0 */ EnGe1ActionFunc actionFunc;
    /* 0x08B4 */ EnGe1AnimFunc animFunc;
    /* 0x08B8 */ void* model;
    /* 0x08BC */ char unk_8BC[0x1CC];
} EnGe1; // size = 0xA88

extern ActorInit En_Ge1_InitVars;

#endif
