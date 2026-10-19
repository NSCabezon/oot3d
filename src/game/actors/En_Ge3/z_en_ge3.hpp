#ifndef _Z_EN_GE3_H_
#define _Z_EN_GE3_H_

#include "global.hpp"

struct EnGe3;

typedef void (*EnGe3ActionFunc)(struct EnGe3*, struct PlayState*);

typedef struct EnGe3 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ColliderCylinder collider;
    /* 0x01FC */ SkelAnime skelAnime;
    /* 0x0280 */ Vec3s jointTable[0x13];
    /* 0x02F2 */ char unk_2F2[0x36A];
    /* 0x065C */ Vec3s morphTable[0x13];
    /* 0x06CE */ char unk_6CE[0x36A];
    /* 0x0A38 */ s16 eyeIndex;
    /* 0x0A3A */ s16 blinkTimer;
    /* 0x0A3C */ Vec3s headRot;
    /* 0x0A42 */ Vec3s unk_306;
    /* 0x0A48 */ u16 unk_30C;
    /* 0x0A4A */ char unk_A4A[0x2];
    /* 0x0A4C */ EnGe3ActionFunc actionFunc;
    /* 0x0A50 */ SkeletonAnimationModel* model;
    /* 0x0A54 */ FaceAnimations faceAnimations;
} EnGe3;

#endif
