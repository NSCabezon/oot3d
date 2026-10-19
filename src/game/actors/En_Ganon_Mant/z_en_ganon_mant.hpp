#ifndef _Z_EN_GANON_MANT_H_
#define _Z_EN_GANON_MANT_H_

#include "global.hpp"

struct EnGanonMant;

typedef struct EnGanonMant {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x1760 - 0x1A4];
    /* 0x1760 */ void* model;     // freed through the 0x0055A1F8 object (Destroy)
    /* 0x1764 */ void* mgrModel;  // freed through GetModelManager().unk_247C (Destroy)
    /* 0x1768 */ char unk_1768[0x17E4 - 0x1768];
} EnGanonMant; // size = 0x17E4

extern ActorInit En_Ganon_Mant_InitVars;

#endif
