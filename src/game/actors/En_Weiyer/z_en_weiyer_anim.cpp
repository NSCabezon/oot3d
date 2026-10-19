extern "C" {
#include "z_en_weiyer.hpp"

// Out-of-line accessor so armcc/armlink reproduce the ROM's pointer-returning call shape.
f32* EnWeiyer_CurFramePtr(SkelAnime* skelAnime) {
    return &skelAnime->curFrame;
}
}
