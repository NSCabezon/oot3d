#ifndef _Z_EN_FHG_FIRE_H_
#define _Z_EN_FHG_FIRE_H_

#include "global.hpp"

struct EnFhgFire;

typedef void (*EnFhgFireUpdateFunc)(struct EnFhgFire*, PlayState*);

typedef enum FhgFireParam {
    /*   1 */ FHGFIRE_LIGHTNING_STRIKE = 1,
    /*  35 */ FHGFIRE_LIGHTNING_SHOCK = 35,
    /*  36 */ FHGFIRE_LIGHTNING_BURST,
    /*  38 */ FHGFIRE_SPEAR_LIGHT = 38,
    /*  39 */ FHGFIRE_WARP_EMERGE,
    /*  40 */ FHGFIRE_WARP_RETREAT,
    /*  41 */ FHGFIRE_WARP_DEATH,
    /*  50 */ FHGFIRE_ENERGY_BALL = 50,
    /* 100 */ FHGFIRE_LIGHTNING_TRAIL = 100
} FhgFireParam;

typedef enum TrailMode {
    /* 0 */ TRAIL_INIT,
    /* 1 */ TRAIL_APPEAR,
    /* 2 */ TRAIL_DISSIPATE
} TrailMode;

typedef enum FhgLightMode {
    /* 0 */ FHGFIRE_LIGHT_GREEN,
    /* 1 */ FHGFIRE_LIGHT_BLUE,
    /* 2 */ FHGFIRE_LIGHT_REFLECT
} FhgLightMode;

typedef enum FhgFireS16Var {
    /*  0 */ FHGFIRE_TIMER,
    /*  1 */ FHGFIRE_FX_TIMER,
    /*  2 */ FHGFIRE_US_2,
    /*  3 */ FHGFIRE_VARIANCE_TIMER,
    /*  4 */ FHGFIRE_FIRE_MODE,
    /*  5 */ FHGFIRE_RETURN_COUNT,
    /*  6 */ FHGFIRE_KILL_TIMER,
    /*  7 */ FHGFIRE_SHORT_COUNT
} FhgFireS16Var;

typedef enum FhgFireF32Var {
    /*  0 */ FHGFIRE_ALPHA,
    /*  1 */ FHGFIRE_UF_1,
    /*  2 */ FHGFIRE_UF_2,
    /*  3 */ FHGFIRE_SCALE,
    /*  4 */ FHGFIRE_UF_4,
    /*  5 */ FHGFIRE_WARP_TEX_1_X,
    /*  6 */ FHGFIRE_WARP_TEX_1_Y,
    /*  7 */ FHGFIRE_WARP_TEX_2_X,
    /*  8 */ FHGFIRE_WARP_TEX_2_Y,
    /*  9 */ FHGFIRE_WARP_TEX_SPEED,
    /* 10 */ FHGFIRE_WARP_ALPHA,
    /* 11 */ FHGFIRE_BURST_SCALE,
    /* 15 */ FHGFIRE_FLOAT_COUNT = 15
} FhgFireF32Var;

// 3DS layout (N64 +0x58 up to lightNode; LightInfo is 0x18 here, ColliderCylinder 0x58; tail partly unknown).
typedef struct EnFhgFire {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnFhgFireUpdateFunc updateFunc;
    /* 0x01A8 */ s16 work[FHGFIRE_SHORT_COUNT];
    /* 0x01B8 */ f32 fwork[FHGFIRE_FLOAT_COUNT];
    /* 0x01F4 */ void* lightNode;
    /* 0x01F8 */ char lightInfo[0x18]; // LightInfo
    /* 0x0210 */ ColliderCylinder collider;
    /* 0x0268 */ u8 lensFlareOn;
    /* 0x026A */ s16 lensFlareTimer;
    /* 0x026C */ f32 lensFlareScale;
    /* 0x0270 */ ZARInfo* zarInfo;
    /* 0x0274 */ void* models[4];
    /* 0x0284 */ u8 warpAnimMode;
    /* 0x0285 */ char unk_285[0x3];
} EnFhgFire; // size = 0x0288 (ROM offsets for collider/models verified by ws build)

extern ActorInit En_Fhg_Fire_InitVars;

#endif
