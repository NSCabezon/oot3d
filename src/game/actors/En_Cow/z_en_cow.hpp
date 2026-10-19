#ifndef _Z_EN_COW_H_
#define _Z_EN_COW_H_

#include "global.hpp"

struct EnCow;

typedef void (*EnCowActionFunc)(struct EnCow* self, PlayState* play);

typedef struct EnCow {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x610];
    /* 0x07B4 */ u8 timerA;
    /* 0x07B5 */ u8 timerB;
    /* 0x07B6 */ char unk_7B6[0x2];
    /* 0x07B8 */ EnCowActionFunc actionFunc;
} EnCow; // size = 0x7BC

extern ActorInit En_Cow_InitVars;

#endif
