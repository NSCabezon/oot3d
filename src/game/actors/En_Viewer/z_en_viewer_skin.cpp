extern "C" {
#include "z_en_viewer.hpp"

void SystemArena_Free(void* ptr);
extern s32 gUnk_0050C058[];

static inline void EnViewer_CallVtblFree(void* obj) {
    void** vtbl = *(void***)obj;
    ((void (*)(void*))vtbl[1])(obj);
}

void FUN_00408aac(u8* skin) {
    s32 zero = 0;

    if (*(void**)(skin + 0x38) != NULL) {
        *(s32*)(skin + 0x30) = zero;
    }
    if (*(u8*)(skin + 0x92) != 0) {
        SystemArena_Free(*(void**)(skin + 0x88));
        SystemArena_Free(*(void**)(skin + 0x8C));
    }
    *(s32*)(skin + 0x88) = zero;
    *(s32*)(skin + 0x8C) = zero;
    *(u8*)(skin + 0x92) = 0;
    if (*(void**)(skin + 0x38) != NULL) {
        EnViewer_CallVtblFree(*(void**)(skin + 0x38));
        *(s32*)(skin + 0x38) = zero;
    }
    if (*(u8*)(skin + 0x87) == 1) {
        s32* counter = gUnk_0050C058;
        counter[2] -= 1;
    }
    *(u8*)(skin + 0x87) = 0;
}

}
