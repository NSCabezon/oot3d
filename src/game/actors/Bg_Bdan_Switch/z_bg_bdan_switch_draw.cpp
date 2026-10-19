extern "C" {
#include "z_bg_bdan_switch.hpp"

// Own TU: the guarded function-local static Vector3f gives this object .data [guard 0x00510A98] + .bss (0x00569908),
// pinned in data/oot3d.ld via scripts/pin_static.py.

void BgBdanSwitch_DrawModel(Actor* actor, PlayState* play, SkeletonAnimationModel* model, f32 scale) __asm("FUN_00323fb4");
void Collider_UpdateSpheres(s32 index, BgBdanSwitchColliderJntSph* collider, MTX34* mtx);

void BgBdanSwitch_Draw(Actor* thisx, PlayState* play) {
    BgBdanSwitch* self = (BgBdanSwitch*)thisx;
    static Vector3f D_8086E0E0(0.0f, 140.0f, 0.0f);
    f32 zz = 0.0f; // dead local: shifts VFP numbering of the guarded static's init (ROM order)
    MTX34 mtx;
    f32 scale;

    MTX34CopyAsm(&mtx, &self->dyna.actor.modelMtx);
    scale = 1.0f;
    if (PauseContext_GetState()) {
        scale = 0.0f;
    }
    switch (self->dyna.actor.params & 0xFF) {
        case YELLOW_HEAVY:
        case YELLOW:
            BgBdanSwitch_DrawModel(thisx, play, self->skelAnimModel[0], scale);
            break;
        case YELLOW_TALL_1:
        case YELLOW_TALL_2:
            BgBdanSwitch_DrawModel(thisx, play, self->skelAnimModel[0], scale);
            Collider_UpdateSpheres(0, &self->collider, &mtx);
            {
                Vector3f* pos = (Vector3f*)&self->dyna.actor.focus.pos;
                VEC3TransformAsm(pos, &mtx, &D_8086E0E0);
            }
            break;
        case BLUE:
            BgBdanSwitch_DrawModel(thisx, play, self->skelAnimModel[1], scale);
            break;
    }
}
}
