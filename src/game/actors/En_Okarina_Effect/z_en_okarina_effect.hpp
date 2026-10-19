#ifndef _Z_EN_OKARINA_EFFECT_H_
#define _Z_EN_OKARINA_EFFECT_H_

#include "global.hpp"

struct EnOkarinaEffect;

typedef void (*EnOkarinaEffectActionFunc)(struct EnOkarinaEffect*, struct PlayState*);

typedef struct EnOkarinaEffect {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ u16 timer;
    /* 0x01A6 */ char unk_1A6[0x2];
    /* 0x01A8 */ EnOkarinaEffectActionFunc actionFunc;
} EnOkarinaEffect; // size = 0x1AC

extern ActorInit En_Okarina_Effect_InitVars;

#endif
