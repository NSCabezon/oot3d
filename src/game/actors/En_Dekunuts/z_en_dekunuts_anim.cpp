extern "C" {
#include "z_en_dekunuts.hpp"

// Tiny accessor in its own TU: armlink --inline turns calls to it into `add r0, r0, #0x3c` (ROM reads of skelAnime.curFrame).
f32* EnDekunuts_CurFramePtr(SkelAnime* skelAnime) {
    return &skelAnime->curFrame;
}

}
