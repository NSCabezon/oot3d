extern "C" {
#include "z_en_m_thunder.hpp"

// N64 sword-charge SFX helper (moved from functions_310000s.cpp). State at EnRiverSound_sState (0x0054AC48).
extern u8 EnRiverSound_sState[];
extern f32 FLOAT_0054acb0;

void FUN_00314988(Vec3f* pos, s32 level) {
    u8* st = EnRiverSound_sState;
    u8 lvl = level & 3;
    u8 prev = st[5];

    if (lvl != prev) {
        f32* tbl = (f32*)(st + 0x110);
        f32 vol = tbl[lvl];
        u32 sfxId = 0x0100006E;

        *(f32*)(st + 0x68) = vol;
        if (lvl == 1 || lvl == 2) {
            Audio_PlaySoundGeneral(sfxId, pos, 4, &FLOAT_0054acb0, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        }
        st[5] = lvl;
    }
    if (lvl != 0) {
        Audio_PlaySoundGeneral(0x01000114, pos, 4, &FLOAT_0054acb0, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    }
}
}
