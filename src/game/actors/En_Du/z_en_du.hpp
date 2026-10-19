#ifndef _Z_EN_DU_H_
#define _Z_EN_DU_H_

#include "global.hpp"

struct EnDu;

typedef void (*EnDuActionFunc)(struct EnDu*, PlayState*);

typedef struct {
    /* 0x00 */ s16 talkState;
    /* 0x02 */ s16 trackingMode;
    /* 0x04 */ s16 autoTurnTimer;
    /* 0x06 */ s16 autoTurnState;
    /* 0x08 */ Vec3s headRot;
    /* 0x0E */ Vec3s torsoRot;
    /* 0x14 */ f32 yOffset;
    /* 0x18 */ Vec3f trackPos;
    /* 0x24 */ char unk_24[0x4];
} EnDuInteractInfo; // size = 0x28

typedef struct EnDu {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ FaceAnimations faceAnim;
    /* 0x03F4 */ EnDuActionFunc actionFunc;
    /* 0x03F8 */ ColliderCylinder collider[3];
    /* 0x0500 */ s16 unk_500;
    /* 0x0502 */ char unk_502[0x4];
    /* 0x0506 */ s16 unk_506;
    /* 0x0508 */ s16 unk_508;
    /* 0x050A */ u8 unk_50A;
    /* 0x050B */ u8 unk_50B;
    /* 0x050C */ u8 unk_50C;
    /* 0x050D */ u8 unk_50D;
    /* 0x050E */ u8 unk_50E;
    /* 0x050F */ u8 unk_50F;
    /* 0x0510 */ s16 blinkTimer;
    /* 0x0512 */ char unk_512[0x42];
    /* 0x0554 */ EnDuInteractInfo interactInfo;
} EnDu; // size = 0x57C

extern ActorInit En_Du_InitVars;

#endif
