extern "C" {
#include "global.hpp"

#include "dup_decls.hpp"

void FUN_001f84ec(Actor* actor, GameState* state) {
    DupModelActor* self = (DupModelActor*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->actor.modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

}
