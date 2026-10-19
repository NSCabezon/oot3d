#ifndef _Z_EN_DIVING_GAME_H_
#define _Z_EN_DIVING_GAME_H_

#include "global.hpp"

struct EnDivingGame;

typedef void (*EnDivingGameActionFunc)(struct EnDivingGame* self, PlayState* play);

typedef struct EnDivingGameInteractInfo {
    /* 0x00 */ char unk_00[0x08];
    /* 0x08 */ Vec3s headRot;
    /* 0x0E */ Vec3s torsoRot;
    /* 0x14 */ char unk_14[0x04];
    /* 0x18 */ Vec3f trackPos;
    /* 0x24 */ char unk_24[0x04];
} EnDivingGameInteractInfo; // size = 0x28

typedef struct EnDivingGame {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char jointTable[0x444];
    /* 0x066C */ char morphTable[0x444];
    /* 0x0AB0 */ char faceAnimations[0x150];
    /* 0x0C00 */ char unk_C00[0x7C];
    /* 0x0C7C */ EnDivingGameActionFunc actionFunc;
    /* 0x0C80 */ Vec3s headRot;
    /* 0x0C86 */ Vec3s torsoRot;
    /* 0x0C8C */ u16 unk_C8C;
    /* 0x0C8E */ u16 nextTextState;
    /* 0x0C90 */ s16 subCamTimer;
    /* 0x0C92 */ s16 throwTimer;
    /* 0x0C94 */ s16 eyeTimer;
    /* 0x0C96 */ s16 spawnRuppyTimer;
    /* 0x0C98 */ s16 phase;
    /* 0x0C9A */ s16 eyeTexIndex;
    /* 0x0C9C */ char unk_C9C[0x08];
    /* 0x0CA4 */ s16 state;
    /* 0x0CA6 */ char unk_CA6[0x73];
    /* 0x0D19 */ u8 notPlayingMinigame;
    /* 0x0D1A */ char unk_D1A[0x01];
    /* 0x0D1B */ u8 isDuplicate;
    /* 0x0D1C */ char unk_D1C[0x04];
    /* 0x0D20 */ EnDivingGameInteractInfo interactInfo;
    /* 0x0D48 */ ColliderCylinder collider;
} EnDivingGame; // size = 0xDA0

extern ActorInit En_Diving_Game_InitVars;

#endif
