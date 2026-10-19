#ifndef _Z_EN_DH_H_
#define _Z_EN_DH_H_

#include "global.hpp"

struct EnDh;

typedef void (*EnDhActionFunc)(struct EnDh*, PlayState*);

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ void* elements;
} EnDhColliderJntSph; // size = 0x20

typedef struct EnDh {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x618];
    /* 0x0840 */ u8 actionState;
    /* 0x0841 */ u8 retreat;
    /* 0x0842 */ u8 drawDirtWave;
    /* 0x0843 */ u8 unk_843;
    /* 0x0844 */ EnDhActionFunc actionFunc;
    /* 0x0848 */ u8 unk_258;
    /* 0x0849 */ char unk_849[0x1];
    /* 0x084A */ u8 alpha;
    /* 0x084B */ u8 curAction;
    /* 0x084C */ s16 timer;
    /* 0x084E */ s16 dirtWavePhase;
    /* 0x0850 */ ColliderCylinder collider1;
    /* 0x08A8 */ EnDhColliderJntSph colliderJntSph;
    /* 0x08C8 */ char colliderJntSphElements[0x50];
    /* 0x0918 */ Vec3f headPos;
    /* 0x0924 */ f32 dirtWaveSpread;
    /* 0x0928 */ f32 dirtWaveHeight;
    /* 0x092C */ f32 dirtWaveAlpha;
    /* 0x0930 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x0934 */ SkeletonAnimationModel_unk_0C* unk_934;
} EnDh; // size = 0x938

extern ActorInit En_Dh_InitVars;

#endif
