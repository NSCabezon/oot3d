#ifndef _Z_OBJ_COMB_H_
#define _Z_OBJ_COMB_H_

#include "global.hpp"

struct ObjComb;

typedef void (*ObjCombActionFunc)(struct ObjComb*, struct PlayState*);

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ u8 unk_00[0x34];
    /* 0x34 */ f32 modelSphereRadius;
    /* 0x38 */ Vec3f worldSphereCenter;
    /* 0x44 */ f32 worldSphereRadius;
    /* 0x48 */ u8 unk_48[0x8];
} ObjCombColliderElement; // size = 0x50

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ ObjCombColliderElement* elements;
} ObjCombColliderJntSph; // size = 0x20

typedef struct ObjComb {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ObjCombActionFunc actionFunc;
    /* 0x01A8 */ ObjCombColliderJntSph collider;
    /* 0x01C8 */ ObjCombColliderElement colliderElements[1];
    /* 0x0218 */ s16 unk_218;
    /* 0x021A */ s16 unk_21A;
    /* 0x021C */ SkeletonAnimationModel* skelAnimModel;
} ObjComb; // size = 0x220

extern ActorInit Obj_Comb_InitVars;

#endif
