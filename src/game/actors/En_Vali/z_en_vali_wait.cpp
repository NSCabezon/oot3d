extern "C" {
#include "z_en_vali.hpp"

void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);
void FUN_003e4744(EnVali* self, PlayState* play);

void FUN_00180868(EnVali* self) {
    Animation_PlayLoop(&self->skelAnime, 0);
    self->actor.draw = NULL;
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = FUN_003e4744;
}

}
