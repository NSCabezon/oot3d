#ifndef _Z_EN_WONDER_TALK_H_
#define _Z_EN_WONDER_TALK_H_

#include "global.hpp"

struct EnWonderTalk;

typedef void (*EnWonderTalkActionFunc)(struct EnWonderTalk*, PlayState*);

typedef struct EnWonderTalk {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnWonderTalkActionFunc actionFunc;
    /* 0x01A8 */ s16 unk_150;
    /* 0x01AA */ s16 unk_152;
    /* 0x01AC */ s16 switchFlag;
    /* 0x01AE */ s16 unk_156;
    /* 0x01B0 */ s16 unk_158;
    /* 0x01B2 */ s16 unk_15A;
    /* 0x01B4 */ f32 unk_15C;
    /* 0x01B8 */ f32 height;
    /* 0x01BC */ u8 unk_164;
} EnWonderTalk; // size = 0x01C0

extern ActorInit En_Wonder_Talk_InitVars;

#endif
