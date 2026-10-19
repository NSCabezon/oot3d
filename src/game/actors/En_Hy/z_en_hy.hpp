#ifndef _Z_EN_HY_H_
#define _Z_EN_HY_H_

#include "global.hpp"

struct EnHy;

typedef void (*EnHyActionFunc)(struct EnHy*, PlayState*);

typedef struct {
    /* 0x00 */ s16 talkState;
    /* 0x02 */ s16 trackingMode;
    /* 0x04 */ s16 autoTurnTimer;
    /* 0x06 */ s16 autoTurnState;
    /* 0x08 */ Vec3s headRot;
    /* 0x0E */ Vec3s torsoRot;
    /* 0x14 */ f32 yOffset;
    /* 0x18 */ Vec3f trackPos;
    /* 0x24 */ char unk_24[0x4];
} EnHyInteractInfo; // size = 0x28

typedef struct EnHy {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char jointTable[0x05B0];
    /* 0x07D8 */ char morphTable[0x05B0];
    /* 0x0D88 */ EnHyActionFunc actionFunc;
    /* 0x0D8C */ char unk_D8C[0x1];
    /* 0x0D8D */ u8 pathReverse;
    /* 0x0D8E */ char unk_D8E[0x2];
    /* 0x0D90 */ s32 unk_D90;
    /* 0x0D94 */ char unk_D94[0x4];
    /* 0x0D98 */ ColliderCylinder collider;
    /* 0x0DF0 */ EnHyInteractInfo interactInfo;
    /* 0x0E18 */ Path* path;
    /* 0x0E1C */ s8 waypoint;
    /* 0x0E1D */ u8 playedSfx;
    /* 0x0E1E */ char unk_E1E[0x2];
    /* 0x0E20 */ s16 curEyeIndex;
    /* 0x0E22 */ s16 nextEyeIndexTimer;
    /* 0x0E24 */ char unk_E24[0x0E94 - 0x0E24];
    /* 0x0E94 */ f32 interactRange;
    /* 0x0E98 */ s32 getItemId;
    /* 0x0E9C */ Vec3f modelOffset;
    /* 0x0EA8 */ u16 talonEventChkInf;
    /* 0x0EAA */ char unk_EAA[0x2];
    /* 0x0EAC */ u32 unk_EAC; // passed by value to FUN_003508b8 in Destroy
    /* 0x0EB0 */ FaceAnimations faceAnim;
    /* 0x107C */ u8 unk_107C;
    /* 0x107D */ char unk_107D[0x1080 - 0x107D];
} EnHy; // size = 0x1080

extern ActorInit En_Hy_InitVars;

#endif
