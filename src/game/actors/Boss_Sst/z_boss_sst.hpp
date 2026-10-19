#ifndef _Z_BOSS_SST_H_
#define _Z_BOSS_SST_H_

#include "global.hpp"

struct BossSst;

typedef struct BossSst {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[4];
    /* 0x022C */ void (*actionFunc)(struct BossSst*, PlayState*);
    /* 0x0230 */ u8 unk_230;
    /* 0x0231 */ u8 ready;
    /* 0x0232 */ char unk_232[2];
    /* 0x0234 */ s16 timer;
    /* 0x0236 */ char unk_236[4];
    /* 0x023A */ s16 unk_23A;
    /* 0x023C */ s16 unk_23C;
    /* 0x023E */ char unk_23E[2];
    /* 0x0240 */ s16 targetYaw;
    /* 0x0242 */ char unk_242[0xCBA];
    /* 0x0EFC */ u8 unk_EFC;
    /* 0x0EFD */ u8 unk_EFD;
    /* 0x0EFE */ char unk_EFE[2];
    /* 0x0F00 */ u8 unk_F00;
    /* 0x0F01 */ char unk_F01[7];
    /* 0x0F08 */ void* unk_F08;
    /* 0x0F0C */ char unk_F0C[0x381];
    /* 0x128D */ u8 unk_128D;
    /* 0x128E */ char unk_128E[0x0E];
    /* 0x129C */ u32 unk_129C;
    /* 0x12A0 */ char unk_12A0[0x60];
    /* 0x1300 */ char unk_1300[0x7E4];
} BossSst; // size = 0x1AE4

typedef char BossSstSizeCheck[(sizeof(BossSst) == 0x1AE4) ? 1 : -1];

extern ActorInit Boss_Sst_InitVars;

#endif
