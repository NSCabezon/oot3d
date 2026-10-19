#ifndef _Z_DEMO_SHD_H_
#define _Z_DEMO_SHD_H_

#include "global.hpp"

struct DemoShd;

typedef void (*DemoShdActionFunc)(struct DemoShd*, PlayState*);

typedef struct DemoShd {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ u16 unk_14C; // N64 0x14C: bit0 = cue 0 active, bit1 = cue 1 active
    /* 0x01A6 */ u16 unk_14E; // N64 0x14E: scroll / frame counter
    /* 0x01A8 */ DemoShdActionFunc actionFunc;
    /* 0x01AC */ SkeletonAnimationModel* skelAnimModel[2];
    /* 0x01B4 */ void* cmab[2];
} DemoShd; // size = 0x1BC

extern ActorInit Demo_Shd_InitVars;

#endif
