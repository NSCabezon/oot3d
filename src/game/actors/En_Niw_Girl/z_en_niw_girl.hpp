#ifndef _Z_EN_NIW_GIRL_H_
#define _Z_EN_NIW_GIRL_H_

#include "global.hpp"

struct EnNiwGirl;
struct EnNiw;

typedef void (*EnNiwGirlActionFunc)(struct EnNiwGirl*, PlayState*);

typedef struct EnNiwGirlInteractInfo {
    /* 0x00 */ char unk_00[0x8];
    /* 0x08 */ Vec3s headRot;
    /* 0x0E */ Vec3s torsoRot;
    /* 0x14 */ f32 yOffset;
    /* 0x18 */ Vec3f trackPos;
    /* 0x24 */ char unk_24[0x4];
} EnNiwGirlInteractInfo; // size = 0x28

typedef struct EnNiwGirl {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x680];
    /* 0x08A8 */ EnNiwGirlActionFunc actionFunc;
    /* 0x08AC */ Vec3s unk_8AC;
    /* 0x08B2 */ Vec3s unk_8B2;
    /* 0x08B8 */ s16 jumpTimer;
    /* 0x08BA */ u16 unkUpTimer;
    /* 0x08BC */ s16 unk_8BC;
    /* 0x08BE */ s16 eyeIndex;
    /* 0x08C0 */ s16 blinkTimer;
    /* 0x08C2 */ s16 path;
    /* 0x08C4 */ char unk_8C4[0x2];
    /* 0x08C6 */ s16 unk_27A;
    /* 0x08C8 */ f32 unk_27C;
    /* 0x08CC */ f32 unk_280;
    /* 0x08D0 */ struct EnNiw* chasedEnNiw;
    /* 0x08D4 */ ColliderCylinder collider;
    /* 0x092C */ EnNiwGirlInteractInfo interactInfo;
    /* 0x0954 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x0958 */ char faceAnims[0x1CC]; // FaceAnimations
    /* 0x0B24 */ u8 unk_B24;
    /* 0x0B25 */ char unk_B25[0x3];
} EnNiwGirl; // size = 0xB28

extern ActorInit En_Niw_Girl_InitVars;

#endif
