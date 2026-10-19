#ifndef _Z_EN_FW_H_
#define _Z_EN_FW_H_

#include "global.hpp"

struct EnFw;

typedef void (*EnFwActionFunc)(struct EnFw* self, PlayState* play);

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ void* elements;
} EnFwColliderJntSph; // size = 0x20

typedef struct EnFwEffect {
    /* 0x0000 */ u8 type;
    /* 0x0001 */ u8 timer;
    /* 0x0002 */ u8 initialTimer;
    /* 0x0004 */ f32 scale;
    /* 0x0008 */ f32 scaleStep;
    /* 0x000C */ char unk_0C[4];
    /* 0x0010 */ char unk_10[4];
    /* 0x0014 */ Vec3f pos;
    /* 0x0020 */ Vec3f velocity;
    /* 0x002C */ Vec3f accel;
} EnFwEffect; // size = 0x38

#define EN_FW_EFFECT_COUNT 20

typedef struct EnFw {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ Vec3s jointTable[11];
    /* 0x026A */ char unk_26A[0x1C6];
    /* 0x0430 */ Vec3s morphTable[11];
    /* 0x0472 */ char unk_472[0x1C6];
    /* 0x0638 */ EnFwActionFunc actionFunc;
    /* 0x063C */ EnFwColliderJntSph collider;
    /* 0x065C */ char unk_65C[0x50];
    /* 0x06AC */ Vec3f bompPos;
    /* 0x06B8 */ u8 lastDmgHook;
    /* 0x06B9 */ char unk_6B9[0x1];
    /* 0x06BA */ s16 runDirection;
    /* 0x06BC */ s16 bounceCnt;
    /* 0x06BE */ char unk_6BE[0x2];
    /* 0x06C0 */ s16 damageTimer;
    /* 0x06C2 */ s16 explosionTimer;
    /* 0x06C4 */ s16 slideTimer;
    /* 0x06C6 */ s16 slideSfxTimer;
    /* 0x06C8 */ s16 returnToParentTimer;
    /* 0x06CA */ s16 turnAround;
    /* 0x06CC */ f32 runRadius;
    /* 0x06D0 */ EnFwEffect effects[EN_FW_EFFECT_COUNT];
    /* 0x0B30 */ void* unk_B30;
} EnFw; // size = 0xB34

extern ActorInit En_Fw_InitVars;

#endif
