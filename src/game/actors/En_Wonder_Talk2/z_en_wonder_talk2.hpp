#ifndef _Z_EN_WONDER_TALK2_H_
#define _Z_EN_WONDER_TALK2_H_

#include "global.hpp"

struct EnWonderTalk2;

typedef void (*EnWonderTalk2ActionFunc)(struct EnWonderTalk2*, PlayState*);

typedef struct EnWonderTalk2 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnWonderTalk2ActionFunc actionFunc;
    /* 0x01A8 */ s16 baseMsgId; // specified message type
    /* 0x01AA */ s16 switchFlag;
    /* 0x01AC */ s16 talkMode;
    /* 0x01AE */ s16 unk_156;
    /* 0x01B0 */ s16 unk_158;
    /* 0x01B2 */ u8 unk_15A;
    /* 0x01B3 */ u8 unk_15B;
    /* 0x01B4 */ f32 triggerRange;
    /* 0x01B8 */ f32 height;
    /* 0x01BC */ Vec3f initPos;
    /* 0x01C8 */ u8 unk_1C8;
    /* 0x01C9 */ u8 unk_1C9;
} EnWonderTalk2; // size = 0x01CC

extern ActorInit En_Wonder_Talk2_InitVars;

#endif
