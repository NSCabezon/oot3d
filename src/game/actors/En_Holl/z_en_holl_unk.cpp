// En_Holl helper kept in its own TU: armcc inlines it into EnHoll_Update when it lives in the same file,
// but the ROM calls it as an opaque function.
extern "C" {
#include "z_en_holl.hpp"

extern s8 gUnk_0054ABD4[]; // unlabeled global at 0x0054ABD4 (EQU in src/game/shared_syms.s)

s8 FUN_00339f0c(void);

s8 FUN_00339f0c(void) {
    return gUnk_0054ABD4[2];
}
}
