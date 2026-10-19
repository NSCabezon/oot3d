extern "C" {
#include "z_obj_makekinsuta.hpp"

#define FLAGS 0x00000010

void ObjMakekinsuta_Init(Actor* actor, GameState* state);
void ObjMakekinsuta_Destroy(Actor* actor, GameState* state);
void ObjMakekinsuta_Update(Actor* actor, GameState* state);

void FUN_003bf620(ObjMakekinsuta* self, PlayState* play); // N64: func_80B98320
void ObjMakekinsuta_DoNothing(ObjMakekinsuta* self, PlayState* play);

ActorInit Obj_Makekinsuta_InitVars = {
    ACTOR_OBJ_MAKEKINSUTA,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ObjMakekinsuta),
    (ActorFunc)ObjMakekinsuta_Init,
    (ActorFunc)Actor_Noop,
    (ActorFunc)ObjMakekinsuta_Update,
    NULL,
};

void ObjMakekinsuta_Init(Actor* actor, GameState* state) {
    ObjMakekinsuta* self = (ObjMakekinsuta*)actor;

    // N64 printed a param-validity message here; 3DS keeps only the assignment
    self->actionFunc = FUN_003bf620;
}

void FUN_003bf620(ObjMakekinsuta* self, PlayState* play) {
    if (self->unk_1AA != 0) {
        if ((self->timer > ADJUST_FRAMECOUNT(59)) && !FUN_0033caf8(GET_PLAYER(play))) {
            Actor_Spawn(&play->actorCtx, play, ACTOR_EN_SW, self->actor.world.pos.x, self->actor.world.pos.y,
                        self->actor.world.pos.z, 0, self->actor.shape.rot.y, 0, self->actor.params | 0x8000, 1);
            self->actionFunc = ObjMakekinsuta_DoNothing;
        } else {
            self->timer++;
        }
    } else {
        self->timer = 0;
    }
}

void ObjMakekinsuta_DoNothing(ObjMakekinsuta* self, PlayState* play) {
}

void ObjMakekinsuta_Update(Actor* actor, GameState* state) {
    ObjMakekinsuta* self = (ObjMakekinsuta*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
}
}
