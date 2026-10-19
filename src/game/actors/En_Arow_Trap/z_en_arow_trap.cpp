extern "C" {
#include "z_en_arow_trap.hpp"

#define FLAGS 0x00000010

void EnArowTrap_Init(Actor* actor, GameState* state);
void EnArowTrap_Destroy(Actor* actor, GameState* state);
void EnArowTrap_Update(Actor* actor, GameState* state);

ActorInit En_Arow_Trap_InitVars = {
    ACTOR_EN_AROW_TRAP,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnArowTrap),
    (ActorFunc)EnArowTrap_Init,
    (ActorFunc)EnArowTrap_Destroy,
    (ActorFunc)EnArowTrap_Update,
    NULL,
};

void EnArowTrap_Init(Actor* actor, GameState* state) {
    EnArowTrap* self = (EnArowTrap*)actor;

    Actor_SetScale(actor, 0.01f);
    self->unk_1A4 = 0;
    self->attackTimer = 80;
    actor->focus.pos = actor->world.pos;
}

void EnArowTrap_Destroy(Actor* actor, GameState* state) {
}

void EnArowTrap_Update(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnArowTrap* self = (EnArowTrap*)actor;

    if (self->actor.xzDistToPlayer <= 400.0f) {
        self->attackTimer--;

        if (self->attackTimer == 0) {
            Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ARROW, self->actor.world.pos.x, self->actor.world.pos.y,
                        self->actor.world.pos.z, self->actor.shape.rot.x, self->actor.shape.rot.y,
                        self->actor.shape.rot.z, -1, 1); // ARROW_NORMAL_SILENT
            self->attackTimer = 80;
        }
    }
}
}
