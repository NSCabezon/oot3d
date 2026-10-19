#ifndef _Z_EN_BOM_BOWL_MAN_H_
#define _Z_EN_BOM_BOWL_MAN_H_

#include "global.hpp"

struct EnBomBowlMan;

typedef void (*EnBomBowlManActionFunc)(struct EnBomBowlMan*, PlayState*);

// Offsets are 3DS (N64 + 0x424 after the SkelAnime block). Fields not yet named stay as pads.
typedef struct EnBomBowlMan {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x410];
    /* 0x0638 */ EnBomBowlManActionFunc actionFunc;
    /* 0x063C */ Vec3s headRot;
    /* 0x0642 */ char unk_642[0x6];
    /* 0x0648 */ Vec3s torsoRot;
    /* 0x064E */ s16 rewardAppearTimer;
    /* 0x0650 */ s16 unk_650; // unused
    /* 0x0652 */ s16 waitMessageState;
    /* 0x0654 */ s16 reward;
    /* 0x0656 */ s16 hasPlayedBefore;
    /* 0x0658 */ s16 eyeTexIndex; // Draw passes (eyeTexIndex >> 8) to FaceAnimations (semantics unverified)
    /* 0x065A */ s16 eyesTimer;
    /* 0x065C */ s16 eyesState;
    /* 0x065E */ s16 wakingUpTimer;
    /* 0x0660 */ s16 isAskingToPlayAgain;
    /* 0x0662 */ s16 wallsState[2]; // State of the first two walls
    /* 0x0666 */ s16 rewardState;
    /* 0x0668 */ s16 gameEndStatus;
    /* 0x066A */ char unk_66A[0x2];
    /* 0x066C */ Vec3f homePos;
    /* 0x0678 */ f32 curAnimFrameCount;
    /* 0x067C */ u8 gameStartStatus;
    /* 0x067D */ char unk_67D[0x3];
    /* 0x0680 */ Actor* finalTargetManager; // EnBomBowlPit; rewardState (u8) at its +0x1BC
    /* 0x0684 */ Actor* rewardItemOnCounter; // EnExItem; killItem (u16) at its +0x1D0
    /* 0x0688 */ void* unk_688; // freed in Destroy with FUN_003508b8
    /* 0x068C */ char faceAnimations[0x1CC]; // FaceAnimations (accessed through a cast)
} EnBomBowlMan; // size = 0x0858

extern ActorInit En_Bom_Bowl_Man_InitVars;

#endif
