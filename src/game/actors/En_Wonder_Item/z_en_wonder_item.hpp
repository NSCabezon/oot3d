#ifndef _Z_EN_WONDER_ITEM_H_
#define _Z_EN_WONDER_ITEM_H_

#include "global.hpp"

struct EnWonderItem;

typedef void (*EnWonderItemUpdateFunc)(struct EnWonderItem*, PlayState*);

typedef struct EnWonderItem {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnWonderItemUpdateFunc updateFunc;
    /* 0x01A8 */ f32 unkHeight; // sets height of dummied out mode 4
    /* 0x01AC */ s16 wonderMode;
    /* 0x01AE */ s16 itemDrop;
    /* 0x01B0 */ s16 numTagPoints;
    /* 0x01B2 */ s16 dropCount;
    /* 0x01B4 */ s16 timer;
    /* 0x01B6 */ s16 tagFlags;
    /* 0x01B8 */ s16 tagCount;
    /* 0x01BA */ s16 switchFlag;
    /* 0x01BC */ char unk_1BC[4];
    /* 0x01C0 */ s16 nextTag;
    /* 0x01C2 */ s16 timerMod;
    /* 0x01C4 */ Vec3f unkPos; // set to initial position by mode bomb soldier, then never used.
    /* 0x01D0 */ char unk_1D0[8];
    /* 0x01D8 */ ColliderCylinder collider;
} EnWonderItem; // size = 0x230

extern ActorInit En_Wonder_Item_InitVars;

#endif
