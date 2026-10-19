#ifndef _Z_EN_TG_H_
#define _Z_EN_TG_H_

#include "global.hpp"

struct EnTg;

typedef void (*EnTgActionFunc)(struct EnTg*, PlayState*);

typedef struct EnTgInteractInfo {
    /* 0x00 */ s16 talkState;
    /* 0x02 */ char unk_02[0x26];
} EnTgInteractInfo; // size = 0x28

typedef struct EnTg {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ Vec3s jointTable[16];
    /* 0x0288 */ char unk_288[0x278];
    /* 0x0500 */ Vec3s morphTable[16];
    /* 0x0560 */ char unk_560[0x278];
    /* 0x07D8 */ EnTgActionFunc actionFunc;
    /* 0x07DC */ ColliderCylinder collider;
    /* 0x0834 */ EnTgInteractInfo interactInfo;
    /* 0x085C */ u8 nextDialogue;
    /* 0x0860 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x0864 */ u8 unk_864;
    /* 0x0865 */ char unk_865[0x3];
} EnTg; // size = 0x868

extern ActorInit En_Tg_InitVars;

#endif
