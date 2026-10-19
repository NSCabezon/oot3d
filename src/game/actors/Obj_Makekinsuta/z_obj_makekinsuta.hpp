#ifndef _Z_OBJ_MAKEKINSUTA_H_
#define _Z_OBJ_MAKEKINSUTA_H_

#include "global.hpp"

struct ObjMakekinsuta;

typedef void (*ObjMakekinsutaActionFunc)(struct ObjMakekinsuta*, PlayState*);

typedef struct ObjMakekinsuta {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ObjMakekinsutaActionFunc actionFunc;
    /* 0x01A8 */ s16 timer;
    /* 0x01AA */ u16 unk_1AA; // N64: unk_152, set by the soil patch when bugs are released
} ObjMakekinsuta; // size = 0x1AC

extern ActorInit Obj_Makekinsuta_InitVars;

#endif
