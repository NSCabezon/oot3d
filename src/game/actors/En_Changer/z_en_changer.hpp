#ifndef _Z_EN_CHANGER_H_
#define _Z_EN_CHANGER_H_

#include "global.hpp"

struct EnChanger;

typedef void (*EnChangerActionFunc)(struct EnChanger* self, GameState* state);

typedef struct EnChanger {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnChangerActionFunc actionFunc;
    /* 0x01A8 */ Actor* leftChest;
    /* 0x01AC */ Actor* rightChest;
    /* 0x01B0 */ Actor* finalChest;
    /* 0x01B4 */ s16 leftChestNum;
    /* 0x01B6 */ s16 rightChestNum;
    /* 0x01B8 */ s16 leftChestGetItemId;
    /* 0x01BA */ s16 rightChestGetItemId;
    /* 0x01BC */ s16 selectedChest;
    /* 0x01BE */ s16 timer;
    /* 0x01C0 */ s16 roomChestsOpened;
} EnChanger; // size = 0x1C4

extern ActorInit En_Changer_InitVars;

#endif
