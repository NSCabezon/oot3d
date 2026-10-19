#ifndef _Z_EN_GOMA_H_
#define _Z_EN_GOMA_H_

#include "global.hpp"

struct EnGoma;

// Model at +0x818: real virtual call, slot 1 (Destroy). _ZTI11EnGomaModel is an EQU in shared_syms.s.
extern "C++" {
struct EnGomaModel {
    virtual void unk_vf0();
    virtual void Destroy();
};
}

typedef void (*EnGomaActionFunc)(struct EnGoma* self, PlayState* play);

// Object-gol static data at 0x00525E58 (data_syms.s EQU, image from data/z_En_Goma.data.s).
typedef struct EnGomaData {
    /* 0x00 */ u8 spawnNum; // sSpawnNum (egg counter)
    /* 0x01 */ char unk_01[0x3];
    /* 0x04 */ void* anim[8]; // anim[0] = stand, anim[1] = running, anim[4] = jump, anim[7] = death
} EnGomaData;

typedef struct EnGoma {
    /* 0x000 */ Actor actor;
    /* 0x1A4 */ SkelAnime skelanime;
    /* 0x228 */ char unk_228[0x4E0];
    /* 0x708 */ EnGomaActionFunc actionFunc;
    /* 0x70C */ s16 slopePitch;
    /* 0x70E */ s16 slopeRoll;
    /* 0x710 */ s16 gomaType;
    /* 0x712 */ s16 eyePitch;
    /* 0x714 */ s16 eyeYaw;
    /* 0x716 */ s16 hatchState;
    /* 0x718 */ s16 eggTimer;
    /* 0x71A */ s16 hurtTimer;
    /* 0x71C */ s16 visualState;
    /* 0x71E */ s16 playerDetectionTimer;
    /* 0x720 */ s16 spawnNum;
    /* 0x722 */ s16 invincibilityTimer;
    /* 0x724 */ s16 actionTimer;
    /* 0x728 */ f32 eggScale;
    /* 0x72C */ f32 eggPitch;
    /* 0x730 */ f32 eggSquishAngle;
    /* 0x734 */ f32 eggSquishAccel;
    /* 0x738 */ f32 eyeEnvColor[3];
    /* 0x744 */ f32 eggSquishAmount;
    /* 0x748 */ f32 eggYOffset;
    /* 0x74C */ s32 unk_74C;
    /* 0x750 */ s16 stunTimer;
    /* 0x754 */ Vec3f shieldKnockbackVel;
    /* 0x760 */ void* bossLimbDL; // set by z_boss_goma
    /* 0x764 */ ColliderCylinder colliderCylinder1;
    /* 0x7BC */ ColliderCylinder colliderCylinder2;
    /* 0x814 */ char unk_814[0x4];
    /* 0x818 */ EnGomaModel* model;
} EnGoma; // size = 0x81C

extern ActorInit En_Goma_InitVars;

#endif
