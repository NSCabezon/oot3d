extern "C" {
#include "z_en_zf.hpp"

s32 FUN_003740fc(Actor* actor, PlayState* play, f32 speed) {
    Vec3f savedPos;
    u16 savedFlags;
    f32 sinSpeed;
    f32 cosSpeed;
    Actor* thisx = actor;

    if (speed == 0.0f) {
        speed = ((actor->speedXZ >= 0.0f) ? 1.0f : -1.0f) * ((actor->params >= 0) ? 45.0f : 30.0f);
    }
    savedPos = actor->world.pos;
    savedFlags = actor->bgCheckFlags;
    sinSpeed = Math_SinS(actor->world.rot.y) * speed;
    cosSpeed = Math_CosS(actor->world.rot.y) * speed;
    actor->world.pos.x += sinSpeed;
    actor->world.pos.z += cosSpeed;
    Actor_UpdateBgCheckInfo(play, thisx, 0.0f, 0.0f, 0.0f, 0x1C);
    actor->world.pos = savedPos;
    {
        u16 flags = actor->bgCheckFlags;
        actor->bgCheckFlags = savedFlags;
        return !(flags & 1);
    }
}

}
