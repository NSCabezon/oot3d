extern "C" {
#include "z_en_fr.hpp"
#include "model_manager.hpp"

// 0x005C1878 = GetModelManager()::sInstance + 0x32C0: an embedded sub-object whose vtable slot 3 is called.
// Reaching it through the singleton (not a separate symbol) gives the ROM's `ldr r0,=obj; ldr r1,[r0]` shape.
typedef struct EnFrMMSubVtbl {
    void* unk_00[3];
    s32 (*unk_0C)(struct EnFrMMSub*);
} EnFrMMSubVtbl;
typedef struct EnFrMMSub {
    EnFrMMSubVtbl* vtbl;
} EnFrMMSub;

s32 FUN_002386c4(PlayState* play) {
    EnFrMMSub* sub = (EnFrMMSub*)((char*)&GetModelManager() + 0x32C0);
    return sub->vtbl->unk_0C(sub);
}

}
