extern "C" {
#include "z_en_am.hpp"

#define UPDBGCHECKINFO_FLAG_2 (1 << 2)
#define BGCHECKFLAG_GROUND (1 << 0)

s32 FUN_00363108(Actor* thisx, PlayState* play, f32 distance, s16 yaw) {
    EnAm* self = (EnAm*)thisx;
    s16 ret;
    s16 curBgCheckFlags;
    f32 sin;
    f32 cos;
    Vec3f curPos;

    curPos = self->dyna.actor.world.pos;
    curBgCheckFlags = self->dyna.actor.bgCheckFlags;

    sin = Math_SinS(yaw) * distance;
    cos = Math_CosS(yaw) * distance;

    self->dyna.actor.world.pos.x += sin;
    self->dyna.actor.world.pos.z += cos;

    Actor_UpdateBgCheckInfo(play, &self->dyna.actor, 0.0f, 0.0f, 0.0f, UPDBGCHECKINFO_FLAG_2);
    self->dyna.actor.world.pos = curPos;
    ret = self->dyna.actor.bgCheckFlags & BGCHECKFLAG_GROUND;

    if (!ret && (self->dyna.actor.floorHeight >= (self->homePos.y - 20.0f))) {
        ret = 1;
    }

    self->dyna.actor.bgCheckFlags = curBgCheckFlags;

    return ret;
}

}
