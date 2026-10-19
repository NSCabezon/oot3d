extern "C" {
#include "z_bg_spot17_funen.hpp"

#define FLAGS 0x80400030

void BgSpot17Funen_Init(Actor* actor, GameState* state);
void BgSpot17Funen_Destroy(Actor* actor, GameState* state);

void BgSpot17Funen_Update(Actor* actor, GameState* state);

ActorInit Bg_Spot17_Funen_InitVars = {
    ACTOR_BG_SPOT17_FUNEN,
    ACTORCAT_SWITCH,
    FLAGS,
    OBJECT_SPOT17_OBJ,
    sizeof(BgSpot17Funen),
    (ActorFunc)BgSpot17Funen_Init,
    (ActorFunc)BgSpot17Funen_Destroy,
    (ActorFunc)BgSpot17Funen_Update,
    NULL,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgSpot17Funen_Init(Actor* actor, GameState* state) {
    BgSpot17Funen* self = (BgSpot17Funen*)actor;
    PlayState* play = (PlayState*)state;
    ZARInfo* zar;

    Actor_ProcessInitChain(actor, sInitChain);
    zar = FUN_00372f38(actor, play, &self->skelAnimModel, 2, 0);
    self->skelAnimModel->unk_0C->FUN_00372d94(zar->GetCMABByIndex(0));
    self->skelAnimModel->unk_0C->SetAnimType(1);
}

void BgSpot17Funen_Destroy(Actor* actor, GameState* state) {
    BgSpot17Funen* self = (BgSpot17Funen*)actor;

    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void FUN_002ad4d4(Actor* actor, GameState* state);
void FUN_002ad610(Actor* actor, GameState* state);

void FUN_002ad610(Actor* actor, GameState* state) {}

void BgSpot17Funen_Update(Actor* actor, GameState* state) {
    actor->draw = (ActorFunc)FUN_002ad4d4;
    actor->update = (ActorFunc)FUN_002ad610;
}

GLOBAL_ASM("../split/asm/FUN_0039c184.s")
GLOBAL_ASM("../split/asm/FUN_0039c220.s")
GLOBAL_ASM("../split/asm/FUN_0039c2d8.s")
GLOBAL_ASM("../split/asm/FUN_0039c390.s")
}
