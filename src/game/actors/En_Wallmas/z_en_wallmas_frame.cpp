// Tiny accessor in its own TU: armlink --inline turns calls to it into `add r0, r0, #0x3c` (the ROM's curFrame reads in
// En_Wallmas show `add r0,r4,#0x1a4; add r0,r0,#0x3c; vldr sN,[r0]`).
extern "C" {
#include "z_en_wallmas.hpp"

f32* EnWallmas_CurFramePtr(SkelAnime* skelAnime) {
    return &skelAnime->curFrame;
}
}
