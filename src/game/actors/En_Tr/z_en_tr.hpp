#ifndef _Z_EN_TR_H_
#define _Z_EN_TR_H_

#include "global.hpp"

struct EnTr;
typedef void (*EnTrActionFunc)(struct EnTr*, PlayState*);

typedef struct EnTr {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0xA90];
    /* 0x0CB8 */ char unk_CB8[0x148]; // FaceAnimations (accessed through a cast)
    /* 0x0E00 */ char unk_E00[0x84];
    /* 0x0E84 */ u16 unk_E84; // set to 0 in Init
    /* 0x0E86 */ u16 timer;
    /* 0x0E88 */ s16 cueChannel;
    /* 0x0E8A */ char unk_E8A[0x2];
    /* 0x0E8C */ EnTrActionFunc actionFunc;
    /* 0x0E90 */ s16 eyeIndex;
    /* 0x0E92 */ s16 blinkTimer;
    /* 0x0E94 */ s32 animIndex; // -1 = none
} EnTr; // size = 0xE98

extern ActorInit En_Tr_InitVars;

#endif
