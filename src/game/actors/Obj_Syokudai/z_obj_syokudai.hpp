#ifndef _Z_OBJ_SYOKUDAI_H_
#define _Z_OBJ_SYOKUDAI_H_

#include "global.hpp"

struct ObjSyokudai;

typedef struct ObjSyokudai {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ColliderCylinder collider1;
    /* 0x01FC */ ColliderCylinder collider2;
    /* 0x0254 */ s32 lightNode;
    /* 0x0258 */ char unk_258[0x18];
    /* 0x0270 */ struct ObjSyokudaiModel* model1;
    /* 0x0274 */ struct ObjSyokudaiModel* model2;
    /* 0x0278 */ SkeletonAnimationModel_unk_0C* animModel;
    /* 0x027C */ s16 litTimer;
    /* 0x027E */ u8 flameTexScroll;
    /* 0x027F */ u8 unk_27F;
    /* 0x0280 */ u8 unk_280;
    /* 0x0281 */ char unk_281[0x3];
} ObjSyokudai; // size = 0x284

extern ActorInit Obj_Syokudai_InitVars;

#endif
