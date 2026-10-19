#ifndef _Z_EN_BUTTE_H_
#define _Z_EN_BUTTE_H_

#include "global.hpp"

struct EnButte;

typedef void (*EnButteActionFunc)(struct EnButte*, struct PlayState*);

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ void* elements;
} EnButteColliderJntSph; // size = 0x20

// Typed SkelAnime / collider members here, or splitting the blob, change armcc's codegen of unrelated
// functions (WaitToDie), so they are reached through the EB_* offset macros in the .cpp.
typedef struct EnButte {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x22C];     // collider (0x1A4), skelAnime (0x214), ...
    /* 0x03D0 */ void* unk_3D0;
    /* 0x03D4 */ void* unk_3D4;
    /* 0x03D8 */ EnButteActionFunc actionFunc;
    /* 0x03DC */ s16 timer;
    /* 0x03DE */ u8 flightParamsIdx;
    /* 0x03DF */ u8 unk_3DF;
    /* 0x03E0 */ u8 drawSkelAnime;
    /* 0x03E1 */ u8 unk_3E1;
    /* 0x03E2 */ s16 swordDownTimer;
    /* 0x03E4 */ s16 unk_25C;
    /* 0x03E6 */ s16 unk_25E;
    /* 0x03E8 */ s16 unk_260;
    /* 0x03EA */ s16 unk_3EA;
    /* 0x03EC */ f32 posYTarget;
} EnButte; // size = 0x3F0

extern ActorInit En_Butte_InitVars;

#endif
