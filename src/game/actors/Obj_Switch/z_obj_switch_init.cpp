extern "C" {
#include "z_obj_switch.hpp"

void ObjSwitch_FloorUp(ObjSwitch* thisx, PlayState* play);
void ObjSwitch_CrystalOff(ObjSwitch* thisx, PlayState* play);

void FUN_001eb278(ObjSwitch* thisx) {
    thisx->crystalR = 0;
    thisx->crystalG = 0;
    thisx->crystalB = 0;
    thisx->actionFunc = ObjSwitch_CrystalOff;
}

void FUN_001cf0e4(ObjSwitch* thisx) {
    thisx->actor.scale.y = 0.165f;
    thisx->actionFunc = ObjSwitch_FloorUp;
}

void FUN_001cf100(ObjSwitch* thisx) {
    thisx->actionFunc = ObjSwitch_EyeOpen;
    thisx->eyeTexIndex = 0;
}

#if 0 // Used only by the unmatched FloorPress draft.
void ObjSwitch_FloorDownInit(ObjSwitch* thisx) {
    thisx->actor.scale.y = 0.0165f;
    thisx->releaseTimer = 9;
    thisx->actionFunc = ObjSwitch_FloorDown;
}
#endif

}
