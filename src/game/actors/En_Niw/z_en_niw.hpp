#ifndef _Z_EN_NIW_H_
#define _Z_EN_NIW_H_

#include "global.hpp"

struct EnNiw;

typedef struct EnNiw {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x4A0];
    /* 0x0644 */ f32 unk_644;
    /* 0x0648 */ f32 unk_648;
    /* 0x064C */ f32 unk_64C;
    /* 0x0650 */ f32 unk_650;
    /* 0x0654 */ f32 unk_654;
    /* 0x0658 */ f32 unk_658;
    /* 0x065C */ f32 unk_65C;
    /* 0x0660 */ f32 unk_660;
    /* 0x0664 */ char unk_664[0x534];
} EnNiw; // size = 0xB98

extern ActorInit En_Niw_InitVars;

#endif
