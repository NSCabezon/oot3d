extern "C" {
#include "z_en_cs.hpp"

f32* EnCs_CurFramePtr(SkelAnime* skelAnime) {
    return &skelAnime->curFrame;
}
}
