#ifndef _Z_EN_HINTNUTS_H_
#define _Z_EN_HINTNUTS_H_

#include "global.hpp"

struct EnHintnuts;

typedef void (*EnHintnutsActionFunc)(struct EnHintnuts*, struct PlayState*);

typedef struct EnHintnuts {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnHintnutsActionFunc actionFunc;
    /* 0x01A8 */ s16 animFlagAndTimer; // 0x1000 bit denotes that projectile has been thrown
    /* 0x01AA */ s16 unk_196;
    /* 0x01AC */ u16 textIdCopy;
    /* 0x01AE */ char unk_1AE[0x2];
    /* 0x01B0 */ ColliderCylinder collider;
    /* 0x0208 */ char jointTable[0x1D4];
    /* 0x03DC */ char morphTable[0x1D4];
    /* 0x05B0 */ SkelAnime skelAnime;
    /* 0x0634 */ SkeletonAnimationModel* model;
} EnHintnuts; // size = 0x0638

typedef struct EnHintnutsData {
    /* 0x00 */ s16 puzzleCounter;
    /* 0x02 */ char unk_02[0x2];
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 burrowAnim;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 spitAnim;
    /* 0x14 */ s32 upAnim;
    /* 0x18 */ s32 standAnim;
    /* 0x1C */ s32 runAnim;
    /* 0x20 */ s32 freezeAnim;
    /* 0x24 */ s32 talkAnim;
} EnHintnutsData;

typedef struct EnHintnutsAllData {
    /* 0x00 */ EnHintnutsData data;
    /* 0x28 */ CollisionCheckInfoInit colChkInfoInit;
    /* 0x30 */ ColliderCylinderInit cylinderInit;
} EnHintnutsAllData;

extern EnHintnutsData EnHintnuts_sData;
extern CollisionCheckInfoInit EnHintnuts_sColChkInfoInit;
extern ActorInit En_Hintnuts_InitVars;

void FUN_001e8b34(EnHintnuts* self);
void FUN_00258bb0(EnHintnuts* self);
f32* EnHintnuts_CurFramePtr(SkelAnime* skelAnime);
f32* EnHintnuts_PlaySpeedPtr(SkelAnime* skelAnime);

#endif
