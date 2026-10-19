// Own TU: the ROM tail-calls FUN_00364aa4 (SetupAttack) from the BackflipForward action, so armcc must not inline it
// into z_en_wf.cpp.
extern "C" {
#include "z_en_wf.hpp"

void FUN_0034c3e4(SkelAnime* skelAnime, void* anim);
void FUN_0037043c(SkelAnime* skelAnime, f32 endFrame);
void FUN_003dd5c8(EnWf* self, PlayState* play);
extern u8 EnWf_sAnimAttack[];

void FUN_00364aa4(EnWf* self) {
    FUN_0034c3e4(&self->skelAnime, EnWf_sAnimAttack);
    self->attackAndBlockCollider[0x10] &= ~4;
    self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
    FUN_0037043c(&self->skelAnime, 20.0f);
    self->action = 8;
    self->actor.speedXZ = 0.0f;
    self->unk_2FA = 0;
    self->timer = 11;
    self->actionFunc = FUN_003dd5c8;
}
}
