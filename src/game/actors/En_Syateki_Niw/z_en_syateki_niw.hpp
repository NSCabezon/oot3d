#ifndef _Z_EN_SYATEKI_NIW_H_
#define _Z_EN_SYATEKI_NIW_H_

#include "global.hpp"

struct EnSyatekiNiw;

typedef void (*EnSyatekiNiwActionFunc)(struct EnSyatekiNiw*, PlayState*);

typedef enum EnSyatekiMinigame {
    SYATEKI_MINIGAME_ARCHERY, // unused archery game behavior
    SYATEKI_MINIGAME_ALLEY // Bombchu Alley behavior
} EnSyatekiMinigame;

// Feather effect (N64 had 200, 3DS has 5, each with its own model at +0x38).
typedef struct EnSyatekiNiwEffect {
    /* 0x00 */ u8 state;
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ Vec3f vel;
    /* 0x1C */ Vec3f accel;
    /* 0x28 */ s16 lifespan;
    /* 0x2A */ s16 rotPulse;
    /* 0x2C */ f32 scale;
    /* 0x30 */ f32 rot;
    /* 0x34 */ u8 timer;
    /* 0x38 */ SkeletonAnimationModel* model;
} EnSyatekiNiwEffect; // size = 0x3C

// 3DS layout: N64 field offsets shifted (+0x58 for the Actor-relative prefix, +0x380 for fields from 0x250 on).
typedef struct EnSyatekiNiw {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ Vec3s jointTable[16];
    /* 0x0288 */ char unk_288[0x174];
    /* 0x03FC */ Vec3s morphTable[16];
    /* 0x045C */ char unk_45C[0x174];
    /* 0x05D0 */ EnSyatekiNiwActionFunc actionFunc;
    /* 0x05D4 */ s16 peckTimer; // intervals of bobbing head on ground
    /* 0x05D6 */ s16 timer1; // sometimes set alongside peckTimer, never read.
    /* 0x05D8 */ s16 flapTimer; // intervals of beating wings
    /* 0x05DA */ s16 archeryTimer;
    /* 0x05DC */ s16 hopTimer; // intervals of jumping
    /* 0x05DE */ s16 movementTimer; // intervals of changing location
    /* 0x05E0 */ s16 sootTimer; // cucco is covered in soot, smoke emits
    /* 0x05E2 */ s16 cluckTimer; // intervals of clucking SFX
    /* 0x05E4 */ f32 headRotXTarget;
    /* 0x05E8 */ f32 rightWingRotXTarget;
    /* 0x05EC */ f32 leftWingRotXTarget;
    /* 0x05F0 */ char unk_5F0[0x8];
    /* 0x05F8 */ f32 rightWingRotYTarget;
    /* 0x05FC */ f32 rightWingRotZTarget;
    /* 0x0600 */ f32 leftWingRotYTarget;
    /* 0x0604 */ f32 leftWingRotZTarget;
    /* 0x0608 */ f32 unkArcheryFloat; // set, but never used. another rot target?
    /* 0x060C */ s16 lifetime; // always ticks up, never read
    /* 0x060E */ s16 headRotXState;
    /* 0x0610 */ s16 unk_290; // set to 0, never read.
    /* 0x0612 */ s16 wingsRotState;
    /* 0x0614 */ s16 targetPosTimer;
    /* 0x0616 */ s16 archeryAnimationType;
    /* 0x0618 */ s16 rotYFlip;
    /* 0x061A */ char unk_61A[0x2];
    /* 0x061C */ s16 isFalling; // set when hit in archery game
    /* 0x061E */ s16 minigameType; // uses EnSyatekiMinigame from params
    /* 0x0620 */ s16 spawnFeathers; // triggers feather particles
    /* 0x0622 */ char unk_622[0x2];
    /* 0x0624 */ Vec3f leftWingRot;
    /* 0x0630 */ Vec3f rightWingRot;
    /* 0x063C */ Vec3f headRot;
    /* 0x0648 */ Vec3f posRotStep;
    /* 0x0654 */ f32 focusYOffset;
    /* 0x0658 */ f32 removeStateYaw;
    /* 0x065C */ Vec3f initPos;
    /* 0x0668 */ Vec3f targetPos;
    /* 0x0674 */ f32 scale;
    /* 0x0678 */ u8 unkAlleyHitByte; // Set when hit in Bombchu Alley. Never read.
    /* 0x0679 */ char unk_679[0x3];
    /* 0x067C */ ColliderCylinder collider;
    /* 0x06D4 */ EnSyatekiNiwEffect effects[5];
} EnSyatekiNiw; // size = 0x0800

extern ActorInit En_Syateki_Niw_InitVars;

#endif
