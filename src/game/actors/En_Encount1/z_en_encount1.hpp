#ifndef _Z_EN_ENCOUNT1_H_
#define _Z_EN_ENCOUNT1_H_

#include "global.hpp"

struct EnEncount1;

typedef void (*EnEncount1UpdateFunc)(struct EnEncount1*, PlayState*);

typedef struct EnEncount1 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnEncount1UpdateFunc updateFunc;
    /* 0x01A8 */ s16 maxCurSpawns;
    /* 0x01AA */ s16 curNumSpawn;
    /* 0x01AC */ s16 spawnType;
    /* 0x01AE */ s16 maxTotalSpawns;
    /* 0x01B0 */ s16 totalNumSpawn;
    /* 0x01B2 */ s16 outOfRangeTimer;
    /* 0x01B4 */ s16 fieldSpawnTimer;
    /* 0x01B6 */ s16 killCount;
    /* 0x01B8 */ s16 numLeeverSpawns;
    /* 0x01BA */ s16 leeverIndex;
    /* 0x01BC */ s16 timer;
    /* 0x01BE */ u8 reduceLeevers;
    /* 0x01C0 */ f32 spawnRange;
    /* 0x01C4 */ Actor* bigLeever; // N64 bigLeever (set by FUN_001cc77c)
    /* 0x01C8 */ Actor* mqActor; // actor 0xA7 spawned in Init (MQ only), killed in Destroy
} EnEncount1; // size = 0x1CC

extern ActorInit En_Encount1_InitVars;

#endif
