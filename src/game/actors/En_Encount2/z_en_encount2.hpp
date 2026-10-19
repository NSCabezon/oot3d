#ifndef _Z_EN_ENCOUNT2_H_
#define _Z_EN_ENCOUNT2_H_

#include "global.hpp"

struct EnEncount2;

typedef void (*EnEncount2ActionFunc)(struct EnEncount2*, PlayState*);

typedef struct EnEncount2 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnEncount2ActionFunc actionFunc;
    /* 0x01A8 */ char unk_1A8[0x04];
    /* 0x01AC */ u16 deathMountainSpawnerTimer;
    /* 0x01AE */ char unk_1AE[0x02];
    /* 0x01B0 */ u16 numSpawnedRocks;
    /* 0x01B2 */ s16 isNotDeathMountain;
    /* 0x01B4 */ u16 collapseSpawnerInactive;
    /* 0x01B6 */ char unk_1B6[0x0A];
    /* 0x01C0 */ char unk_1C0[0x10];
    /* 0x01D0 */ s16 envEffectsTimer;
    /* 0x01D2 */ char unk_1D2[0x06];
    /* 0x01D8 */ u64 isQuaking;
    /* 0x01E0 */ char unk_1E0[0x708 - 0x1E0];
    /* 0x0708 */ SkeletonAnimationModel* models[30];
} EnEncount2; // size = 0x780

extern ActorInit En_Encount2_InitVars;

#endif
