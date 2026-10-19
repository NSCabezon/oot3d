#ifndef _Z_END_TITLE_H_
#define _Z_END_TITLE_H_

#include "global.hpp"

struct EndTitle;

typedef struct EndTitle {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01A8 */ void* zarCtxb[4];
    /* 0x01B8 */ void* unk_1B8;
    /* 0x01BC */ f32 endAlpha;
    /* 0x01C0 */ f32 tlozAlpha;
    /* 0x01C4 */ f32 ootAlpha;
    /* 0x01C8 */ u32 logoTimer; // params == 2 (FUN_00250540)
} EndTitle; // size = 0x1CC

extern ActorInit End_Title_InitVars;

#endif
