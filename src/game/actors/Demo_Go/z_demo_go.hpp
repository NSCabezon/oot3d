#ifndef _Z_DEMO_GO_H_
#define _Z_DEMO_GO_H_

#include "global.hpp"

struct DemoGo;

typedef struct DemoGo {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x1CC]; // FaceAnimations (accessed through a cast)
    /* 0x03F4 */ s16 eyeIdx;
    /* 0x03F6 */ char unk_3F6[0x2];
    /* 0x03F8 */ s32 action;
    /* 0x03FC */ s32 drawConfig;
    /* 0x0400 */ char unk_400[0x4];
} DemoGo; // size = 0x404

extern ActorInit Demo_Go_InitVars;

#endif
