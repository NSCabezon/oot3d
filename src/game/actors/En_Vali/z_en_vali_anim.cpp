extern "C" {
#include "z_en_vali.hpp"

// Out-of-line accessor (own TU): ROM calls it, armlink inlines the tiny body.
f32* EnVali_CurFramePtr(SkelAnime* skelAnime) {
    return &skelAnime->curFrame;
}

// Out-of-line setter (own TU): ROM calls it, armlink inlines the tiny body (`str r1,[r0]`).
void EnVali_SetMat(SkeletonAnimationModel_unk_0C* a, AnimatedMaterial* m) {
    a->unk_00 = m;
}

}
