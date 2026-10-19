// Tiny accessor in its own TU: armlink --inline turns calls to it into `add r0, r0, #0x3c` (ROM reads curFrame as
// `add r0,rX,#0x1a4; add r0,r0,#0x3c; vldr sN,[r0]`). Same trick as En_Horse z_en_horse_anim.cpp.
extern "C" {
#include "z_en_heishi2.hpp"

f32* EnHeishi2_CurFramePtr(SkelAnime* skelAnime) {
    return &skelAnime->curFrame;
}
}
