#ifndef _Z_EN_GIRLA_H_
#define _Z_EN_GIRLA_H_

#include "global.hpp"

struct EnGirlA;

typedef void (*EnGirlAActionFunc)(struct EnGirlA*, PlayState*);

typedef struct EnGirlA {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnGirlAActionFunc actionFunc;
    /* 0x01A8 */ s8 requiredObjectSlot;
    /* 0x01A9 */ char unk_1A9[0x3];
    /* 0x01AC */ EnGirlAActionFunc actionFunc2;
    /* 0x01B0 */ char unk_1B0[0xC];
    /* 0x01BC */ s16 isInvisible;
    /* 0x01BE */ char unk_1BE[0xA];
    /* 0x01C8 */ s16 isSelected;
    /* 0x01CA */ char unk_1CA[0x2];
    /* 0x01CC */ s16 yRotation;
    /* 0x01CE */ char unk_1CE[0xE];
    /* 0x01DC */ s16 basePrice;
    /* 0x01DE */ s16 itemCount;
    /* 0x01E0 */ char unk_1E0[0x8];
    /* 0x01E8 */ struct SkeletonAnimationModel* skelAnimModel;
    /* 0x01EC */ struct SkeletonAnimationModel* skelAnimModel2;
    /* 0x01F0 */ f32 unk_1F0;
    /* 0x01F4 */ f32 unk_1F4;
    /* 0x01F8 */ f32 unk_1F8;
    /* 0x01FC */ f32 unk_1FC;
} EnGirlA; // size = 0x200

extern ActorInit En_GirlA_InitVars;

#endif
