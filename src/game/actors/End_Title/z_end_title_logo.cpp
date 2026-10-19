extern "C" {
#include "z_end_title.hpp"
#include "model_manager.hpp"
#include "mtx34_identity.hpp"

typedef struct {
    u32 unk_00; // frames of the logo fade (limit for logoTimer)
    f32 unk_04;
} EndTitleGlobal;
extern EndTitleGlobal gUnk_005311b0; // data_syms.s

s32 FUN_003fda88(void* ctx, s32 idx, void* a2, f32* rect, s32 a4, s32* a5, void* a6, s32 a7);

// DRAFT (NOT compiled; FUN_00250540 stays GLOBAL_ASM in functions_240000s.cpp): 16 words off in fastchk, all in the
// inlined identity-MTX34 init (VFP copy numbering of the 1.0/0.0 ctor args; the same header ctor matches the
// out-of-line MTX34Identity exactly). Everything else (logic, frame, pool order, both guards) matches.
#if 0
// params == 2 draw (N64 EndTitle_DrawNintendoLogo). Inlines two guarded statics: the shared identity MTX34
// (include/mtx34_identity.hpp) and GetModelManager() (its 2D drawer sub-object at +0x25F0).
void FUN_00250540(EndTitle* self, PlayState* play) {
    f32 color[4];
    f32 rect[4];

    color[0] = 1.0f;
    color[1] = 1.0f;
    color[2] = 1.0f;
    color[3] = 1.0f;
    rect[2] = 136.0f;
    rect[0] = 104.0f;
    rect[3] = 264.0f;
    rect[1] = 136.0f;

    if (*(s32*)((u8*)play + 0x7C60) >= 0x4A7E8B) {
        if (self->logoTimer == 0) {
            self->logoTimer = 1;
        }
    }
    if (self->logoTimer != 0 && self->logoTimer < gUnk_005311b0.unk_00) {
        color[3] = 1.0f;
        if (self->logoTimer >= gUnk_005311b0.unk_00 - 30) {
            color[3] = (f32)(gUnk_005311b0.unk_00 - self->logoTimer) * (1.0f / 30.0f);
        }
        self->logoTimer++;
    } else {
        color[3] = 0.0f;
    }

    if (color[3] != 0.0f) {
        const MTX34* mtx = &MTX34_Identity();
        FUN_003fda88((u8*)&GetModelManager() + 0x25F0, 5, color, rect, (s32)self->unk_1B8, (s32*)0x004FA518,
                     (void*)mtx, 10);
    }
}
#endif
}
