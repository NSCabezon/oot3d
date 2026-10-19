#ifndef _Z_DEMO_DU_H_
#define _Z_DEMO_DU_H_

#include "global.hpp"

struct DemoDu;

typedef void (*DemoDuUpdateFunc)(struct DemoDu*, PlayState*);
typedef void (*DemoDuDrawFunc)(Actor*, GameState*);

typedef struct DemoDu {
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
} DemoDu; // size = 0x418

extern ActorInit Demo_Du_InitVars;

#endif
