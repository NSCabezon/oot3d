extern "C" {
#include "global.hpp"

#include "dup_decls.hpp"

void FUN_003b3218(DupActionActor1A4* self, PlayState* play);

void FUN_0012b6b8(DupActionActor1A4* self) {
    self->actionFunc = FUN_003b3218;
}

}
