#ifndef _Z_EN_BROB_H_
#define _Z_EN_BROB_H_

#include "global.hpp"

struct EnBrob;

typedef struct EnBrob {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ SkelAnime skelAnime;
    /* 0x0240 */ f32* unk_240;
    /* 0x0244 */ char unk_244[0x270];
    /* 0x04B4 */ void (*actionFunc)(struct EnBrob*, PlayState*);
    /* 0x04B8 */ s16 timer;
    /* 0x04BA */ s16 modelOffsetY;
    /* 0x04BC */ char unk_4BC[0x4];
    /* 0x04C0 */ ColliderCylinder colliders[2];
} EnBrob; // size = 0x570

extern ActorInit En_Brob_InitVars;

#endif
