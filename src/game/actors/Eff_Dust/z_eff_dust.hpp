#ifndef _Z_EFF_DUST_H_
#define _Z_EFF_DUST_H_

#include "global.hpp"

struct EffDust;

typedef void (*EffDustActionFunc)(struct EffDust* self, PlayState* play);
typedef void (*EffDustDrawFunc)(Actor* actor, PlayState* play);

typedef struct EffDust {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ f32 distanceTraveled[64];
    /* 0x02A4 */ Vec3f initialPositions[64];
    /* 0x05A4 */ u8 index;
    /* 0x05A5 */ u8 life;
    /* 0x05A8 */ f32 dx;
    /* 0x05AC */ f32 dy;
    /* 0x05B0 */ f32 dz;
    /* 0x05B4 */ f32 scalingFactor;
    /* 0x05B8 */ EffDustActionFunc actionFunc;
    /* 0x05BC */ EffDustDrawFunc drawFunc;
    /* 0x05C0 */ void* unk_5C0[2]; // [0] = FUN_00348f34 result, [1] = FUN_00340d00 result (set in Init)
} EffDust;

extern ActorInit Eff_Dust_InitVars;

#endif
