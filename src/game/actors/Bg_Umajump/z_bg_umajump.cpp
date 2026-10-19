extern "C" {
#include "z_bg_umajump.hpp"

#define FLAGS 0x80400030

void BgUmajump_Init(BgUmajump* self, PlayState* play);
void BgUmajump_Destroy(BgUmajump* self, PlayState* play);
void BgUmajump_Update(BgUmajump* self, PlayState* play);
void BgUmajump_Draw(BgUmajump* self, PlayState* play);

ActorInit Bg_Umajump_InitVars = {
    ACTOR_BG_UMAJUMP,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_UMAJUMP,
    sizeof(BgUmajump),
    (ActorFunc)BgUmajump_Init,
    (ActorFunc)BgUmajump_Destroy,
    (ActorFunc)BgUmajump_Update,
    (ActorFunc)BgUmajump_Draw,
};


static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgUmajump_Init(BgUmajump* self, PlayState* play) {
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    FUN_00372f38(&self->dyna.actor, play, &self->skelAnimModel, 0, NULL);
    DynaPolyActor_Init(&self->dyna, 0);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);

    if (self->dyna.actor.params == 1) {
        if (!(Flags_GetEventChkInf(0x18) /* EVENTCHKINF_EPONA_OBTAINED */ || DREG(1) /* R_DEBUG_FORCE_EPONA_OBTAINED */)) {
            Actor_Kill(&self->dyna.actor);
            return;
        }
        self->dyna.actor.flags |= 0x30; // UPDATE_CULLING_DISABLED | DRAW_CULLING_DISABLED
    }
}

void BgUmajump_Destroy(BgUmajump* self, PlayState* play) {
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(&self->dyna.actor, &self->skelAnimModel, 0);
}

void BgUmajump_Update(BgUmajump* self, PlayState* play) {
}

void BgUmajump_Draw(BgUmajump* self, PlayState* play) {
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
