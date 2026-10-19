#ifndef _Z_DEMO_SA_H_
#define _Z_DEMO_SA_H_

#include "global.hpp"

struct DemoSa;

typedef void (*DemoSaUpdateFunc)(struct DemoSa*, PlayState*);
typedef void (*DemoSaDrawFunc)(Actor*, GameState*);

typedef struct DemoSa {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x84];
    /* 0x0228 */ char unk_228[0x1CC]; // FaceAnimations (accessed through a cast)
    /* 0x03F4 */ s16 unk_3F4;
    /* 0x03F6 */ char unk_3F6[0x2];
    /* 0x03F8 */ s16 unk_3F8;
    /* 0x03FA */ char unk_3FA[0x2];
    /* 0x03FC */ s32 updateIndex;
    /* 0x0400 */ s32 drawIndex;
    /* 0x0404 */ char unk_404[0x14];
} DemoSa; // size = 0x418

extern ActorInit Demo_Sa_InitVars;

#endif
