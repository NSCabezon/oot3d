#ifndef _Z_BG_ICE_SHELTER_H_
#define _Z_BG_ICE_SHELTER_H_

#include "global.hpp"

struct BgIceShelter;

typedef void (*BgIceShelterActionFunc)(struct BgIceShelter*, PlayState*);

typedef struct BgIceShelter {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgIceShelterActionFunc actionFunc;
    /* 0x01C0 */ ColliderCylinder cylinder1; // Used to detect blue fire and also as OC for no-dynapoly types
    /* 0x0218 */ ColliderCylinder cylinder2; // Only used by no-dynapoly types to make weapons bounce off
    /* 0x0270 */ s16 alpha;
    /* 0x0272 */ char unk_272[0x2];
    /* 0x0274 */ SkeletonAnimationModel* skelAnimModel[3]; // [0] large/small/king zora, [1] platform, [2] wall
} BgIceShelter; // size = 0x280

extern ActorInit Bg_Ice_Shelter_InitVars;

#endif
