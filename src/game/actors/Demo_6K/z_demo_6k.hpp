#ifndef _Z_DEMO_6K_H_
#define _Z_DEMO_6K_H_

#include "global.hpp"

struct Demo6K;

typedef void (*Demo6KActionFunc)(struct Demo6K*, PlayState*);

typedef struct Demo6K {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ Demo6KActionFunc initActionFunc;
    /* 0x01A8 */ char lightInfo[0x18];
    /* 0x01C0 */ void* lightNode;
    /* 0x01C4 */ f32 unk_164;
    /* 0x01C8 */ f32 unk_168;
    /* 0x01CC */ f32 unk_16C;
    /* 0x01D0 */ f32 unk_170;
    /* 0x01D4 */ f32 unk_174[16];
    /* 0x0214 */ f32 unk_1B4[16];
    /* 0x0254 */ f32 unk_1F4[16];
    /* 0x0294 */ f32 unk_234[16];
    /* 0x02D4 */ u8 unk_274[16];
    /* 0x02E4 */ ActorFunc drawFunc;
    /* 0x02E8 */ ActorFunc actionFunc;
    /* 0x02EC */ u16 flags;
    /* 0x02EE */ u16 timer1;
    /* 0x02F0 */ u16 timer2;
    /* 0x02F2 */ u8 requiredObjectSlot;
    /* 0x02F3 */ u8 unk_293;
    /* 0x02F4 */ u8 unk_294;
    /* 0x02F5 */ char unk_2F5[0x3];
    /* 0x02F8 */ SkeletonAnimationModel* skelAnimModel[29];
    /* 0x036C */ ZARInfo* zarInfo;
} Demo6K; // size = 0x370

extern ActorInit Demo_6K_InitVars;

#endif
