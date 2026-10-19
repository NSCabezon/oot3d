#ifndef _Z_DEMO_IK_H_
#define _Z_DEMO_IK_H_

#include "global.hpp"

struct DemoIk;

typedef struct DemoIk {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char jointTable[0x68];
    /* 0x0290 */ char morphTable[0x68];
    /* 0x02F8 */ void* zarInfo;
    /* 0x02FC */ s32 actionMode;
    /* 0x0300 */ s32 drawMode;
    /* 0x0304 */ char unk_304[0x4];
} DemoIk; // size = 0x308

extern ActorInit Demo_Ik_InitVars;

#endif
