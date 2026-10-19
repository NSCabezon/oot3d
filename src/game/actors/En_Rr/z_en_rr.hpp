#ifndef _Z_EN_RR_H_
#define _Z_EN_RR_H_

#include "global.hpp"

struct EnRr;

typedef void (*EnRrActionFunc)(struct EnRr*, PlayState*);

typedef struct EnRrBodySegment {
    /* 0x00 */ f32 height;
    /* 0x04 */ f32 heightTarget;
    /* 0x08 */ Vec3f scale;
    /* 0x14 */ Vec3f scaleTarget;
    /* 0x20 */ Vec3f scaleMod;
    /* 0x2C */ Vec3f rotTarget;
    /* 0x38 */ Vec3s rot;
} EnRrBodySegment; // size = 0x40

typedef struct EnRr {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnRrActionFunc actionFunc;
    /* 0x01A8 */ ColliderCylinder collider1;
    /* 0x0200 */ ColliderCylinder collider2;
    /* 0x0258 */ s16 frameCount;
    /* 0x025A */ s16 actionTimer;
    /* 0x025C */ s16 scrollTimer;
    /* 0x025E */ s16 grabTimer;
    /* 0x0260 */ s16 invincibilityTimer;
    /* 0x0262 */ s16 effectTimer;
    /* 0x0264 */ s16 ocTimer;
    /* 0x0266 */ s16 segMovePhase;
    /* 0x0268 */ f32 segPhaseVel;
    /* 0x026C */ f32 segPhaseVelTarget;
    /* 0x0270 */ f32 segPulsePhaseDiff;
    /* 0x0274 */ f32 segWobblePhaseDiffX;
    /* 0x0278 */ f32 segWobbleXTarget;
    /* 0x027C */ f32 segWobblePhaseDiffZ;
    /* 0x0280 */ f32 segWobbleZTarget;
    /* 0x0284 */ f32 pulseSize;
    /* 0x0288 */ f32 pulseSizeTarget;
    /* 0x028C */ f32 wobbleSize;
    /* 0x0290 */ f32 wobbleSizeTarget;
    /* 0x0294 */ EnRrBodySegment bodySegs[5];
    /* 0x03D4 */ f32 segMoveRate;
    /* 0x03D8 */ f32 shrinkRate;
    /* 0x03DC */ f32 swallowOffset;
    /* 0x03E0 */ u8 reachState;
    /* 0x03E1 */ u8 isDead;
    /* 0x03E2 */ u8 eatenShield;
    /* 0x03E3 */ u8 eatenTunic;
    /* 0x03E4 */ u8 dropType;
    /* 0x03E5 */ u8 retreat;
    /* 0x03E6 */ u8 stopScroll;
    /* 0x03E8 */ s16 hasPlayer;
    /* 0x03EC */ Vec3f mouthPos;
    /* 0x03F8 */ Vec3f effectPos[5];
    /* 0x0434 */ char unk_434[0x2004];
    /* 0x2438 */ SkelAnime skelAnime;
    /* 0x24BC */ char unk_24BC[0x208];
    /* 0x26C4 */ MTX34 limbMtx[5]; // per-limb matrices copied out by FUN_0015064c (PostLimbDraw)
    /* 0x27B4 */ char animModel[0x98];
    /* 0x284C */ u8 unk_284C; // 3DS-only grab-state flag (EnRr_GrabPlayer)
    /* 0x284D */ u8 unk_284D; // 3DS-only flag read by Stunned/GrabPlayer
    /* 0x284E */ char unk_284E[0x2];
} EnRr; // size = 0x2850

extern ActorInit En_Rr_InitVars;

#endif
