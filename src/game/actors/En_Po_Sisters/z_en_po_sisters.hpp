#ifndef _Z_EN_PO_SISTERS_H_
#define _Z_EN_PO_SISTERS_H_

#include "global.hpp"

struct EnPoSisters;

typedef void (*EnPoSistersActionFunc)(struct EnPoSisters*, PlayState*);

typedef struct EnPoSisters {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x750];
    /* 0x0978 */ void* unk_978; // SkeletonAnimationModel-like, freed with FUN_00350f34
    /* 0x097C */ char unk_97C[8][12];
    /* 0x09DC */ EnPoSistersActionFunc actionFunc;
    /* 0x09E0 */ u8 unk_194;
    /* 0x09E1 */ u8 unk_195;
    /* 0x09E2 */ u8 unk_196;
    /* 0x09E3 */ u8 unk_197;
    /* 0x09E4 */ u8 unk_198;
    /* 0x09E5 */ u8 unk_199;
    /* 0x09E6 */ s16 unk_19A;
    /* 0x09E8 */ s16 unk_19C;
    /* 0x09EA */ char unk_9EA[0x3];
    /* 0x09ED */ u8 unk_9ED; // unk_22E.a on N64 (color alpha)
    /* 0x09EE */ char unk_9EE[2];
    /* 0x09F0 */ Vec3f unk_9F0[8];
    /* 0x0A50 */ f32 unk_A50;
    /* 0x0A54 */ void* lightNode; // LightNode*
    /* 0x0A58 */ char lightInfo[0x18];
    /* 0x0A70 */ ColliderCylinder collider;
    /* 0x0AC8 */ char unk_AC8[0x30];
} EnPoSisters; // size = 0x0AF8

extern ActorInit En_Po_Sisters_InitVars;

f32* EnPoSisters_CurFramePtr(SkelAnime* skel);
f32* EnPoSisters_EndFramePtr(SkelAnime* skel);

#endif
