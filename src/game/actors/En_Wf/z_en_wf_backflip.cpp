// Own TU: the ROM tail-calls SetupBackflipBackward out of line from the run action.
extern "C" {
#include "z_en_wf.hpp"

void FUN_0035a49c(SkelAnime* skelAnime, void* anim, f32 morph);
void FUN_00173e68(EnWf* self, PlayState* play);
extern u8 EnWf_sAnimBackflip[];

void FUN_00364a2c(EnWf* self) {
    FUN_0035a49c(&self->skelAnime, EnWf_sAnimBackflip, -3.0f);
    self->timer = 0;
    self->unk_300 = 1;
    self->actor.speedXZ =-6.0f;
    self->action = 5;
    self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    Audio_PlayActorSound2(&self->actor, 0x010002DC);
    self->actionFunc = FUN_00173e68;
}
}
