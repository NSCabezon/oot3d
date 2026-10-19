extern "C" {
#include "z_en_box.hpp"

s32 OnePointCutscene_CheckForCategory(PlayState* play, s32 category) {
    s16 idx = *(s16*)(*(u8**)((u8*)play + 0xA54) + 0x196);

    while (idx != 0) {
        u8* p = *(u8**)((u8*)play + idx * 4 + 0xA54);

        if (p == NULL) {
            return 0;
        }
        if (*(u16*)(p + 0x18A) != 0x2B) {
            return 0;
        }
        if (*(u8*)(*(u8**)(p + 0xF0) + 2) == category) {
            return 1;
        }
        idx = *(s16*)(p + 0x196);
    }
    return 0;
}

void FUN_003dee3c(void);

s32 FUN_00387fd0(Actor* actor) {
    return *(void**)((u8*)actor + 0x1BC) == (void*)FUN_003dee3c;
}

}
