// Tiny accessor in its own TU: armlink --inline turns calls to it into `add r0, r0, #0x3c` (the ROM's curFrame reads in
// En_Horse show `add r0,r4,#0x1c4; add r0,r0,#0x3c; vldr sN,[r0]`, and the call acts as a memory barrier for armcc).
extern "C" {
#include "z_en_horse.hpp"

f32* EnHorse_CurFramePtr(SkelAnime* skelAnime) {
    return &skelAnime->curFrame;
}
}
