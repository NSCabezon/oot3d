#ifndef _Z_ACTOR_1D8_H_
#define _Z_ACTOR_1D8_H_

#include "global.hpp"

struct Actor1D8;

typedef struct Actor1D8Entry {
    /* 0x00 */ u8 type;
    /* 0x01 */ char unk_01[0x03];
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ char unk_10[0x1C];
    /* 0x2C */ s16 unk_2C;
    /* 0x2E */ char unk_2E[0x06];
    /* 0x34 */ f32 scaleX;
    /* 0x38 */ char unk_38[0x08];
    /* 0x40 */ f32 scaleY;
    /* 0x44 */ char unk_44[0x08];
} Actor1D8Entry; // size = 0x4C

typedef struct Actor1D8 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ u32 unk_1A4;
    /* 0x01A8 */ SkelAnime skelAnime;
    /* 0x022C */ SkeletonAnimationModel* skelAnimModel[2];
    /* 0x0234 */ char unk_234[0x04];
    /* 0x0238 */ void (*actionFunc)(struct Actor1D8*, PlayState*);
    /* 0x023C */ u8 unk_23C;
    /* 0x023D */ char unk_23D[0x27];
    /* 0x0264 */ f32 unk_264;
    /* 0x0268 */ char unk_268[0x2C];
    /* 0x0294 */ Vec3f head;
    /* 0x02A0 */ Vec3f pos;
    /* 0x02AC */ char unk_2AC[0xCC];
    /* 0x0378 */ ColliderCylinder collider0;
    /* 0x03D0 */ ColliderCylinder collider1;
    /* 0x0428 */ ColliderCylinder collider2;
    /* 0x0480 */ ColliderCylinder collider3;
    /* 0x04D8 */ Vec3f limbPoints[4];
    /* 0x0508 */ char unk_508[0x08];
    /* 0x0510 */ s32 unk_510;
    /* 0x0514 */ s16 unk_514;
    /* 0x0516 */ char unk_516[0x92];
    /* 0x05A8 */ f32 unk_5A8;
    /* 0x05AC */ f32 unk_5AC;
} Actor1D8; // size = 0x5B0

extern ActorInit Actor_1D8_InitVars;

#endif
