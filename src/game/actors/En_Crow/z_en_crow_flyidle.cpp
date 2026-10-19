extern "C" {
#include "z_en_crow.hpp"

void SkelAnime_SetPlaySpeed(SkelAnime* skelAnime, f32 speed);
void EnCrow_FlyIdle(EnCrow* self, PlayState* play);

// Own TU: the ROM calls this out of line from EnCrow_TurnAway (armcc would inline it in the same file).
void FUN_003662d4(EnCrow* self) {
    self->timer = 150;
    SkelAnime_SetPlaySpeed(&self->skelAnime, 1.0f);
    self->colliderAcFlags |= AC_ON;
    self->actionFunc = EnCrow_FlyIdle;
}

}
