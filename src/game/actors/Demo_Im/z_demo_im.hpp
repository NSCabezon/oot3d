#ifndef _Z_DEMO_IM_H_
#define _Z_DEMO_IM_H_

#include "global.hpp"

struct DemoIm;

typedef void (*DemoImActionFunc)(struct DemoIm*, struct PlayState*);
typedef void (*DemoImDrawFunc)(struct DemoIm*, struct PlayState*);

typedef struct DemoIm {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x83C];     // SkelAnime skelAnime at 0x1A4, FaceAnimations faceAnim at 0x9E0
    /* 0x09E0 */ char unk_9E0[0x1D4];
    /* 0x0BB4 */ s32 action;
    /* 0x0BB8 */ s32 drawConfig;
    /* 0x0BBC */ char unk_BBC[0x1C];
    /* 0x0BD8 */ char unk_BD8[0x84];      // ColliderCylinder collider at 0xBD8
} DemoIm; // size = 0xC5C

extern ActorInit Demo_Im_InitVars;

#endif
