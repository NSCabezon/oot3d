#ifndef _Z_EN_NWC_H_
#define _Z_EN_NWC_H_

#include "global.hpp"

struct EnNwc;
struct EnNwcChick;

typedef void (*EnNwcUpdateFunc)(struct EnNwc*, struct PlayState*);
typedef void (*EnNwcChickFunc)(struct EnNwcChick*, struct EnNwc*, struct PlayState*);

// JntSph collider layout (3DS: elements pointer at +0x1C, count lives in EnNwc)
typedef struct EnNwcJntSph {
    /* 0x00 */ Collider base;
    /* 0x18 */ char unk_18[0x4];
    /* 0x1C */ struct EnNwcJntSphElement* elements;
} EnNwcJntSph; // size = 0x20

typedef struct EnNwcJntSphElement {
    /* 0x00 */ char unk_00[0x38];
    /* 0x38 */ Vec3f center;
    /* 0x44 */ char unk_44[0xC];
} EnNwcJntSphElement; // size = 0x50

typedef struct EnNwcChick {
    /* 0x00 */ s8 type;
    /* 0x01 */ u8 bgFlags;
    /* 0x04 */ f32 floorY;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ char unk_14[8];
    /* 0x1C */ Vec3f lastPos;
    /* 0x28 */ char unk_28[4];
    /* 0x2C */ f32 velY;
    /* 0x30 */ Vec3s rot;
    /* 0x36 */ u16 height;
    /* 0x38 */ CollisionPoly* floorPoly;
    /* 0x3C */ char unk_3C[0x20];
} EnNwcChick; // size = 0x5C

typedef struct EnNwc {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnNwcJntSph collider;
    /* 0x01C4 */ u8 count;
    /* 0x01C8 */ EnNwcChick chicks[16];
    /* 0x0788 */ EnNwcUpdateFunc updateFunc;
} EnNwc; // size = 0x078C

extern ActorInit En_Nwc_InitVars;

#endif
