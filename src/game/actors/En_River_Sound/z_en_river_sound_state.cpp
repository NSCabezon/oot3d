extern "C" {
#include "z_en_river_sound.hpp"

void FUN_00214848(void) {
    EnRiverSound_sState.unk_80 = 0;
}

void FUN_0021485c(u8 arg) {
    EnRiverSound_sState.unk_31 = arg;
    EnRiverSound_sState.unk_33 = 1;
}

void FUN_00285f34(void* arg) {
    if (EnRiverSound_sState.unk_80 == (s32)arg) {
        EnRiverSound_sState.unk_80 = 0;
    }
}

void FUN_00355fac(s32 a, s32 b, s32 c, s32 d);

// Ganon's Tower BGM volume level (0..7; 7 = final room sets the flag at unk_06).
void FUN_00285f50(u8 level) {
    if (level == 7) {
        EnRiverSound_sState.unk_06 = 1;
    } else {
        u8 volume = EnRiverSound_sGanonVolTable[level & 7];

        if (EnRiverSound_sState.unk_35 != volume) {
            FUN_00355fac(0, 0, volume, 2);
            EnRiverSound_sState.unk_35 = volume;
        }
    }
}

void FUN_0030ee14(void** out, void** slot);
void FUN_0030ede0(void** out);
void FUN_00404b20(void* arg, s32 sfxId, f32 freq);
extern void* EnRiverSound_sSfxHandles[];

void FUN_003ff980(s32 index, s32 sfxId, s32 arg2) {
    void* handle;
    f32 freq;

    FUN_0030ee14(&handle, &EnRiverSound_sSfxHandles[index]);
    freq = (f32)arg2 * (1.0f / 127.0f);

    if (handle != NULL) {
        FUN_00404b20(handle, sfxId, freq);
    }

    FUN_0030ede0(&handle);
}

s32 FUN_004896d4(s32 sfxId);

// Plays the 0x0100015D river sfx, fading its volume toward the requested level.
void FUN_0014eef8(Vec3f* pos, f32 volume) {
    if (FUN_004896d4(0x0100015D) == 0) {
        EnRiverSound_sVolFade.prevVolume = volume;
    } else if (EnRiverSound_sVolFade.prevVolume != volume) {
        EnRiverSound_sVolFade.volume = volume;
        EnRiverSound_sVolFade.volumeStep = (volume - EnRiverSound_sVolFade.prevVolume) * 0.025f;
        EnRiverSound_sVolFade.framesLeft = 0x28;
    }

    Audio_PlaySoundGeneral(0x0100015D, pos, 4, &EnRiverSound_sVolFade.prevVolume, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
}

u32 SurfaceType_GetData(void* colCtx, u32 poly, s32 bgId, s32 index);

// Conveyor speed of the floor poly (kept in this TU so the Update caller does not inline it).
s32 FUN_003232a4(void* colCtx, u32 poly, s32 bgId) {
    return (SurfaceType_GetData(colCtx, poly, bgId, 1) << 11) >> 29;
}
}
