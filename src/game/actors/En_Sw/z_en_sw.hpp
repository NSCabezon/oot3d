#ifndef _Z_EN_SW_H_
#define _Z_EN_SW_H_

#include "global.hpp"

struct EnSw;

typedef void (*EnSwActionFunc)(struct EnSw*, PlayState*);

// JntSph collider layout (3DS), local to this actor for now.
typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ void* elements;
} EnSwColliderJntSph; // size = 0x20

typedef struct EnSw {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char jointTable[0x23C];
    /* 0x0464 */ char morphTable[0x23C];
    /* 0x06A0 */ EnSwActionFunc actionFunc;
    /* 0x06A4 */ EnSwColliderJntSph collider;
    /* 0x06C4 */ char unk_6C4[0x54];
    /* 0x0718 */ u8 unk_718;
    /* 0x0719 */ char unk_719[0x3];
    /* 0x071C */ Vec3f unk_71C;
    /* 0x0728 */ Vec3f unk_728;
    /* 0x0734 */ Vec3f unk_734;
    /* 0x0740 */ s16 unk_740;
    /* 0x0742 */ s16 unk_742;
    /* 0x0744 */ s16 unk_744;
    /* 0x0746 */ s16 unk_746;
    /* 0x0748 */ s16 unk_748;
    /* 0x074A */ s16 unk_74A;
    /* 0x074C */ s16 unk_74C;
    /* 0x074E */ char unk_74E[0x32];
    /* 0x0780 */ MTX34 unk_780;
    /* 0x07B0 */ char unk_7B0[0x8];
    /* 0x07B8 */ f32 unk_7B8;
    /* 0x07BC */ char unk_7BC[0x18];
    /* 0x07D4 */ s16 unk_7D4;
    /* 0x07D6 */ s16 unk_7D6;
    /* 0x07D8 */ s16 unk_7D8;
    /* 0x07DA */ char unk_7DA[0x2];
    /* 0x07DC */ Vec3f unk_7DC;
    /* 0x07E8 */ Vec3f unk_7E8[5];
    /* 0x0824 */ f32 unk_824;
    /* 0x0828 */ f32 unk_828;
    /* 0x082C */ char unk_82C[0x4];
} EnSw; // size = 0x830

extern ActorInit En_Sw_InitVars;

#endif
