#ifndef _Z_BG_HIDAN_SEKIZOU_H_
#define _Z_BG_HIDAN_SEKIZOU_H_

#include "global.hpp"

struct BgHidanSekizou;

typedef void (*BgHidanSekizouUpdateFunc)(struct BgHidanSekizou*, PlayState*);

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ u8 unk_00[0x50];
} BgHidanSekizouColliderElement; // size = 0x50

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ BgHidanSekizouColliderElement* elements;
} BgHidanSekizouColliderJntSph; // size = 0x20

typedef struct BgHidanSekizou {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgHidanSekizouUpdateFunc updateFunc;
    /* 0x01C0 */ s16 unk_168[4];
    /* 0x01C8 */ s16 unk_170;
    /* 0x01CC */ BgHidanSekizouColliderJntSph collider;
    /* 0x01EC */ BgHidanSekizouColliderElement colliderElements[6];
    /* 0x03CC */ SkeletonAnimationModel* skelAnimModel[18];
} BgHidanSekizou; // size = 0x414

extern ActorInit Bg_Hidan_Sekizou_InitVars;

#endif
