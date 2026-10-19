extern "C" {
#include "z_en_zf.hpp"

void FUN_0034f724(PlayState* play);
void FUN_0034eb00(EnZf* self);
void FUN_00380604(EnZf* self, PlayState* play);
s16 FUN_00373fa4(Vec3f* pos, s16 preferred);
s16 FUN_00373d98(Vec3f* pos, s16 curPlatform, s16 homePlatform, PlayState* play);
s32 FUN_003731e0(SkelAnime* skelAnime);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void Audio_PlayActorSound2(Actor* actor, s32 sfxId);
void FUN_0036f00c(PlayState* play, Actor* actor, Vec3f* pos, f32 a, s32 b, f32 c, s32 d, s32 e, s32 f);

void FUN_0038894c(EnZf* self, PlayState* play) {
    if (self->timer == 2) {
        Audio_PlayActorSound2(&self->actor, 0x01000299);
        self->actor.flags |= 1;
        if (self->actor.params == 0) {
            FUN_0034f724(play);
        }
    }
    if (self->timer != 0) {
        if (self->actor.params != -1) {
            self->timer--;
        } else if (self->actor.xzDistToPlayer <= 160.0f) {
            self->timer = 0;
            self->actor.flags |= 1;
            Audio_PlayActorSound2(&self->actor, 0x01000299);
        }
        self->actor.world.pos.y = self->actor.floorHeight + 300.0f;
    } else {
        if (self->alpha < 255) {
            self->alpha += 51;
        }
    }
    if ((self->actor.bgCheckFlags & 3) && (self->actionState != 0)) {
        Audio_PlayActorSound2(&self->actor, 0x01000298);
        Animation_Change_00375c08(&self->skelAnime, 1, 1.0f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 1), 2, 0.0f);
        self->actor.world.pos.y = self->actor.floorHeight;
        self->actor.velocity.y = 0.0f;
        self->actionState = 0;
        self->actor.bgCheckFlags &= ~2;
        FUN_0036f00c(play, &self->actor, &self->leftFootPos, 3.0f, 2, 2.0f, 0, 0, 0);
        FUN_0036f00c(play, &self->actor, &self->rightFootPos, 3.0f, 2, 2.0f, 0, 0, 0);
    }
    if (FUN_003731e0(&self->skelAnime)) {
        self->alpha = 255;
        if (self->actor.params > 0) {
            f32 animMorphFrames = 1.0f;

            if (self->action < 0x11) {
                animMorphFrames = -4.0f;
            }
            Animation_Change_00375c08(&self->skelAnime, 8, 2.0f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 8), 2, animMorphFrames);
            self->actor.speedXZ = 0.0f;
            self->action = 0x12;
            self->curPlatform = FUN_00373fa4(&self->actor.world.pos, self->curPlatform);
            self->targetPlatform = FUN_00373d98(&self->actor.world.pos, self->curPlatform, self->homePlatform, play);
            self->actor.world.rot.y = self->actor.shape.rot.y;
            self->actionFunc = FUN_00380604;
        } else {
            FUN_0034eb00(self);
        }
    }
    self->actor.shape.shadowAlpha = self->alpha;
}

}
