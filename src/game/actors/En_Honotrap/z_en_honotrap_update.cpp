extern "C" {
#include "z_en_honotrap.hpp"
#include <math.h>

// Own TU: EnHonotrap_Update has a guarded function-local static whose guard is pinned at its ROM address
// (pin_static --guard-sel "z_en_honotrap_update.o(+DATA)"); the actor TU's .data sits elsewhere.
void EffectSsBomb2_SpawnLayered(PlayState* play, Vector3f* pos, Vector3f* velocity, Vector3f* accel, s16 scale, s16 duration);
void EnHonotrap_Update(Actor* thisx, PlayState* play) {
    EnHonotrap* self = (EnHonotrap*)thisx;

    if (self->timer > 0) {
        self->timer--;
    }
    if (self->actor.params == 0) {
        if ((self->actor.child != NULL) && (self->actor.child->update == NULL)) {
            self->actor.child = NULL;
        }
    } else {
        self->colChkFlags = 0;
        self->bobPhase += 0x640;
        self->actor.shape.yOffset = (Math_SinS(self->bobPhase) * 1000.0f) + 600.0f;
        Actor_SetFocus(&self->actor, 5.0f);
        Audio_PlayActorSound2(&self->actor, 0x010001B2);
    }
    self->actionFunc(self, play);
    if (thisx->params == 0) {
        if (self->collider.tris.base.acFlags & AC_HIT) {
            static Vector3f sVelocity(0.0f, 0.0f, 0.0f);
            static Vector3f sAccel(0.0f, 0.1f, 0.0f);
            EffectSsBomb2_SpawnLayered(play, (Vector3f*)&self->actor.world.pos, &sVelocity, &sAccel, 15, 8);
            Actor_Kill(&self->actor);
        } else if (self->eyeState < 3) {
            self->collider.tris.base.acFlags &= ~AC_HIT;
            CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.tris.base);
        }
    }
}

}
