#ifndef _Z_EN_PART_H_
#define _Z_EN_PART_H_

#include "global.hpp"

struct EnPart;

// Virtual class of the object at 0x1C4 (vtable slot 2 = Destroy). _ZTI11EnPartModel is an EQU in syms.s.
extern "C++" {
struct EnPartModel {
    virtual void unk_vf0();
    virtual void unk_vf1();
    virtual void Destroy();
};
}

// 3DS keeps an array of 0x80-byte particles (EnPart+0x1C8, count at +0x1CC) when action == 0xFF.
typedef struct EnPartParticle {
    /* 0x00 */ MTX34 mtx;
    /* 0x30 */ Vec3f pos;
    /* 0x3C */ Vec3f home;
    /* 0x48 */ Vec3f prevPos;
    /* 0x54 */ Vec3s rot;
    /* 0x5A */ char unk_5A[0x2];
    /* 0x5C */ Vec3f velocity;
    /* 0x68 */ f32 gravity;
    /* 0x6C */ f32 speedXZ;
    /* 0x70 */ f32 rotZSpeed;
    /* 0x74 */ f32 rotZ;
    /* 0x78 */ s16 action;
    /* 0x7A */ s16 timer;
    /* 0x7C */ s16 type;
    /* 0x7E */ char unk_7E[0x2];
} EnPartParticle; // size = 0x80

typedef struct EnPart {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01A8 */ s32 unk_1A8;
    /* 0x01AC */ char unk_1AC[0x18];
    /* 0x01C4 */ EnPartModel* unk_1C4;
    /* 0x01C8 */ EnPartParticle* particles;
    /* 0x01CC */ s32 numParticles;
    /* 0x01D0 */ u8 unk_1D0;
    /* 0x01D1 */ u8 action;
    /* 0x01D2 */ s16 timer;
    /* 0x01D4 */ f32 rotZ;
    /* 0x01D8 */ f32 rotZSpeed;
    /* 0x01DC */ char unk_1DC[0x4];
    /* 0x01E0 */ MTX34 drawMtx;
    /* 0x0210 */ char unk_210[0xC];
} EnPart; // size = 0x21C

extern ActorInit En_Part_InitVars;

#endif
