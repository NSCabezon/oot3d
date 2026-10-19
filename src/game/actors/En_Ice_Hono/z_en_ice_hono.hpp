#ifndef _Z_EN_ICE_HONO_H_
#define _Z_EN_ICE_HONO_H_

#include "global.hpp"

struct EnIceHono;

typedef void (*EnIceHonoActionFunc)(struct EnIceHono*, PlayState*);

// Object at +0x228: real virtual call, slot 1 (Destroy). _ZTI14EnIceHonoModel is an EQU in
// src/game/shared_syms.s (absolute placeholder) so armlink VFE knows the call class.
extern "C++" {
class EnIceHonoModel {
public:
    virtual void vf00();
    virtual void Destroy();
};
}

typedef struct EnIceHono {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnIceHonoActionFunc actionFunc;
    /* 0x01A8 */ s16 alpha;
    /* 0x01AA */ s16 timer;
    /* 0x01AC */ s16 unk_154;
    /* 0x01AE */ s16 unk_156;
    /* 0x01B0 */ f32 smallFlameTargetYScale;
    /* 0x01B4 */ ColliderCylinder collider;
    /* 0x020C */ void* lightNode; // LightNode*
    /* 0x0210 */ char lightInfo[0x18]; // LightInfo, 3DS layout (local, as En_Light)
    /* 0x0228 */ EnIceHonoModel* model;
} EnIceHono; // size = 0x022C

extern ActorInit En_Ice_Hono_InitVars;

#endif
