#ifndef _Z_EN_MA2_H_
#define _Z_EN_MA2_H_

#include "global.hpp"

struct EnMa2;

typedef void (*EnMa2ActionFunc)(struct EnMa2*, PlayState*);

typedef struct EnMa2InteractInfo {
    /* 0x00 */ u16 talkState;
    /* 0x02 */ char unk_02[0x06];
    /* 0x08 */ Vec3s torsoRot;
    /* 0x0E */ Vec3s headRot;
    /* 0x14 */ f32 yOffset;
    /* 0x18 */ Vec3f trackPos;
    /* 0x24 */ char unk_24[0x04];
} EnMa2InteractInfo; // size = 0x28

typedef struct EnMa2 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x1CC]; // FaceAnimations (accessed through a cast)
    /* 0x03F4 */ EnMa2ActionFunc actionFunc;
    /* 0x03F8 */ ColliderCylinder collider;
    /* 0x0450 */ EnMa2InteractInfo interactInfo;
    /* 0x0478 */ s16 timer;
    /* 0x047A */ u16 singingDisabled;
    /* 0x047C */ s16 blinkTimer;
    /* 0x047E */ s16 eyeIndex;
    /* 0x0480 */ s16 mouthIndex;
    /* 0x0482 */ char unk_482[0x7A];
} EnMa2; // size = 0x4FC

extern ActorInit En_Ma2_InitVars;

#endif
