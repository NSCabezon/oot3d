extern "C" {
#include "z_en_tana.hpp"

#define FLAGS 0x80400009

void EnTana_Init(Actor* actor, GameState* state);
void EnTana_Destroy(Actor* actor, GameState* state);
void EnTana_Update(Actor* actor, GameState* state);

void FUN_001f84ec(Actor* actor, GameState* state);
void FUN_00212df4(Actor* actor, GameState* state);

ActorInit En_Tana_InitVars = {
    ACTOR_EN_TANA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_SHOP_DUNGEN,
    sizeof(EnTana),
    (ActorFunc)EnTana_Init,
    (ActorFunc)EnTana_Destroy,
    (ActorFunc)EnTana_Update,
    NULL,
};

static ActorFunc sDrawFuncs[] = {
    FUN_001f84ec,
    FUN_00212df4,
    FUN_00212df4,
};

static s32 sModelIndices[] = { 0, 1, 2 };

void EnTana_Init(Actor* actor, GameState* state) {
    EnTana* self = (EnTana*)actor;
    PlayState* play = (PlayState*)state;

    FUN_00372f38(actor, play, &self->skelAnimModel, sModelIndices[actor->params], 0);
    Actor_SetScale(actor, 1.0f);
    actor->flags &= ~1;
    actor->draw = sDrawFuncs[actor->params];
}

void EnTana_Destroy(Actor* actor, GameState* state) {
    EnTana* self = (EnTana*)actor;

    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void EnTana_Update(Actor* actor, GameState* state) {
}
}
