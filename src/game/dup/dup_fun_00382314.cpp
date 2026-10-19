extern "C" {
#include "global.hpp"

#include "dup_decls.hpp"

void FUN_00382314(Actor* actor, GameState* state) {
    DupModelActor1C8* self = (DupModelActor1C8*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->actor.modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

}
