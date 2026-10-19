#ifndef _Z_EN_TK_H_
#define _Z_EN_TK_H_

#include "global.hpp"

/* Dirt particle effect */
typedef struct EnTkEff {
    /* 0x00 */ u8 active;
    /* 0x01 */ u8 timeLeft;
    /* 0x02 */ u8 timeTotal;
    /* 0x04 */ f32 size;
    /* 0x08 */ f32 growth;
    /* 0x0C */ char unk_0C[0x8];
    /* 0x14 */ Vec3f pos;
    /* 0x20 */ Vec3f speed;
    /* 0x2C */ Vec3f accel;
} EnTkEff; // size = 0x38

struct EnTk;

typedef void (*EnTkActionFunc)(struct EnTk*, PlayState*);

typedef struct EnTk {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime; // 3DS skeleton block is 0x974 bytes
    /* 0x0228 */ char jointTable[0x478];
    /* 0x06A0 */ char morphTable[0x478];
    /* 0x0B18 */ EnTkActionFunc actionFunc;
    /* 0x0B1C */ ColliderCylinder collider;
    /* 0x0B74 */ char unk_B74[0x28]; // NpcInteractInfo (talkState at +0)
    /* 0x0B9C */ u8 validDigHere;
    /* 0x0B9D */ u8 rewardCount[4];
    /* 0x0BA1 */ char unk_BA1[0x3];
    /* 0x0BA4 */ Actor* currentSpot;
    /* 0x0BA8 */ s32 currentReward;
    /* 0x0BAC */ s16 blinkCycles;
    /* 0x0BAE */ s16 rewardTimer;
    /* 0x0BB0 */ s16 actionCountdown;
    /* 0x0BB2 */ s16 h_21E;
    /* 0x0BB4 */ char unk_BB4[0x2];
    /* 0x0BB6 */ s16 eyeTextureIdx;
    /* 0x0BB8 */ s16 blinkCountdown;
    /* 0x0BBA */ s16 headRot;
    /* 0x0BBC */ s16 currentWaypoint;
    /* 0x0BBE */ char unk_BBE[0x2];
    /* 0x0BC0 */ Vec3f v3f_304;
    /* 0x0BCC */ EnTkEff eff[20];
    /* 0x102C */ struct SkeletonAnimationModel* skelAnimModel;
    /* 0x1030 */ char unk_1030[0x1CC]; // FaceAnimations
    /* 0x11FC */ void* unk_11FC;
} EnTk; // size = 0x1200

extern ActorInit En_Tk_InitVars;

#endif
