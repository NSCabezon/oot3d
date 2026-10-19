// FUN_001ebf08 (Actor_SetParentAndHeld, N64 func_8002F5C4). Own TU so that callers keep calling it out of line.
extern "C" {
#include "z_en_bombf.hpp"

void FUN_001ebf08(Actor* actorA, Actor* actorB, PlayState* play) {
    Actor* parent = actorA->parent;

    if (parent->id == ACTOR_PLAYER) {
        Player* player = (Player*)parent;

        *(Actor**)((u8*)player + 0x1224) = actorB;
        player->interactRangeActor = actorB;
    }

    parent->child = actorB;
    actorB->parent = parent;
    actorA->parent = NULL;
}
}
