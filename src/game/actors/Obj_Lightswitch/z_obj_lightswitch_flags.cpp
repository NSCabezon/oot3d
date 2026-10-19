extern "C" {
#include "z_obj_lightswitch.hpp"

#define PARAMS_GET_U(params, shift, count) ((((u16)(params)) >> (shift)) & ((1 << (count)) - 1))

void FUN_0036a2dc(PlayState* play, Actor* actor, s32 sfxId, s32 arg3, s32 arg4);

void FUN_0036a84c(Actor* thisx, PlayState* play) {
    s32 type;

    if (Flags_GetSwitch(play, PARAMS_GET_U(thisx->params, 8, 6))) {
        return;
    }
    type = PARAMS_GET_U(thisx->params, 4, 2);
    if (type != OBJLIGHTSWITCH_TYPE_1) {
        if (type == OBJLIGHTSWITCH_TYPE_BURN) {
            FUN_0036a2dc(play, thisx, 0x0100048B, 0, 0);
            return;
        }
        if (gSaveContext.masterQuestFlag != 0 && *(u16*)((u8*)play + 0x104) == 6 &&
            *(u8*)((u8*)play + 0x4C30) == 4) {
            FUN_0036a2dc(play, thisx, 0, 0x1392, 0x64);
            return;
        }
    }
    FUN_0036a2dc(play, thisx, 0, 0, 0);
}

}
