#ifndef _Z_EN_RL_H_
#define _Z_EN_RL_H_

#include "global.hpp"

struct EnRl;

typedef void (*EnRlActionFunc)(struct EnRl*, PlayState*);

typedef struct EnRl {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char jointTable[0x4AC];
    /* 0x06D4 */ char morphTable[0x4AC];
    /* 0x0B80 */ s16 eyeIdx;
    /* 0x0B82 */ char unk_B82[0x02];
    /* 0x0B84 */ s32 action;
    /* 0x0B88 */ s32 drawConfig;
    /* 0x0B8C */ char unk_B8C[0x04];
    /* 0x0B90 */ u32 unk_B90;
    /* 0x0B94 */ char unk_B94[0x08];
    /* 0x0B9C */ SkeletonAnimationModel* skelAnimModel;
    /* 0x0BA0 */ FaceAnimations faceAnims;
} EnRl; // size = 0xD6C

extern ActorInit En_Rl_InitVars;

#endif
