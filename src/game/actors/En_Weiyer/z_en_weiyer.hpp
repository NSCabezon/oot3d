#ifndef _Z_EN_WEIYER_H_
#define _Z_EN_WEIYER_H_

#include "global.hpp"

struct EnWeiyer;

typedef void (*EnWeiyerActionFunc)(struct EnWeiyer*, struct PlayState*);

typedef struct EnWeiyer {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ EnWeiyerActionFunc actionFunc;
    /* 0x022C */ s16 timer;
    /* 0x022E */ s16 targetYaw;
    /* 0x0230 */ Vec3s jointTable[19];
    /* 0x02A2 */ char unk_2A2[0x266];
    /* 0x0508 */ Vec3s morphTable[19];
    /* 0x057A */ char unk_57A[0x266];
    /* 0x07E0 */ f32 swimHeight;
    /* 0x07E4 */ f32 targetSwimHeight;
    /* 0x07E8 */ ColliderCylinder collider;
} EnWeiyer; // size = 0x840

extern ActorInit En_Weiyer_InitVars;

#endif
