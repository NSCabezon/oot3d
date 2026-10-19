#ifndef _Z_EN_EX_ITEM_H_
#define _Z_EN_EX_ITEM_H_

#include "global.hpp"

struct EnExItem;

extern "C++" {
struct EnExItemModel {
    virtual void unk_vf0();
    virtual void Destroy();
};
}

typedef void (*EnExItemActionFunc)(struct EnExItem* self, struct PlayState* play);
typedef void (*EnExItemLightFunc)(Actor*, struct PlayState*, s32);

typedef struct EnExItem {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnExItemActionFunc actionFunc;
    /* 0x01A8 */ Vec3f initPos; // unused
    /* 0x01B4 */ f32 scale;
    /* 0x01B8 */ f32 unkFloat; // set to the same value as scale, but unused
    /* 0x01BC */ EnExItemModel* model; // virtual Destroy = slot 1
    /* 0x01C0 */ s16 getItemObjectId;
    /* 0x01C2 */ s16 type;
    /* 0x01C4 */ s16 unusedParam;
    /* 0x01C6 */ s16 giDrawId;
    /* 0x01C8 */ s16 stopRotate;
    /* 0x01CA */ s16 timer;
    /* 0x01CC */ s16 chestKillTimer;
    /* 0x01CE */ s16 prizeRotateTimer;
    /* 0x01D0 */ s16 killItem;
    /* 0x01D2 */ s8 requiredObjectSlot;
    /* 0x01D3 */ char unk_1D3[0x1];
} EnExItem; // size = 0x1D4

typedef enum EnExItemType {
    /*  0 */ EXITEM_BOMB_BAG_BOWLING,
    /*  1 */ EXITEM_HEART_PIECE_BOWLING,
    /*  2 */ EXITEM_BOMBCHUS_BOWLING,
    /*  3 */ EXITEM_BOMBS_BOWLING,
    /*  4 */ EXITEM_PURPLE_RUPEE_BOWLING,
    /*  5 */ EXITEM_BOMB_BAG_COUNTER,
    /*  6 */ EXITEM_HEART_PIECE_COUNTER,
    /*  7 */ EXITEM_BOMBCHUS_COUNTER,
    /*  8 */ EXITEM_BOMBS_COUNTER,
    /*  9 */ EXITEM_PURPLE_RUPEE_COUNTER,
    /* 10 */ EXITEM_GREEN_RUPEE_CHEST,
    /* 11 */ EXITEM_BLUE_RUPEE_CHEST,
    /* 12 */ EXITEM_RED_RUPEE_CHEST,
    /* 13 */ EXITEM_13,
    /* 14 */ EXITEM_14,
    /* 15 */ EXITEM_SMALL_KEY_CHEST,
    /* 16 */ EXITEM_MAGIC_FIRE,
    /* 17 */ EXITEM_MAGIC_WIND,
    /* 18 */ EXITEM_MAGIC_DARK,
    /* 19 */ EXITEM_BULLET_BAG
} EnExItemType;

extern ActorInit En_Ex_Item_InitVars;

#endif
