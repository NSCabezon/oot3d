#ifndef _Z_EN_G_SWITCH_H_
#define _Z_EN_G_SWITCH_H_

#include "global.hpp"

struct EnGSwitch;

typedef struct EnGSwitchEffect {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ s16 scale;
    /* 0x0E */ s16 timer;
    /* 0x10 */ s16 colorIdx;
    /* 0x12 */ u8 flag;
    /* 0x14 */ Vec3f velocity;
    /* 0x20 */ Vec3f rot;
} EnGSwitchEffect; // size = 0x2C

typedef struct EnGSwitch {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ void (*actionFunc)(struct EnGSwitch*, PlayState*);
    /* 0x01A8 */ s16 type;
    /* 0x01AA */ s16 silverCount;
    /* 0x01AC */ s16 switchFlag;
    /* 0x01AE */ s16 killTimer;
    /* 0x01B0 */ s16 colorIdx;
    /* 0x01B2 */ s16 broken;
    /* 0x01B4 */ s16 numEffects;
    /* 0x01B6 */ s16 objectId;
    /* 0x01B8 */ s16 index;
    /* 0x01BA */ s16 delayTimer;
    /* 0x01BC */ s16 waitTimer;
    /* 0x01BE */ s16 moveMode;
    /* 0x01C0 */ s16 moveState;
    /* 0x01C2 */ s16 noteIndex;
    /* 0x01C4 */ Vec3f targetPos;
    /* 0x01D0 */ s8 requiredObjectSlot;
    /* 0x01D4 */ ColliderCylinder collider;
    /* 0x022C */ EnGSwitchEffect effects[40];
    /* 0x090C */ s32 modelIds[42];
    /* 0x09B4 */ SkeletonAnimationModel* models[42];
} EnGSwitch; // size = 0xA5C

extern ActorInit En_G_Switch_InitVars;

#endif
