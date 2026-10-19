#ifndef _Z_EN_SHOPNUTS_H_
#define _Z_EN_SHOPNUTS_H_

#include "global.hpp"

struct EnShopnuts;

#define SHOPNUTS_GET_TYPE(thisx) ((thisx)->params)

typedef void (*EnShopnutsActionFunc)(struct EnShopnuts*, PlayState*);

// 3DS layout. Offsets confirmed from ROM: actionFunc 0x1A4, animFlagAndTimer 0x1A8,
// collider 0x1AC (ColliderCylinder, 0x58), skelAnime 0x204 (SkelAnime, 0x84),
// jointTable 0x288, morphTable 0x594 (0x30C each). ROM collider.base.acFlags = 0x1BD,
// collider.dim.radius/height = 0x1EC/0x1F0.
typedef struct EnShopnuts {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnShopnutsActionFunc actionFunc;
    /* 0x01A8 */ s16 animFlagAndTimer; // 0x1000 bit denotes that projectile has been thrown
    /* 0x01AA */ char unk_1AA[0x2];
    /* 0x01AC */ ColliderCylinder collider;
    /* 0x0204 */ SkelAnime skelAnime;
    /* 0x0288 */ Vec3s jointTable[130];
    /* 0x0594 */ Vec3s morphTable[130];
} EnShopnuts; // size = 0x8A0

extern ActorInit En_Shopnuts_InitVars;

#endif
