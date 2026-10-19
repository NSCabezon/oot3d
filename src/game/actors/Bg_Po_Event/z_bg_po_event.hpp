#ifndef _Z_BG_PO_EVENT_H_
#define _Z_BG_PO_EVENT_H_

#include "global.hpp"

struct BgPoEvent;

typedef void (*BgPoEventActionFunc)(struct BgPoEvent*, PlayState*);

// Collision types not yet in the shared headers (3DS layout), same as Bg_Hidan_Hamstep.
typedef struct {
    /* 0x00 */ u8 bytes[0x5C];
} ColliderTrisElement;

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ ColliderTrisElement* elements;
} ColliderTris; // size = 0x20

typedef struct BgPoEvent {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgPoEventActionFunc actionFunc;
    /* 0x01C0 */ u8 type;
    /* 0x01C1 */ u8 index;
    /* 0x01C2 */ s8 direction;
    /* 0x01C4 */ s16 timer;
    /* 0x01C8 */ ColliderTris collider;
    /* 0x01E8 */ ColliderTrisElement colliderElements[2];
    /* 0x02A0 */ void* unk_2A0[5];
} BgPoEvent; // size = 0x02B4

extern ActorInit Bg_Po_Event_InitVars;

#endif
