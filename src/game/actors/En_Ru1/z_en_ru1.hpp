#ifndef _Z_EN_RU1_H_
#define _Z_EN_RU1_H_

#include "global.hpp"

struct EnRu1;

typedef void (*EnRu1DrawFunc)(struct EnRu1*, struct PlayState*);

typedef struct EnRu1BigOctoPlatform {
    /* 0x0000 */ char unk_00[0x21C];
    /* 0x021C */ s32 cameraSetting;
} EnRu1BigOctoPlatform;

typedef struct EnRu1BlueWarp {
    /* 0x0000 */ char unk_00[0x4CC];
    /* 0x04CC */ s32 rutoWarpState;
} EnRu1BlueWarp;

typedef struct EnRu1InteractInfo {
    /* 0x0000 */ char unk_00[0x8];
    /* 0x0008 */ Vec3s headRot;
    /* 0x000E */ Vec3s torsoRot;
    /* 0x0014 */ f32 yOffset;
    /* 0x0018 */ Vec3f trackPos;
} EnRu1InteractInfo; // size = 0x24

typedef struct EnRu1 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x1CC - 0x1A4]; // SkelAnime at 0x1A4
    /* 0x01CC */ void* unk_1CC;
    /* 0x01D0 */ char unk_1D0[0x219 - 0x1D0];
    /* 0x0219 */ u8 unk_219;
    /* 0x021A */ char unk_21A[0x9E0 - 0x21A];
    /* 0x09E0 */ ZARInfo* zarInfo; // = &ObjectStatus->zarInfo (EnRu1_Init)
    /* 0x09E4 */ char unk_9E4[0xBB0 - 0x9E4]; // FaceAnimations at 0x9E4
    /* 0x0BB0 */ u16 unk_BB0;
    /* 0x0BB2 */ s16 unk_BB2;
    /* 0x0BB4 */ s16 eyes;
    /* 0x0BB6 */ s16 blinkTimer;
    /* 0x0BB8 */ s16 mouth;
    /* 0x0BBA */ char unk_BBA[0xBBC - 0xBBA];
    /* 0x0BBC */ s32 action;
    /* 0x0BC0 */ s32 drawConfig;
    /* 0x0BC4 */ f32 unk_BC4;
    /* 0x0BC8 */ f32 unk_BC8;
    /* 0x0BCC */ char unk_BCC[0xBD0 - 0xBCC];
    /* 0x0BD0 */ EnRu1BlueWarp* blueWarp;
    /* 0x0BD4 */ f32 xzDistToPlayerInBlueWarp;
    /* 0x0BD8 */ char unk_BD8[0xBDC - 0xBD8];
    /* 0x0BDC */ s8 roomNum1;
    /* 0x0BDD */ s8 roomNum2;
    /* 0x0BDE */ s8 unk_BDE;
    /* 0x0BDF */ char unk_BDF[0xBE0 - 0xBDF];
    /* 0x0BE0 */ f32 unk_BE0;
    /* 0x0BE4 */ EnRu1BigOctoPlatform* bigOctoPlatform;
    /* 0x0BE8 */ s32 unk_BE8;
    /* 0x0BEC */ char unk_BEC[0xBFC - 0xBEC];
    /* 0x0BFC */ f32 sinkTimer;
    /* 0x0C00 */ u32 alpha;
    /* 0x0C04 */ char unk_C04[0xC0C - 0xC04];
    /* 0x0C0C */ ColliderCylinder standingCollider;
    /* 0x0C64 */ ColliderCylinder sittingCollider;
    /* 0x0CBC */ s32 isSittingOCActive;
    /* 0x0CC0 */ char unk_CC0[0xCE4 - 0xCC0];
    /* 0x0CE4 */ EnRu1InteractInfo interactInfo;
    /* 0x0D08 */ char unk_D08[0xD0C - 0xD08];
} EnRu1; // size = 0xD0C

extern ActorInit En_Ru1_InitVars;

#endif
