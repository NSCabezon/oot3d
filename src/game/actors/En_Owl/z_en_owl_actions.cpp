extern "C" {
#include "z_en_owl.hpp"

void FUN_0035ff80(EnOwl* self) {
    if (Rand_CenteredFloat(1.0f) < 0.0f) {
        self->actionFlags |= 0x20;
    } else {
        self->actionFlags &= ~0x20;
    }
    self->actionFlags |= 0x10;
    self->unk_1352 = 0;
    self->unk_1368[0] = 0;
    self->unk_1368[1] = 4;
    self->unk_1364[0] = 0;
    self->unk_1366 = 0;
    self->unk_1364[1] = 4;
}
}
