extern "C" {
#include "z_bg_spot11_bakudankabe.hpp"

#define FLAGS 0x80400030
#define PARAMS_GET_U(params, start, count) (((params) >> (start)) & ((1 << (count)) - 1))

void BgSpot11Bakudankabe_Init(Actor* actor, GameState* state);
void BgSpot11Bakudankabe_Destroy(Actor* actor, GameState* state);
void BgSpot11Bakudankabe_Update(Actor* actor, GameState* state);
void BgSpot11Bakudankabe_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Spot11_Bakudankabe.data.s")
GLOBAL_ASM("../src/game/actors/Bg_Spot11_Bakudankabe/data_syms.s")
extern ColliderCylinderInit BgSpot11Bakudankabe_sCylinderInit;

ActorInit Bg_Spot11_Bakudankabe_InitVars = {
    ACTOR_BG_SPOT11_BAKUDANKABE,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT11_OBJ,
    sizeof(BgSpot11Bakudankabe),
    (ActorFunc)BgSpot11Bakudankabe_Init,
    (ActorFunc)BgSpot11Bakudankabe_Destroy,
    (ActorFunc)BgSpot11Bakudankabe_Update,
    (ActorFunc)BgSpot11Bakudankabe_Draw,
};

static inline void func_808B2180(BgSpot11Bakudankabe* self, PlayState* play) {
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->dyna.actor, &BgSpot11Bakudankabe_sCylinderInit);
    self->collider.dim.pos.x += self->dyna.actor.world.pos.x;
    self->collider.dim.pos.y += self->dyna.actor.world.pos.y;
    self->collider.dim.pos.z += self->dyna.actor.world.pos.z;
}

static inline void func_808B2180x(Actor* thisx, PlayState* play) {
    BgSpot11Bakudankabe* self = (BgSpot11Bakudankabe*)thisx;
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->dyna.actor, &BgSpot11Bakudankabe_sCylinderInit);
    self->collider.dim.pos.x += self->dyna.actor.world.pos.x;
    self->collider.dim.pos.y += self->dyna.actor.world.pos.y;
    self->collider.dim.pos.z += self->dyna.actor.world.pos.z;
}
void BgSpot11Bakudankabe_Init(Actor* actor, GameState* state) {
    BgSpot11Bakudankabe* self = (BgSpot11Bakudankabe*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, 0);
    FUN_00372f38(actor, play, &self->skelAnimModel, 1, 0);
    if (Flags_GetSwitch(play, PARAMS_GET_U(actor->params, 0, 6))) {
        Actor_Kill(actor);
        return;
    }
    func_808B2180x(actor, play);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc((DynaPolyActor*)actor, state, 0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    Actor_SetScale(actor, 1.0f);
}

void BgSpot11Bakudankabe_Destroy(Actor* actor, GameState* state) {
    BgSpot11Bakudankabe* self = (BgSpot11Bakudankabe*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyCylinder(play, &self->collider);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

GLOBAL_ASM("asm/BgSpot11Bakudankabe_Update.s")

void BgSpot11Bakudankabe_Draw(Actor* actor, GameState* state) {
    BgSpot11Bakudankabe* self = (BgSpot11Bakudankabe*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}
}
