extern "C" {
#include "z_en_dekubaba.hpp"

// EnDekubaba_ComputeHeadPos. Own TU so armcc does not inline it into the
// action functions that tail-call it (ROM: `b FUN_00366044`).
void FUN_00366044(EnDekubaba* self) {
    f32 dxz;

    dxz = (Math_CosS(self->stemPartsRot[0]) + Math_CosS(self->stemPartsRot[1]) + Math_CosS(self->stemPartsRot[2])) *
          20.0f;
    self->actor.world.pos.x = self->actor.home.pos.x + (dxz * self->scaleFac * Math_SinS(self->actor.shape.rot.y));
    self->actor.world.pos.y =
        self->actor.home.pos.y -
        ((Math_SinS(self->stemPartsRot[0]) + Math_SinS(self->stemPartsRot[1]) + Math_SinS(self->stemPartsRot[2])) *
         20.0f * self->scaleFac);
    self->actor.world.pos.z = self->actor.home.pos.z + (dxz * self->scaleFac * Math_CosS(self->actor.shape.rot.y));
}

}
