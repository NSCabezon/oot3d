extern "C" {
#include "global.hpp"

GLOBAL_ASM("../src/game/dup/dup_isfatalerrmode_syms.s")

extern s8 gUnk_00558A98[]; // unlabeled global at 0x00558A98 (EQU in dup_isfatalerrmode_syms.s)

s8 IsFatalErrMode(void) {
    return gUnk_00558A98[2];
}

}
