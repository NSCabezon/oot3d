extern "C" {
#include "z_en_zf.hpp"

extern s16 EnZf_sPairedFlags[];

s32 FUN_0032fbc0(PlayState* play, EnZf* self);
s32 FUN_0032fdf8(PlayState* play, EnZf* self);
s32 FUN_003740fc(Actor* actor, PlayState* play, f32 speed);
s32 FUN_003731e0(SkelAnime* skelAnime);
s32 FUN_00328cac(PlayState* play, EnZf* self);
void FUN_00328c18(EnZf* self, PlayState* play);
void FUN_00328d50(EnZf* self);
void FUN_003301b8(EnZf* self);

void FUN_00284fd0(EnZf* self, PlayState* play) {
    s16 absRelWallYaw;

    if (self->actor.bgCheckFlags & 2) {
        self->actor.speedXZ = 0.0f;
    }
    if (self->actor.bgCheckFlags & 1) {
        if (self->actor.speedXZ < 0.0f) {
            self->actor.speedXZ += 0.05f;
        }
        self->actionState = 0;
    }
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0x1194, 0);
    if (!((self->actor.params == -2) && FUN_0032fbc0(play, self)) && FUN_003731e0(&self->skelAnime) &&
        (self->actor.bgCheckFlags & 1)) {
        if (EnZf_sPairedFlags[1] != -1) {
            if (self->lastDamageReaction == 13) {
                EnZf_sPairedFlags[0] += 1;
            } else {
                self->actor.world.rot.y = self->actor.shape.rot.y;
                if (!FUN_003740fc(&self->actor, play, 135.0f) && (self->actor.xzDistToPlayer < 90.0f)) {
                    FUN_00328d50(self);
                } else if ((self->actor.xzDistToPlayer <= 100.0f) && ((*(u32*)((u8*)play + 0x5BF4) % 4) == 0)) {
                    FUN_003301b8(self);
                } else {
                    FUN_00328c18(self, play);
                }
            }
        } else {
            absRelWallYaw = self->actor.wallYaw - self->actor.shape.rot.y;
            absRelWallYaw = ABS(absRelWallYaw);
            if ((self->actor.params == -2) && (self->actor.bgCheckFlags & 8) && (ABS(absRelWallYaw) < 0x2EE0) &&
                (self->actor.xzDistToPlayer < 90.0f)) {
                FUN_00328d50(self);
            } else if (!FUN_0032fdf8(play, self)) {
                if (self->actor.params != -2) {
                    self->actor.world.rot.y = self->actor.shape.rot.y;
                    if (!FUN_003740fc(&self->actor, play, 135.0f) && (self->actor.xzDistToPlayer < 90.0f)) {
                        FUN_00328d50(self);
                    } else if ((self->actor.xzDistToPlayer <= 100.0f) && ((*(u32*)((u8*)play + 0x5BF4) % 4) == 0)) {
                        FUN_003301b8(self);
                    } else {
                        FUN_00328c18(self, play);
                    }
                } else if ((self->actor.xzDistToPlayer <= 100.0f) && ((*(u32*)((u8*)play + 0x5BF4) % 4) == 0) &&
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
