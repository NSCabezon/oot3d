#ifndef _Z_EN_HORSE_NORMAL_H_
#define _Z_EN_HORSE_NORMAL_H_
#include "global.hpp"
#include <stddef.h>
typedef struct { char unk_00[0x10]; SkelAnime skelAnime; } EnHorseNormalSkin;
typedef struct { char unk_00[0x28]; Vec3f modelCenter; f32 modelRadius; Vec3f worldCenter; f32 worldRadius; f32 scale; u8 limb; char pad_4D[3]; } EnHorseNormalJntSphDim;
typedef struct { Collider base; s32 count; EnHorseNormalJntSphDim* elements; } EnHorseNormalJntSph;
typedef struct EnHorseNormal {
    Actor actor;
    u8 action; u8 animationIdx; char pad_1A6[2];
    EnHorseNormalSkin skin;
    char pad_23C[4];
    char jointTable[0x4E0];
    char morphTable[0x4E0];
    EnHorseNormalSkin cloneSkin;
    char pad_C94[4];
    char cloneJointTable[0x4E0];
    SkeletonAnimationModel* shadowModel;
    MTX34 cloneMtx;
    s16 cloneRotY; u16 flags;
    Vec3f clonePos; Vec3f cloneSoundPos;
    s32 stepIndex; Vec3f soundPos; char pad_11D8[8];
    f32 accel; s16 yawDelta; s16 yawVel; f32 ground; char pad_11EC[4];
    ColliderCylinder bodyCollider;
    EnHorseNormalJntSph headCollider;
    EnHorseNormalJntSphDim headElements[1];
    ColliderCylinder cloneCollider;
    char pad_1310[4]; s32 waypoint;
} EnHorseNormal;
typedef char EnHorseNormal_SizeCheck[sizeof(EnHorseNormal)==0x1318?1:-1];
typedef char EnHorseNormal_SkinCheck[offsetof(EnHorseNormal,skin)==0x1A8?1:-1];
typedef char EnHorseNormal_JointCheck[offsetof(EnHorseNormal,jointTable)==0x240?1:-1];
typedef char EnHorseNormal_CloneCheck[offsetof(EnHorseNormal,cloneSkin)==0xC00?1:-1];
typedef char EnHorseNormal_ShadowCheck[offsetof(EnHorseNormal,shadowModel)==0x1178?1:-1];
typedef char EnHorseNormal_FlagsCheck[offsetof(EnHorseNormal,flags)==0x11AE?1:-1];
typedef char EnHorseNormal_ColliderCheck[offsetof(EnHorseNormal,bodyCollider)==0x11F0?1:-1];
typedef char EnHorseNormal_HeadCheck[offsetof(EnHorseNormal,headCollider)==0x1248?1:-1];
typedef char EnHorseNormal_WaypointCheck[offsetof(EnHorseNormal,waypoint)==0x1314?1:-1];
extern ActorInit En_Horse_Normal_InitVars;
#endif
