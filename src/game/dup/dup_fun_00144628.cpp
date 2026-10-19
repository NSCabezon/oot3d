extern "C" {
#include "global.hpp"

#include "dup_decls.hpp"

void FUN_003bf9b0(DupActionActor1BC* self, PlayState* play);

void FUN_00144628(DupActionActor1BC* self) {
    self->actionFunc = FUN_003bf9b0;
}

}
