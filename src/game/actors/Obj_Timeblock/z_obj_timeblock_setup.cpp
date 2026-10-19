// Separate TU on purpose: the ROM tail-calls FUN_00239fbc from AltBehaviorVisible (opaque, not inlined).
extern "C" {
#include "z_obj_timeblock.hpp"

void ObjTimeblock_AltBehaviourNotVisible(ObjTimeblock* self, PlayState* play);

// FUN_00239fbc (was functions_200000s.cpp), N64: ObjTimeblock_SetupAltBehaviourNotVisible
void FUN_00239fbc(ObjTimeblock* self) {
    self->actionFunc = ObjTimeblock_AltBehaviourNotVisible;
}

}
