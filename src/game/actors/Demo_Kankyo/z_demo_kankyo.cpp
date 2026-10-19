extern "C" {
#include "z_demo_kankyo.hpp"

#define FLAGS 0x00000030

void DemoKankyo_Init(Actor* actor, GameState* state);
void DemoKankyo_Destroy(Actor* actor, GameState* state);
void DemoKankyo_Update(Actor* actor, GameState* state);
void DemoKankyo_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Demo_Kankyo.data.s")

ActorInit Demo_Kankyo_InitVars = {
    ACTOR_DEMO_KANKYO,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(DemoKankyo),
    (ActorFunc)DemoKankyo_Init,
    (ActorFunc)DemoKankyo_Destroy,
    (ActorFunc)DemoKankyo_Update,
    (ActorFunc)DemoKankyo_Draw,
};

GLOBAL_ASM("asm/DemoKankyo_Init.s")

#if 0
void DemoKankyo_Destroy(DemoKankyo* self, PlayState* play) {}
#endif
GLOBAL_ASM("asm/DemoKankyo_Destroy.s")

void DemoKankyo_Update(Actor* actor, GameState* state) {
    DemoKankyo* self = (DemoKankyo*)actor;
    self->actionFunc(self, (PlayState*)state);
}

GLOBAL_ASM("asm/DemoKankyo_Draw.s")

void DemoKankyo_DoNothing(DemoKankyo* self, PlayState* play) {}

void DemoKankyo_DoNothing2(DemoKankyo* self, PlayState* play) {
    self->actionFunc = DemoKankyo_DoNothing;
}

void DemoKankyo_KillDoorOfTimeCollision(DemoKankyo* self, PlayState* play) {
    Actor_Kill(self->actor.child);
}

GLOBAL_ASM("asm/DemoKankyo_SetupType.s")

#if 0
// N64 logic is a 30-entry loop. The 3DS body uses update-rate-scaled
// movement and a different packed actor layout, so this draft is not enabled.
void DemoKankyo_UpdateClouds(DemoKankyo* self, PlayState* play) {
    u8 i;
    for (i = 0; i < 30; i++) {
        self->unk_150[i].unk_20 += (s16)self->unk_150[i].unk_18;
    }
}
#endif
GLOBAL_ASM("asm/DemoKankyo_UpdateClouds.s")

#if 0
void DemoKankyo_UpdateDoorOfTime(DemoKankyo* self, PlayState* play) {
    Actor_PlaySfx(&self->actor, NA_SE_EV_STONE_STATUE_OPEN - SFX_FLAG);
    self->unk_150[0].unk_18 += 1.0f;
    if (self->unk_150[0].unk_18 >= 102.0f) {
        Actor_PlaySfx(&self->actor, NA_SE_EV_STONEDOOR_STOP);
        SET_EVENTCHKINF(EVENTCHKINF_OPENED_DOOR_OF_TIME);
        Actor_Kill(self->actor.child);
        DemoKankyo_SetupAction(self, DemoKankyo_KillDoorOfTimeCollision);
    }
}
#endif
GLOBAL_ASM("asm/DemoKankyo_UpdateDoorOfTime.s")

#if 0
void DemoKankyo_UpdateRock(DemoKankyo* self, PlayState* play) {
    if (play->csCtx.state != CS_STATE_IDLE && play->csCtx.actorCues[self->actor.params - DEMOKANKYO_ROCK_1] != NULL) {
        DemoKankyo_SetPosFromCue(self, play, self->actor.params - DEMOKANKYO_ROCK_1);
    }
}
#endif
GLOBAL_ASM("asm/DemoKankyo_UpdateRock.s")

}
