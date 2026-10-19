#ifndef _Z_EN_ELF_H_
#define _Z_EN_ELF_H_

#include "global.hpp"

struct EnElf;

typedef void (*EnElfActionFunc)(struct EnElf*, struct PlayState*);

struct EnElfColor {
    EnElfColor(f32 r, f32 g, f32 b, f32 a) {
        v[0] = r;
        v[1] = g;
        v[2] = b;
        v[3] = a;
    }
    f32 v[4];
};

typedef struct {
    /* 0x000 */ char unk_00[0x3C];
    /* 0x03C */ Vec3f pos;
    /* 0x048 */ Vec3f scale;
    /* 0x054 */ char unk_54[0x9C];
    /* 0x0F0 */ f32 color[4];
    /* 0x100 */ char unk_100[0x70];
    /* 0x170 */ s32 visible;
} EnElfGlow;

typedef struct EnElf {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x6E8];
    /* 0x0910 */ void* unk_910[2];
    /* 0x0918 */ EnElfGlow* unk_918[2];
    /* 0x0920 */ f32 shadowAlpha;
    /* 0x0924 */ f32 intensity;
    /* 0x0928 */ f32 innerColor[4];
    /* 0x0938 */ f32 outerColor[4];
    /* 0x0948 */ char unk_948[0x18];
    /* 0x0960 */ void* lightNodeGlow;
    /* 0x0964 */ char unk_964[0x18];
    /* 0x097C */ void* lightNodeNoGlow;
    /* 0x0980 */ char unk_980[0x28];
    /* 0x09A8 */ s16 type;
    /* 0x09AA */ char unk_9AA[0x12];
    /* 0x09BC */ s16 unk_9BC;
    /* 0x09BE */ u16 timer;
    /* 0x09C0 */ s16 unk_9C0;
    /* 0x09C2 */ s16 disappearTimer;
    /* 0x09C4 */ u16 fairyFlags;
    /* 0x09C6 */ char unk_9C6[6];
    /* 0x09CC */ EnElfActionFunc actionFunc;
} EnElf; // size = 0x9D0

extern ActorInit En_Elf_InitVars;

#endif
