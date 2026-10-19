// Tiny accessor in its own TU: armlink --inline turns calls to it into `add r0, r0, #0x3c` (the ROM's curFrame reads in
// EnSb_Open / EnSb_Bounce show `add r0,r0,#0x1a4; add r0,r0,#0x3c; vldr sN,[r0]`).
extern "C" {
#include "z_en_sb.hpp"

f32* EnSb_CurFramePtr(SkelAnime* skelAnime) {
    return &skelAnime->curFrame;
}
}
