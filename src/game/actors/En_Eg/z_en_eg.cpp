extern "C" {
#include "z_en_eg.hpp"

#define FLAGS 0x00000010

void EnEg_Init(Actor* actor, GameState* state);
void EnEg_Destroy(Actor* actor, GameState* state);
void EnEg_Update(Actor* actor, GameState* state);
void EnEg_Draw(Actor* actor, GameState* state);

void FUN_00249af8(EnEg* self, PlayState* play); // N64: func_809FFDC8

ActorInit En_Eg_InitVars = {
    ACTOR_EN_EG,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_ZL2,
    sizeof(EnEg),
    (ActorFunc)EnEg_Init,
    (ActorFunc)EnEg_Destroy,
    (ActorFunc)EnEg_Update,
    (ActorFunc)EnEg_Draw,
};

// 0x00522DE4 (previously GLOBAL_ASM("data/z_En_Eg.data.s"), placed right after InitVars)
static EnEgActionFunc sActionFuncs[] = {
    FUN_00249af8,
};

// On 3DS the N64 file-static "sVoided" lives in PlayState (+0x7FA0), past the currently mapped struct.
#define EN_EG_VOIDED(play) (*(s32*)((u8*)(play) + 0x7FA0))

void EnEg_Destroy(Actor* actor, GameState* state) {
}

void EnEg_Init(Actor* actor, GameState* state) {
    EnEg* self = (EnEg*)actor;

    self->action = 0;
}

void FUN_00249af8(EnEg* self, PlayState* play) {
    if (!EN_EG_VOIDED(play) && (gSaveContext.timer2Value <= 0) && Flags_GetSwitch(play, 0x36)) {
        FUN_0036f4f0(play); // Play_TriggerRespawn
        gSaveContext.respawnFlag = -2;
        Audio_SetBGM(0, 0);
        play->fadeOutTransition = 2;
        Audio_PlaySoundGeneral(0x10004E1, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0); // NA_SE_OC_ABYSS
        EN_EG_VOIDED(play) = true;
    }
}

void EnEg_Update(Actor* actor, GameState* state) {
    EnEg* self = (EnEg*)actor;
    PlayState* play = (PlayState*)state;
    s32 action = self->action;

    if ((action < 0) || (0 < action) || (sActionFuncs[action] == NULL)) {
        // "The main mode is wrong!!!" (debug print stripped)
    } else {
        sActionFuncs[action](self, play);
    }
}

void EnEg_Draw(Actor* actor, GameState* state) {
}
}
