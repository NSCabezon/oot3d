extern "C" {
#include "global.hpp"

#define PARAMS_GET_U(params, start, count) ((((u16)(params)) >> (start)) & ((1 << (count)) - 1))

void FUN_0010dc24(Actor* actor, PlayState* play) {
    if (Flags_GetTreasure(play, PARAMS_GET_U(actor->params, 8, 5))) {
        Actor_Kill(actor);
    }
}

}
