#ifndef _Z_EN_SIOFUKI_H_
#define _Z_EN_SIOFUKI_H_

#include "global.hpp"

typedef enum EnSiofukiType {
    /* 0x00 */ EN_SIOFUKI_RAISING,
    /* 0x01 */ EN_SIOFUKI_LOWERING
} EnSiofukiType;

struct EnSiofuki;

typedef void (*EnSiofukiActionFunc)(struct EnSiofuki*, PlayState*);

typedef struct EnSiofuki {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ EnSiofukiActionFunc actionFunc;
    /* 0x01C0 */ s32 timer;
    /* 0x01C4 */ f32 initPosY;
    /* 0x01C8 */ f32 unk_170;
    /* 0x01CC */ f32 unk_174;
    /* 0x01D0 */ f32 oscillation;
    /* 0x01D4 */ f32 targetHeight;
    /* 0x01D8 */ f32 currentHeight;
    /* 0x01DC */ s32 splashTimer;
    /* 0x01E0 */ s32 applySpeed;
    /* 0x01E4 */ f32 appliedSpeed;
    /* 0x01E8 */ f32 targetAppliedSpeed;
    /* 0x01EC */ s16 appliedYaw;
    /* 0x01EE */ s16 activeTime;
    /* 0x01F0 */ f32 maxHeight;
    /* 0x01F4 */ u8 sfxFlags;
    /* 0x01F8 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01FC */ s32 unk_1FC;
    /* 0x0200 */ Vec3f unk_200; // per-axis scale applied to the model matrix in Draw
} EnSiofuki; // size = 0x020C

extern ActorInit En_Siofuki_InitVars;

#endif
