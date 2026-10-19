extern "C" {
#include "z_en_bili.hpp"

void EnBili_FloatIdle(EnBili* self, PlayState* play);

void FUN_0036f1b4(EnBili* self) {
    self->actor.speedXZ = 0.7f;
    self->collider.info.bumper.effect = 1;
    self->timer = 48;
    self->actor.home.pos.y = self->actor.world.pos.y;
    self->actor.gravity = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->collider.base.atFlags |= AT_ON;
    self->collider.base.acFlags |= AC_ON;
    self->actionFunc = EnBili_FloatIdle;
}

}
