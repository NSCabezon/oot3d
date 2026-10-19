#ifndef _Z_OBJ_ROOMTIMER_H_
#define _Z_OBJ_ROOMTIMER_H_

#include "global.hpp"

struct ObjRoomtimer;

typedef void (*ObjRoomtimerActionFunc)(struct ObjRoomtimer*, PlayState*);

typedef struct ObjRoomtimer {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ObjRoomtimerActionFunc actionFunc;
    /* 0x01A8 */ u32 switchFlag;
} ObjRoomtimer; // size = 0x1AC

extern ActorInit Obj_Roomtimer_InitVars;

#endif
