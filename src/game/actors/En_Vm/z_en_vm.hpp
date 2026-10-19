#ifndef _Z_EN_VM_H_
#define _Z_EN_VM_H_

#include "global.hpp"

struct EnVm;

typedef struct EnVmQuad {
    /* 0x00 */ char unk_00[0x10];
    /* 0x10 */ u8 atFlags;
    /* 0x11 */ u8 acFlags;
    /* 0x12 */ char unk_12[0x2E];
    /* 0x40 */ Vec3f quad[4];
    /* 0x70 */ char unk_70[0x10];
} EnVmQuad; // size = 0x80

typedef void (*EnVmActionFunc)(struct EnVm*, PlayState*);

typedef struct EnVm {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char jointTable[0x138];
    /* 0x0360 */ char morphTable[0x138];
    /* 0x0498 */ EnVmActionFunc actionFunc;
    /* 0x049C */ f32 beamSightRange;
    /* 0x04A0 */ s32 unk_21C;
    /* 0x04A4 */ s32 timer;
    /* 0x04A8 */ Vec3f beamPos1;
    /* 0x04B4 */ Vec3f beamPos2;
    /* 0x04C0 */ Vec3f beamPos3;
    /* 0x04CC */ Vec3f beamScale;
    /* 0x04D8 */ Vec3s beamRot;
    /* 0x04DE */ u16 beamTexScroll;
    /* 0x04E0 */ s16 headRotY;
    /* 0x04E2 */ s16 unk_25E;
    /* 0x04E4 */ s16 unk_260;
    /* 0x04E6 */ char unk_4E6[0x2];
    /* 0x04E8 */ f32 beamSpeed;
    /* 0x04EC */ s8 unk_4EC;
    /* 0x04ED */ char unk_4ED[0x3];
    /* 0x04F0 */ ColliderCylinder colliderCylinder;
    /* 0x0548 */ EnVmQuad colliderQuad1;
    /* 0x05C8 */ EnVmQuad colliderQuad2;
    /* 0x0648 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x064C */ SkeletonAnimationModel_unk_0C animModel;
} EnVm; // size = 0x6E4

extern ActorInit En_Vm_InitVars;

#endif
