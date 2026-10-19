#ifndef _Z_EN_HATA_H_
#define _Z_EN_HATA_H_

#include "global.hpp"

struct EnHata;

typedef enum FlagpoleLimb {
    FLAGPOLE_LIMB_POLE_BASE,
    FLAGPOLE_LIMB_POLE,
    FLAGPOLE_LIMB_FLAG_1_BASE,
    FLAGPOLE_LIMB_FLAG_1_HOIST_END_BASE,
    FLAGPOLE_LIMB_FLAG_1_HOIST_MID_BASE,
    FLAGPOLE_LIMB_FLAG_1_FLY_MID_BASE,
    FLAGPOLE_LIMB_FLAG_1_FLY_END_BASE,
    FLAGPOLE_LIMB_FLAG_1_FLY_END,
    FLAGPOLE_LIMB_FLAG_1_FLY_MID,
    FLAGPOLE_LIMB_FLAG_1_HOIST_MID,
    FLAGPOLE_LIMB_FLAG_1_HOIST_END,
    FLAGPOLE_LIMB_FLAG_2_BASE,
    FLAGPOLE_LIMB_FLAG_2_HOIST_END_BASE,
    FLAGPOLE_LIMB_FLAG_2_HOIST_MID_BASE,
    FLAGPOLE_LIMB_FLAG_2_FLY_MID_BASE,
    FLAGPOLE_LIMB_FLAG_2_FLY_END_BASE,
    FLAGPOLE_LIMB_FLAG_2_FLY_END,
    FLAGPOLE_LIMB_FLAG_2_FLY_MID,
    FLAGPOLE_LIMB_FLAG_2_HOIST_MID,
    FLAGPOLE_LIMB_FLAG_2_HOIST_END,
    FLAGPOLE_LIMB_MAX,
} FlagpoleLimb;

typedef struct EnHata {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ SkelAnime skelAnime;
    /* 0x0240 */ Vec3s limbs[FLAGPOLE_LIMB_MAX];
    /* 0x02B8 */ ColliderCylinder collider; // Unused
    /* 0x0310 */ s16 invScale;
    /* 0x0312 */ s16 maxStep;
    /* 0x0314 */ s16 minStep;
    /* 0x0316 */ s16 unk_316;
    /* 0x0318 */ f32 unk_318;
} EnHata; // size = 0x31C

extern ActorInit En_Hata_InitVars;

#endif
