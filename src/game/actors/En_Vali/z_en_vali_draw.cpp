extern "C" {
#include "z_en_vali.hpp"

#define M_PI_F 3.14159265358979323846f

void FUN_0011ca08(EnVali* self, PlayState* play); // EnVali_Spin
void FUN_00120ad0(EnVali* self, PlayState* play); // EnVali_Damaged
void FUN_00105e30(EnVali* self, PlayState* play); // EnVali_Recover
void FUN_001094c0(EnVali* self, PlayState* play); // EnVali_Electrify
void FUN_001565fc(EnVali* self, PlayState* play); // EnVali_Frozen
void Matrix_Scale(MTX34* mtx, s32 mode, f32 x, f32 y, f32 z);
f32* EnVali_CurFramePtr(SkelAnime* skelAnime);
f32 __hardfp_sinf(f32 x);
f32 __hardfp_cosf(f32 x);

s32 FUN_001c4b18(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnVali* self = (EnVali*)thisx;
    f32 frame = *EnVali_CurFramePtr(&self->skelAnime);
    f32 target;

    if (limbIndex == 4 || limbIndex == 9) {
        if (self->actionFunc == FUN_0011ca08) {
            if (frame <= 10.0f) {
                target = (frame * 0.05f) + 1.0f;
            } else if (frame > 70.0f) {
                target = ((80.0f - frame) * 0.05f) + 1.0f;
            } else {
                target = 1.5f;
            }
        } else if (self->actionFunc == FUN_00120ad0) {
            target = 1.0f - (__hardfp_sinf((M_PI_F / 10) * frame) * 0.35f);
        } else if (self->actionFunc == FUN_00105e30) {
            target = 1.0f - (__hardfp_cosf((M_PI_F / 50) * frame) * 0.35f);
        } else if ((self->actionFunc == FUN_001094c0) || (self->actionFunc == FUN_001565fc)) {
            target = self->tentaclesLengthFactor;
        } else {
            target = 1.0f;
        }
        Math_StepToF(&self->tentaclesLengthFactor, target, 0.1f);
        if (self->tentaclesLengthFactor != 1.0f) {
            Matrix_Scale(mtx, 1, self->tentaclesLengthFactor, 1.0f, 1.0f);
        }
    }
    return 0;
}


}
