extern "C" {
#include "z_door_toki.hpp"

#define FLAGS 0x80000000

void DoorToki_Init(DoorToki* self, PlayState* play);
void DoorToki_Destroy(DoorToki* self, PlayState* play);
void DoorToki_Update(DoorToki* self, PlayState* play);

ActorInit Door_Toki_InitVars = {
    ACTOR_DOOR_TOKI,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_TOKI_OBJECTS,
    sizeof(DoorToki),
    (ActorFunc)DoorToki_Init,
    (ActorFunc)DoorToki_Destroy,
    (ActorFunc)DoorToki_Update,
    NULL,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_STOP),
};

void DoorToki_Init(DoorToki* self, PlayState* play) {
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, 0);
    self->dyna.actor.unk_19A = 1;
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
}

void DoorToki_Destroy(DoorToki* self, PlayState* play) {
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
}

void DoorToki_Update(DoorToki* self, PlayState* play) {
    if (gSaveContext.eventChkInf[4] & 0x800) {
        FUN_0036b940(play, &play->colCtx.dyna, self->dyna.bgId);
    } else {
        FUN_0036d15c(play, &play->colCtx.dyna, self->dyna.bgId);
    }
}
}
