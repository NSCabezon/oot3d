#ifndef _Z_EN_DAIKU_KAKARIKO_H_
#define _Z_EN_DAIKU_KAKARIKO_H_

#include "global.hpp"

struct EnDaikuKakariko;

typedef void (*EnDaikuKakarikoActionFunc)(struct EnDaikuKakariko*, PlayState*);

typedef struct EnDaikuKakarikoAnimInfo {
    /* 0x00 */ s32 animation;
    /* 0x04 */ f32 frameCount;
    /* 0x08 */ u8 mode;
    /* 0x0C */ f32 morphFrames;
} EnDaikuKakarikoAnimInfo; // size = 0x10

typedef struct EnDaikuKakarikoInteractInfo {
    /* 0x00 */ char unk_00[0x8];
    /* 0x08 */ Vec3s headRot;
    /* 0x0E */ Vec3s torsoRot;
    /* 0x14 */ f32 yOffset;
    /* 0x18 */ Vec3f trackPos;
    /* 0x24 */ char unk_24[0x4];
} EnDaikuKakarikoInteractInfo; // size = 0x28

typedef struct EnDaikuKakarikoModels {
    /* 0x00 */ SkeletonAnimationModel* models[41];
} EnDaikuKakarikoModels; // size = 0xA4

typedef struct EnDaikuKakariko {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x410];
    /* 0x0638 */ char unk_638[0x410];
    /* 0x0A48 */ EnDaikuKakarikoActionFunc actionFunc;
    /* 0x0A4C */ ColliderCylinder collider;
    /* 0x0AA4 */ s32 talkState;
    /* 0x0AA8 */ s32 waypoint;
    /* 0x0AAC */ f32 runSpeed;
    /* 0x0AB0 */ s32 currentAnimIndex;
    /* 0x0AB4 */ char unk_AB4[0x8];
    /* 0x0ABC */ s32 pathContinue;
    /* 0x0AC0 */ s32 run;
    /* 0x0AC4 */ u16 flags;
    /* 0x0AC6 */ u16 runFlag;
    /* 0x0AC8 */ EnDaikuKakarikoInteractInfo interactInfo;
    /* 0x0AF0 */ s32 timer;
    /* 0x0AF4 */ Vec3s neckAngle;
    /* 0x0AFA */ Vec3s neckAngleTarget;
    /* 0x0B00 */ SkeletonAnimationModel* skelAnimModel[3];
    /* 0x0B0C */ EnDaikuKakarikoModels skelAnimModelRest;
} EnDaikuKakariko; // size = 0x0BB0

extern ActorInit En_Daiku_Kakariko_InitVars;

#endif
