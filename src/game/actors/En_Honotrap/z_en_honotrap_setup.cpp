extern "C" {
#include "z_en_honotrap.hpp"

void EnHonotrap_EyeIdle(EnHonotrap* self, PlayState* play);

void EnHonotrap_SetupEyeIdle(EnHonotrap* self) {
    self->actionFunc = EnHonotrap_EyeIdle;
    self->eyeState = 3;
}

}
