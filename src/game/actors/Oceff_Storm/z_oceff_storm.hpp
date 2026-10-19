#ifndef _Z_OCEFF_STORM_H_
#define _Z_OCEFF_STORM_H_

#include "global.hpp"

struct OceffStorm;

typedef void (*OceffStormActionFunc)(struct OceffStorm*, struct PlayState*);

// Model at +0x1B4: real virtual call, slot 1 (Destroy). _ZTI15OceffStormModel is an EQU in
// src/game/shared_syms.s (absolute placeholder) so armlink VFE knows the call class.
extern "C++" {
class OceffStormModel {
public:
    virtual void vf00();
    virtual void Destroy();
};
}

typedef struct OceffStorm {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ u16 counter;
    /* 0x01A6 */ u8 primColorAlpha;
    /* 0x01A7 */ u8 vtxAlpha;
    /* 0x01A8 */ f32 posYOff;
    /* 0x01AC */ f32 posYOffAdd;
    /* 0x01B0 */ OceffStormActionFunc actionFunc;
    /* 0x01B4 */ SkeletonAnimationModel* model;
    /* 0x01B8 */ void* unk_1B8;
    /* 0x01BC */ void* unk_1BC;
    /* 0x01C0 */ void* unk_1C0;
    /* 0x01C4 */ void* unk_1C4;
} OceffStorm; // size = 0x1C8

extern ActorInit Oceff_Storm_InitVars;

#endif
