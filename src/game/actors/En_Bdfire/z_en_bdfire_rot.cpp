extern "C" {
#include "z_en_bdfire.hpp"

// Own TU: called out of line (tail) from EnBdfire_Update.
void FUN_0033a61c(Actor* actor) {
    actor->shape.rot = actor->world.rot;
}

}
