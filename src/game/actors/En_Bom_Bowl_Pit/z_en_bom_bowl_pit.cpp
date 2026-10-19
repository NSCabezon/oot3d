extern "C" {
#include "z_en_bom_bowl_pit.hpp"

#define FLAGS 0x80000010

void EnBomBowlPit_Init(Actor* actor, GameState* state);
void EnBomBowlPit_Destroy(Actor* actor, GameState* state);
void EnBomBowlPit_Update(Actor* actor, GameState* state);

void FUN_003e870c(EnBomBowlPit* self, PlayState* play); // N64: EnBomBowlPit_WaitGameStarted

GLOBAL_ASM("data/z_En_Bom_Bowl_Pit.data.s")

ActorInit En_Bom_Bowl_Pit_InitVars = {
    ACTOR_EN_BOM_BOWL_PIT,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnBomBowlPit),
    (ActorFunc)EnBomBowlPit_Init,
    (ActorFunc)EnBomBowlPit_Destroy,
    (ActorFunc)EnBomBowlPit_Update,
    NULL,
};

void EnBomBowlPit_Init(Actor* actor, GameState* state) {
    EnBomBowlPit* self = (EnBomBowlPit*)actor;

    self->actor.unk_19A = 1;
    self->actionFunc = FUN_003e870c;
}

void EnBomBowlPit_Destroy(Actor* actor, GameState* state) {
}

void EnBomBowlPit_Update(Actor* actor, GameState* state) {
    EnBomBowlPit* self = (EnBomBowlPit*)actor;

    self->actionFunc(self, (PlayState*)state);

    if (self->timer != 0) {
        self->timer--;
    }
}
}
