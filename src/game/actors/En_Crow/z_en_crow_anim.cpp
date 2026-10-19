extern "C" {
#include "z_en_crow.hpp"

// Out-of-line accessor (own TU so armlink/armcc do not inline it into EnCrow_OverrideLimbDraw).
f32* EnCrow_CurFramePtr(SkelAnime* skelAnime) {
    return &skelAnime->curFrame;
}

}
