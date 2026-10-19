extern "C" {
#include "z_bg_jya_1flift.hpp"

void BgJya1flift_Move(BgJya1flift* self, PlayState* play);

// ChangeDirection. Name pinned by the link map (was GLOBAL_ASM in functions_180000s.cpp).
void FUN_00185384(BgJya1flift* self) {
    self->actionFunc = BgJya1flift_Move;
    self->isMovingDown ^= true;
    self->dyna.actor.velocity.y = 0.0f;
}

}
