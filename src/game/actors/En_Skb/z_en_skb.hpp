#ifndef _Z_EN_SKB_H_
#define _Z_EN_SKB_H_

#include "global.hpp"

struct EnSkb;

typedef void (*EnSkbActionFunc)(struct EnSkb*, PlayState*);

typedef struct EnSkb {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x0BC8];
    /* 0x0DF0 */ u8 unk_DF0;
    /* 0x0DF1 */ u8 unk_DF1;
    /* 0x0DF2 */ u8 unk_DF2;
    /* 0x0DF3 */ u8 unk_DF3;
    /* 0x0DF4 */ EnSkbActionFunc actionFunc;
    /* 0x0DF8 */ char unk_DF8[0x04];
    /* 0x0DFC */ char bodyBreak[0x14];
    /* 0x0E10 */ char collider[0x10]; // ColliderJntSph (base)
    /* 0x0E20 */ u8 unk_E20;
    /* 0x0E21 */ u8 unk_E21;
    /* 0x0E22 */ char unk_E22[0xAE];
    /* 0x0ED0 */ u8 unk_ED0;
    /* 0x0ED1 */ char unk_ED1[0x3];
} EnSkb; // size = 0xED4

extern ActorInit En_Skb_InitVars;

#endif
