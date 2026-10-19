// Own TU: the ROM calls FUN_00364b1c (EnWf_ReactToProjectile) out of line from several actions, so armcc must
// not inline the same-file SetupBlock / SetupBackflipBackward / ShortCirclePlayer helpers into it.
extern "C" {
#include "z_en_wf.hpp"

void FUN_0035b578(EnWf* self);
void FUN_00364a2c(EnWf* self);
void FUN_00362f48(EnWf* self, PlayState* play);

Actor* FUN_00346e2c(PlayState* play, Actor* actor, f32 range, f32 unk);
s16 Actor_WorldYawTowardActor(Actor* actor, Actor* other);
f32 Actor_WorldDistXYZToPoint(Actor* actor, Vec3f* point);

// EnWf_ReactToProjectile (N64 shape): (PlayState*, EnWf*) signature as the ROM passes it.
s32 FUN_00364b1c(PlayState* play, EnWf* self) {
    Actor* projectile = FUN_00346e2c(play, &self->actor, 600.0f, 1.0f);
    s16 relYaw;
    f32 projectileDist;

    if (projectile != NULL) {
        relYaw = Actor_WorldYawTowardActor(&self->actor, projectile) - self->actor.shape.rot.y;
        self->actor.world.rot.y = self->actor.shape.rot.y + 0;
        projectileDist = Actor_WorldDistXYZToPoint(&self->actor, &projectile->world.pos);
        if ((ABS(relYaw) < 0x2EE0) && (sqrtf(projectileDist) < 400.0f)) {
            FUN_0035b578(self);
        } else {
            self->actor.world.rot.y = self->actor.shape.rot.y + 0x3FFF;
            if ((ABS(relYaw) < 0x2000) || (ABS(relYaw) >= 0x6000)) {
                FUN_00362f48(self, play);
                self->actor.speedXZ *= 2.0f;
            } else if (ABS(relYaw) < 0x5FFF) {
                FUN_00364a2c(self);
            }
        }
        return 1;
    }
    return 0;
}

}
