extern "C" {
#include "z_en_zf.hpp"

s32 FUN_00369608(PlayState* play, Actor* actor);

s32 FUN_00328cac(PlayState* play, EnZf* self) {
    Player* player = GET_PLAYER(play);
    Actor* focusActor;

    if (self->actor.params >= 0) {
        if (*(u32*)((u8*)player + 0x1710) & 0x6000) {
            return 0;
        }
        return 1;
    }
    if (!FUN_00369608(play, &self->actor)) {
        return 1;
    }
    if (self->actor.params != -2) {
        return 0;
    }
    focusActor = *(Actor**)((u8*)player + 0x16F8);
    if (focusActor == NULL) {
        return 0;
    }
    if (focusActor->type != ACTORCAT_ENEMY) {
        return 1;
    }
    if (focusActor->id != ACTOR_EN_ZF) {
        return 0;
    }
    if (*(u16*)((u8*)focusActor + 0x11A) != 0) {
        return 1;
    }
    return 0;
}

}
