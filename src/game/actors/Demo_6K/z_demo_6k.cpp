extern "C" {
#include "z_demo_6k.hpp"

#define FLAGS 0x00000010

void Demo6K_Init(Actor* actor, PlayState* play);
void Demo6K_Destroy(Actor* actor, GameState* state);
void Demo6K_Update(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Demo_6K.data.s")

GLOBAL_ASM("../src/game/actors/Demo_6K/data_syms.s")

ActorInit Demo_6K_InitVars = {
    ACTOR_DEMO_6K,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(Demo6K),
    (ActorFunc)Demo6K_Init,
    (ActorFunc)Demo6K_Destroy,
    (ActorFunc)Demo6K_Update,
    NULL,
};

extern s16 Demo6K_sObjectIds[];
extern s32 Demo6K_sModelIndex[];
s32 Object_GetIndex(void* objectCtx, s16 id);
void* LightContext_InsertLight(PlayState* play, void* lightCtx, void* lightInfo);
void Lights_PointNoGlowSetInfo(void* light, f32 x, f32 y, f32 z, u8 r, u8 g, u8 b, s16 radius, s32 pad);
void FUN_003f1510(Demo6K* self, PlayState* play);
void FUN_001d1b9c(Actor* thisx, PlayState* play);
void FUN_0014111c(Demo6K* self, PlayState* play);
void FUN_001f010c(Actor* thisx, PlayState* play);
void FUN_0020783c(Demo6K* self, PlayState* play);
void FUN_001d1a2c(Actor* thisx, PlayState* play);
void FUN_00140f84(Demo6K* self, PlayState* play);
void FUN_001eff74(Actor* thisx, PlayState* play);
void FUN_0015abf8(Demo6K* self, PlayState* play);
void FUN_0015d1f0(Demo6K* self, PlayState* play);
void FUN_0015cd7c(Demo6K* self, PlayState* play);
void FUN_00182b3c(Demo6K* self, PlayState* play);
void FUN_0020ce24(Actor* thisx, PlayState* play);
void FUN_00225580(Actor* thisx, PlayState* play);
void FUN_001a0c6c(Demo6K* self, PlayState* play);
void FUN_001ea610(Demo6K* self, PlayState* play);

static inline void Demo6K_SetupAction(Demo6K* self, ActorFunc f) {
    self->actionFunc = f;
}

static void Demo6K_InitParticle(Demo6K* self, s32 i) {
    self->unk_174[i] = (i * 10.0f) - 80.0f;
    self->unk_1B4[i] = Rand_ZeroFloat(100.0f) + 100.0f;
    self->unk_1F4[i] = -3.0f - Rand_ZeroFloat(6.0f);
    self->unk_274[i] = (s32)Rand_ZeroFloat(6.0f);
    self->unk_234[i] = Rand_ZeroFloat(0.02f) + 0.01f;
    if (self->zarInfo != NULL) {
        void* ptr = self->zarInfo->GetCMABByIndex(Demo6K_sModelIndex[self->unk_274[i]]);

        self->skelAnimModel[8 + i]->unk_0C->FUN_00372d94(ptr);
    }
}

void Demo6K_Init(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    Demo6K* self = (Demo6K*)thisx;
    s32 params = thisx->params;
    s32 objectSlot;
    s32 i;

    if (Demo6K_sObjectIds[params] != 1) {
        objectSlot = Object_GetIndex(&play->objectCtx, Demo6K_sObjectIds[params]);
    } else {
        objectSlot = 0;
    }
    if (objectSlot >= 0) {
        self->requiredObjectSlot = objectSlot;
    }

    Demo6K_SetupAction(self, (ActorFunc)FUN_003f1510);
    self->timer1 = 0;
    self->flags = 0;
    self->timer2 = 0;
    self->skelAnimModel[0] = NULL;
    self->skelAnimModel[1] = NULL;
    self->skelAnimModel[2] = NULL;
    self->skelAnimModel[3] = NULL;
    self->skelAnimModel[4] = NULL;
    self->skelAnimModel[5] = NULL;
    self->skelAnimModel[6] = NULL;
    self->skelAnimModel[7] = NULL;
    self->skelAnimModel[8] = NULL;
    self->skelAnimModel[9] = NULL;
    self->skelAnimModel[10] = NULL;
    self->skelAnimModel[11] = NULL;
    self->skelAnimModel[12] = NULL;
    self->skelAnimModel[13] = NULL;
    self->skelAnimModel[14] = NULL;
    self->skelAnimModel[15] = NULL;
    self->skelAnimModel[16] = NULL;
    self->skelAnimModel[17] = NULL;
    self->skelAnimModel[18] = NULL;
    self->skelAnimModel[19] = NULL;
    self->skelAnimModel[20] = NULL;
    self->skelAnimModel[21] = NULL;
    self->skelAnimModel[22] = NULL;
    self->skelAnimModel[23] = NULL;
    self->skelAnimModel[24] = NULL;
    self->skelAnimModel[25] = NULL;
    self->skelAnimModel[26] = NULL;
    self->skelAnimModel[27] = NULL;
    self->skelAnimModel[28] = NULL;

    switch (params) {
        case 0:
            self->drawFunc = (ActorFunc)FUN_001d1b9c;
            self->initActionFunc = (Demo6KActionFunc)FUN_0014111c;
            Actor_SetScale(thisx, 1.0f);

            for (i = 0; i < 16; i++) {
                Demo6K_InitParticle(self, i);
                self->unk_1B4[i] = 0.0f;
            }

            self->unk_170 = 0.0f;
            break;
        case 1:
            self->drawFunc = (ActorFunc)FUN_001f010c;
            self->initActionFunc = (Demo6KActionFunc)FUN_0020783c;
            Actor_SetScale(thisx, 0.228f);
            break;
        case 2:
            self->drawFunc = (ActorFunc)FUN_001d1a2c;
            self->initActionFunc = (Demo6KActionFunc)FUN_00140f84;
            Actor_SetScale(thisx, 0.1f);
            self->unk_164 = 1.0f;
            self->unk_168 = 1.0f;
            self->unk_16C = 0.0f;
            self->unk_170 = 0.0f;
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            self->drawFunc = (ActorFunc)FUN_001eff74;
            self->initActionFunc = (Demo6KActionFunc)FUN_0015abf8;
            Actor_SetScale(thisx, 0.0f);
            self->unk_293 = params - 3;
            break;
        case 9:
        case 10:
            self->drawFunc = (ActorFunc)FUN_001eff74;
            Actor_SetScale(thisx, 0.0f);
            self->initActionFunc = (Demo6KActionFunc)FUN_0015d1f0;
            break;
        case 11:
            self->drawFunc = (ActorFunc)FUN_001eff74;
            Actor_SetScale(thisx, 0.0f);
            self->initActionFunc = (Demo6KActionFunc)FUN_0015cd7c;
            thisx->velocity.x = thisx->velocity.y = thisx->velocity.z = 0.0f;
            Audio_PlayActorSound2(thisx, 0x010001FC);
            break;
        case 12:
            Actor_SetScale(thisx, 0.0f);
            self->initActionFunc = (Demo6KActionFunc)FUN_00182b3c;
            self->drawFunc = (ActorFunc)FUN_0020ce24;
            Actor_ChangeType(play, (ActorContext*)((u8*)play + 0x208C), thisx, 7);
            break;
        case 13:
            Actor_SetScale(thisx, 0.14f);
            Demo6K_SetupAction(self, (ActorFunc)FUN_001a0c6c);
            thisx->draw = (ActorFunc)FUN_00225580;
            self->unk_293 = 0;
            thisx->objBankIndex = self->requiredObjectSlot;
            self->unk_294 = 1;
            FUN_00372f38(thisx, play, &self->skelAnimModel[28], 1, 0);
            break;
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
            thisx->flags |= 0x20;
            self->drawFunc = (ActorFunc)FUN_001eff74;
            self->initActionFunc = (Demo6KActionFunc)FUN_001ea610;
            self->flags |= 1;
            Actor_SetScale(thisx, 0.2f);
            self->unk_293 = params - 14;
            break;
    }

    switch (params) {
        case 9:
            self->unk_293 = 0;
            break;
        case 10:
            self->unk_293 = 5;
            break;
        case 11:
            self->unk_293 = 4;
            break;
    }

    Lights_PointNoGlowSetInfo(&self->lightInfo, thisx->world.pos.x, thisx->world.pos.y, thisx->world.pos.z, 255, 255,
                              255, 100, 0);
    self->lightNode = LightContext_InsertLight(play, (u8*)play + 0xA70, &self->lightInfo);
}


typedef void (*Demo6KFreeModelsFunc)(Actor*, ...);
void LightContext_RemoveLight(PlayState* play, void* lightCtx, void* lightNode);

void Demo6K_Destroy(Actor* thisx, GameState* state) {
    Demo6K* self = (Demo6K*)thisx;
    PlayState* play = (PlayState*)state;
    Demo6KFreeModelsFunc freeModels = (Demo6KFreeModelsFunc)FUN_00350f34;

    LightContext_RemoveLight(play, (u8*)play + 0xA70, self->lightNode);
    switch (thisx->params) {
        case 0:
            freeModels(thisx, &self->skelAnimModel[8], &self->skelAnimModel[9], &self->skelAnimModel[10],
                       &self->skelAnimModel[11], &self->skelAnimModel[12], &self->skelAnimModel[13],
                       &self->skelAnimModel[14], &self->skelAnimModel[15], &self->skelAnimModel[16],
                       &self->skelAnimModel[17], &self->skelAnimModel[18], &self->skelAnimModel[19],
                       &self->skelAnimModel[20], &self->skelAnimModel[21], &self->skelAnimModel[22],
                       &self->skelAnimModel[23], &self->skelAnimModel[24], NULL);
            break;
        case 1:
            freeModels(thisx, &self->skelAnimModel[0], &self->skelAnimModel[1], &self->skelAnimModel[2],
                       &self->skelAnimModel[3], &self->skelAnimModel[4], &self->skelAnimModel[5], NULL);
            break;
        case 2:
            freeModels(thisx, &self->skelAnimModel[7], &self->skelAnimModel[6], NULL);
            break;
        case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11:
        case 14: case 15: case 16: case 17: case 18: case 19:
            freeModels(thisx, &self->skelAnimModel[8], &self->skelAnimModel[9], &self->skelAnimModel[25],
                       &self->skelAnimModel[26], NULL);
            break;
        case 12:
            freeModels(thisx, &self->skelAnimModel[27], NULL);
            break;
        case 13:
            freeModels(thisx, &self->skelAnimModel[28], NULL);
            break;
    }
}


void Demo6K_Update(Actor* actor, GameState* state) {
    Demo6K* self = (Demo6K*)actor;

    self->actionFunc(actor, state);
}
}
