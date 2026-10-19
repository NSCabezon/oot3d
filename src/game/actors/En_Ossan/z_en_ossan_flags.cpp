extern "C" {
#include "global.hpp"

// Own TU: these are called out of line from En_Ossan (shop item availability checks);
// in the actor TU armcc would inline them.

s32 FUN_00192860(s32 value) {
    return (gSaveContext.itemGetInf[3] & 0x100) ? value : -1;
}

void FUN_00195ac8(void) {}

void FUN_0039dc38(PlayState* play) {
    if (*(u16*)((u8*)gGameInfo + 0x4B2) == 0x10) {
        *((u8*)play + 0x7F24) = 2;
    }
}

s32 FUN_001e1220(s32 value) {
    return (gSaveContext.itemGetInf[3] & 0x8000) ? value : -1;
}

s32 FUN_001e2d60(s32 value) {
    return (gSaveContext.itemGetInf[3] & 0x400) ? value : -1;
}

s32 FUN_001e2f04(s32 value) {
    return (gSaveContext.itemGetInf[3] & 0x8000) ? value : -1;
}

s32 FUN_001fe388(s32 value) {
    return (gSaveContext.itemGetInf[3] & 0x8000) ? value : -1;
}

s32 FUN_001ff768(s32 value) {
    return (gSaveContext.itemGetInf[3] & 0x200) ? value : -1;
}

}
