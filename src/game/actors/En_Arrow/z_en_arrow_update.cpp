extern "C" {
#include "z_en_arrow.hpp"

GLOBAL_ASM("../src/game/actors/En_Arrow/data_syms.s")

struct EnArrowPlayerView {
    Actor actor;
    char pad[0x247E - sizeof(Actor)];
    u8 projectileState;
};
typedef char EnArrowPlayerProjectileOffsetCheck[(offsetof(EnArrowPlayerView, projectileState) == 0x247E) ? 1 : -1];

s32 Player_InBlockingCsMode(PlayState* play);
void FUN_00365d20(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* prim, Color_RGBA8* env,
                  s32 life, s32 scale, s32 unk);
extern Color_RGBA8 EnArrow_sFlamePrimColor;
extern Color_RGBA8 EnArrow_sFlameEnvColor;

#if 0 // Luna first-pass draft: five variants, best semantically correct candidate 460/468B,66/117 words.
void EnArrow_Update(Actor* actor, GameState* state) {
    EnArrow* self = (EnArrow*)actor;
    PlayState* play = (PlayState*)state;
    static Vector3f velocity(0.5f, 0.0f, 0.5f);
    static Vector3f accel(0.5f, 0.0f, 0.5f);
    s16 params = self->actor.params;
    EnArrowPlayerView* player = (EnArrowPlayerView*)GET_PLAYER(play);

    if (self->isCsNut || (params >= 0 && player->projectileState != 0) || !Player_InBlockingCsMode(play)) {
        self->actionFunc(self, play);
    }

    if (params >= 3 && params <= 8) {
        if (self->actor.child == NULL) {
            s16 actorIds[6] = {ACTOR_ARROW_FIRE, ACTOR_ARROW_ICE, ACTOR_ARROW_LIGHT,
                               ACTOR_ARROW_FIRE, ACTOR_ARROW_FIRE, ACTOR_ARROW_FIRE};
            Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, actorIds[params - 3], self->actor.world.pos.x,
                               self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0);
        }
    } else if (params == 0) {
        FUN_00365d20(play, &self->flamePos, (Vec3f*)&velocity, (Vec3f*)&accel, &EnArrow_sFlamePrimColor,
                     &EnArrow_sFlameEnvColor, 100, 0, 8);
    }
}
#endif
}
