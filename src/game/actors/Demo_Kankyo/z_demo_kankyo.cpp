extern "C" {
#include "z_demo_kankyo.hpp"

#define FLAGS 0x00000030
#define R_UPDATE_RATE SREG(30)

void FUN_0034fc7c(void* p);
void SystemArena_Free(void* p);
f32 FUN_00361490(u16 endFrame, u16 startFrame, u16 curFrame);

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

void DemoKankyo_Destroy(Actor* actor, GameState* state) {
    DemoKankyo* self = (DemoKankyo*)actor;
    s32 i;
    if (self->resourcesLoaded) {
        switch (actor->params) {
            case 0:
            case 1:
                for (i = 0; i < 150; i++) {
                    FUN_00350f34(actor, &self->models[i], 0);
                }
                break;
            case 13:
                FUN_00350f34(actor, &self->models[0], &self->models[1], 0);
                break;
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 14:
                FUN_00350f34(actor, &self->models[0], 0);
                break;
            case 7:
                for (i = 0; i < 30; i++) {
                    FUN_00350f34(actor, &self->models[i], 0);
                }
                break;
            case 15:
            case 16:
            case 17:
            case 18:
                if (self->effectResource != NULL) {
                    FUN_0034fc7c(self->effectResource);
                    SystemArena_Free(self->effectResource);
                }
                self->effectResource = NULL;
                break;
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
                break;
        }
    }
}

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

void DemoKankyo_UpdateClouds(DemoKankyo* self, PlayState* play) {
    u8 i;
    for (i = 0; i < 30; i++) {
        self->effects[i].angle += (s16)(R_UPDATE_RATE * self->effects[i].translation / 3.0f);
    }
}

void DemoKankyo_UpdateDoorOfTime(DemoKankyo* self, PlayState* play) {
    Audio_PlayActorSound2(&self->actor, 0x01000175);
    self->effects[0].translation += R_UPDATE_RATE / 3.0f;
    if (self->effects[0].translation >= 102.0f) {
        Audio_PlayActorSound2(&self->actor, 0x010001EA);
        gSaveContext.eventChkInf[4] |= 0x800;
        Actor_Kill(self->actor.child);
        self->actionFunc = DemoKankyo_KillDoorOfTimeCollision;
    }
}

static inline void DemoKankyo_SetPosFromCue(DemoKankyo* self, PlayState* play, s32 cueChannel) {
    Vec3f startPos;
    Vec3f endPos;
    CsCmdActorAction* cue = play->csCtx.npcActions[cueChannel];
    f32 lerp;

    startPos.x = cue->startPos.x;
    startPos.y = cue->startPos.y;
    startPos.z = cue->startPos.z;
    endPos.x = cue->endPos.x;
    endPos.y = cue->endPos.y;
    endPos.z = cue->endPos.z;
    lerp = FUN_00361490(cue->endFrame, cue->startFrame, play->csCtx.frames);
    self->actor.world.pos.x = (endPos.x - startPos.x) * lerp + startPos.x;
    self->actor.world.pos.y = (endPos.y - startPos.y) * lerp + startPos.y;
    self->actor.world.pos.z = (endPos.z - startPos.z) * lerp + startPos.z;
}

void DemoKankyo_UpdateRock(DemoKankyo* self, PlayState* play) {
    CsCmdActorAction* cue;
    if (play->GetCutsceneState() != 0 && (cue = play->csCtx.npcActions[self->actor.params - 2]) != NULL &&
        cue->action == 2) {
        DemoKankyo_SetPosFromCue(self, play, self->actor.params - 2);
        if (play->csCtx.frames < play->csCtx.npcActions[self->actor.params - 2]->endFrame - 5) {
            self->effects[0].angle = 255;
        } else {
            self->effects[0].angle = (play->csCtx.npcActions[self->actor.params - 2]->endFrame - play->csCtx.frames) * 0.2f * 255.0f;
            if (self->effects[0].angle < 0) {
                self->effects[0].angle = 0;
            }
        }
    } else {
        self->effects[0].angle = 0;
    }
    self->effects[0].offset.x += self->effects[0].velocity.x * (R_UPDATE_RATE / 3.0f);
    self->effects[0].offset.y += self->effects[0].velocity.y * (R_UPDATE_RATE / 3.0f);
    self->effects[0].offset.z += self->effects[0].velocity.z * (R_UPDATE_RATE / 3.0f);
}

}
