extern "C" {
#include "z_bg_jya_block.hpp"

#define FLAGS 0x80000000

void BgJyaBlock_Init(Actor* actor, GameState* state);
void BgJyaBlock_Destroy(Actor* actor, GameState* state);
void BgJyaBlock_Update(Actor* actor, GameState* state);
void BgJyaBlock_Draw(Actor* actor, GameState* state);

ActorInit Bg_Jya_Block_InitVars = {
    ACTOR_BG_JYA_BLOCK,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_DANGEON_KEEP,
    sizeof(BgJyaBlock),
    (ActorFunc)BgJyaBlock_Init,
    (ActorFunc)BgJyaBlock_Destroy,
    (ActorFunc)BgJyaBlock_Update,
    (ActorFunc)BgJyaBlock_Draw,
};

static InitChainEntry sBgJyaBlockInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 333, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1500, ICHAIN_STOP),
};

void BgJyaBlock_Init(Actor* actor, GameState* state) {
    BgJyaBlock* self = (BgJyaBlock*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, 0);
    FUN_00372f38(actor, play, &self->skelAnimModel, 0x1A, 0);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 1), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    Actor_ProcessInitChain(actor, sBgJyaBlockInitChain);

    if ((LINK_AGE_IN_YEARS != YEARS_CHILD) || !Flags_GetSwitch(play, actor->params & 0x3F)) {
        Actor_Kill(actor);
    }
}

void BgJyaBlock_Destroy(Actor* actor, GameState* state) {
    BgJyaBlock* self = (BgJyaBlock*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgJyaBlock_Update(Actor* actor, GameState* state) {
    BgJyaBlock* self = (BgJyaBlock*)actor;
    PlayState* play = (PlayState*)state;
    Player* player = GET_PLAYER(play);

    player->stateFlags2 &= ~0x10; // PLAYER_STATE2_4 (not defined in repo headers)
    self->dyna.unk_1A8 = 0.0f;
}

void BgJyaBlock_Draw(Actor* actor, GameState* state) {
    BgJyaBlock* self = (BgJyaBlock*)actor;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &actor->modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&mtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
