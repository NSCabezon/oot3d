#ifndef _Z_EN_MM_H_
#define _Z_EN_MM_H_

#include "global.hpp"

struct EnMm;

typedef void (*EnMmActionFunc)(struct EnMm*, PlayState*);

typedef struct EnMm {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x840 - 0x228];
    /* 0x0840 */ EnMmActionFunc actionFunc;
    /* 0x0844 */ ColliderCylinder collider;
    /* 0x089C */ s32 unk_89C;
    /* 0x08A0 */ s32 unk_8A0;
    /* 0x08A4 */ s32 unk_8A4;
    /* 0x08A8 */ s32 unk_8A8;
    /* 0x08AC */ s32 unk_8AC;
    /* 0x08B0 */ char unk_8B0[0x8BC - 0x8B0];
    /* 0x08BC */ f32 unk_8BC;
    /* 0x08C0 */ s32 unk_8C0;
    /* 0x08C4 */ Vec3s unk_8C4;
    /* 0x08CA */ Vec3s unk_8CA;
    /* 0x08D0 */ u32 flags;
    /* 0x08D4 */ s32 unk_8D4;
    /* 0x08D8 */ s32 unk_8D8;
    /* 0x08DC */ void* model;
    /* 0x08E0 */ char unk_8E0[0xAAC - 0x8E0];
    /* 0x0AAC */ void* unk_AAC;
    /* 0x0AB0 */ MTX34 unk_AB0;
} EnMm; // size = 0xAE0

extern ActorInit En_Mm_InitVars;

#endif
