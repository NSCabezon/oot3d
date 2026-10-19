#ifndef _Z_EN_JS_H_
#define _Z_EN_JS_H_

#include "global.hpp"

struct EnJs;

typedef void (*EnJsActionFunc)(struct EnJs*, PlayState*);

typedef struct EnJs {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ColliderCylinder collider;
    /* 0x01FC */ SkelAnime skelAnime;
    /* 0x0280 */ char jointTable[0x23C];
    /* 0x04BC */ char morphTable[0x23C];
    /* 0x06F8 */ Vec3s unk_278;
    /* 0x06FE */ Vec3s unk_27E;
    /* 0x0704 */ u16 unk_284;
    /* 0x0706 */ s16 unk_286;
    /* 0x0708 */ s16 unk_288;
    /* 0x070C */ EnJsActionFunc actionFunc;
} EnJs; // size = 0x710

extern ActorInit En_Js_InitVars;

#endif
