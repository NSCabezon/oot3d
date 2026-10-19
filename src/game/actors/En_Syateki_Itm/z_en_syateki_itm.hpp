#ifndef _Z_EN_SYATEKI_ITM_H_
#define _Z_EN_SYATEKI_ITM_H_

#include "global.hpp"

struct EnSyatekiItm;
struct EnSyatekiMan;
struct EnGSwitch;

typedef void (*EnSyatekiItmActionFunc)(struct EnSyatekiItm*, PlayState*);

typedef enum EnSyatekiSignal {
    /* 0 */ ENSYATEKI_NONE,
    /* 1 */ ENSYATEKI_START,
    /* 2 */ ENSYATEKI_END,
    /* 3 */ ENSYATEKI_RESULTS
} EnSyatekiSignal;

typedef enum EnSyatekiHitState {
    /* 0 */ ENSYATEKIHIT_NONE,
    /* 1 */ ENSYATEKIHIT_MISS,
    /* 2 */ ENSYATEKIHIT_HIT
} EnSyatekiHitState;

// En_Ex_Ruppy as spawned by this actor (only the fields this actor touches; En_Ex_Ruppy header is opaque)
typedef struct EnSyatekiItmMarker {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x4];
    /* 0x01A8 */ s16 colorIdx;
    /* 0x01AA */ char unk_1AA[0xA];
    /* 0x01B4 */ s16 galleryFlag;
} EnSyatekiItmMarker;

// 3DS layout = N64 layout (0x14C-based) + 0x58 (Actor is 0x1A4 bytes on 3DS)
typedef struct EnSyatekiItm {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnSyatekiItmActionFunc actionFunc;
    /* 0x01A8 */ s16 timer; // timer for next round
    /* 0x01AA */ s16 unkTimer; // unk timer
    /* 0x01AC */ s16 signal; // signal between this and shopkeeper
    /* 0x01AE */ s16 hitCount; // total rupees hit
    /* 0x01B0 */ s16 roundNum; // current round
    /* 0x01B2 */ s16 roundFlags[6]; // flags for each round happening
    /* 0x01BE */ s16 targetState[2]; // current state of target rupees
    /* 0x01C2 */ s16 numTargets; // number of target rupees for this round
    /* 0x01C4 */ Vec3f targetHome[2]; // initial position of target rupees
    /* 0x01DC */ Vec3f targetFinal[2]; // target position of target rupees
    /* 0x01F4 */ EnSyatekiItmMarker* markers[10]; // marker rupees for hits
    /* 0x021C */ struct EnGSwitch* targets[2]; // currently spawned target rupees
    /* 0x0224 */ struct EnSyatekiMan* man; // shopkeeper
    /* 0x0228 */ EnSyatekiItmMarker* curMarkers[2]; // marker rupees for the current round
} EnSyatekiItm; // size = 0x230

extern ActorInit En_Syateki_Itm_InitVars;

#endif
