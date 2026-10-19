#ifndef _Z_EN_DEKUBABA_H_
#define _Z_EN_DEKUBABA_H_

#include "global.hpp"

struct EnDekubaba;

typedef void (*EnDekubabaActionFunc)(struct EnDekubaba*, PlayState*);

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ Vec3f center;
    /* 0x0C */ f32 radius;
} EnDekubabaSphere; // size = 0x10

typedef struct {
    /* 0x00 */ ColliderInfo base;
    /* 0x28 */ EnDekubabaSphere modelSphere;
    /* 0x38 */ EnDekubabaSphere worldSphere;
    /* 0x48 */ u8 unk_48[0x08];
} EnDekubabaColliderElement; // size = 0x50

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ EnDekubabaColliderElement* elements;
} EnDekubabaColliderJntSph; // size = 0x20

typedef struct EnDekubaba {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ Vec3f unk_1A4[2]; // unused
    /* 0x01BC */ Vec3f unk_1BC;    // unused
    /* 0x01C8 */ Vec3f homePos;    // unused
    /* 0x01D4 */ SkelAnime skelAnime;
    /* 0x0258 */ EnDekubabaActionFunc actionFunc;
    /* 0x025C */ char pad_25C[2];
    /* 0x025E */ s16 actionState;
    /* 0x0260 */ s16 wobbleTarget;
    /* 0x0262 */ s16 stemPartsRot[3];
    /* 0x0268 */ Vec3s jointTable[26];
    /* 0x0304 */ Vec3s morphTable[26];
    /* 0x03A0 */ f32 scaleFac;
    /* 0x03A4 */ CollisionPoly* floorPoly;
    /* 0x03A8 */ EnDekubabaColliderJntSph collider;
    /* 0x03C8 */ EnDekubabaColliderElement colliderElements[7];
    /* 0x05F8 */ SkeletonAnimationModel* skelAnimModel[5];
} EnDekubaba; // size = 0x60C

extern ActorInit En_Dekubaba_InitVars;

#endif
