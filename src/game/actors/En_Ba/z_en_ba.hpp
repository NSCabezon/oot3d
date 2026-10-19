#ifndef _Z_EN_BA_H_
#define _Z_EN_BA_H_

#include "global.hpp"

struct EnBa;

typedef void (*EnBaActionFunc)(struct EnBa*, PlayState*);


// Tail fields at 0xB74 (names kept from the first pass; N64 unk_318..unk_31E)
typedef struct {
    /* 0x00 */ s16 unk_74;
    /* 0x02 */ s16 unk_76;
    /* 0x04 */ s16 unk_78;
    /* 0x06 */ s16 unk_7A; // frame counter
} EnBaSub; // size = 0x8

typedef struct EnBa {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x7D8 - 0x228];
    /* 0x07D8 */ SkeletonAnimationModel_unk_0C* unk_7D8;
    /* 0x07DC */ FaceAnimations faceAnim; // size = 0x1CC
    /* 0x09A8 */ s32 unk_9A8; // state (N64 unk_14C)
    /* 0x09AC */ EnBaActionFunc actionFunc;
    /* 0x09B0 */ s16 upperParams;
    /* 0x09B2 */ s16 unk_9B2;
    /* 0x09B4 */ Vec3f unk_158[14];
    /* 0x0A5C */ Vec3f unk_200[14];
    /* 0x0B04 */ Vec3s unk_2A8[14];
    /* 0x0B58 */ char unk_B58[0x1C];
    /* 0x0B74 */ EnBaSub sub;
    /* 0x0B7C */ u8 unk_B7C[0xC00 - 0xB7C]; // ColliderJntSph
    /* 0x0C00 */ char unk_C00[0x3C];
    /* 0x0C3C */ SkeletonAnimationModel* skelAnimModel;
    /* 0x0C40 */ s32 camId;
    /* 0x0C44 */ Actor* unk_C44;
    /* 0x0C48 */ u8 unk_C48;
    /* 0x0C49 */ s8 unk_C49;
    /* 0x0C4A */ char unk_C4A[0x2];
} EnBa; // size = 0xC4C

extern ActorInit En_Ba_InitVars;

#endif
