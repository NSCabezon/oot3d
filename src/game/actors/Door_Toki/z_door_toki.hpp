#ifndef _Z_DOOR_TOKI_H_
#define _Z_DOOR_TOKI_H_

#include "global.hpp"

struct DoorToki;

typedef struct DoorToki {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ char unk_1BC[0x4];
} DoorToki; // size = 0x1C0

extern ActorInit Door_Toki_InitVars;

#endif
