extern "C" {
#include "z_en_pu_box.hpp"

#define FLAGS 0x80000010

void EnPubox_Init(Actor* actor, GameState* state);
void EnPubox_Destroy(Actor* actor, GameState* state);
void EnPubox_Update(Actor* actor, GameState* state);
void EnPubox_Draw(Actor* actor, GameState* state);

ActorInit En_Pu_box_InitVars = {
    ACTOR_EN_PU_BOX,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_PU_BOX,
    sizeof(EnPubox),
    (ActorFunc)EnPubox_Init,
    (ActorFunc)EnPubox_Destroy,
    (ActorFunc)EnPubox_Update,
    (ActorFunc)EnPubox_Draw,
};

void EnPubox_Init(Actor* actor, GameState* state) {
    EnPubox* self = (EnPubox*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    switch (actor->params) {
        case 0:
            Actor_SetScale(actor, 0.0025f);
            break;
        case 1:
            Actor_SetScale(actor, 0.005f);
            break;
        case 2:
            Actor_SetScale(actor, 0.0075f);
            break;
        case 3:
            Actor_SetScale(actor, 0.01f);
        default:
            break;
    }
    self->unused_1BC = 1;
    actor->uncullZoneDownward = 1200.0f;
    actor->uncullZoneScale = 720.0f;
    actor->colChkInfo.cylRadius = 20;
    actor->colChkInfo.cylHeight = 50;
    ActorShape_Init(&actor->shape, 0.0f, ActorShadow_DrawCircle, 6.0f);
    self->transformFlags = 0;
    self->interactFlags = 0;
    actor->targetMode = 1;
    actor->gravity = -2.0f;
    FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc((DynaPolyActor*)actor, state, 1), &colHeader);
    self->bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
}

void EnPubox_Destroy(Actor* actor, GameState* state) {
    EnPubox* self = (EnPubox*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void EnPubox_Update(Actor* actor, GameState* state) {
    EnPubox* self = (EnPubox*)actor;
    PlayState* play = (PlayState*)state;

    actor->world.rot.y = self->unk_1B0;
    actor->speedXZ += self->unk_1A8;
    actor->speedXZ = CLAMP(actor->speedXZ, -2.5f, 2.5f);
    Math_SmoothStepToF(&actor->speedXZ, 0.0f, 1.0f, 1.0f, 0.0f);
    if (actor->speedXZ != 0.0f) {
        Audio_PlaySoundGeneral(0x01000161, &actor->world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    }
    self->unk_1AC = 0.0f;
    self->unk_1A8 = 0.0f;
    Actor_MoveForward(actor);
    Actor_UpdateBgCheckInfo(play, actor, actor->colChkInfo.cylHeight, actor->colChkInfo.cylRadius,
                            actor->colChkInfo.cylRadius, 0x1D);
    actor->focus.pos = actor->world.pos;
}

void EnPubox_Draw(Actor* actor, GameState* state) {
    EnPubox* self = (EnPubox*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}
}
