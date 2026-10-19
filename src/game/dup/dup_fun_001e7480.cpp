extern "C" {
#include "global.hpp"

#include "dup_decls.hpp"

void FUN_003672b8(Actor* actor);

void FUN_001e7480(DupSkelActor* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime)) {
        FUN_003672b8(&self->actor);
    }
}

}
