#ifndef _Z_EN_MU_H_
#define _Z_EN_MU_H_

#include "global.hpp"

struct EnMu;

typedef void (*EnMuActionFunc)(struct EnMu*, PlayState*);

typedef struct EnMuInteractInfo {
    /* 0x00 */ s16 talkState;
    /* 0x02 */ char unk_02[0x26];
} EnMuInteractInfo; // size = 0x28

typedef struct EnMu {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ Vec3s jointTable[16];
    /* 0x0288 */ char unk_288[0x140];
    /* 0x03C8 */ Vec3s morphTable[16];
    /* 0x0428 */ char unk_428[0x140];
    /* 0x0568 */ EnMuActionFunc actionFunc;
    /* 0x056C */ ColliderCylinder collider;
    /* 0x05C4 */ EnMuInteractInfo interactInfo;
    /* 0x05EC */ u16 defaultTextId;
    /* 0x05EE */ s16 fidgetTableY[16];
    /* 0x060E */ s16 fidgetTableZ[16];
    /* 0x062E */ char unk_62E[0x2];
    /* 0x0630 */ SkeletonAnimationModel* skelAnimModel;
} EnMu; // size = 0x634

typedef struct EnMuColors {
    /* 0x00 */ f32 c[2][5][4];
} EnMuColors; // size = 0xA0

extern ActorInit En_Mu_InitVars;

#endif
