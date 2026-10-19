extern "C" {
#include "z_en_skj.hpp"

extern EnSkjUnkStruct EnSkj_sSmallStumpSkullKid;

f32 FUN_0035dd04(EnSkj* self) {
    EnSkj* skullkid = EnSkj_sSmallStumpSkullKid.skullkid;
    f32 xDiff = skullkid->actor.world.pos.x - self->actor.world.pos.x;
    f32 zDiff = skullkid->actor.world.pos.z - self->actor.world.pos.z;

    return sqrtf(SQ(xDiff) + SQ(zDiff)) + 26.0f;
}

}
