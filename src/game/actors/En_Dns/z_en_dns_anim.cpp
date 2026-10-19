extern "C" {
#include "z_en_dns.hpp"

f32* EnDns_CurFramePtr(SkelAnime* skelAnime) {
    return &skelAnime->curFrame;
}
}
