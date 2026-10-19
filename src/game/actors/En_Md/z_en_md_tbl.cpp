extern "C" {
#include "z_en_md.hpp"

// Shared table at 0x0050CBA4 (24-byte entries, s16 at +0x14); raw address, no symbol yet (debt).
// Own TU so armcc does not inline it into EnMd_Update.
s16 FUN_001c4ddc(s32 index) {
    u8* base = (u8*)0x0050CBA4;

    return *(s16*)(base + index * 24 + 0x14);
}

}
