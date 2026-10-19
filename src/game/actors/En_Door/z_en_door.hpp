#ifndef _Z_EN_DOOR_H_
#define _Z_EN_DOOR_H_

#include "global.hpp"

struct EnDoor;

typedef void (*EnDoorActionFunc)(struct EnDoor*, PlayState*);

typedef struct EnDoor {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x1BC];
    /* 0x03E4 */ EnDoorActionFunc actionFunc;
    /* 0x03E8 */ s16 lockTimer;
    /* 0x03EA */ u8 openAnim;
    /* 0x03EB */ u8 playerIsOpening;
    /* 0x03EC */ char unk_3EC[0x1];
    /* 0x03ED */ s8 requiredObjectSlot;
    /* 0x03EE */ s8 dListIndex;
    /* 0x03EF */ char unk_3EF[0x1];
} EnDoor; // size = 0x3F0

extern ActorInit En_Door_InitVars;

#endif
