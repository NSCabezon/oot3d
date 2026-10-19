// En_Ny: N64 func_80ABCDBC (EnNy_SetupMove). Separate TU on purpose: the ROM caller (FUN_003cf564) keeps a real
// `bl FUN_003ce888`; defining it in z_en_ny.cpp lets armcc inline it.
extern "C" {
#include "z_en_ny.hpp"

void FUN_003ae2a0(EnNy* self, PlayState* play);

static inline void EnNy_SetupAction(EnNy* self, EnNyActionFunc func) {
    self->actionFunc = func;
}

void FUN_003ce888(EnNy* self, PlayState* play) {
    self->unk_1F4 = 0.0f;
    self->unk_1E8 = ((self->actor.yDistToWater > 0.0f) ? 0.7f : 1.0f) * 2.8f;
    self->stoneTimer = 0x10E;
    EnNy_SetupAction(self, FUN_003ae2a0);
}
}
