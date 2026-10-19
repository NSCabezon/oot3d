#ifndef _Z_DEMO_TRE_LGT_H_
#define _Z_DEMO_TRE_LGT_H_

#include "global.hpp"

struct DemoTreLgt;

typedef void (*DemoTreLgtActionFunc)(struct DemoTreLgt*, PlayState*);

typedef struct DemoTreLgt {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ void* cmb;
    /* 0x01A8 */ char unk_1A8[0x8];
    /* 0x01B0 */ ZARInfo* zarInfo;
    /* 0x01B4 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01B8 */ char unk_1B8[0x38];
    /* 0x01F0 */ char unk_1F0[0x390];
    /* 0x0580 */ s32 action;
    /* 0x0584 */ f32 unk_170;
    /* 0x0588 */ f32 unk_174;
    /* 0x058C */ u8 status;
} DemoTreLgt; // size = 0x590

extern ActorInit Demo_Tre_Lgt_InitVars;

#endif
