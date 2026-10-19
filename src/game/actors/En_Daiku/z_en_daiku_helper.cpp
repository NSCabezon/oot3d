extern "C" {
#include "z_en_daiku.hpp"

u32 FUN_00117e44(Actor* actor) {
    return *(u32*)((u8*)actor + 0x134) == 0;
}
}
