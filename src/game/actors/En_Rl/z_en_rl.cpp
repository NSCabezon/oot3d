extern "C" {
#include "z_en_rl.hpp"

#define FLAGS 0x00000010

void EnRl_Init(Actor* actor, PlayState* play);
void EnRl_Destroy(Actor* actor, PlayState* play);
void EnRl_Update(Actor* actor, PlayState* play);
void EnRl_Draw(Actor* actor, PlayState* play);

GLOBAL_ASM("data/z_En_Rl.data.s")
GLOBAL_ASM("../src/game/actors/En_Rl/EnRl_data.s")

// Tables live in the data dump above; EnRl_data.s gives them names (EQU).
extern EnRlActionFunc EnRl_sActionFuncs[];
extern EnRlActionFunc EnRl_sDrawFuncs[];

ActorInit En_Rl_InitVars = {
    ACTOR_EN_RL,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_RL,
    sizeof(EnRl),
    (ActorFunc)EnRl_Init,
    (ActorFunc)EnRl_Destroy,
    (ActorFunc)EnRl_Update,
    (ActorFunc)EnRl_Draw,
};

// Inline wrapper: passing the shadow scale as a float arg orders `add r0,#0xbc` before the vldr loads.
static inline void EnRl_InitShape(ActorShape* shape, f32 shadowScale) {
    ActorShape_Init(shape, 0.0f, ActorShadow_DrawCircle, shadowScale);
}

void EnRl_Init(Actor* actor, PlayState* play) {
    EnRl* self = (EnRl*)actor;
    ActorShape* shape;

    EnRl_InitShape(&actor->shape, 50.0f);
    if (actor->params == 2) {
        shape = &actor->shape;
        FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
        FUN_00353c9c(actor, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 0x17);
        SkelAnime_SetFaceAnimations(&self->faceAnims, &self->skelAnime, 0, -1, -1);
        self->action = 4;
        shape->shadowAlpha = 0;
    } else {
        FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
        FUN_00353c9c(actor, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 0x17);
        SkelAnime_SetFaceAnimations(&self->faceAnims, &self->skelAnime, 0, -1, -1);
    }
}

void EnRl_Destroy(Actor* actor, PlayState* play) {
    EnRl* self = (EnRl*)actor;

    FUN_003508b8(actor, self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
    self->faceAnims.Destroy();
}

void EnRl_Update(Actor* actor, PlayState* play) {
    EnRl* self = (EnRl*)actor;

    if ((u32)self->action < 8) {
        EnRlActionFunc* table = EnRl_sActionFuncs;
        EnRlActionFunc fn = table[self->action];

        if (fn != NULL) {
            fn(self, play);
        }
    }
}

void EnRl_Draw(Actor* actor, PlayState* play) {
    EnRl* self = (EnRl*)actor;

    if ((u32)self->drawConfig < 3) {
        EnRlActionFunc* table = EnRl_sDrawFuncs;
        EnRlActionFunc fn = table[self->drawConfig];

        if (fn != NULL) {
            fn(self, play);
        }
    }
}
}
