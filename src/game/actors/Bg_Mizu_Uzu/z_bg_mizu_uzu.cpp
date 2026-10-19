extern "C" {
#include "z_bg_mizu_uzu.hpp"

#define FLAGS 0x00000000

void BgMizuUzu_Init(Actor* actor, GameState* state);
void BgMizuUzu_Destroy(Actor* actor, GameState* state);
void BgMizuUzu_Update(Actor* actor, GameState* state);
void BgMizuUzu_Draw(Actor* actor, GameState* state);

void FUN_001273e8(BgMizuUzu* self, PlayState* play); // N64: func_8089F788

ActorInit Bg_Mizu_Uzu_InitVars = {
    ACTOR_BG_MIZU_UZU,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_MIZU_OBJECTS,
    sizeof(BgMizuUzu),
    (ActorFunc)BgMizuUzu_Init,
    (ActorFunc)BgMizuUzu_Destroy,
    (ActorFunc)BgMizuUzu_Update,
    (ActorFunc)BgMizuUzu_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneScale, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgMizuUzu_Init(Actor* actor, GameState* state) {
    BgMizuUzu* self = (BgMizuUzu*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, 0);
    actor->unk_19A = 1;
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 10), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    self->actionFunc = FUN_001273e8;
}

void BgMizuUzu_Destroy(Actor* actor, GameState* state) {
    DynaPoly_DeleteBgActor((PlayState*)state, &((PlayState*)state)->colCtx.dyna, ((BgMizuUzu*)actor)->dyna.bgId);
}

void FUN_001273e8(BgMizuUzu* self, PlayState* play) {
    if (((u8*)GET_PLAYER(play))[0x1A7] == 1) { // player->currentBoots == PLAYER_BOOTS_IRON
        FUN_0036b940(play, &play->colCtx.dyna, self->dyna.bgId); // DynaPoly_DisableCollision
    } else {
        FUN_0036d15c(play, &play->colCtx.dyna, self->dyna.bgId); // DynaPoly_EnableCollision
    }
    Audio_PlayActorSound2(&self->dyna.actor, 0x01000224); // NA_SE_EV_WATER_CONVECTION
    self->dyna.actor.shape.rot.y += 0x12B;
}

void BgMizuUzu_Update(Actor* actor, GameState* state) {
    BgMizuUzu* self = (BgMizuUzu*)actor;

    self->actionFunc(self, (PlayState*)state);
}

void BgMizuUzu_Draw(Actor* actor, GameState* state) {
}
}
