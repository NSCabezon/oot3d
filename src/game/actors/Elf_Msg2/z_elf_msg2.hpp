#ifndef _Z_ELF_MSG2_H_
#define _Z_ELF_MSG2_H_

#include "global.hpp"

struct ElfMsg2;

typedef void (*ElfMsg2ActionFunc)(struct ElfMsg2*, PlayState*);

typedef struct ElfMsg2 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ElfMsg2ActionFunc actionFunc;
} ElfMsg2; // size = 0x1A8

extern ActorInit Elf_Msg2_InitVars;

#endif
