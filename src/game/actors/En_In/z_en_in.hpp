#ifndef _Z_EN_IN_H_
#define _Z_EN_IN_H_

#include "global.hpp"

struct EnIn;

typedef void (*EnInActionFunc)(struct EnIn* self, PlayState* play);

typedef struct EnInInteractInfo {
    /* 0x00 */ u16 talkState;
    /* 0x02 */ char unk_02[0x06];
    /* 0x08 */ Vec3s torsoRot;
    /* 0x0E */ Vec3s headRot;
    /* 0x14 */ f32 yOffset;
    /* 0x18 */ Vec3f trackPos;
    /* 0x24 */ char unk_24[0x04];
} EnInInteractInfo; // size = 0x28

typedef struct EnIn {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x1CC];
    /* 0x03F4 */ f32 unk_3F4;
    /* 0x03F8 */ f32 unk_3F8;
    /* 0x03FC */ EnInActionFunc actionFunc;
    /* 0x0400 */ ColliderCylinder collider;
    /* 0x0458 */ char unk_458[0x4];
    /* 0x045C */ s8 requiredObjectSlot;
    /* 0x045D */ char unk_45D[0x1];
    /* 0x045E */ s16 horsebackAnimationIdx;
    /* 0x0460 */ s16 unusedAnimationIdx;
    /* 0x0462 */ s16 blinkTimer;
    /* 0x0464 */ s16 losingScreamTimer;
    /* 0x0466 */ s16 eyeIndex;
    /* 0x0468 */ char unk_468[0x8];
    /* 0x0470 */ s16 textState;
    /* 0x0472 */ s16 prevTextState;
    /* 0x0474 */ char unk_474[0x2];
    /* 0x0476 */ u16 unk_476;
    /* 0x0478 */ f32 unk_478;
    /* 0x047C */ char unk_47C[0x90C];
    /* 0x0D88 */ EnInInteractInfo interactInfo;
    /* 0x0DB0 */ Vec3s fidgetVec[22];
} EnIn; // size = 0xE34

extern ActorInit En_In_InitVars;

#endif
