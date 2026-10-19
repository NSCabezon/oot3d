extern "C" {
#include "z_en_hy.hpp"

static inline u8 EnHy_InvSlot(s32 i) {
    return gSaveContext.inventory.items[i];
}

s32 FUN_00341e1c(s32 item) {
    return (item == EnHy_InvSlot(18)) || (item == EnHy_InvSlot(19)) || (item == EnHy_InvSlot(20)) ||
           (item == EnHy_InvSlot(21));
}

}
