extern "C" {
#include "z_actor_1d9.hpp"

#define FLAGS 0x00000009

f32 sins(f32 x);
void FUN_0033dd8c(SkelAnime* skelAnime, s32 arg1, s32 arg2, s32 arg3, f32 f0, f32 f1, f32 f2, f32 f3);
void FUN_00381a8c(void);
void FUN_0034f6bc(PlayState* play, DynaCollisionContext* dyna, s32 bgId);
void FUN_00358ea8(ZARInfo* zar, PlayState* play, SkelAnime* skel, void* cmb, void* p, s32 b, void* jointTable,
                  void* morphTable, s32 limbCount);
void* ZAR_GetCMBByIndex(ZARInfo* zar, s32 index);
void FUN_003d0ed4(Actor1D9* self, PlayState* play);

void Actor1D9_Init(Actor* actor, GameState* state);
void Actor1D9_Destroy(Actor* actor, GameState* state);
void Actor1D9_Update(Actor* actor, GameState* state);
void Actor1D9_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Actor_1D9.data.s")

ActorInit Actor_1D9_InitVars = {
    ACTOR_1D9,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_HINTSTONE,
    sizeof(Actor1D9),
    (ActorFunc)Actor1D9_Init,
    (ActorFunc)Actor1D9_Destroy,
    (ActorFunc)Actor1D9_Update,
    (ActorFunc)Actor1D9_Draw,
};

void Actor1D9_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    Actor1D9* self = (Actor1D9*)actor;
    CollisionHeader* colHeader = NULL;
    ZARInfo* zar;
    void* cmb;

    Actor_ProcessInitChain(actor, (InitChainEntry*)0x00526458);
    zar = &play->GetObjectStatus(self->dyna.actor.objBankIndex)->zarInfo;
    DynaPolyActor_Init(&self->dyna, 0);
    ReadCollisionHeaderFromZsi(zar->FUN_003532c0(0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    FUN_0034f6bc(play, &play->colCtx.dyna, self->dyna.bgId);
    if (gSaveContext.masterQuestFlag) {
        Actor_Kill(actor);
    }
    cmb = ZAR_GetCMBByIndex(zar, 0);
    FUN_00358ea8(zar, play, &self->skelAnime, cmb, self->dyna.actor.unk_178, 0, self->jointTable, self->morphTable, 4);
    self->dyna.actor.targetMode = 0;
    self->actionFunc = FUN_003d0ed4;
}

void Actor1D9_Destroy(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    Actor1D9* self = (Actor1D9*)actor;

    SkelAnime_Free2(&self->skelAnime);
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
}

// The constants are locals assigned up front: armcc keeps 27.5/200/0 live in s16-s18 across the actionFunc call
// (vpush {d8,d9}) and loads 11.5 before Math_SinS, as in the ROM. The timer re-read through the `actor` param
// (not `self`) forces the unconditional reload after the conditional wrap store.
void Actor1D9_Update(Actor* actor, GameState* state) {
    Actor1D9* self = (Actor1D9*)actor;
    PlayState* play = (PlayState*)state;
    f32 wrap = 27.5f;
    f32 scale = 200.0f;
    f32 zero = 0.0f;
    f32 sway;
    f32 step;
    f32 dist;
    s16 rotY;

    self->actionFunc(self, play);

    sway = zero;
    step = 1.0f;
    if (self->swayState == 2) {
        self->timer += step;
        if (self->timer > wrap) {
            self->timer -= wrap;
        }
        sway = ((Actor1D9*)actor)->timer * 0.114239708f; // 2pi / 55
        sway = sins(sway * 40.7436638f);                // 256 / 2pi
    }
    self->swayOffset = sway * scale * 0.00392156886f; // 1 / 255

    dist = 11.5f;
    rotY = self->dyna.actor.shape.rot.y;
    self->dyna.actor.focus.pos.x = self->dyna.actor.world.pos.x + Math_SinS(rotY) * dist;
    self->dyna.actor.focus.pos.y = self->dyna.actor.world.pos.y + 53.5f;
    self->dyna.actor.focus.pos.z = self->dyna.actor.world.pos.z + Math_CosS(rotY) * dist;
    self->dyna.actor.focus.rot = self->dyna.actor.world.rot;
}

void Actor1D9_Draw(Actor* actor, GameState* state) {
    Actor1D9* self = (Actor1D9*)actor;

    FUN_0033dd8c(&self->skelAnime, 0, 4, 0, 0.0f, self->swayOffset, 0.0f, 1.0f);
    SkelAnime_DrawOpa(&self->skelAnime, &self->dyna.actor.modelMtx, (OverrideLimbDrawOpa)0x00381a84,
                      (PostLimbDrawOpa)FUN_00381a8c, actor, 0);
}
}
