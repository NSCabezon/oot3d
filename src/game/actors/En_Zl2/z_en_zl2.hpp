#ifndef _Z_EN_ZL2_H_
#define _Z_EN_ZL2_H_

#include "global.hpp"

struct EnZl2;

typedef void (*EnZl2ActionFunc)(struct EnZl2*, PlayState*);
typedef void (*EnZl2DrawFunc)(struct EnZl2*, PlayState*);

typedef struct EnZl2 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ FaceAnimations faceAnims;
    /* 0x03F4 */ void* zar;
    /* 0x03F8 */ char unk_3F8[0x150];
    /* 0x0548 */ s16 unk_548; // face texture index, eyes
    /* 0x054A */ char unk_54A[0x2];
    /* 0x054C */ s16 unk_54C; // face texture index, eyes
    /* 0x054E */ char unk_54E[0x2];
    /* 0x0550 */ s16 unk_550; // face texture index, mouth
    /* 0x0552 */ char unk_552[0x2];
    /* 0x0554 */ s32 action;
    /* 0x0558 */ s32 drawConfig;
    /* 0x055C */ char unk_55C[0xDC];
} EnZl2; // size = 0x638

extern ActorInit En_Zl2_InitVars;

#endif
