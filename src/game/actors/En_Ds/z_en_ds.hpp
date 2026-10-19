#ifndef _Z_EN_DS_H_
#define _Z_EN_DS_H_

#include "global.hpp"

struct EnDs;

typedef void (*EnDsActionFunc)(struct EnDs* self, PlayState* play);

typedef struct EnDs {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x104]; // jointTable
    /* 0x032C */ char unk_32C[0x104]; // morphTable
    /* 0x0430 */ Vec3s unk_1D8;
    /* 0x0436 */ Vec3s unk_1DE;
    /* 0x043C */ f32 unk_1E4;
    /* 0x0440 */ u16 unk_1E8;
    /* 0x0442 */ u16 brewTimer;
    /* 0x0444 */ EnDsActionFunc actionFunc;
    /* 0x0448 */ void* model;
} EnDs; // size = 0x44C

extern ActorInit En_Ds_InitVars;

#endif
