extern "C" {
#include "z_en_arrow.hpp"

GLOBAL_ASM("../src/game/actors/En_Arrow/data_syms.s")

struct EnArrowPlayerView {
    Actor actor;
    char pad[0x247E - sizeof(Actor)];
    u8 projectileState;
};
typedef char EnArrowPlayerProjectileOffsetCheck[(offsetof(EnArrowPlayerView, projectileState) == 0x247E) ? 1 : -1];

s32 Player_InBlockingCsMode(PlayState* play, Player* player);
void FUN_00365d20(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* prim, Color_RGBA8* env,
                  s32 life, s32 scale, s32 unk);
extern Color_RGBA8 EnArrow_sFlameColors[2];

#if 0 // Sol best: ABI-correct464/468B16words; whole-TU guard-base relationship unresolved.
void EnArrow_Update(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnArrow* self = (EnArrow*)actor;
    static Vector3f velocity(0.5f, 0.0f, 0.5f);
    static Vector3f accel(0.5f, 0.0f, 0.5f);
    Player* player = GET_PLAYER(play);

    if (self->isCsNut || (self->actor.params >= 0 && ((EnArrowPlayerView*)player)->projectileState != 0) || !Player_InBlockingCsMode(play, player)) {
        self->actionFunc(self, play);
    }

    if (self->actor.params >= 3 && self->actor.params <= 8) {
        s16 actorIds[6] = {ACTOR_ARROW_FIRE, ACTOR_ARROW_ICE, ACTOR_ARROW_LIGHT,
                           ACTOR_ARROW_FIRE, ACTOR_ARROW_FIRE, ACTOR_ARROW_FIRE};
        if (self->actor.child == NULL) {
            Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, actorIds[self->actor.params - 3], self->actor.world.pos.x,
                               self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0);
        }
    } else if (self->actor.params == 0) {
        FUN_00365d20(play, &self->flamePos, (Vec3f*)&velocity, (Vec3f*)&accel, &EnArrow_sFlameColors[0],
                     &EnArrow_sFlameColors[1], 100, 0, 8);
    }
}
#endif
}
