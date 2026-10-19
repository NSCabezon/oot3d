// Tiny accessor in its own TU: armlink --inline turns calls to it into `add r0, r0, #0x3c` (ROM reads curFrame as
// `add r0,rX,#0x1a4; add r0,r0,#0x3c; vldr sN,[r0]`). Same trick as En_Heishi2 z_en_heishi2_anim.cpp.
extern "C" {
#include "z_en_bom_bowl_man.hpp"

f32* EnBomBowlMan_CurFramePtr(SkelAnime* skelAnime) {
    return &skelAnime->curFrame;
}
}
