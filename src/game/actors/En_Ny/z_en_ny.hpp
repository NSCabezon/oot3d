#ifndef _Z_EN_NY_H_
#define _Z_EN_NY_H_

#include "global.hpp"

struct EnNy;

typedef void (*EnNyActionFunc)(struct EnNy*, PlayState*);

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ void* elements;
} EnNyColliderJntSph; // size = 0x20

typedef struct EnNy {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnNyActionFunc actionFunc;
    /* 0x01A8 */ EnNyColliderJntSph collider;
    /* 0x01C8 */ char unk_1C8[0x68];
    /* 0x0230 */ s16 timer;
    /* 0x0232 */ s16 unk_1CA;
    /* 0x0234 */ s16 hitPlayer;
    /* 0x0236 */ u16 unk_1CE;
    /* 0x0238 */ u8 unk_1D0;
    /* 0x0239 */ s8 unk_1D1;
    /* 0x023A */ char unk_23A[0x2];
    /* 0x023C */ s32 unk_1D4;
    /* 0x0240 */ s32 unk_1D8;
    /* 0x0244 */ s32 stoneTimer; // Delay for when to attempt to change forms
    /* 0x0248 */ f32 unk_1E0;
    /* 0x024C */ f32 unk_1E4; // This, unk_1E8, and unk_1EC have to do with movement speed
    /* 0x0250 */ f32 unk_1E8;
    /* 0x0254 */ f32 unk_1EC;
    /* 0x0258 */ f32 unk_1F0;
    /* 0x025C */ f32 unk_1F4;
    /* 0x0260 */ Vec3f unk_1F8[16];
    /* 0x0320 */ Vec3f unk_320[8];
    /* 0x0380 */ SkeletonAnimationModel* models[15];
} EnNy; // size = 0x03BC

extern ActorInit En_Ny_InitVars;

#endif
