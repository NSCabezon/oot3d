#ifndef _Z_EN_BOM_CHU_H_
#define _Z_EN_BOM_CHU_H_

#include "global.hpp"

struct EnBomChu;

typedef void (*EnBomChuActionFunc)(struct EnBomChu*, PlayState*);

// JntSph collider types are not in the repo headers yet (3DS layout, local for now; same as Bg_Bdan_Switch).
typedef struct {
    /* 0x00 */ Vec3f center;
    /* 0x0C */ f32 radius;
} EnBomChuSphere; // size = 0x10

typedef struct {
    /* 0x00 */ u8 unk_00[0x28];
    /* 0x28 */ EnBomChuSphere modelSphere;
    /* 0x38 */ EnBomChuSphere worldSphere;
    /* 0x48 */ u8 unk_48[0x08];
} EnBomChuColliderElement; // size = 0x50

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ EnBomChuColliderElement* elements;
} EnBomChuColliderJntSph; // size = 0x20

typedef struct {
    /* 0x00 */ ColliderInit base;
    /* 0x08 */ s32 count;
    /* 0x0C */ void* elements;
} EnBomChuJntSphInit;

typedef struct EnBomChu {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnBomChuActionFunc actionFunc;
    /* 0x01A8 */ s16 timer;
    /* 0x01AA */ u8 unk_1AA;
    /* 0x01AB */ u8 unk_1AB;
    /* 0x01AC */ u8 unk_1AC;
    /* 0x01AD */ char unk_1AD[0x3];
    /* 0x01B0 */ Vec3f axisForwards;
    /* 0x01BC */ Vec3f axisUp;
    /* 0x01C8 */ Vec3f axisLeft;
    /* 0x01D4 */ f32 visualJitter;
    /* 0x01D8 */ s32 blure1Index;
    /* 0x01DC */ s32 blure2Index;
    /* 0x01E0 */ EnBomChuColliderJntSph collider;
    /* 0x0200 */ EnBomChuColliderElement colliderElements[1];
    /* 0x0250 */ SkeletonAnimationModel* model;
} EnBomChu; // size = 0x0254

extern ActorInit En_Bom_Chu_InitVars;

#endif
