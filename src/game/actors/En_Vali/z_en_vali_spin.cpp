extern "C" {
#include "z_en_vali.hpp"

#define ACTOR_FLAG_UPDATE_CULLING_DISABLED (1 << 4)
#define ACTOR_FLAG_ATTENTION_ENABLED (1 << 0)
#define DEG_TO_BINANG(deg) ((s16)((deg) * 182.04445f))
#define SCALE23(v) (((v) > 0) ? (s32)(((v) * 2.0f) / 3.0f + 0.5f) : (s32)(((v) * 2.0f) / 3.0f - 0.5f))

void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);
void FUN_003b97bc(EnVali* self, PlayState* play);
f32* EnVali_CurFramePtr(SkelAnime* skelAnime);
f32 __hardfp_sinf(f32 x);
void Animation_MorphToPlayOnce(SkelAnime* skelAnime, s32 idx, f32 morph);

s32 FUN_00370734(SkelAnime* skelAnime);
void FUN_00370634(EnVali* self);
void FUN_0011ca08(EnVali* self, PlayState* play);

void FUN_0011ca08(EnVali* self, PlayState* play) {
    s32 animCurFrame;

    FUN_00370734(&self->skelAnime);
    if (self->spinTimer != 0) {
        self->spinTimer--;
    }
    animCurFrame = (s32)*EnVali_CurFramePtr(&self->skelAnime);
    Math_StepToF(&self->spinHeight, self->actor.floorHeight + 40.0f, 1.2f);
    self->actor.world.pos.y = self->spinHeight - (__hardfp_sinf(animCurFrame * M_PI * 0.0125f) * 8.0f);
    if (self->spinTimer != 0) {
        self->actor.shape.rot.y += 0x555;
        if (((self->spinTimer % 6) == 0) && (animCurFrame >= 16) && (animCurFrame <= 55)) {
            Audio_PlayActorSound2(&self->actor, 0x0100030D);
        }
    } else if ((animCurFrame == 16) || (animCurFrame == 30) || (animCurFrame == 42) || (animCurFrame == 55)) {
        Audio_PlayActorSound2(&self->actor, 0x0100030D);
    }
    if (animCurFrame > 40) {
        animCurFrame = 80 - animCurFrame;
    }
    self->actor.shape.rot.y += SCALE23(DEG_TO_BINANG((animCurFrame + 4) * 0.4f));
    if (self->actor.xzDistToPlayer > 250.0f) {
        Animation_MorphToPlayOnce(&self->skelAnime, 0, 10.0f);
        self->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
        self->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        self->actionFunc = FUN_003b97bc;
    }
}

void FUN_00370634(EnVali* self) {
    Animation_MorphToLoop(&self->skelAnime, 1, -3.0f);
    self->rightTentacleCollider.dim.quad[1] = self->actor.world.pos;
    self->rightTentacleCollider.dim.quad[0] = self->rightTentacleCollider.dim.quad[1];
    self->leftTentacleCollider.dim.quad[1] = self->rightTentacleCollider.dim.quad[0];
    self->leftTentacleCollider.dim.quad[0] = self->leftTentacleCollider.dim.quad[1];
    self->rightTentacleCollider.dim.quad[3] = self->leftTentacleCollider.dim.quad[0];
    self->rightTentacleCollider.dim.quad[2] = self->rightTentacleCollider.dim.quad[3];
    self->leftTentacleCollider.dim.quad[3] = self->rightTentacleCollider.dim.quad[2];
    self->leftTentacleCollider.dim.quad[2] = self->leftTentacleCollider.dim.quad[3];
    self->leftTentacleCollider.dim.quad[2].y = self->leftTentacleCollider.dim.quad[3].y =
        self->rightTentacleCollider.dim.quad[2].y = self->rightTentacleCollider.dim.quad[3].y =
            self->leftTentacleCollider.dim.quad[0].y = self->leftTentacleCollider.dim.quad[1].y =
                self->rightTentacleCollider.dim.quad[0].y = self->rightTentacleCollider.dim.quad[1].y =
                    self->actor.world.pos.y - 10.0f;
    self->actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    self->spinHeight = self->actor.world.pos.y;
    self->collider.base.acFlags |= AC_ON;
    self->spinTimer = 0;
    self->actionFunc = FUN_0011ca08;
}

}
