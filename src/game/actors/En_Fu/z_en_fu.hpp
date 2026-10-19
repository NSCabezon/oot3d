#ifndef _Z_EN_FU_H_
#define _Z_EN_FU_H_

#include "global.hpp"

struct EnFu;

typedef void (*EnFuActionFunc)(struct EnFu*, PlayState*);

typedef struct EnFuSub {
    /* 0x00 */ char unk_00[0x30];
    /* 0x30 */ Vec3s lookAngleOffset; // head rotation, applied by the head limb override
    /* 0x36 */ Vec3s unk_36; // second look-angle vector, tracked alongside lookAngleOffset
    /* 0x3C */ u16 behaviorFlags;
    /* 0x3E */ u16 facialExpression;
    /* 0x40 */ EnFuActionFunc actionFunc;
    /* 0x44 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x48 */ FaceAnimations faceAnim;
} EnFuSub;

typedef struct EnFu {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ColliderCylinder collider;
    /* 0x01FC */ SkelAnime skelAnime;
    /* 0x0280 */ Vec3s jointTable[14];
    /* 0x02D4 */ char unk_2D4[0x284];
    /* 0x0558 */ Vec3s morphTable[14];
    /* 0x05AC */ char unk_5AC[0x254];
    /* 0x0800 */ EnFuSub sub;
} EnFu;

extern ActorInit En_Fu_InitVars;

#endif
