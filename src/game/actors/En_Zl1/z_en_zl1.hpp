#ifndef _Z_EN_ZL1_H_
#define _Z_EN_ZL1_H_

#include "global.hpp"

struct EnZl1;

typedef void (*EnZl1ActionFunc)(struct EnZl1*, PlayState*);

typedef struct EnZl1 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ FaceAnimations faceAnims;
    /* 0x03F4 */ EnZl1ActionFunc actionFunc;
    /* 0x03F8 */ ColliderCylinder collider;
    /* 0x0450 */ char unk_450[0x2];
    /* 0x0452 */ s16 unk_452;
    /* 0x0454 */ s16 unk_454;
    /* 0x0456 */ s16 unk_456;
    /* 0x0458 */ s16 subCamId;
    /* 0x045A */ char unk_45A[0x4];
    /* 0x045E */ u8 mouthIdx;
    /* 0x045F */ u8 eyeIdx1;
    /* 0x0460 */ u8 eyeIdx2;
    /* 0x0461 */ char unk_461[0x1];
    /* 0x0462 */ s16 blinkTimer;
    /* 0x0464 */ s16 blinkIdx;
    /* 0x0466 */ Vec3s unk_466;
    /* 0x046C */ Vec3s unk_46C;
    /* 0x0472 */ char unk_472[0x484 - 0x472];
} EnZl1; // size = 0x484

extern ActorInit En_Zl1_InitVars;

#endif
