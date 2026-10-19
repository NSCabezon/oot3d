#ifndef _Z_BG_BDAN_SWITCH_H_
#define _Z_BG_BDAN_SWITCH_H_

#include "global.hpp"

struct BgBdanSwitch;

typedef enum BgBdanSwitchType {
    /* 0x00 */ BLUE,
    /* 0x01 */ YELLOW_HEAVY,
    /* 0x02 */ YELLOW,
    /* 0x03 */ YELLOW_TALL_1,
    /* 0x04 */ YELLOW_TALL_2
} BgBdanSwitchType;

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ Vec3f center;
    /* 0x0C */ f32 radius;
} BgBdanSwitchSphere; // size = 0x10

typedef struct {
    /* 0x00 */ u8 unk_00[0x28];
    /* 0x28 */ BgBdanSwitchSphere modelSphere;
    /* 0x38 */ BgBdanSwitchSphere worldSphere;
    /* 0x48 */ u8 unk_48[0x08];
} BgBdanSwitchColliderElement; // size = 0x50

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ BgBdanSwitchColliderElement* elements;
} BgBdanSwitchColliderJntSph; // size = 0x20

typedef struct {
    /* 0x00 */ ColliderInit base;
    /* 0x08 */ s32 count;
    /* 0x0C */ void* elements;
} BgBdanSwitchJntSphInit;

typedef void (*BgBdanSwitchActionFunc)(struct BgBdanSwitch*, PlayState*);

typedef struct BgBdanSwitch {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgBdanSwitchActionFunc actionFunc;
    /* 0x01C0 */ BgBdanSwitchColliderJntSph collider;
    /* 0x01E0 */ BgBdanSwitchColliderElement colliderElements[1];
    /* 0x0230 */ f32 unk_1C8;
    /* 0x0234 */ s16 unk_1CC;
    /* 0x0236 */ char unk_236[0x2];
    /* 0x0238 */ f32 unk_1D0;
    /* 0x023C */ f32 unk_1D4;
    /* 0x0240 */ s16 unk_1D8;
    /* 0x0242 */ s16 unk_1DA;
    /* 0x0244 */ u8 unk_1DC;
    /* 0x0245 */ char unk_245[0x3];
    /* 0x0248 */ SkeletonAnimationModel* skelAnimModel[2];
} BgBdanSwitch; // size = 0x250

extern ActorInit Bg_Bdan_Switch_InitVars;

#endif
