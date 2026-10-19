#ifndef _Z_EN_ST_H_
#define _Z_EN_ST_H_

#include "global.hpp"

struct EnSt;

typedef void (*EnStActionFunc)(struct EnSt* self, struct PlayState* play);

typedef struct EnSt {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x478];
    /* 0x06A0 */ EnStActionFunc actionFunc;
    /* 0x06A4 */ ColliderCylinder colliderCylinders[6];
    /* 0x08B4 */ char colliderJntSph[0x70];
    /* 0x0924 */ s16 initialYaw;
    /* 0x0926 */ s16 deathYawTarget;
    /* 0x0928 */ s16 groundBounces;
    /* 0x092A */ s16 animFrames;
    /* 0x092C */ s16 swayTimer;
    /* 0x092E */ s16 setTargetYawTimer;
    /* 0x0930 */ s16 rotAwayTimer;
    /* 0x0932 */ s16 rotTowardsTimer;
    /* 0x0934 */ s16 takeDamageSpinTimer;
    /* 0x0936 */ s16 stunTimer;
    /* 0x0938 */ s16 invulnerableTimer;
    /* 0x093A */ s16 sfxTimer;
    /* 0x093C */ s16 gaveDamageSpinTimer;
    /* 0x093E */ s16 finishDeathTimer;
    /* 0x0940 */ s16 deathTimer;
    /* 0x0942 */ s16 absPrevSwayAngle;
    /* 0x0944 */ u8 playSwayFlag;
    /* 0x0945 */ u8 teethR;
    /* 0x0946 */ u8 teethG;
    /* 0x0947 */ u8 teethB;
    /* 0x0948 */ char unk_948[0x4];
    /* 0x094C */ Vec3f ceilingPos;
    /* 0x0958 */ Vec3f lineHitPos;
    /* 0x0964 */ char unk_964[0x8];
    /* 0x096C */ s32 blureIdx;
    /* 0x0970 */ f32 colliderScale;
    /* 0x0974 */ f32 floorHeightOffset;
    /* 0x0978 */ char unk_978[0x4];
    /* 0x097C */ f32 unk_97C;
    /* 0x0980 */ s32 unk_980;
} EnSt; // size = 0x984

extern ActorInit En_St_InitVars;

#endif
