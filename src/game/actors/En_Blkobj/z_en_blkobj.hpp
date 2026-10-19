#ifndef _Z_EN_BLKOBJ_H_
#define _Z_EN_BLKOBJ_H_

#include "global.hpp"

struct EnBlkobj;

typedef void (*EnBlkobjActionFunc)(struct EnBlkobj*, PlayState*);

typedef struct EnBlkobj {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ s32 bgId;
    /* 0x01A8 */ char unk_1A8[0x14];
    /* 0x01BC */ s16 alpha;
    /* 0x01BE */ s16 timer;
    /* 0x01C0 */ EnBlkobjActionFunc actionFunc;
    /* 0x01C4 */ SkeletonAnimationModel* skel[2];
} EnBlkobj; // size = 0x1CC

extern ActorInit En_Blkobj_InitVars;

#endif
