extern "C" {
#include "z_en_zf.hpp"

extern s16 EnZf_sPairedFlags[];

s32 FUN_0032fbc0(PlayState* play, EnZf* self);
s32 FUN_0032fdf8(PlayState* play, EnZf* self);
s32 FUN_00328cac(PlayState* play, EnZf* self);
void FUN_00328c18(EnZf* self, PlayState* play);
void FUN_00328d50(EnZf* self);
void FUN_003301b8(EnZf* self);
void FUN_0024f670(EnZf* self, PlayState* play);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void Audio_PlayActorSound2(Actor* actor, s32 sfxId);

void FUN_002a71d0(EnZf* self, PlayState* play) {
    s16 absRelWallYaw;
    Actor* other;

    if (self->actor.bgCheckFlags & 2) {
        self->actor.speedXZ = 0.0f;
    }
    if (self->actor.bgCheckFlags & 1) {
        if (self->actor.speedXZ < 0.0f) {
            self->actor.speedXZ += 0.05f;
        }
        self->actionState = 0;
    }
    if ((*(u16*)((u8*)self + 0x11A) == 0) && (self->actor.bgCheckFlags & 1)) {
        if (self->actor.colChkInfo.health == 0) {
            Animation_Change_00375c08(&self->skelAnime, 10, 1.5f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 10), 2, -4.0f);
            if ((self->actor.bgCheckFlags & 1) && ((self->actor.velocity.y == 0.0f) || (self->actor.velocity.y == -4.0f))) {
                self->actor.speedXZ = 0.0f;
                self->actionState = 0;
            } else {
                self->actionState = 1;
            }
            self->action = 15;
            self->actor.flags &= ~1;
            if (EnZf_sPairedFlags[1] != -1) {
                other = self->actor.prev;
                if (other != NULL) {
                    ((EnZf*)other)->timer2 = 135;
                    other = self->actor.prev;
                    if (self->actor.prev->colChkInfo.health < 3) {
                        self->actor.prev->colChkInfo.health = 3;
                    }
                } else {
                    ((EnZf*)self->actor.next)->timer2 = 135;
                    other = self->actor.next;
                    if (other->colChkInfo.health < 3) {
                        other->colChkInfo.health = 3;
                    }
                }
            }
            EnZf_sPairedFlags[0] = 0;
            Audio_PlayActorSound2(&self->actor, 0x0100029D);
            self->actionFunc = FUN_0024f670;
        } else if (!((self->actor.params == -2) && FUN_0032fbc0(play, self))) {
            if (EnZf_sPairedFlags[1] != -1) {
                FUN_00328c18(self, play);
            } else {
                absRelWallYaw = self->actor.wallYaw - self->actor.shape.rot.y;
                absRelWallYaw = ABS(absRelWallYaw);
                if ((self->actor.params == -2) && (self->actor.bgCheckFlags & 8) && (ABS(absRelWallYaw) < 0x2EE0) &&
                    (self->actor.xzDistToPlayer < 90.0f)) {
                    self->actor.world.rot.y = self->actor.shape.rot.y;
                    FUN_00328d50(self);
                } else if (!FUN_0032fdf8(play, self)) {
                    if (self->actor.params != -2) {
                        FUN_00328c18(self, play);
                    } else if ((self->actor.xzDistToPlayer <= 100.0f) && ((*(u32*)((u8*)play + 0x5BF4) % 4) != 0) &&
                               FUN_00328cac(play, self)) {
                        FUN_003301b8(self);
                    } else {
                        FUN_00328c18(self, play);
                    }
                }
            }
        }
    }
}

}
