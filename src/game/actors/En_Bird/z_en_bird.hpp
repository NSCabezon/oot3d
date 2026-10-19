#ifndef _Z_EN_BIRD_H_
#define _Z_EN_BIRD_H_

#include "global.hpp"

struct EnBird;

typedef void (*EnBirdActionFunc)(struct EnBird*, PlayState*);

typedef struct EnBird {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnBirdActionFunc actionFunc;
    /* 0x01A8 */ s32 unk_1A8;
    /* 0x01AC */ s32 timer;
    /* 0x01B0 */ u16 unk_1B0;
    /* 0x01B4 */ f32 posYMag;
    /* 0x01B8 */ f32 rotYMag;
    /* 0x01BC */ f32 speedTarget;
    /* 0x01C0 */ f32 speedStep;
    /* 0x01C4 */ f32 flightDistance;
    /* 0x01C8 */ f32 posYPhase;
    /* 0x01CC */ f32 posYPhaseStep;
    /* 0x01D0 */ f32 unk_1D0;
    /* 0x01D4 */ s16 rotYStep;
    /* 0x01D6 */ char unk_1D6[0x1A];
} EnBird; // size = 0x1F0

extern ActorInit En_Bird_InitVars;

#endif
