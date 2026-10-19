extern "C" {
#include "z_en_ssh.hpp"

#if 0
s32 EnSsh_IsCloseToLink(EnSsh* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 yDist;

    if (self->stateFlags & 4) {
        return true;
    }
    if (self->unkTimer != 0) {
        return true;
    }
    if (self->swayTimer != 0) {
        return true;
    }
    if (self->animTimer != 0) {
        return true;
    }
    if (self->actor.xzDistToPlayer > 160.0f) {
        return false;
    }
    yDist = self->actor.world.pos.y - player->actor.world.pos.y;
    if ((yDist < 0.0f) || (yDist > 400.0f)) {
        return false;
    }
    return player->actor.world.pos.y >= self->actor.floorHeight;
}
#endif

GLOBAL_ASM("asm/EnSsh_IsCloseToLink.s")
}
