#ifndef _Z_DEMO_EC_H_
#define _Z_DEMO_EC_H_

#include "global.hpp"

struct DemoEc;

typedef struct DemoEc {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x10C8];
    /* 0x126C */ s32 updateMode;
    /* 0x1270 */ s32 drawConfig;
    /* 0x1274 */ char unk_1274[0xC];
    /* 0x1280 */ u32 model[0xB0/4];
    /* 0x1330 */ char unk_1330[0x1CC];
    /* 0x14FC */ s32 unk_4FC;
} DemoEc; // size = 0x1500

extern ActorInit Demo_Ec_InitVars;

#endif
