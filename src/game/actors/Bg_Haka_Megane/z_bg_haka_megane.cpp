extern "C" {
#include "z_bg_haka_megane.hpp"

#define FLAGS 0x004000B0

void BgHakaMegane_Init(Actor* actor, GameState* state);
void BgHakaMegane_Destroy(Actor* actor, GameState* state);
void BgHakaMegane_Update(Actor* actor, GameState* state);

void FUN_00154cb0(BgHakaMegane* self, PlayState* play); // N64: BgHakaMegane_WaitForObject
void FUN_003e7da0(BgHakaMegane* self, PlayState* play); // N64: BgHakaMegane_UpdateState
void FUN_003e1fb8(BgHakaMegane* self, PlayState* play); // N64: BgHakaMegane_DoNothing
void FUN_0029ac3c(Actor* actor, GameState* state);      // N64: BgHakaMegane_Draw

s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 bankIndex);

// 7-arg form of the model loader: also loads a secondary model into skelAnimModel2

ActorInit Bg_Haka_Megane_InitVars = {
    ACTOR_BG_HAKA_MEGANE,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(BgHakaMegane),
    (ActorFunc)BgHakaMegane_Init,
    (ActorFunc)BgHakaMegane_Destroy,
    (ActorFunc)BgHakaMegane_Update,
    NULL,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

static s32 sModelIndices[] = { 3, 5, 0, 5, 7, 3, 8, 9, 4, 6, 2, 10, 1 };

// 0 = no collision
static s32 sCollisionIndices[] = { 2, 3, 0, 1, 3, 0, 4, 5, 0, 2, 0, 6, 0 };

void BgHakaMegane_Init(Actor* actor, GameState* state) {
    BgHakaMegane* self = (BgHakaMegane*)actor;
    PlayState* play = (PlayState*)state;

    Actor_ProcessInitChain(actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, 0);

    if (actor->params < 3) {
        self->requiredObjectSlot = Object_GetIndex(&play->objectCtx, 0x8D); // OBJECT_HAKACH_OBJECTS
    } else {
        self->requiredObjectSlot = Object_GetIndex(&play->objectCtx, 0x69); // OBJECT_HAKA_OBJECTS
    }

    if (self->requiredObjectSlot < 0) {
        Actor_Kill(actor);
    } else {
        self->actionFunc = FUN_00154cb0;
    }
}

void BgHakaMegane_Destroy(Actor* actor, GameState* state) {
    BgHakaMegane* self = (BgHakaMegane*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, &self->skelAnimModel2, 0);
}

void FUN_00154cb0(BgHakaMegane* self, PlayState* play) {
    if (Object_IsLoaded(&play->objectCtx, self->requiredObjectSlot)) {
        self->dyna.actor.objBankIndex = self->requiredObjectSlot;
        self->dyna.actor.draw = FUN_0029ac3c;
        // N64: Actor_SetObjectDependency(play, actor). Empty on 3DS; any C-compiled empty function
        // reproduces the call, which the linker (--inline) replaces with a nop.
        ((void (*)(PlayState*, Actor*))GraphicsContext_UnknownStub)(play, &self->dyna.actor);
        FUN_00372f38(&self->dyna.actor, play, &self->skelAnimModel,
                                                   sModelIndices[self->dyna.actor.params], &self->skelAnimModel2, 3,
                                                   0);

        if (*((u8*)&play->roomCtx + 5) == 0) { // N64: lensMode == LENS_MODE_SHOW_ACTORS
            self->actionFunc = FUN_003e1fb8;
        } else {
            CollisionHeader* colHeader;

            self->actionFunc = FUN_003e7da0;
            if (sCollisionIndices[self->dyna.actor.params] != 0) {
                ReadCollisionHeaderFromZsi(
                    DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, sCollisionIndices[self->dyna.actor.params]),
                    &colHeader);
                self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
            }
        }
    }
}

void FUN_003e7da0(BgHakaMegane* self, PlayState* play) {
    if (play->actorCtx.unk_03 != 0) { // N64: lensActive
        self->dyna.actor.flags |= 0x80;
        FUN_0036b940(play, &play->colCtx.dyna, self->dyna.bgId); // N64: DynaPoly_DisableCollision
    } else {
        self->dyna.actor.flags &= ~0x80;
        FUN_0036d15c(play, &play->colCtx.dyna, self->dyna.bgId); // N64: DynaPoly_EnableCollision
    }
}

void FUN_003e1fb8(BgHakaMegane* self, PlayState* play) {
}

void BgHakaMegane_Update(Actor* actor, GameState* state) {
    BgHakaMegane* self = (BgHakaMegane*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
}

void FUN_0029ac3c(Actor* actor, GameState* state) {
    BgHakaMegane* self = (BgHakaMegane*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    if ((self->dyna.actor.flags & 0x80) == 0x80) {
        if (self->skelAnimModel != NULL) {
            self->skelAnimModel->SetUnkAC(1);
            self->skelAnimModel->FUN_003721e0(&modelMtx);
            self->skelAnimModel->FUN_00372170(0);
        }
    } else if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }

    if (self->dyna.actor.params == 0 && self->skelAnimModel2 != NULL) { // BotW blood splatter
        self->skelAnimModel2->SetUnkAC(1);
        self->skelAnimModel2->FUN_003721e0(&modelMtx);
        self->skelAnimModel2->FUN_00372170(0);
    }
}
}
