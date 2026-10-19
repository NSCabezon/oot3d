#ifndef _Z_EN_ZF_H_
#define _Z_EN_ZF_H_

#include "global.hpp"

struct EnZf;

typedef void (*EnZfActionFunc)(struct EnZf*, PlayState*);

typedef struct EnZfSwordQuad {
    /* 0x00 */ char unk_00[0x10];
    /* 0x10 */ u8 atFlags;
    /* 0x11 */ char unk_11[0x6F];
} EnZfSwordQuad; // size = 0x80

typedef struct EnZf {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x820];
    /* 0x0A48 */ s32 action;
    /* 0x0A4C */ s32 unk_A4C; // unused
    /* 0x0A50 */ s32 actionState;
    /* 0x0A54 */ EnZfActionFunc actionFunc;
    /* 0x0A58 */ s16 headRot;
    /* 0x0A5A */ s16 headRotStep;
    /* 0x0A5C */ s32 timer;
    /* 0x0A60 */ s16 timer2;
    /* 0x0A62 */ s16 frozenTimer;
    /* 0x0A64 */ s16 noFloorAhead;
    /* 0x0A66 */ s16 isSwordSheathed;
    /* 0x0A68 */ s16 switchFlag;
    /* 0x0A6A */ s16 curPlatform;
    /* 0x0A6C */ s16 homePlatform;
    /* 0x0A6E */ s16 targetPlatform;
    /* 0x0A70 */ s16 alpha;
    /* 0x0A72 */ char pad_A72[2];
    /* 0x0A74 */ f32 dodgeSpeed;
    /* 0x0A78 */ f32 dodgeVelocityY;
    /* 0x0A7C */ u8 lastDamageReaction;
    /* 0x0A7D */ char pad_A7D[3];
    /* 0x0A80 */ s32 swordBlureEffectIndex;
    /* 0x0A84 */ ColliderCylinder bodyCollider;
    /* 0x0ADC */ EnZfSwordQuad swordCollider; // ColliderQuad
    /* 0x0B5C */ Vec3f rightFootPos;
    /* 0x0B68 */ Vec3f leftFootPos;
    /* 0x0B74 */ Vec3f bodyPartsPos[9];
    /* 0x0BE0 */ void* unk_BE0;
    /* 0x0BE4 */ void* unk_BE4;
} EnZf; // size = 0xBE8

extern ActorInit En_Zf_InitVars;

#endif
