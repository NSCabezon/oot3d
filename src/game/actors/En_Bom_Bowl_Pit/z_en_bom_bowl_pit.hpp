#ifndef _Z_EN_BOM_BOWL_PIT_H_
#define _Z_EN_BOM_BOWL_PIT_H_

#include "global.hpp"

struct EnBomBowlPit;

typedef void (*EnBomBowlPitActionFunc)(struct EnBomBowlPit*, PlayState*);

typedef struct EnBomBowlPit {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnBomBowlPitActionFunc actionFunc;
    /* 0x01A8 */ char unk_1A8[0x8];
    /* 0x01B0 */ s16 timer;
    /* 0x01B2 */ char unk_1B2[0x8A];
} EnBomBowlPit; // size = 0x23C

extern ActorInit En_Bom_Bowl_Pit_InitVars;

#endif
