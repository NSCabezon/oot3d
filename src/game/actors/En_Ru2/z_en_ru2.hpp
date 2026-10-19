#ifndef _Z_EN_RU2_H_
#define _Z_EN_RU2_H_

#include "global.hpp"

struct EnRu2;

typedef void (*EnRu2ActionFunc)(struct EnRu2*, PlayState*);
typedef void (*EnRu2DrawFunc)(struct EnRu2*, PlayState*);

typedef struct EnRu2 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x4E0]; // passed to FUN_00353e78
    /* 0x0708 */ char unk_708[0x4E0]; // passed to FUN_00353e78
    /* 0x0BE8 */ FaceAnimations faceAnims;
    /* 0x0DB4 */ char unk_DB4[0x4];
    /* 0x0DB8 */ s32 action;
    /* 0x0DBC */ s32 drawConfig;
    /* 0x0DC0 */ char unk_DC0[0x12];
    /* 0x0DD2 */ u8 unk_DD2;
    /* 0x0DD3 */ u8 unk_DD3;
    /* 0x0DD4 */ char unk_DD4[0x4];
    /* 0x0DD8 */ ColliderCylinder collider;
    /* 0x0E30 */ char unk_E30[0x38];
} EnRu2; // size = 0xE68

extern ActorInit En_Ru2_InitVars;

#endif
