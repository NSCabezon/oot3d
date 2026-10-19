extern "C" {
#include "z_en_eiyer.hpp"

#define ATTENTION_ENABLED 0x1

void FUN_00199700(EnEiyer* self, PlayState* play); // Inactive

// Kept out of z_en_eiyer.cpp so the calls from CircleUnderground/WanderUnderground stay real tail calls.
void FUN_00239050(EnEiyer* self) {
    self->actor.flags |= ATTENTION_ENABLED;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->actionFunc = FUN_00199700;
}

// Tiny accessor kept in its own TU: armlink inlines it, so the call site reads like the ROM.
f32* EnEiyer_CurFramePtr(SkelAnime* skelAnime) {
    return &skelAnime->curFrame;
}
}
