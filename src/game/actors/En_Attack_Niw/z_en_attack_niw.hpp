#ifndef _Z_EN_ATTACK_NIW_H_
#define _Z_EN_ATTACK_NIW_H_

#include "global.hpp"

struct EnAttackNiw;
struct PlayState;

typedef void (*EnAttackNiwActionFunc)(struct EnAttackNiw*, struct PlayState*);

typedef struct EnAttackNiw {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x84];
    /* 0x0228 */ Vec3s jointTable[16];
    /* 0x0288 */ char unk_288_pad[0x174];
    /* 0x03FC */ Vec3s morphTable[16];
    /* 0x045C */ char unk_45C[0x174];
    /* 0x05D0 */ EnAttackNiwActionFunc actionFunc;
    /* 0x05D4 */ s16 unk_254;
    /* 0x05D6 */ s16 unk_256;
    /* 0x05D8 */ s16 unk_258_;
    /* 0x05DA */ s16 unk_25A;
    /* 0x05DC */ s16 unk_25C;
    /* 0x05DE */ s16 unk_25E;
    /* 0x05E0 */ s16 unk_260;
    /* 0x05E2 */ s16 unk_262;
    /* 0x05E4 */ f32 unk_264;
    /* 0x05E8 */ f32 unk_268;
    /* 0x05EC */ f32 unk_26C;
    /* 0x05F0 */ char unk_270[0x8];
    /* 0x05F8 */ f32 unk_278;
    /* 0x05FC */ f32 unk_27C;
    /* 0x0600 */ f32 unk_280;
    /* 0x0604 */ f32 unk_284;
    /* 0x0608 */ f32 unk_288;
    /* 0x060C */ s16 unk_28C;
    /* 0x060E */ s16 unk_28E;
    /* 0x0610 */ char unk_290[0x2];
    /* 0x0612 */ s16 unk_292;
    /* 0x0614 */ char unk_294[0x2];
    /* 0x0616 */ s16 unk_296;
    /* 0x0618 */ Vec3f unk_298;
    /* 0x0624 */ Vec3f unk_2A4;
    /* 0x0630 */ Vec3f unk_2B0;
    /* 0x063C */ f32 unk_2BC;
    /* 0x0640 */ f32 unk_2C0;
    /* 0x0644 */ char unk_2C4[0xC];
    /* 0x0650 */ f32 unk_2D0;
    /* 0x0654 */ f32 unk_2D4;
    /* 0x0658 */ char unk_2D8[0x4];
    /* 0x065C */ f32 unk_2DC;
    /* 0x0660 */ f32 unk_2E0;
    /* 0x0664 */ f32 unk_2E4;
} EnAttackNiw; // size = 0x668

extern ActorInit En_Attack_Niw_InitVars;

#endif
