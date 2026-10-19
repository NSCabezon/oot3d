#ifndef _Z_DEMO_GT_H_
#define _Z_DEMO_GT_H_

#include "global.hpp"

struct DemoGt;

typedef struct DemoGt {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ s32 bgId;
    /* 0x01A8 */ char unk_1A8[0x14];
    /* 0x01BC */ s32 updateMode;
    /* 0x01C0 */ s32 drawConfig;
    /* 0x01C4 */ char unk_1C4[0x06];
    /* 0x01CA */ s16 unk_1CA;
    /* 0x01CC */ s16 unk_1CC;
    /* 0x01CE */ char unk_1CE[0x02];
    /* 0x01D0 */ s32 unk_1D0[3];
    /* 0x01DC */ char unk_1DC[0x04];
    /* 0x01E0 */ s32 unk_1E0[3];
    /* 0x01EC */ char unk_1EC[0x04];
    /* 0x01F0 */ s32 unk_1F0[2];
    /* 0x01F8 */ char unk_1F8[0x08];
    /* 0x0200 */ SkeletonAnimationModel* unk_200[2];
    /* 0x0208 */ SkeletonAnimationModel* skelAnimModel[2];
    /* 0x0210 */ SkeletonAnimationModel* unk_210[3];
} DemoGt; // size = 0x21C

extern ActorInit Demo_Gt_InitVars;

#endif
