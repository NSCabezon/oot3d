#ifndef _Z_EN_SKJ_H_
#define _Z_EN_SKJ_H_

#include "global.hpp"

struct EnSkj;

typedef void (*EnSkjActionFunc)(struct EnSkj*, PlayState*);

typedef struct EnSkjUnkStruct {
    u8 unk_0;
    struct EnSkj* skullkid;
} EnSkjUnkStruct;

typedef struct EnSkj {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x7D4]; // SkelAnime at +0x1A4 (SkelAnime_Free2)
    /* 0x0978 */ void* model0; // model slot loaded by FUN_00372f38 / freed by FUN_00350f34
    /* 0x097C */ char unk_97C[0x30];
    /* 0x09AC */ EnSkjActionFunc actionFunc;
    /* 0x09B0 */ char unk_9B0[0x50]; // ColliderCylinder at +0x9B0
    /* 0x0A00 */ char unk_A00[0x8];
    /* 0x0A08 */ u16 textId;
    /* 0x0A0A */ u16 timer;
    /* 0x0A0C */ u16 unk_A0C;
    /* 0x0A0E */ s16 unk_A0E;
    /* 0x0A10 */ s16 unk_A10;
    /* 0x0A12 */ s16 unk_A12;
    /* 0x0A14 */ u8 animIndex;
    /* 0x0A15 */ u8 action;
    /* 0x0A16 */ u8 unk_A16;
    /* 0x0A17 */ u8 unk_A17;
    /* 0x0A18 */ u8 unk_A18;
    /* 0x0A19 */ char unk_A19[0x1];
    /* 0x0A1A */ u8 unk_A1A;
    /* 0x0A1B */ u8 unk_A1B;
    /* 0x0A1C */ char unk_A1C[0x4];
    /* 0x0A20 */ s32 unk_A20;
    /* 0x0A24 */ Vec3f unk_A24;
    /* 0x0A30 */ f32 unk_A30;
    /* 0x0A34 */ f32 unk_A34;
    /* 0x0A38 */ Vec3f unk_A38;
    /* 0x0A44 */ char unk_A44[0x4];
} EnSkj; // size = 0xA48

typedef struct EnSkjAnimInfo {
    void* animation;
    u8 mode;
    f32 morphFrames;
} EnSkjAnimInfo;

extern ActorInit En_Skj_InitVars;

#endif
