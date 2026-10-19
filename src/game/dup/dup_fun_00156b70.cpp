extern "C" {
#include "global.hpp"

#include "dup_decls.hpp"

void FUN_00180d0c(Actor* actor);

void FUN_00156b70(DupSkelActor* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime)) {
        FUN_00180d0c(&self->actor);
    }
}

}
