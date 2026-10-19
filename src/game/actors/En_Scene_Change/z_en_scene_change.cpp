extern "C" {
#include "z_en_scene_change.hpp"

#define FLAGS 0x80000000

void EnSceneChange_Init(Actor* actor, GameState* state);
void EnSceneChange_Destroy(Actor* actor, GameState* state);
void EnSceneChange_Update(Actor* actor, GameState* state);
void EnSceneChange_Draw(Actor* actor, GameState* state);

ActorInit En_Scene_Change_InitVars = {
    ACTOR_EN_SCENE_CHANGE,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_JJ,
    sizeof(EnSceneChange),
    (ActorFunc)EnSceneChange_Init,
    (ActorFunc)EnSceneChange_Destroy,
    (ActorFunc)EnSceneChange_Update,
    (ActorFunc)EnSceneChange_Draw,
};

void EnSceneChange_DoNothing(EnSceneChange* self, GameState* state);

void EnSceneChange_Init(Actor* actor, GameState* state) {
    EnSceneChange* self = (EnSceneChange*)actor;

    self->actor.unk_19A = 1;
    self->skelAnimModel = NULL;
    self->actionFunc = EnSceneChange_DoNothing;
}

void EnSceneChange_Destroy(Actor* actor, GameState* state) {
    EnSceneChange* self = (EnSceneChange*)actor;

    FUN_00350f34(&self->actor, &self->skelAnimModel, 0);
}

void EnSceneChange_Update(Actor* actor, GameState* state) {
    EnSceneChange* self = (EnSceneChange*)actor;

    self->actionFunc(self, state);
}

void EnSceneChange_Draw(Actor* actor, GameState* state) {
    EnSceneChange* self = (EnSceneChange*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->actor.modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

void EnSceneChange_DoNothing(EnSceneChange* self, GameState* state) {
}

}
