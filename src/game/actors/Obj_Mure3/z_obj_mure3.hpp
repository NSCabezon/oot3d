#ifndef _Z_OBJ_MURE3_H_
#define _Z_OBJ_MURE3_H_

#include "global.hpp"

struct ObjMure3;

typedef void (*ObjMure3ActionFunc)(struct ObjMure3*, PlayState*);
typedef void (*ObjMure3SpawnFunc)(struct ObjMure3*, PlayState*);

typedef struct ObjMure3 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ObjMure3ActionFunc actionFunc;
    /* 0x01A8 */ struct EnItem00* unk_150[7]; // N64 name kept (N64 offset 0x150)
    /* 0x01C4 */ u16 unk_16C;                // N64 name kept (N64 offset 0x16C)
    /* 0x01C6 */ char unk_1C6[0x2];
} ObjMure3; // size = 0x1C8

extern ActorInit Obj_Mure3_InitVars;

#endif
