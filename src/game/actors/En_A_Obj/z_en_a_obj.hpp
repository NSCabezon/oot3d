#ifndef _Z_EN_A_OBJ_H_
#define _Z_EN_A_OBJ_H_

#include "global.hpp"

struct EnAObj;

typedef struct EnAObj {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ s32 transformFlags;
    /* 0x01A8 */ ActorFunc actionFunc;
    /* 0x01AC */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01B0 */ ColliderCylinder collider;
    /* 0x0208 */ f32 unk_208;
    /* 0x020C */ s32 bgId;
    /* 0x0210 */ s32 rotateWaitTimer;
    /* 0x0214 */ char unk_214[0xA];
    /* 0x021E */ s16 textIdParam;
    /* 0x0220 */ s16 rotateState;
    /* 0x0222 */ char unk_222[0x6];
    /* 0x0228 */ u8 interactFlags;
    /* 0x0229 */ char unk_229[0x3];
} EnAObj; // size = 0x22C

extern ActorInit En_A_Obj_InitVars;

#endif
