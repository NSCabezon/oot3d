#ifndef _Z_BG_JYA_MEGAMI_H_
#define _Z_BG_JYA_MEGAMI_H_

#include "global.hpp"

struct BgJyaMegami;

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ void* elements;
} BgJyaMegamiColliderJntSph; // size = 0x20

typedef struct {
    /* 0x00 */ ColliderInit base;
    /* 0x08 */ s32 count;
    /* 0x0C */ void* elements;
} BgJyaMegamiJntSphInit;

typedef struct {
    /* 0x00 */ u8 unk_00[0x50];
} BgJyaMegamiColliderElement; // size = 0x50

typedef void (*BgJyaMegamiActionFunc)(struct BgJyaMegami*, PlayState*);

typedef struct BgJyaMegamiPiece {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f vel;
    /* 0x18 */ s16 rotVelX;
    /* 0x1A */ s16 rotVelY;
} BgJyaMegamiPiece; // size = 0x1C

typedef struct BgJyaMegami {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgJyaMegamiActionFunc actionFunc;
    /* 0x01C0 */ BgJyaMegamiColliderJntSph collider;
    /* 0x01E0 */ BgJyaMegamiColliderElement colliderElements[1];
    /* 0x0230 */ s16 lightTimer;
    /* 0x0232 */ s16 explosionTimer;
    /* 0x0234 */ s16 crumbleIndex;
    /* 0x0238 */ BgJyaMegamiPiece pieces[13];
    /* 0x03A4 */ SkeletonAnimationModel* models[14];
} BgJyaMegami; // size = 0x03DC

extern ActorInit Bg_Jya_Megami_InitVars;

#endif
