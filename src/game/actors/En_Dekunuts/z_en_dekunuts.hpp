#ifndef _Z_EN_DEKUNUTS_H_
#define _Z_EN_DEKUNUTS_H_

#include "global.hpp"

struct EnDekunuts;

typedef void (*EnDekunutsActionFunc)(struct EnDekunuts*, struct PlayState*);

// Model virtual class (vtable slot 1 = Destroy). _ZTI15EnDekunutsModel is an EQU in syms.s (name length must match the class: "EnDekunutsModel" = 15).
extern "C++" {
struct EnDekunutsModel {
    virtual void unk_vf0();
    virtual void Destroy();
};
}

typedef struct EnDekunuts {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnDekunutsActionFunc actionFunc;
    /* 0x01A8 */ u8 playWalkSfx;
    /* 0x01A9 */ u8 runAwayCount;
    /* 0x01AA */ s16 animFlagAndTimer;
    /* 0x01AC */ s16 runDirection;
    /* 0x01AE */ s16 shotsPerRound;
    /* 0x01B0 */ ColliderCylinder collider;
    /* 0x0208 */ SkelAnime skelAnime;
    /* 0x028C */ Vec3s jointTable[25];
    /* 0x0322 */ char unk_322[0x172];
    /* 0x0494 */ Vec3s morphTable[25];
    /* 0x052A */ char unk_52A[0x172];
    /* 0x069C */ SkeletonAnimationModel* model; // flower (params == 10) only
} EnDekunuts; // size = 0x6A0

// Animation indices from the object data block at 0x00522000 (data/z_En_Dekunuts.data.s); named by EQU in syms.s.
typedef struct {
    /* 0x00 */ s32 unk_00[6];
    /* 0x18 */ s32 animUp;
    /* 0x1C */ s32 animStand;
    /* 0x20 */ s32 animIdx20;
    /* 0x24 */ s32 animIdx24;
} EnDekunutsAnimData;

extern EnDekunutsAnimData sEnDekunutsAnimData;

extern ActorInit En_Dekunuts_InitVars;

#endif
