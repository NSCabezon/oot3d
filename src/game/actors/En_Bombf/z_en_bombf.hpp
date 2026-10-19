#ifndef _Z_EN_BOMBF_H_
#define _Z_EN_BOMBF_H_

#include "global.hpp"

struct EnBombf;

typedef void (*EnBombfActionFunc)(struct EnBombf*, PlayState*);

typedef struct {
    /* 0x00 */ u8 unk_00[0x28];
    /* 0x28 */ Vec3f modelSphereCenter;
    /* 0x34 */ f32 modelSphereRadius;
    /* 0x38 */ Vec3f worldSphereCenter;
    /* 0x44 */ f32 worldSphereRadius;
    /* 0x48 */ u8 unk_48[0x08];
} EnBombfColliderElement; // size = 0x50

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ EnBombfColliderElement* elements;
} EnBombfColliderJntSph; // size = 0x20

typedef struct EnBombf {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ColliderCylinder bombCollider;
    /* 0x01FC */ EnBombfColliderJntSph explosionCollider;
    /* 0x021C */ EnBombfColliderElement explosionColliderElements[1];
    /* 0x026C */ s16 timer;
    /* 0x0270 */ EnBombfActionFunc actionFunc;
    /* 0x0274 */ s32 isFuseEnabled; // enables the ability to ignite and tick down to explode
    /* 0x0278 */ u8 colliderSetOC;
    /* 0x027A */ s16 flashSpeedScale;
    /* 0x027C */ f32 flashIntensity;
    /* 0x0280 */ f32 flowerBombScale;
    /* 0x0284 */ SkeletonAnimationModel* skelAnimModel[3];
} EnBombf; // size = 0x290

typedef enum EnBombfType {
    /* 0xFFFF */ BOMBFLOWER_FLOWER = -1,
    /* 0x0000 */ BOMBFLOWER_BODY,
    /* 0x0001 */ BOMBFLOWER_EXPLOSION
} EnBombfType;

extern ActorInit En_Bombf_InitVars;

#endif
