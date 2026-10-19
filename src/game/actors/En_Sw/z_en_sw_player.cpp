extern "C" {
#include "z_en_sw.hpp"

s32 FUN_0035f250(PlayState* play) {
    Player* player = GET_PLAYER(play);

    return player->stateFlags2 & 0x1000;
}
}
