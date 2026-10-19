#ifndef _Z_EN_WOOD02_H_
#define _Z_EN_WOOD02_H_

#include "global.hpp"

struct EnWood02;

extern "C++" {
// Model object at +0x20C / +0x210. Virtual Destroy = slot 1. _ZTI14EnWood02Model is an EQU in shared_syms.s (VFE key).
struct EnWood02Model {
    virtual void unk_vf0();
    virtual void Destroy();
};
}

typedef struct EnWood02 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ s16 unk_14C;
    /* 0x01A6 */ u8 unk_14E[5];
    /* 0x01AB */ u8 spawnType;
    /* 0x01AC */ u8 drawType;
    /* 0x01AD */ char unk_1AD[0x3];
    /* 0x01B0 */ ColliderCylinder collider;
    /* 0x0208 */ u32 unk_208;
    /* 0x020C */ EnWood02Model* model;
    /* 0x0210 */ EnWood02Model* model2;
} EnWood02; // size = 0x214

extern ActorInit En_Wood02_InitVars;

#endif
