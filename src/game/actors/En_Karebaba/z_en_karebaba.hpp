#ifndef _Z_EN_KAREBABA_H_
#define _Z_EN_KAREBABA_H_

#include "global.hpp"

struct EnKarebaba;

typedef void (*EnKarebabaActionFunc)(struct EnKarebaba* self, PlayState* play);

typedef struct EnKarebaba {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ EnKarebabaActionFunc actionFunc;
    /* 0x022C */ char jointTable[0x9C];
    /* 0x02C8 */ char morphTable[0x9C];
    /* 0x0364 */ CollisionPoly* boundFloor; // floor polygon under the home position, cached once
    /* 0x0368 */ ColliderCylinder headCollider;
    /* 0x03C0 */ ColliderCylinder bodyCollider;
    /* 0x0418 */ void* skelAnimModel0; // model slots, loaded by FUN_00372f38 in Init
    /* 0x041C */ void* skelAnimModel1;
    /* 0x0420 */ void* skelAnimModel2;
    /* 0x0424 */ void* skelAnimModel4;
    /* 0x0428 */ void* skelAnimModel3;
    /* 0x042C */ s16 timer; // head AT cooldown: set to 30 after the head lands a hit
    /* 0x042E */ char unk_42E[0x02];
} EnKarebaba; // size = 0x430

extern ActorInit En_Karebaba_InitVars;

#endif
