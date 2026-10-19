extern "C" {
#include "z_obj_roomtimer.hpp"

#define FLAGS 0x00000010

void ObjRoomtimer_Init(Actor* actor, GameState* state);
void ObjRoomtimer_Destroy(Actor* actor, GameState* state);
void ObjRoomtimer_Update(Actor* actor, GameState* state);

void FUN_003e7c50(ObjRoomtimer* self, PlayState* play); // N64: func_80B9D054 (start timer)
void FUN_0010862c(ObjRoomtimer* self, PlayState* play); // N64: func_80B9D0B0 (wait for room clear)

ActorInit Obj_Roomtimer_InitVars = {
    ACTOR_OBJ_ROOMTIMER,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ObjRoomtimer),
    (ActorFunc)ObjRoomtimer_Init,
    (ActorFunc)ObjRoomtimer_Destroy,
    (ActorFunc)ObjRoomtimer_Update,
    NULL,
};

void ObjRoomtimer_Init(Actor* actor, GameState* state) {
    ObjRoomtimer* self = (ObjRoomtimer*)actor;

    self->switchFlag = (self->actor.params >> 10) & 0x3F;
    self->actor.params = self->actor.params & 0x3FF;

    if (self->actor.params != 0x3FF) {
        self->actor.params = CLAMP_MAX(self->actor.params, 600);
    }

    self->actionFunc = FUN_003e7c50;
}

void ObjRoomtimer_Destroy(Actor* actor, GameState* state) {
    ObjRoomtimer* self = (ObjRoomtimer*)actor;

    if ((self->actor.params != 0x3FF) && (gSaveContext.timer1Value > 0)) {
        gSaveContext.timer1State = 10; // TIMER_STATE_STOP
    }
}

void FUN_003e7c50(ObjRoomtimer* self, PlayState* play) {
    if (self->actor.params != 0x3FF) {
        FUN_0035239c(self->actor.params); // N64: Interface_SetTimer
    }

    Actor_ChangeType(play, &play->actorCtx, &self->actor, ACTORCAT_PROP);
    self->actionFunc = FUN_0010862c;
}

void FUN_0010862c(ObjRoomtimer* self, PlayState* play) {
    if (Flags_GetTempClear(play, self->actor.room)) {
        if (self->actor.params != 0x3FF) {
            gSaveContext.timer1State = 10; // TIMER_STATE_STOP
        }
        Flags_SetClear(play, self->actor.room);
        Flags_SetSwitch(play, self->switchFlag);
        // 3DS addition: no chime in Master Quest scene 0x0B room 1
        if (gSaveContext.masterQuestFlag && (play->sceneNum == 0xB) && (self->actor.room == 1)) {
            Actor_Kill(&self->actor);
            return;
        }
        FUN_00372244(&play->unk_5FCC, 0x1E, 0x1000487); // NA_SE_SY_CORRECT_CHIME
        Actor_Kill(&self->actor);
        return;
    }

    if ((self->actor.params != 0x3FF) && (gSaveContext.timer1Value == 0)) {
        Audio_PlaySoundGeneral(0x10004E1, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0); // NA_SE_OC_ABYSS
        Gameplay_TriggerVoidOut(play);
        Actor_Kill(&self->actor);
    }
}

void ObjRoomtimer_Update(Actor* actor, GameState* state) {
    ObjRoomtimer* self = (ObjRoomtimer*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
}
}
