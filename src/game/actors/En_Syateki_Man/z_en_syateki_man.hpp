#ifndef _Z_EN_SYATEKI_MAN_H_
#define _Z_EN_SYATEKI_MAN_H_

#include "global.hpp"

struct EnSyatekiMan;

typedef void (*EnSyatekiManActionFunc)(struct EnSyatekiMan*, PlayState*);
typedef void (*EnSyatekiManOtherFunc)(struct EnSyatekiMan*);

// 3DS layout. Fields from skelAnime to actionFunc are not mapped yet (joint/morph tables, 0x1A4..0x568).
// From actionFunc on: N64 offset + 0x36C (verified against ROM: actionFunc 0x568, blinkFunc 0x590, blinkTimer 0x57A).
typedef struct EnSyatekiMan {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x340];
    /* 0x0568 */ EnSyatekiManActionFunc actionFunc;
    /* 0x056C */ Vec3s headRot;
    /* 0x0572 */ Vec3s bodyRot;
    /* 0x0578 */ s16 eyeState; // Unused
    /* 0x057A */ s16 blinkTimer; // Unused
    /* 0x057C */ s16 textIdx;
    /* 0x057E */ s16 numTextBox;
    /* 0x0580 */ s16 gameResult;
    /* 0x0582 */ s16 timer;
    /* 0x0584 */ s32 getItemId;
    /* 0x0588 */ u8 cameraHold;
    /* 0x058C */ Actor* tempGallery;
    /* 0x0590 */ EnSyatekiManOtherFunc blinkFunc; // Seems to be part of a blink system, but it's unused.
    /* 0x0594 */ s16 subCamId;
    /* 0x0598 */ void* model;
    /* 0x059C */ s32 counter;
} EnSyatekiMan; // size = 0x05A0

extern ActorInit En_Syateki_Man_InitVars;

#endif
