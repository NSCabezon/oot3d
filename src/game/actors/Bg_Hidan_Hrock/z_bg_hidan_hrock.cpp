extern "C" {
#include "z_bg_hidan_hrock.hpp"

#define FLAGS 0x00400000

void BgHidanHrock_Init(Actor* actor, GameState* state);
void BgHidanHrock_Destroy(Actor* actor, GameState* state);
void BgHidanHrock_Update(Actor* actor, GameState* state);
void BgHidanHrock_Draw(Actor* actor, GameState* state);

void Collider_DestroyTris(PlayState* play, void* collider);

GLOBAL_ASM("data/z_Bg_Hidan_Hrock.data.s")

ActorInit Bg_Hidan_Hrock_InitVars = {
    ACTOR_BG_HIDAN_HROCK,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanHrock),
    (ActorFunc)BgHidanHrock_Init,
    (ActorFunc)BgHidanHrock_Destroy,
    (ActorFunc)BgHidanHrock_Update,
    (ActorFunc)BgHidanHrock_Draw,
};

// NON_MATCHING draft (Haiku stage 1): still GLOBAL_ASM, see STATUS.md.
GLOBAL_ASM("asm/BgHidanHrock_Init.s")

#if 0
void BgHidanHrock_Init(Actor* actor, GameState* state) {
    BgHidanHrock* self = (BgHidanHrock*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;
    s32 idx;

    Actor_ProcessInitChain(actor, sInitChain);
    self->switchFlag = PARAMS_GET_U(actor->params, 0, 6);
    actor->params = PARAMS_GET_U(actor->params, 8, 8);
    Collider_InitTris(play, &self->collider);
    Collider_SetTris(play, &self->collider, actor, &sTrisInit, &self->colliderElements);
    DynaPolyActor_Init(&self->dyna, 0);
    /* 2 triangles of the collider rotated by shape.rot.y (scaled x1.5 for params 0) ... */
    if (Flags_GetSwitch(play, self->switchFlag)) {
        self->actionFunc = FUN_003e7e50; /* empty, N64 func_808894A4 */
        if (actor->params == 0) {
            actor->world.pos.y -= 2800.0f;
            actor->cullingVolumeDistance = 3000.0f;
        } else if (actor->params == 1) {
            actor->world.pos.y -= 800.0f;
        } else if (actor->params == 2) {
            actor->world.pos.y -= 240.0f;
        }
    } else {
        if (actor->params == 0) {
            actor->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_DRAW_CULLING_DISABLED;
            actor->cullingVolumeDistance = 3000.0f;
        }
        self->actionFunc = FUN_001c7cf8;
    }
    idx = sModelIdx[actor->params]; /* 0x511A04: {0x11, 0x12, 0x12} */
    FUN_00372f38(actor, play, &self->skelAnimModel, idx, 0);
    colHeader = DynaPolyInfo_Alloc(&self->dyna, state, actor->params == 0 ? 0xF : 0x10);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
}
#endif

void BgHidanHrock_Destroy(Actor* actor, GameState* state) {
    BgHidanHrock* self = (BgHidanHrock*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    // Raw offset: armcc emits a single `add r1, r4, #0x1c4` only this way (typed &self->collider splits it)
    Collider_DestroyTris(play, (char*)self + 0x1C4);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgHidanHrock_Update(Actor* actor, GameState* state) {
    BgHidanHrock* self = (BgHidanHrock*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
}

void BgHidanHrock_Draw(Actor* actor, GameState* state) {
    BgHidanHrock* self = (BgHidanHrock*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
