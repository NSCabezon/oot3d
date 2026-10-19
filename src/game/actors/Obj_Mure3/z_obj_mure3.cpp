extern "C" {
#include "z_obj_mure3.hpp"

#define FLAGS 0x00000000

void ObjMure3_Init(Actor* actor, GameState* state);
void ObjMure3_Destroy(Actor* actor, GameState* state);
void ObjMure3_Update(Actor* actor, GameState* state);

void FUN_0021b634(ObjMure3* self, PlayState* play); // N64: func_80B9A9D0 (spawn blue rupee column)
void FUN_00258950(ObjMure3* self, PlayState* play); // N64: func_80B9AA90 (spawn green rupee line)
void FUN_001e6440(ObjMure3* self, PlayState* play); // N64: func_80B9ABA0 (spawn green circle + red)

// N64: func_80B9AF34. On 3DS this tiny body was folded by the linker into an unlabeled
// stub inside ObjMure2_Wait.s (0x0015ddc4), so there is no symbol to reference.
#define ObjMure3_SetupWaitForPlayer ((ObjMure3ActionFunc)0x0015ddc4)

ActorInit Obj_Mure3_InitVars = {
    ACTOR_OBJ_MURE3,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ObjMure3),
    (ActorFunc)ObjMure3_Init,
    (ActorFunc)ObjMure3_Destroy,
    (ActorFunc)ObjMure3_Update,
    NULL,
};

// sRupeeCounts and sSpawnFuncs are only used by the helpers still in asm (FUN_003af0b4,
// FUN_003b3218), which reference them by absolute address; "used" keeps them emitted.
__attribute__((used)) static s16 sRupeeCounts[] = { 5, 5, 7, 0 };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 1800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 100, ICHAIN_STOP),
};

__attribute__((used)) static ObjMure3SpawnFunc sSpawnFuncs[] = { FUN_0021b634, FUN_00258950, FUN_001e6440 };

void ObjMure3_Init(Actor* actor, GameState* state) {
    ObjMure3* self = (ObjMure3*)actor;
    PlayState* play = (PlayState*)state;

    if (Flags_GetSwitch(play, self->actor.params & 0x3F)) {
        Actor_Kill(&self->actor);
        return;
    }
    Actor_ProcessInitChain(&self->actor, sInitChain);
    self->actionFunc = ObjMure3_SetupWaitForPlayer;
}

void ObjMure3_Destroy(Actor* actor, GameState* state) {
}

void ObjMure3_Update(Actor* actor, GameState* state) {
    ObjMure3* self = (ObjMure3*)actor;

    self->actionFunc(self, (PlayState*)state);
}
}
