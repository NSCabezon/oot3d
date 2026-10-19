extern "C" {
#include "z_bg_ingate.hpp"

#define FLAGS 0x80400030

#define SCENE_LON_LON_RANCH 0x63
#define CS_INDEX_0 0xFFF0
#define EVENTCHKINF_EPONA_OBTAINED_FLAG 0x100 // eventChkInf[1], bit 8
#define INGO_RACE_STATE_TRAPPED_WIN_EPONA 6

#define PARAMS_GET_U(params, start, count) (((params) >> (start)) & ((1 << (count)) - 1))
#define PARAMS_GET_NOSHIFT(params, start, count) ((params) & ((1 << (count)) - 1) << (start))

void BgIngate_Init(Actor* actor, GameState* state);
void BgIngate_Destroy(Actor* actor, GameState* state);
void BgIngate_Update(Actor* actor, GameState* state);
void BgIngate_Draw(Actor* actor, GameState* state);
void BgIngate_DoNothing(BgIngate* self, PlayState* play);
void FUN_00108890(BgIngate* self, PlayState* play);

ActorInit Bg_Ingate_InitVars = {
    ACTOR_BG_INGATE,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_INGATE,
    sizeof(BgIngate),
    (ActorFunc)BgIngate_Init,
    (ActorFunc)BgIngate_Destroy,
    (ActorFunc)BgIngate_Update,
    (ActorFunc)BgIngate_Draw,
};


void BgIngate_Init(Actor* actor, GameState* state) {
    BgIngate* self = (BgIngate*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
    DynaPolyActor_Init(&self->dyna, 0);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);

    if ((play->sceneNum != SCENE_LON_LON_RANCH || !LINK_IS_ADULT) ||
        ((gSaveContext.eventChkInf[1] & EVENTCHKINF_EPONA_OBTAINED_FLAG) && (gSaveContext.cutsceneIndex != CS_INDEX_0))) {
        Actor_Kill(actor);
        return;
    }

    Actor_SetScale(actor, 0.1f);
    if ((PARAMS_GET_U(actor->params, 0, 1) != 0) && ((gSaveContext.eventInf[0] & 0xF) == INGO_RACE_STATE_TRAPPED_WIN_EPONA)) {
        play->csCtx.frames = 0;
        self->actionFunc = FUN_00108890;
    } else {
        self->actionFunc = BgIngate_DoNothing;
    }
}

void BgIngate_Destroy(Actor* actor, GameState* state) {
    BgIngate* self = (BgIngate*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void FUN_00108890(BgIngate* self, PlayState* play) {
    s32 phi0;
    s16 phi1;
    s16 csFrames;

    if (play->csCtx.frames >= 50) {
        phi0 = 0x4000;
        if (!PARAMS_GET_NOSHIFT(self->dyna.actor.params, 1, 1)) {
            phi0 = -0x4000;
        }
        Audio_PlayActorSound2(&self->dyna.actor, 0x0100023D); // NA_SE_EV_GATE_OPEN (3DS addition)
        self->dyna.actor.shape.rot.y = self->dyna.actor.world.rot.y + phi0;
        self->actionFunc = BgIngate_DoNothing;
    } else if (play->csCtx.frames >= 10) {
        csFrames = play->csCtx.frames - 10;
        csFrames *= 400;
        phi1 = csFrames;
        if (csFrames > 0x4000) {
            csFrames = 0x4000;
        }
        csFrames = (Math_SinS(csFrames) * 16384.0f);
        phi1 = csFrames;
        if (!PARAMS_GET_NOSHIFT(self->dyna.actor.params, 1, 1)) {
            phi1 = -phi1;
        }
        self->dyna.actor.shape.rot.y = self->dyna.actor.world.rot.y + phi1;
    }
}

void BgIngate_DoNothing(BgIngate* self, PlayState* play) {
}

void BgIngate_Update(Actor* actor, GameState* state) {
    BgIngate* self = (BgIngate*)actor;

    self->actionFunc(self, (PlayState*)state);
}

void BgIngate_Draw(Actor* actor, GameState* state) {
    BgIngate* self = (BgIngate*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

}
