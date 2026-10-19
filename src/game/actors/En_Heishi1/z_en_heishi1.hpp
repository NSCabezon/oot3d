#ifndef _Z_EN_HEISHI1_H_
#define _Z_EN_HEISHI1_H_

#include "global.hpp"

struct EnHeishi1;

typedef void (*EnHeishi1ActionFunc)(struct EnHeishi1*, PlayState*);

typedef struct EnHeishi1 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x680];
    /* 0x08A8 */ EnHeishi1ActionFunc actionFunc;
    /* 0x08AC */ s16 activeTimer;
    /* 0x08AE */ s16 headDirection;
    /* 0x08B0 */ s16 headBehaviorDecided;
    /* 0x08B2 */ s16 linkDetected;
    /* 0x08B4 */ s16 loadStarted;
    /* 0x08B6 */ s16 type;
    /* 0x08B8 */ s16 path;
    /* 0x08BA */ s16 unused1;
    /* 0x08BC */ s16 waypointTimer;
    /* 0x08BE */ s16 unused2;
    /* 0x08C0 */ f32 moveSpeed;
    /* 0x08C4 */ f32 bodyTurnSpeed;
    /* 0x08C8 */ f32 headAngle;
    /* 0x08CC */ f32 headAngleTarget;
    /* 0x08D0 */ f32 animSpeed;
    /* 0x08D4 */ f32 animMorphFrames;
    /* 0x08D8 */ f32 moveSpeedTarget;
    /* 0x08DC */ f32 moveSpeedMax;
    /* 0x08E0 */ f32 bodyTurnSpeedTarget;
    /* 0x08E4 */ f32 bodyTurnSpeedMax;
    /* 0x08E8 */ f32 headTurnSpeedScale;
    /* 0x08EC */ f32 headTurnSpeedMax;
    /* 0x08F0 */ s16 headTimer;
    /* 0x08F2 */ s16 waitTimer;
    /* 0x08F4 */ s16 kickTimer;
    /* 0x08F6 */ s16 waypoint;
    /* 0x08F8 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x08FC */ s32 unk_8FC;
} EnHeishi1; // size = 0x900

extern ActorInit En_Heishi1_InitVars;

#endif
