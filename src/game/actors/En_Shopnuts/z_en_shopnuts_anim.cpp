extern "C" {
#include "z_en_shopnuts.hpp"

// Tiny accessor in its own TU: armlink --inline turns calls to it into `add r0, r0, #0x3c`
// (ROM reads of skelAnime.curFrame, which the compiler does not fold/CSE).
f32* EnShopnuts_CurFramePtr(SkelAnime* skelAnime) {
    return &skelAnime->curFrame;
}

}
