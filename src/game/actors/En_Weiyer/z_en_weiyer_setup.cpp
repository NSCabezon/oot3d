extern "C" {
#include "z_en_weiyer.hpp"

void FUN_001236dc(EnWeiyer* self, PlayState* play);
void FUN_003e2240(EnWeiyer* self, PlayState* play);
f32 __hardfp_cosf(f32);
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morphFrames);
void SkelAnime_SetPlaySpeed(SkelAnime* skelAnime, f32 speed);
void FUN_00126840(EnWeiyer* self, PlayState* play);

// Own translation unit so armcc does not inline these into their callers in z_en_weiyer.cpp.
void FUN_001cca7c(EnWeiyer* self) {
    Animation_MorphToLoop(&self->skelAnime, 2, -5.0f);
    self->actor.speedXZ = 5.0f;
    self->collider.base.atFlags |= AT_ON;
    self->timer = 0;
    self->actionFunc = FUN_00126840;
}

void FUN_00364394(EnWeiyer* self) {
    self->targetYaw = self->actor.shape.rot.y;
    self->swimHeight = self->actor.world.pos.y + __hardfp_cosf(-M_PI / 8) * 3.0f;
    Animation_MorphToLoop(&self->skelAnime, 2, -5.0f);
    self->timer = 45;
    self->actor.speedXZ = CLAMP_MAX(self->actor.speedXZ, 2.5f);
    self->targetSwimHeight = self->actor.floorHeight;
    self->collider.base.atFlags &= ~AT_ON;
    self->actionFunc = FUN_003e2240;
}

void FUN_001c8264(EnWeiyer* self) {
    self->timer = 300;
    self->targetYaw = self->actor.yawTowardsPlayer + 0x8000;
    self->swimHeight = self->actor.world.pos.y;
    self->actor.speedXZ = CLAMP_MAX(self->actor.speedXZ, 4.0f);
    self->collider.base.atFlags &= ~AT_ON;
    SkelAnime_SetPlaySpeed(&self->skelAnime, 1.0f);
    self->actionFunc = FUN_001236dc;
}

}
