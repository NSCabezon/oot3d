extern "C" {
#include "z_en_dy_extra.hpp"

#define FLAGS 0x00000030

void EnDyExtra_Init(Actor* actor, GameState* state);
void EnDyExtra_Destroy(Actor* actor, GameState* state);
void EnDyExtra_Update(Actor* actor, GameState* state);
void EnDyExtra_Draw(Actor* actor, GameState* state);

void EnDyExtra_WaitForTrigger(EnDyExtra* self, PlayState* play);
void EnDyExtra_FallAndKill(EnDyExtra* self, PlayState* play);

ActorInit En_Dy_Extra_InitVars = {
    ACTOR_EN_DY_EXTRA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_DY_OBJ,
    sizeof(EnDyExtra),
    (ActorFunc)EnDyExtra_Init,
    (ActorFunc)EnDyExtra_Destroy,
    (ActorFunc)EnDyExtra_Update,
    (ActorFunc)EnDyExtra_Draw,
};

void Actor_MoveXZGravity(Actor* actor);

GLOBAL_ASM("asm/EnDyExtra_Init.s")

#if 0
void EnDyExtra_Init(Actor* actor, GameState* state) {
    EnDyExtra* self = (EnDyExtra*)actor;
    PlayState* play = (PlayState*)state;
    ZARInfo* zar;

    self->type = self->actor.params;
    self->scale.x = 0.025f;
    self->scale.y = 0.039f;
    self->scale.z = 0.025f;
    self->homePos = self->actor.world.pos;
    self->actor.gravity = -0.2f;
    self->alphaFac = 1.0f;
    self->timer = 90;
    self->actionFunc = EnDyExtra_WaitForTrigger;
    zar = FUN_00372f38(&self->actor, play, &self->skelAnimModel, 2, 0);
    if (self->type == 0) {
        self->skelAnimModel->unk_0C->FUN_00372d94(zar->GetCMABByIndex(2));
    } else {
        self->skelAnimModel->unk_0C->FUN_00372d94(zar->GetCMABByIndex(3));
    }
    self->skelAnimModel->unk_0C->animType = 1;
    self->skelAnimModel->unk_0C->animSpeed = 2.0f;
}
#endif

void EnDyExtra_Destroy(Actor* actor, GameState* state) {
    EnDyExtra* self = (EnDyExtra*)actor;
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

GLOBAL_ASM("asm/EnDyExtra_Update.s")

#if 0
void EnDyExtra_Update(Actor* actor, GameState* state) {
    EnDyExtra* self = (EnDyExtra*)actor;
    PlayState* play = (PlayState*)state;

    DECR(self->timer);
    actor->scale.x = self->scale.x;
    actor->scale.y = self->scale.y;
    actor->scale.z = self->scale.z;
    self->actionFunc(self, play);
    Actor_MoveXZGravity(actor);
}
#endif

GLOBAL_ASM("asm/EnDyExtra_Draw.s")

void EnDyExtra_FallAndKill(EnDyExtra* self, PlayState* play) {
    Math_ApproachF(&self->actor.gravity, 0.0f, 0.1f, 0.005f);
    if (self->alphaFac > 0.07f) {
        Audio_PlayActorSound2(&self->actor, 0x010000C5);
    }
    if ((self->timer == 0) || (self->alphaFac < 0.02f)) {
        Actor_Kill(&self->actor);
        return;
    }
    Math_ApproachZeroF(&self->alphaFac, 0.03f, 0.05f);
    if (self->actor.world.pos.y < -55.0f) {
        self->actor.velocity.y = 0.0f;
    }
}

void EnDyExtra_WaitForTrigger(EnDyExtra* self, PlayState* play) {
    Audio_PlayActorSound2(&self->actor, 0x010000C5);
    Math_ApproachF(&self->actor.gravity, 0.0f, 0.1f, 0.005f);
    if (self->actor.world.pos.y < -55.0f) {
        self->actor.velocity.y = 0.0f;
    }
    if ((self->timer == 0) && self->startDisappearing) {
        self->timer = 300;
        self->actionFunc = EnDyExtra_FallAndKill;
    }
}

}
