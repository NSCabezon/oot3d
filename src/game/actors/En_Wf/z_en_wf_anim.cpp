// Own TU: tiny out-of-line accessor that armlink inlines into callers (ROM: add r0,r0,#0x4c).
extern "C" {
#include "z_en_wf.hpp"

f32* EnWf_AnimLengthPtr(SkelAnime* skelAnime) {
    return &skelAnime->animLength;
}

f32* EnWf_PlaySpeedPtr(SkelAnime* skelAnime) {
    return &skelAnime->playSpeed;
}
}
