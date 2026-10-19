#ifndef _Z_EN_NIW_LADY_H_
#define _Z_EN_NIW_LADY_H_

#include "global.hpp"

struct EnNiwLady;

typedef void (*EnNiwLadyActionFunc)(struct EnNiwLady*, PlayState*);

typedef struct EnNiwLady {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char jointTable[0x30C];
    /* 0x0534 */ char morphTable[0x30C];
    /* 0x0840 */ EnNiwLadyActionFunc actionFunc;
    /* 0x0844 */ Vec3s headRot;
    /* 0x084A */ Vec3s torsoRot;
    /* 0x0850 */ s16 unusedTimer;
    /* 0x0852 */ s16 unk_262; // "message_end_code"
    /* 0x0854 */ s16 unusedTimer2;
    /* 0x0856 */ s16 unusedRandomTimer;
    /* 0x0858 */ s16 cuccosInPen;
    /* 0x085A */ s16 unk_26A;
    /* 0x085C */ s16 unk_26C;
    /* 0x085E */ s16 unk_26E;
    /* 0x0860 */ s16 unk_270;
    /* 0x0862 */ u8 unk_272;
    /* 0x0863 */ u8 unk_273;
    /* 0x0864 */ u8 unk_274;
    /* 0x0865 */ u8 unk_275;
    /* 0x0866 */ u8 unk_276;
    /* 0x0867 */ u8 unk_277;
    /* 0x0868 */ s16 unk_278;
    /* 0x086A */ s16 unk_27A;
    /* 0x086C */ s16 faceState;
    /* 0x086E */ s16 unk_27E;
    /* 0x0870 */ s8 aneObjectSlot;
    /* 0x0871 */ s8 osAnimeObjectSlot;
    /* 0x0874 */ s32 getItemId;
    /* 0x0878 */ char interactInfo[0x28];
    /* 0x08A0 */ ColliderCylinder collider;
    /* 0x08F8 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x08FC */ char faceAnims[0x1CC]; // FaceAnimations
} EnNiwLady; // size = 0xAC8

extern ActorInit En_Niw_Lady_InitVars;

#endif
