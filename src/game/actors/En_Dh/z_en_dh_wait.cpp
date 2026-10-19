extern "C" {
#include "z_en_dh.hpp"

void FUN_003586ec(SkeletonAnimationModel_unk_0C* anim);
void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);
void EnDh_Wait(EnDh* self, PlayState* play);
extern s32 gUnk_0054C8CC; // src/game/shared_syms.s

void FUN_002b6748(EnDh* self) {
    Animation_PlayLoop(&self->skelAnime, 3);
    {
        SkeletonAnimationModel_unk_0C* anim = self->unk_934;
        f32 zero = 0.0f;

        if (gUnk_0054C8CC == 0) {
            *(f32*)&anim->curFrame = zero;
            FUN_003586ec(anim);
        }
    }
    self->curAction = 0;
    self->actor.world.pos.x = Rand_CenteredFloat(600.0f) + self->actor.home.pos.x;
    self->actor.world.pos.z = Rand_CenteredFloat(600.0f) + self->actor.home.pos.z;
    self->actor.shape.yOffset = -15000.0f;
    self->actor.speedXZ = 0.0f;
    self->dirtWaveSpread = 0.0f;
    self->actor.params = 0;
    self->actionState = 0;
    self->dirtWavePhase = 0;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->actor.flags |= 0x80;
    self->actionFunc = EnDh_Wait;
}
}
