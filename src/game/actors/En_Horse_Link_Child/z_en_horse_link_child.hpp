#ifndef _Z_EN_HORSE_LINK_CHILD_H_
#define _Z_EN_HORSE_LINK_CHILD_H_

#include "global.hpp"

struct EnHorseLinkChild;

typedef struct {
    /* 0x00 */ char unk_00[0x10];
    /* 0x10 */ SkelAnime skelAnime;
} EnHorseLinkChildSkin; // size = 0x94

typedef struct {
    /* 0x00 */ char unk_00[0x28];
    /* 0x28 */ Vec3f modelCenter;
    /* 0x34 */ f32 modelRadius;
    /* 0x38 */ Vec3f worldCenter;
    /* 0x44 */ f32 worldRadius;
    /* 0x48 */ f32 scale;
    /* 0x4C */ u8 limb;
    /* 0x4D */ char unk_4D[0x3];
} EnHorseLinkChildJntSphDim; // size = 0x50

typedef struct {
    /* 0x00 */ char unk_00[0x18];
    /* 0x18 */ s32 count;
    /* 0x1C */ EnHorseLinkChildJntSphDim* elements;
} EnHorseLinkChildJntSph; // size = 0x20

typedef struct EnHorseLinkChild {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ u8 action;
    /* 0x01A5 */ u8 animationIdx;
    /* 0x01A6 */ char unk_1A6[0x2];
    /* 0x01A8 */ EnHorseLinkChildSkin skin;
    /* 0x023C */ char unk_23C[0xC00 - 0x23C];
    /* 0x0C00 */ char unk_C00[0xDCC - 0xC00]; // FaceAnimations (accessed through a cast)
    /* 0x0DCC */ s32 timer;
    /* 0x0DD0 */ s32 unk_1E8;
    /* 0x0DD4 */ u8 eyeTexIndex;
    /* 0x0DD5 */ char unk_DD5[0xDD8 - 0xDD5];
    /* 0x0DD8 */ s32 unk_1F0;
    /* 0x0DDC */ ColliderCylinder bodyCollider;
    /* 0x0E34 */ EnHorseLinkChildJntSph headCollider;
    /* 0x0E54 */ char unk_E54[0xEA4 - 0xE54];
    /* 0x0EA4 */ s32 unk_2A0;
} EnHorseLinkChild; // size = 0xEA8

extern ActorInit En_Horse_Link_Child_InitVars;

#endif
