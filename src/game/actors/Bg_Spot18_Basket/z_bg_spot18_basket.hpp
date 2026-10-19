#ifndef _Z_BG_SPOT18_BASKET_H_
#define _Z_BG_SPOT18_BASKET_H_

#include "global.hpp"

struct BgSpot18Basket;

typedef void (*BgSpot18BasketActionFunc)(struct BgSpot18Basket* self, PlayState* play);

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ void* elements;
} BgSpot18BasketColliderJntSph; // size = 0x20

typedef struct {
    /* 0x00 */ u8 unk_00[0x50];
} BgSpot18BasketColliderElement; // size = 0x50

typedef struct BgSpot18Basket {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgSpot18BasketColliderJntSph colliderJntSph;
    /* 0x01DC */ BgSpot18BasketColliderElement colliderJntSphElements[2];
    /* 0x027C */ BgSpot18BasketActionFunc actionFunc;
    /* 0x0280 */ f32 circleRadius;
    /* 0x0284 */ s16 circleRate;
    /* 0x0286 */ s16 circleMoveAngle;
    /* 0x0288 */ s16 spinRate;
    /* 0x028A */ s16 pivotAzimuth;
    /* 0x028C */ s16 pivotAltitude;
    /* 0x028E */ s16 timer;
    /* 0x0290 */ s16 prize;
    /* 0x0292 */ u8 isHeartPieceGiven;
    /* 0x0293 */ u8 pad_0293;
    /* 0x0294 */ SkeletonAnimationModel* skelAnimModel;
} BgSpot18Basket; // size = 0x298

extern ActorInit Bg_Spot18_Basket_InitVars;

#endif
