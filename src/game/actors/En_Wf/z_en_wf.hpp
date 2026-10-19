#ifndef _Z_EN_WF_H_
#define _Z_EN_WF_H_

#include "global.hpp"

struct EnWf;

typedef void (*EnWfActionFunc)(struct EnWf*, PlayState*);

typedef struct EnWf {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ Vec3s bodyPartsPos[10];
    /* 0x01E0 */ SkelAnime skelAnime;
    /* 0x0264 */ FaceAnimations faceAnimations;
    /* 0x0430 */ char jointTable[0x444];
    /* 0x0874 */ char morphTable[0x444];
    /* 0x0CB8 */ s32 action;
    /* 0x0CBC */ char unk_CBC[0x4];
    /* 0x0CC0 */ EnWfActionFunc actionFunc;
    /* 0x0CC4 */ s16 unk_CC4;
    /* 0x0CC6 */ s16 unk_CC6;
    /* 0x0CC8 */ s16 onFireTimer;
    /* 0x0CCA */ char unk_CCA[0x2];
    /* 0x0CCC */ s32 timer;
    /* 0x0CD0 */ f32 circlePlayerExtraSpeed;
    /* 0x0CD4 */ char unk_CD4[0x4];
    /* 0x0CD8 */ f32 unk_2F4;
    /* 0x0CDC */ s16 attackState;
    /* 0x0CDE */ s16 unk_2FA;
    /* 0x0CE0 */ s16 switchFlag;
    /* 0x0CE2 */ s16 circlePlayerDirection;
    /* 0x0CE4 */ s16 unk_300;
    /* 0x0CE6 */ u8 eyeIndex;
    /* 0x0CE7 */ char unk_CE7[0x1];
    /* 0x0CE8 */ u8 attackAndBlockCollider[0x160];
    /* 0x0E48 */ ColliderCylinder bodyCollider;
    /* 0x0EA0 */ ColliderCylinder tailCollider;
    /* 0x0EF8 */ Vec3f unk_4BC;
    /* 0x0F04 */ Vec3f unk_4C8;
    /* 0x0F10 */ char unk_F10[0x2];
    /* 0x0F12 */ s16 headRot;
    /* 0x0F14 */ char unk_F14[0x4];
} EnWf; // size = 0xF18

extern ActorInit En_Wf_InitVars;

#endif
