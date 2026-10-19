#ifndef _Z_BG_SPOT06_OBJECTS_H_
#define _Z_BG_SPOT06_OBJECTS_H_

#include "global.hpp"

struct BgSpot06Objects;

typedef void (*BgSpot06ObjectsActionFunc)(struct BgSpot06Objects* self, PlayState* play);

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ Vec3f center;
    /* 0x0C */ f32 radius;
} BgSpot06Sphere; // size = 0x10

typedef struct {
    /* 0x00 */ u8 unk_00[0x28];
    /* 0x28 */ BgSpot06Sphere modelSphere;
    /* 0x38 */ BgSpot06Sphere worldSphere;
    /* 0x48 */ u8 unk_48[0x08];
} BgSpot06ColliderDim; // size = 0x50

typedef struct {
    /* 0x00 */ BgSpot06ColliderDim dim;
} BgSpot06ColliderElement; // size = 0x50

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ BgSpot06ColliderElement* elements;
} BgSpot06ColliderJntSph; // size = 0x20

typedef struct BgSpot06Objects {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgSpot06ObjectsActionFunc actionFunc;
    /* 0x01C0 */ s16 switchFlag;
    /* 0x01C2 */ s16 timer;
    /* 0x01C4 */ f32 lakeHyliaWaterLevel;
    /* 0x01C8 */ BgSpot06ColliderJntSph collider;
    /* 0x01E8 */ char unk_1E8[0x50];
    /* 0x0238 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x023C */ SkeletonAnimationModel* skelAnimModel1;
    /* 0x0240 */ SkeletonAnimationModel* skelAnimModel2;
    /* 0x0244 */ f32 unk_244;
} BgSpot06Objects; // size = 0x0248

extern ActorInit Bg_Spot06_Objects_InitVars;

#endif
