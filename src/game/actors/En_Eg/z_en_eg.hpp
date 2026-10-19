#ifndef _Z_EN_EG_H_
#define _Z_EN_EG_H_

#include "global.hpp"

struct EnEg;

typedef void (*EnEgActionFunc)(struct EnEg*, PlayState*);

typedef struct EnEg {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ s32 action;
} EnEg; // size = 0x1A8

extern ActorInit En_Eg_InitVars;

#endif
