#ifndef _Z_EFC_ERUPC_H_
#define _Z_EFC_ERUPC_H_

#include "global.hpp"

struct EfcErupc;

// Virtual object held in the 0x1F68 / 0x1F70 slots (virtual Destroy = slot 1).
struct EfcErupcFx;

typedef struct EfcErupcEffect {
    /* 0x00 */ char unk_00[4];
    /* 0x04 */ f32 scale;
    /* 0x08 */ char unk_08[8];
    /* 0x10 */ Vec3f pos;
    /* 0x1C */ Vec3f vel;
    /* 0x28 */ Vec3f accel;
    /* 0x34 */ u8 isActive;
    /* 0x35 */ u8 animTimer;
    /* 0x36 */ s16 alpha;
    /* 0x38 */ char unk_38[4];
    /* 0x3C */ u8 colorR;
    /* 0x3D */ u8 colorG;
    /* 0x3E */ u8 colorB;
    /* 0x3F */ char unk_3F[0xD];
} EfcErupcEffect; // size = 0x4C

typedef struct EfcErupc {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ s16 unk_1A4;
    /* 0x01A6 */ s16 unk_1A6;
    /* 0x01A8 */ s16 unk_1A8;
    /* 0x01AA */ s16 unk_1AA;
    /* 0x01AC */ s16 unk_1AC;
    /* 0x01AE */ s16 unk_1AE;
    /* 0x01B0 */ char unk_1B0[4];
    /* 0x01B4 */ EfcErupcEffect effects[100];
    /* 0x1F64 */ void (*actionFunc)(struct EfcErupc* self, PlayState* play);
    /* 0x1F68 */ struct EfcErupcFx* unk_1F68[2];
    /* 0x1F70 */ struct EfcErupcFx* unk_1F70[100];
} EfcErupc; // size = 0x2100

extern ActorInit Efc_Erupc_InitVars;

#endif
