#ifndef _Z_OBJ_MURE_H_
#define _Z_OBJ_MURE_H_

#include "global.hpp"

struct ObjMure;

typedef void (*ObjMureActionFunc)(struct ObjMure*, struct PlayState*);

#define OBJMURE_MAX_SPAWNS 15

typedef struct ObjMure {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ObjMureActionFunc actionFunc;
    /* 0x01A8 */ s16 chNum;
    /* 0x01AA */ s16 ptn;
    /* 0x01AC */ s16 svNum;
    /* 0x01AE */ s16 type;
    /* 0x01B0 */ Actor* children[OBJMURE_MAX_SPAWNS];
    /* 0x01EC */ u8 childrenStates[OBJMURE_MAX_SPAWNS];
    /* 0x01FC */ s16 unk_1FC;
    /* 0x01FE */ s16 unk_1FE;
    /* 0x0200 */ s16 unk_200;
} ObjMure; // size = 0x204

extern ActorInit Obj_Mure_InitVars;

#endif
