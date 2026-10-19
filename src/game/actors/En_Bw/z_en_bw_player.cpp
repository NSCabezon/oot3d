// FUN_00342abc is called out of line (bl) by the ROM from FUN_002a3a58: keep it in its own TU so armcc does not inline it.
extern "C" {
#include "z_en_bw.hpp"

s32 FUN_00342abc(PlayState* play, Actor* actor) {
    u8* p = (u8*)GET_PLAYER(play);

    if (((u8*)actor)[0x11] & 8) {
        p += 0x2200;
        if (p[0x27] != 0) {
            if (p[0x26] == 0x16) {
                return 1;
            }
        }
    }
    return 0;
}
}
