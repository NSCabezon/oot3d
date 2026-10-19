extern "C" {
#include "z_en_ssh.hpp"

f32 EnSsh_SetAnimation(EnSsh* self, s32 animIndex);

s32 EnSsh_IsCloseToLink(EnSsh* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 yDist;

    if (self->stateFlags & 4) {
        return true;
    }
    if (self->unkTimer != 0) {
        return true;
    }
    if (self->swayTimer != 0) {
        return true;
    }
    if (self->animTimer != 0) {
        return true;
    }
    if (self->actor.xzDistToPlayer > 160.0f) {
        return false;
    }
    yDist = self->actor.world.pos.y - player->actor.world.pos.y;
    if ((yDist < 0.0f) || (yDist > 400.0f)) {
        return false;
    }
    if (player->actor.world.pos.y < self->actor.floorHeight) {
        return false;
    }
    return true;
}



void EnSsh_Return(EnSsh* self, PlayState* play);

void EnSsh_Drop(EnSsh* self, PlayState* play);
void EnSsh_Wait(EnSsh* self, PlayState* play);
void EnSsh_Land(EnSsh* self, PlayState* play);
void EnSsh_Idle(EnSsh* self, PlayState* play);
void Audio_PlayActorSound2(Actor* actor, s32 soundId);
void FUN_00369b58(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s32 radius, s32 scale, s32 life);

static inline s32 EnSsh_IsCloseToHome(EnSsh* self) {
    f32 vel = self->actor.velocity.y;
    f32 nextY = self->actor.world.pos.y + 2.0f * vel;
    if ((self->actor.home.pos.y - nextY) <= 0.0f) {
        return 1;
    }
    return 0;
}

void EnSsh_Return(EnSsh* self, PlayState* play) {
    f32 frameRatio = self->skelAnime.curFrame / (self->skelAnime.animLength - 1.0f);

    if (frameRatio == 1.0f) {
        Audio_PlayActorSound2(&self->actor, 0x010002D9);
        EnSsh_SetAnimation(self, 1);
    }
    if (EnSsh_IsCloseToLink(self, play)) {
        if (self->unkTimer == 0) {
            self->animTimer = (s16)EnSsh_SetAnimation(self, 4);
        }
        self->actor.velocity.y = -10.0f;
        self->actionFunc = EnSsh_Drop;
        self->sfxTimer = 0;
    } else {
        if (!EnSsh_IsCloseToHome(self)) {
            self->actor.velocity.y = 4.0f * frameRatio;
        } else {
            EnSsh_SetAnimation(self, 2);
            self->actionFunc = EnSsh_Wait;
        }
    }
}
static inline s32 EnSsh_IsCloseToGround(EnSsh* self) {
    f32 vel = self->actor.velocity.y;
    f32 nextY = self->actor.world.pos.y + 2.0f * vel;
    if ((nextY - self->actor.floorHeight) <= self->floorHeightOffset) {
        return 1;
    }
    return 0;
}

void EnSsh_Drop(EnSsh* self, PlayState* play) {
    s16 timer;

    if ((self->unkTimer != 0) && (--self->unkTimer == 0)) {
        EnSsh_SetAnimation(self, 4);
    }
    if (!EnSsh_IsCloseToLink(self, play)) {
        Audio_PlayActorSound2(&self->actor, 0x010002D9);
        EnSsh_SetAnimation(self, 1);
        self->actionFunc = EnSsh_Return;
    } else {
        if (EnSsh_IsCloseToGround(self)) {
            Vector3f zero(0.0f, 0.0f, 0.0f);
            Vec3f pos;

            pos.x = self->actor.world.pos.x;
            pos.y = self->actor.floorHeight;
            pos.z = self->actor.world.pos.z;
            FUN_00369b58(play, &pos, (Vec3f*)&zero, (Vec3f*)&zero, 100, 220, 8);
            self->actor.world.pos.y = self->floorHeightOffset + self->actor.floorHeight;
            timer = (s16)EnSsh_SetAnimation(self, 3);
            self->animTimer = timer;
            self->actionFunc = EnSsh_Land;
        } else {
            timer = self->sfxTimer;
            if (timer != 0) {
                timer--;
                self->sfxTimer = timer;
            }
            if (timer == 0) {
                Audio_PlayActorSound2(&self->actor, 0x010002D8);
                self->sfxTimer = 3;
            }
        }
    }
}

void EnSsh_Start(EnSsh* self, PlayState* play) {
    f32 bobVel;

    if (EnSsh_IsCloseToGround(self)) {
        self->actor.world.pos.y = self->floorHeightOffset + self->actor.floorHeight;
        self->animTimer = (s16)EnSsh_SetAnimation(self, 3);
        self->stateFlags |= 4;
        self->actionFunc = EnSsh_Land;

        if ((self->unkTimer != 0) && (--self->unkTimer == 0)) {
            EnSsh_SetAnimation(self, 2);
        }
        if ((self->animTimer != 0) && (--self->animTimer == 0)) {
            EnSsh_SetAnimation(self, 2);
        }
        if ((self->actor.floorHeight + self->floorHeightOffset) <= self->actor.world.pos.y) {
            self->actionFunc = EnSsh_Idle;
            self->sfxTimer = 0;
        } else {
            Math_SmoothStepToF(&self->actor.velocity.y, 2.0f, 0.6f, 1000.0f, 0.0f);
        }
    } else {
        self->actionFunc = EnSsh_Wait;
        if (EnSsh_IsCloseToLink(self, play)) {
            if (self->unkTimer == 0) {
                self->animTimer = (s16)EnSsh_SetAnimation(self, 4);
            }
            self->actor.velocity.y = -10.0f;
            self->actionFunc = EnSsh_Drop;
            self->sfxTimer = 0;
        } else {
            bobVel = 0.5f;
            if ((play->state.frames & 8) != 0) {
                bobVel *= -1.0f;
            }
            Math_SmoothStepToF(&self->actor.velocity.y, bobVel, 0.4f, 1000.0f, 0.0f);
        }
    }
}





}
