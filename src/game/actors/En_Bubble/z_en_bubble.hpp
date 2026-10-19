#ifndef _Z_EN_BUBBLE_H_
#define _Z_EN_BUBBLE_H_

#include "global.hpp"

struct EnBubble;

typedef void (*EnBubbleActionFunc)(struct EnBubble*, PlayState*);

typedef struct {
    /* 0x00 */ Vec3f modelCenter;
    /* 0x0C */ f32 modelRadius;
    /* 0x10 */ Vec3f worldCenter;
    /* 0x1C */ f32 worldRadius;
    /* 0x20 */ f32 unk_20;
    /* 0x24 */ f32 unk_24;
} EnBubbleDim; // size = 0x28

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ u32 dmgFlags;
    /* 0x04 */ u8 hitSpecialEffect;
    /* 0x05 */ u8 damage;
    /* 0x06 */ u8 unk_06[0x0F];
    /* 0x15 */ u8 atElemFlags;
    /* 0x16 */ u8 acElemFlags;
    /* 0x17 */ u8 unk_17[0x11];
    /* 0x28 */ EnBubbleDim dim;
} EnBubbleColliderElement; // size = 0x50

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ EnBubbleColliderElement* elements;
} EnBubbleColliderJntSph; // size = 0x20

typedef struct EnBubble {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnBubbleActionFunc actionFunc;
    /* 0x01A8 */ EnBubbleColliderJntSph colliderJntSph;
    /* 0x01C8 */ EnBubbleColliderElement colliderJntSphElements[2];
    /* 0x0268 */ Vec3f unk_268; // set but never used (N64 unk_1F0)
    /* 0x0274 */ Vec3f unk_274; // randomly generated, set but never used (N64 unk_1FC)
    /* 0x0280 */ s16 unk_280; // set to 12 when about to pop (3DS timer)
    /* 0x0282 */ s16 explosionCountdown;
    /* 0x0284 */ char unk_284[0x4];
    /* 0x0288 */ f32 graphicRotSpeed;
    /* 0x028C */ f32 graphicEccentricity;
    /* 0x0290 */ f32 unk_290; // set to 1.0f, never used
    /* 0x0294 */ f32 unk_294; // set to 1.0f, never used
    /* 0x0298 */ f32 expansionWidth;
    /* 0x029C */ f32 expansionHeight;
    /* 0x02A0 */ Vec3f bounceDirection;
    /* 0x02AC */ Vec3f velocityFromBounce;
    /* 0x02B8 */ Vec3f normalizedAttackerVelocity;
    /* 0x02C4 */ Vec3f velocityFromAttack;
    /* 0x02D0 */ f32 sinkSpeed;
    /* 0x02D4 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x02D8 */ char unk_2D8[0x4];
    /* 0x02DC */ u32 frameCount;
} EnBubble; // size = 0x2E0

extern ActorInit En_Bubble_InitVars;

#endif
