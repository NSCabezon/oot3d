#ifndef _Z_EN_ANI_H_
#define _Z_EN_ANI_H_

#include "global.hpp"

struct EnAni;

typedef void (*EnAniActionFunc)(struct EnAni* self, PlayState* play);

typedef struct EnAni {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ColliderCylinder collider;
    /* 0x01FC */ SkelAnime skelAnime;
    /* 0x0280 */ char unk_280[0x618];
    /* 0x0898 */ Vec3s limbRot; // applied to limb 0xE in the override callback
    /* 0x089E */ Vec3s unk_89E;
    /* 0x08A4 */ u16 unk_8A4;
    /* 0x08A6 */ u16 unk_8A6;
    /* 0x08A8 */ s16 eyeIndex;
    /* 0x08AA */ s16 blinkTimer;
    /* 0x08AC */ EnAniActionFunc actionFunc;
    /* 0x08B0 */ s16 unk_8B0;
    /* 0x08B2 */ char unk_8B2[0x2];
    /* 0x08B4 */ struct SkeletonAnimationModel* skelAnimModel;
    /* 0x08B8 */ char faceAnimations[0x1CC]; // FaceAnimations object
} EnAni; // size = 0xA84

extern ActorInit En_Ani_InitVars;

#endif
