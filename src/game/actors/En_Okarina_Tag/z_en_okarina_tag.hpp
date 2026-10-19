#ifndef _Z_EN_OKARINA_TAG_H_
#define _Z_EN_OKARINA_TAG_H_

#include "global.hpp"

struct EnOkarinaTag;

typedef void (*EnOkarinaTagActionFunc)(struct EnOkarinaTag*, PlayState*);

typedef struct EnOkarinaTag {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnOkarinaTagActionFunc actionFunc;
    /* 0x01A8 */ s16 type;
    /* 0x01AA */ u16 ocarinaSong;
    /* 0x01AC */ s16 switchFlag;
    /* 0x01AE */ char unk_1AE[0x2];
    /* 0x01B0 */ s16 unk_1B0;
    /* 0x01B2 */ s16 unk_1B2;
    /* 0x01B4 */ f32 interactRange;
} EnOkarinaTag; // size = 0x01B8

extern ActorInit En_Okarina_Tag_InitVars;

#endif
