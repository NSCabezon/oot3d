#ifndef _Z_EN_DAIKU_H_
#define _Z_EN_DAIKU_H_

#include "global.hpp"

struct EnDaiku;

typedef void (*EnDaikuActionFunc)(struct EnDaiku*, struct PlayState*);

typedef struct EnDaikuModels {
    /* 0x00 */ SkeletonAnimationModel* models[41];
} EnDaikuModels; // size = 0xA4

typedef struct EnDaiku {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x410];
    /* 0x0638 */ char unk_638[0x410];
    /* 0x0A48 */ EnDaikuActionFunc actionFunc;
    /* 0x0A4C */ ColliderCylinder collider;
    /* 0x0AA4 */ char unk_AA4[0xAA8 - 0xAA4];
    /* 0x0AA8 */ s32 talkState;
    /* 0x0AAC */ char unk_AAC[0xAB0 - 0xAAC];
    /* 0x0AB0 */ s32 waypoint;
    /* 0x0AB4 */ f32 runSpeed;
    /* 0x0AB8 */ s32 currentAnimIndex;
    /* 0x0ABC */ char unk_ABC[0xAC0 - 0xABC];
    /* 0x0AC0 */ s32 subCamActive;
    /* 0x0AC4 */ s32 escapeSubCamTimer;
    /* 0x0AC8 */ s32 subCamId;
    /* 0x0ACC */ s16 rotYtowardsPath;
    /* 0x0ACE */ char unk_ACE[0xAD0 - 0xACE];
    /* 0x0AD0 */ Vec3f subCamEyeInit;
    /* 0x0ADC */ Vec3f subCamAtNext;
    /* 0x0AE8 */ Vec3f subCamEye;
    /* 0x0AF4 */ Vec3f subCamAt;
    /* 0x0B00 */ s32 stateFlags;
    /* 0x0B04 */ s32 startFightSwitchFlag;
    /* 0x0B08 */ char unk_B08[0xB30 - 0xB08];
    /* 0x0B30 */ Vec3s initRot;
    /* 0x0B36 */ char unk_B36[2];
    /* 0x0B38 */ Vec3f initPos;
    /* 0x0B44 */ SkeletonAnimationModel* skelAnimModel[3];
    /* 0x0B50 */ EnDaikuModels skelAnimModelRest;
} EnDaiku; // size = 0xBF4

extern ActorInit En_Daiku_InitVars;

#endif
