extern "C" {
#include "z_en_skj.hpp"

f32* EnSkj_CurFramePtr(SkelAnime* skelAnime) {
    return &skelAnime->curFrame;
}

}
