extern "C" {
#include "z_en_wall_tubo.hpp"

#define FLAGS 0x80000010

void EnWallTubo_Init(Actor* actor, GameState* state);
void EnWallTubo_Destroy(Actor* actor, GameState* state);
void EnWallTubo_Update(Actor* actor, GameState* state);

void FUN_001810b4(EnWallTubo* self, PlayState* play); // N64: EnWallTubo_FindGirl
void FUN_003e7098(EnWallTubo* self, PlayState* play); // N64: EnWallTubo_DetectChu

ActorInit En_Wall_Tubo_InitVars = {
    ACTOR_EN_WALL_TUBO,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnWallTubo),
    (ActorFunc)EnWallTubo_Init,
    (ActorFunc)EnWallTubo_Destroy,
    (ActorFunc)EnWallTubo_Update,
    NULL,
};

void EnWallTubo_Init(Actor* actor, GameState* state) {
    EnWallTubo* self = (EnWallTubo*)actor;

    self->homePos = self->actor.world.pos;
    self->actionFunc = FUN_001810b4;
}

void EnWallTubo_Destroy(Actor* actor, GameState* state) {
}

void FUN_001810b4(EnWallTubo* self, PlayState* play) {
    Actor* actor = play->actorCtx.actorLists[ACTORCAT_NPC].head;

    while (actor != NULL) {
        if (actor->id != ACTOR_EN_BOM_BOWL_MAN) {
            actor = actor->next;
            continue;
        }
        self->bowlingGirl = actor;
        break;
    }

    self->actionFunc = FUN_003e7098;
}

void EnWallTubo_Update(Actor* actor, GameState* state) {
    EnWallTubo* self = (EnWallTubo*)actor;
    PlayState* play = (PlayState*)state;

    if (self->timer != 0) {
        self->timer--;
    }

    self->actionFunc(self, play);
}
}
