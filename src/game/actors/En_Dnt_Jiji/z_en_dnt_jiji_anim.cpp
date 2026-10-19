// Tiny accessor in its own TU: armlink --inline turns calls to it into `add r0, r0, #0x3c` (ROM curFrame reads show
// `add r0,r0,#0x22c; add r0,r0,#0x3c; vldr`). Same recipe as En_Dnt_Nomal.
extern "C" {
#include "z_en_dnt_jiji.hpp"

f32* EnDntJiji_CurFramePtr(SkelAnime* skelAnime) {
    return &skelAnime->curFrame;
}
}
