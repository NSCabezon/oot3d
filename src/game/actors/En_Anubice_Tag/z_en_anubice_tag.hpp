#ifndef _Z_EN_ANUBICE_TAG_H_
#define _Z_EN_ANUBICE_TAG_H_

#include "global.hpp"

struct EnAnubiceTag;

typedef void (*EnAnubiceTagActionFunc)(struct EnAnubiceTag*, PlayState*);

typedef struct EnAnubiceTag {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnAnubiceTagActionFunc actionFunc;
    /* 0x01A8 */ Actor* anubis;
    /* 0x01AC */ f32 extraTriggerRange;
} EnAnubiceTag; // size = 0x1B0

extern ActorInit En_Anubice_Tag_InitVars;

#endif
