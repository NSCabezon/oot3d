extern "C" {
#include "z_en_zf.hpp"

s32 FUN_003731e0(SkelAnime* skelAnime);
void FUN_0034eb00(EnZf* self);
void FUN_0032ffc0(EnZf* self, PlayState* play);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_0032fb3c(EnZf* self, PlayState* play);
s32 FUN_00328cac(PlayState* play, EnZf* self);
void FUN_003301b8(EnZf* self);

void FUN_002b4a18(EnZf* self, PlayState* play) {
    if (self->timer != 0) {
        if (self->actor.floorHeight >= self->actor.world.pos.y) {
            self->actor.world.pos.y = self->actor.floorHeight;
            self->actor.velocity.y = 0.0f;
            self->actor.speedXZ = 0.0f;
            self->actionState = 0;
        }
    }
    if (FUN_003731e0(&self->skelAnime)) {
        if (self->timer == 0) {
            Animation_Change_00375c08(&self->skelAnime, 1, 3.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 1), 2, -3.0f);
            self->timer = 15;
            Audio_PlayActorSound2(&self->actor, 0x0100029F);
        } else if ((*(u32*)((u8*)play + 0x5BF4) % 2) != 0) {
            FUN_0032ffc0(self, play);
        } else {
            FUN_0034eb00(self);
        }
    }
    if ((*(u32*)((u8*)play + 0xF8) & 0x5F) == 0) {
        Audio_PlayActorSound2(&self->actor, 0x01000299);
    }
}


void FUN_0029664c(EnZf* self, PlayState* play) {
    if (self->timer != 0) {
        if (self->actor.floorHeight >= self->actor.world.pos.y) {
            self->actor.world.pos.y = self->actor.floorHeight;
            self->actor.velocity.y = 0.0f;
            self->actor.speedXZ = 0.0f;
            self->actionState = 0;
        }
    }
    if (FUN_003731e0(&self->skelAnime)) {
        if (self->timer == 0) {
            Animation_Change_00375c08(&self->skelAnime, 1, 3.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 1), 2, -3.0f);
            self->timer = 15;
            Audio_PlayActorSound2(&self->actor, 0x0100029F);
        } else {
            self->actor.speedXZ = 0.0f;
            self->actionState = 0;
            FUN_0032fb3c(self, play);
        }
    }
    if ((*(u32*)((u8*)play + 0x5BF4) & 0x5F) == 0) {
        Audio_PlayActorSound2(&self->actor, 0x01000299);
    }
    if ((self->actor.params == -2) && (self->actor.bgCheckFlags & 3)) {
        if (FUN_00328cac(play, self)) {
            FUN_003301b8(self);
        } else {
            FUN_0032ffc0(self, play);
        }
    }
}

}
