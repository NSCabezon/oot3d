extern "C" {
#include "z_bg_spot03_taki.hpp"

#define FLAGS 0x80400030

void FUN_003e7f18(BgSpot03Taki* self, PlayState* play);

void BgSpot03Taki_Init(Actor* actor, GameState* state);
void BgSpot03Taki_Destroy(Actor* actor, PlayState* play);
void BgSpot03Taki_Update(Actor* actor, PlayState* play);
void BgSpot03Taki_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Spot03_Taki.data.s")

ActorInit Bg_Spot03_Taki_InitVars = {
    ACTOR_BG_SPOT03_TAKI,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT03_OBJECT,
    sizeof(BgSpot03Taki),
    (ActorFunc)BgSpot03Taki_Init,
    (ActorFunc)BgSpot03Taki_Destroy,
    (ActorFunc)BgSpot03Taki_Update,
    (ActorFunc)BgSpot03Taki_Draw,
};

void BgSpot03Taki_Init(Actor* actor, GameState* state) {
    BgSpot03Taki* self = (BgSpot03Taki*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;
    ZARInfo* zarInfo;

    self->switchFlag = actor->params & 0x3F;
    DynaPolyActor_Init(&self->dyna, 0);
    zarInfo = FUN_00372f38(&self->dyna.actor, play, &self->skelAnimModel0, 0, &self->skelAnimModel1, 1, 0);
    {
        void* cmab0 = zarInfo->GetCMABByIndex(0);
        SkeletonAnimationModel* m0 = self->skelAnimModel0;
        m0->unk_0C->FUN_00372d94(cmab0);
    }
    self->skelAnimModel0->unk_0C->animType = 1;
    BgSpot03Taki* s2 = self; // dead alias: fixes reg allocation of the 2nd (r2-shape) GetCMABByIndex call
    s2->skelAnimModel1->unk_0C->FUN_00372d94(zarInfo->GetCMABByIndex(1));
    self->skelAnimModel1->unk_0C->SetAnimType(1);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
    // sInitChain is in the GLOBAL_ASM data block (0x005136F4); raw address until data is split.
    Actor_ProcessInitChain(&self->dyna.actor, (InitChainEntry*)0x005136F4);
    self->openingAlpha = 255.0f;
    self->bufferIndex = 0;
    self->actionFunc = FUN_003e7f18;
}

void BgSpot03Taki_Destroy(Actor* actor, PlayState* play) {
    BgSpot03Taki* self = (BgSpot03Taki*)actor;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel0, &self->skelAnimModel1, 0);
}

void BgSpot03Taki_Update(Actor* actor, PlayState* play) {
    BgSpot03Taki* self = (BgSpot03Taki*)actor;

    self->actionFunc(self, play);
}

GLOBAL_ASM("asm/BgSpot03Taki_Draw.s")

GLOBAL_ASM("asm/FUN_00323f04.s")

}
