#ifndef _Z_EN_RIVER_SOUND_H_
#define _Z_EN_RIVER_SOUND_H_

#include "global.hpp"

struct EnRiverSound;

typedef struct EnRiverSound {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0xC];
    /* 0x01B0 */ u8 playSfx;
    /* 0x01B1 */ u8 sfxFreqIndex;
    /* 0x01B2 */ s16 pathIndex;
} EnRiverSound; // size = 0x1B4

// Shared river sound state at EnRiverSound_sState (EQU in data_syms.s); only the fields the helpers touch.
typedef struct {
    /* 0x00 */ u8 unk_00[0x6];
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ u8 unk_07[0x2A];
    /* 0x31 */ u8 unk_31;
    /* 0x32 */ u8 unk_32;
    /* 0x33 */ u8 unk_33;
    /* 0x34 */ u8 unk_34;
    /* 0x35 */ u8 unk_35;
    /* 0x36 */ u8 unk_36[0x2];
    /* 0x38 */ u8 unk_38;
    /* 0x39 */ u8 unk_39[0x47];
    /* 0x80 */ s32 unk_80;
    /* 0x84 */ f32 unk_84;
} EnRiverSoundState;
extern EnRiverSoundState EnRiverSound_sState;
extern const u8 EnRiverSound_sGanonVolTable[];
extern const u32 EnRiverSound_sSoundEffects[];
extern const f32 EnRiverSound_sSfxFreqs[];

// Volume fade state for the river sfx (EnRiverSound_sVolFade, 3DS 0x005B12C8).
typedef struct {
    /* 0x00 */ f32 prevVolume;
    /* 0x04 */ f32 volume;
    /* 0x08 */ f32 volumeStep;
    /* 0x0C */ s32 framesLeft;
} EnRiverSoundVolFade;
extern EnRiverSoundVolFade EnRiverSound_sVolFade;

extern ActorInit En_River_Sound_InitVars;

#endif
