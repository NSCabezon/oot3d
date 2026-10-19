extern "C" {
#include "z_en_po_sisters.hpp"

// Own TU: out-of-line frame accessors (the ROM reads curFrame/endFrame through a call that armlink inlines;
// in the caller's TU armcc would fold the field loads instead).
f32* EnPoSisters_CurFramePtr(SkelAnime* skel) {
    return &skel->curFrame;
}

f32* EnPoSisters_EndFramePtr(SkelAnime* skel) {
    return &skel->endFrame;
}
}
