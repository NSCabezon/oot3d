#ifndef _Z_EN_TORYO_H_
#define _Z_EN_TORYO_H_

#include "global.hpp"

struct EnToryo;

typedef void (*EnToryoActionFunc)(struct EnToryo*, PlayState*);

typedef struct {
    /* 0x00 */ char unk_00[0x08];
    /* 0x08 */ Vec3s headRot;
    /* 0x0E */ Vec3s torsoRot;
    /* 0x14 */ char unk_14[0x04];
    /* 0x18 */ Vec3f trackPos;
    /* 0x24 */ char unk_24[0x04];
} EnToryoInteractInfo; // size = 0x28

typedef struct EnToryo {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ Vec3s jointTable[17];
    /* 0x028E */ char unk_28E[0x3DE];
    /* 0x066C */ Vec3s morphTable[17];
    /* 0x06D2 */ char unk_6D2[0x3DE];
    /* 0x0AB0 */ EnToryoActionFunc actionFunc;
    /* 0x0AB4 */ ColliderCylinder collider;
    /* 0x0B0C */ char unk_B0C[0x08];
    /* 0x0B14 */ u16 stateFlags;
    /* 0x0B16 */ char unk_B16[0x02];
    /* 0x0B18 */ EnToryoInteractInfo interactInfo;
    /* 0x0B40 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x0B44 */ SkeletonAnimationModel* skelAnimModel2;
    /* 0x0B48 */ SkeletonAnimationModel* skelAnimModel3;
    /* 0x0B4C */ char unk_B4C[0xA4];
} EnToryo;

extern ActorInit En_Toryo_InitVars;

#endif
