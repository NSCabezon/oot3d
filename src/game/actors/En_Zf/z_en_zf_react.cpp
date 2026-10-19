extern "C" {
#include "z_en_zf.hpp"

Actor* FUN_00346e2c(PlayState* play, Actor* actor, f32 range, f32 range2);
s32 FUN_003740fc(Actor* actor, PlayState* play, f32 speed);
void FUN_00328d50(EnZf* self);
void FUN_003170f4(EnZf* self, f32 speed);
s16 Actor_WorldYawTowardActor(Actor* actor, Actor* target);

s32 FUN_0032fdf8(PlayState* play, EnZf* self) {
    Actor* projectile = FUN_00346e2c(play, &self->actor, 600.0f, 1.0f);
    s16 relYawTowardsProjectile;
    s16 noFloorAheadFlags;
    s16 directionFlags;

    if (projectile != NULL) {
        relYawTowardsProjectile = Actor_WorldYawTowardActor(&self->actor, projectile) - (s16)(self->actor.shape.rot.y + 0);
        self->actor.world.rot.y = self->actor.shape.rot.y + 0x3FFF;
        noFloorAheadFlags = 0;
        if (FUN_003740fc(&self->actor, play, -8.0f)) {
            noFloorAheadFlags |= 1;
        }
        if (FUN_003740fc(&self->actor, play, 8.0f)) {
            noFloorAheadFlags |= 2;
        }
        self->actor.world.rot.y = self->actor.shape.rot.y;
        if ((((self->actor.xzDistToPlayer < 90.0f) || (noFloorAheadFlags == 3)) && !FUN_003740fc(&self->actor, play, 135.0f)) ||
            (projectile->id == 0x66)) {
            FUN_00328d50(self);
        } else {
            self->actor.world.rot.y = self->actor.shape.rot.y + 0x3FFF;
            if (noFloorAheadFlags == 0) {
                directionFlags = *(u32*)((u8*)play + 0x5BF4) & 1;
            } else {
                directionFlags = noFloorAheadFlags;
            }
            if ((ABS(relYawTowardsProjectile) < 0x2000) || (ABS(relYawTowardsProjectile) >= 0x6000)) {
                if (directionFlags & 1) {
                    FUN_003170f4(self, 8.0f);
                } else {
                    FUN_003170f4(self, -8.0f);
                }
            } else if (ABS(relYawTowardsProjectile) < 0x5FFF) {
                if (directionFlags & 1) {
                    FUN_003170f4(self, 4.0f);
                } else {
                    FUN_003170f4(self, -4.0f);
                }
            }
        }
        return 1;
    }
    return 0;
}

}
