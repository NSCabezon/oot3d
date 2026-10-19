extern "C" {
#include "z_obj_lightswitch.hpp"

void ObjLightswitch_Off(ObjLightswitch* self, PlayState* play);

void FUN_001a24d4(ObjLightswitch* self) {
    self->actionFunc = ObjLightswitch_Off;
    self->faceTextureIndex = 0;
    self->color[0] = 155 << 6;
    self->color[1] = 125 << 6;
    self->color[2] = 255 << 6;
    self->alpha = 255 << 6;
}

}
