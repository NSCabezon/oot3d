#ifndef _Z_DEMO_KANKYO_H_
#define _Z_DEMO_KANKYO_H_

#include "global.hpp"

struct DemoKankyo;

typedef struct DemoKankyo {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x1774];
    /* 0x1918 */ void (*actionFunc)(DemoKankyo*, PlayState*);
    /* 0x191C */ char unk_191C[0x260];
} DemoKankyo; // size = 0x1B7C

typedef char DemoKankyoSizeCheck[(sizeof(DemoKankyo) == 0x1B7C) ? 1 : -1];

extern ActorInit Demo_Kankyo_InitVars;

#endif
