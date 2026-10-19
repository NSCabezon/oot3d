extern "C" {
#include "z_en_anubice_tag.hpp"

#define FLAGS 0x00000010

void EnAnubiceTag_Init(Actor* actor, GameState* state);
void EnAnubiceTag_Destroy(Actor* actor, GameState* state);
void EnAnubiceTag_Update(Actor* actor, GameState* state);
void EnAnubiceTag_Draw(Actor* actor, GameState* state);

void FUN_001786c4(EnAnubiceTag* self, PlayState* play); // N64: EnAnubiceTag_SpawnAnubis

ActorInit En_Anubice_Tag_InitVars = {
    ACTOR_EN_ANUBICE_TAG,
    ACTORCAT_SWITCH,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnAnubiceTag),
    (ActorFunc)EnAnubiceTag_Init,
    (ActorFunc)EnAnubiceTag_Destroy,
    (ActorFunc)EnAnubiceTag_Update,
    (ActorFunc)EnAnubiceTag_Draw,
};

void EnAnubiceTag_Init(Actor* actor, GameState* state) {
    EnAnubiceTag* self = (EnAnubiceTag*)actor;

    self->actor.unk_19A = 1; // 3DS addition

    if (self->actor.params < -1) {
        self->actor.params = 0;
    } else if (self->actor.params != 0) {
        self->extraTriggerRange = self->actor.params * 40.0f;
    }
    self->actionFunc = FUN_001786c4;
}

void EnAnubiceTag_Destroy(Actor* actor, GameState* state) {
}

void EnAnubiceTag_Update(Actor* actor, GameState* state) {
    EnAnubiceTag* self = (EnAnubiceTag*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
}

void EnAnubiceTag_Draw(Actor* actor, GameState* state) {
}
}
