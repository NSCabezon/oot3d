// Obj_Bean setup helper. Separate TU from z_obj_bean.cpp on purpose: FUN_0036ac40 tail-calls it in ROM
// (bleq + b), so armcc must not inline it there.
extern "C" {
#include "z_obj_bean.hpp"

void FUN_001100a0(ObjBean* self, PlayState* play); // ObjBean_WaitForPlayer

void FUN_001a23f4(ObjBean* self) { // ObjBean_SetupWaitForPlayer
    self->actionFunc = FUN_001100a0;
    ObjBean_SetDrawMode(self, 0x4);
}
}
