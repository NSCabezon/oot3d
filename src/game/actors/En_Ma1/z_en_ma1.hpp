#ifndef _Z_EN_MA1_H_
#define _Z_EN_MA1_H_

#include "global.hpp"

struct EnMa1;

typedef void (*EnMa1ActionFunc)(struct EnMa1*, PlayState*);

typedef struct EnMa1InteractInfo {
    /* 0x00 */ char unk_00[0x08];
    /* 0x08 */ u16 singingDisabled;
    /* 0x0A */ s16 blinkTimer;
    /* 0x0C */ s16 eyeIndex;
    /* 0x0E */ s16 mouthIndex;
    /* 0x10 */ u16 talkState;
    /* 0x12 */ char unk_12[0x06];
    /* 0x18 */ Vec3s torsoRot;
    /* 0x1E */ Vec3s headRot;
    /* 0x24 */ char unk_24[0x04];
    /* 0x28 */ Vec3f trackPos;
    /* 0x34 */ char unk_34[0x04];
} EnMa1InteractInfo; // size = 0x38

typedef struct EnMa1AnimInfo {
    /* 0x00 */ s32 animIdx;
    /* 0x04 */ f32 playbackSpeed;
    /* 0x08 */ u8 mode;
    /* 0x0C */ f32 morphFrames;
} EnMa1AnimInfo; // size = 0x10

typedef struct EnMa1 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x7B8];
    /* 0x095C */ SkelAnime skelAnime;
    /* 0x09E0 */ char unk_9E0[0x1CC]; // FaceAnimations (accessed through a cast)
    /* 0x0BAC */ EnMa1ActionFunc actionFunc;
    /* 0x0BB0 */ char unk_BB0[0x50]; // ColliderCylinder (accessed through a cast)
    /* 0x0C00 */ EnMa1InteractInfo interactInfo;
} EnMa1; // size = 0xC38

extern ActorInit En_Ma1_InitVars;

#endif
