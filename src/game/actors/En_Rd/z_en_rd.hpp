#ifndef _Z_EN_RD_H_
#define _Z_EN_RD_H_

#include "global.hpp"

struct EnRd;

typedef void (*EnRdActionFunc)(struct EnRd*, PlayState*);

// 3DS layout. Offsets up to 0x35C follow the N64 layout shifted by Actor (+0x58); the rest is 3DS-only (not mapped yet).
typedef struct EnRd {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x3C];
    /* 0x01E0 */ SkelAnime skelAnime;
    /* 0x0264 */ char unk_264[0x94D - 0x264];
    /* 0x094D */ u8 unk_94D;
    /* 0x094E */ u8 unk_94E;
    /* 0x094F */ char unk_94F[0x950 - 0x94F];
    /* 0x0950 */ EnRdActionFunc actionFunc;
    /* 0x0954 */ s16 unk_954;
    /* 0x0956 */ s16 unk_956;
    /* 0x0958 */ s16 unk_958;
    /* 0x095A */ char unk_95A[0x95E - 0x95A];
    /* 0x095E */ s16 timer;
    /* 0x0960 */ char unk_960[0x2];
    /* 0x0962 */ u8 isMourning;
    /* 0x0963 */ u8 unk_963;
    /* 0x0964 */ u8 action;
    /* 0x0965 */ u8 unk_965;
    /* 0x0966 */ u8 unk_966;
    /* 0x0967 */ char unk_967[0x1];
    /* 0x0968 */ ColliderCylinder collider;
    /* 0x09C0 */ void* unk_9C0;
    /* 0x09C4 */ void* unk_9C4;
    /* 0x09C8 */ u8 unk_9C8;
    /* 0x09C9 */ u8 unk_9C9;
} EnRd; // size = 0x9CC

extern ActorInit En_Rd_InitVars;

#endif
