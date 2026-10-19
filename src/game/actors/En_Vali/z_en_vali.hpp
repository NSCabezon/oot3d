#ifndef _Z_EN_VALI_H_
#define _Z_EN_VALI_H_

#include "global.hpp"

struct EnVali;

typedef void (*EnValiActionFunc)(struct EnVali*, PlayState*);

// 0x80 bytes: Collider base + info + 4 corner points (same shape as Collider_*Quad users)
typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ ColliderInfo info;
    /* 0x40 */ struct {
        Vec3f quad[4];
        char unk_30[0x10];
    } dim;
} ColliderQuad;

typedef struct EnVali {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char jointTable[0x23C];
    /* 0x0464 */ char morphTable[0x23C];
    /* 0x06A0 */ EnValiActionFunc actionFunc;
    /* 0x06A4 */ u8 electrifyTimer;
    /* 0x06A5 */ u8 spinTimer;
    /* 0x06A6 */ s16 timer;
    /* 0x06A8 */ f32 tentaclesLengthFactor;
    /* 0x06AC */ f32 spinHeight;
    /* 0x06B0 */ ColliderQuad leftTentacleCollider;
    /* 0x0730 */ ColliderQuad rightTentacleCollider;
    /* 0x07B0 */ ColliderCylinder collider;
    /* 0x0808 */ void* unk_808[6];
    /* 0x0820 */ SkeletonAnimationModel_unk_0C anim;
    /* 0x08B8 */ u8 unk_8B8;
    /* 0x08B9 */ char unk_8B9[0x3];
    /* 0x08BC */ Vec3f unk_8BC;
} EnVali; // size = 0x8C8

extern ActorInit En_Vali_InitVars;

#endif
