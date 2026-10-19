extern "C" {
#include "z_en_bigokuta.hpp"


void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morphFrames);
void FUN_003def48(EnBigokuta* self, PlayState* play);

void FUN_00362998(EnBigokuta* self) {
    Animation_MorphToLoop(&self->skelAnime, 1, -5.0f);
    self->unk_196 = 350 * 3.0f / 2.0f;
    self->unk_198 = 80 * 3.0f / 2.0f;
    self->unk_19A = self->unk_194 * -0x200;
    func_809BCE3C(self);
    self->colliderCylinders[0].base.atFlags |= AT_ON;
    self->collider.base.acFlags |= AC_ON;
    self->actionFunc = FUN_003def48;
}

extern "C" {
#include "z_en_bigokuta.hpp"


}
}
