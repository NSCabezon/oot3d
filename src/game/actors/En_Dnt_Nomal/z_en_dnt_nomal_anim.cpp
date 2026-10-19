// Tiny accessor in its own TU: armlink --inline turns calls to it into `add r0, r0, #0x3c` (ROM curFrame reads show
// `add r0,r0,#0x2fc; add r0,r0,#0x3c; vldr`). Same recipe as En_Horse.
extern "C" {
#include "z_en_dnt_nomal.hpp"

f32* EnDntNomal_CurFramePtr(SkelAnime* skelAnime) {
    return &skelAnime->curFrame;
}
}
