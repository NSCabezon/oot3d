extern "C" {
#include "z_en_owl.hpp"

#define FLAGS 0x00000019

void EnOwl_Init(Actor* actor, GameState* state);
void EnOwl_Destroy(Actor* actor, PlayState* state);
void EnOwl_Update(Actor* actor, GameState* state);
void EnOwl_Draw(Actor* actor, PlayState* state);
void EnOwl_WaitDeathMountainShortcut(EnOwl* self, PlayState* play);
void FUN_0035ff80(EnOwl* self);

extern u8 EnRiverSound_sState[];
s32 FUN_00369a48(Actor* actor, PlayState* play);

void FUN_00231364(s32 arg0, s32 arg1) {
    EnRiverSound_sState[1] = 1;
    EnRiverSound_sState[2] = arg1;
    EnRiverSound_sState[3] = arg0;
}

void FUN_001eb37c(EnOwl* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        Audio_SetBGM(1, 0);
        *(void**)((u8*)self + 0x1370) = (void*)EnOwl_WaitDeathMountainShortcut;
    }
}

void FUN_0012bee0(EnOwl* self, PlayState* play);
void FUN_0012bf54(EnOwl* self, PlayState* play);

void FUN_0012be6c(EnOwl* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 5 && Message_ShouldAdvance(play)) {
        Message_ContinueTextbox(play, 0x2069);
        self->actionFunc = (void*)FUN_0012bee0;
        self->actionFlags &= ~2;
        FUN_0035ff80(self);
    }
}

void FUN_0012bee0(EnOwl* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 5 && Message_ShouldAdvance(play)) {
        Message_ContinueTextbox(play, 0x206A);
        self->actionFunc = (void*)FUN_0012bf54;
        self->actionFlags |= 2;
        FUN_0035ff80(self);
    }
}

GLOBAL_ASM("data/z_En_Owl.data.s")

ActorInit En_Owl_InitVars = {
    ACTOR_EN_OWL,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_OWL,
    sizeof(EnOwl),
    (ActorFunc)EnOwl_Init,
    (ActorFunc)EnOwl_Destroy,
    (ActorFunc)EnOwl_Update,
    (ActorFunc)EnOwl_Draw,
};

GLOBAL_ASM("asm/EnOwl_Init.s")

#if 0
void EnOwl_Destroy(Actor* actor, PlayState* play) {
    u8* self = (u8*)actor;

    SkelAnime_Free2((SkelAnime*)(self + 0x1FC));
    SkelAnime_Free2((SkelAnime*)(self + 0xC6C));
    ((FaceAnimations*)(self + 0xAA0))->Destroy();
    ((FaceAnimations*)(self + 0x1168))->Destroy();
}
#endif
GLOBAL_ASM("asm/EnOwl_Destroy.s")

s32 EnOwl_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnOwl_PostLimbUpdate(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);

#if 0
void EnOwl_Draw(Actor* actor, PlayState* play) {
    u8* self = (u8*)actor;
    s16 eyeIndex = *(s16*)(self + 0x1354);

    ((FaceAnimations*)*(void**)(self + 0x1338))->FUN_0035e3a4(0, eyeIndex);
    ((FaceAnimations*)*(void**)(self + 0x1338))->FUN_0035e330();
    SkelAnime_DrawOpa(*(SkelAnime**)(self + 0x1334), (MTX34*)(self + 0x148), EnOwl_OverrideLimbDraw,
                       EnOwl_PostLimbUpdate, actor, 0);
    *(u8*)(self + 0x133C) = 1;
}
#endif

GLOBAL_ASM("asm/EnOwl_Update.s")

GLOBAL_ASM("asm/EnOwl_Draw.s")

GLOBAL_ASM("asm/EnOwl_ChangeMode.s")

GLOBAL_ASM("asm/EnOwl_CheckInitTalk.s")

GLOBAL_ASM("asm/EnOwl_ConfirmKokiriMessage.s")

GLOBAL_ASM("asm/EnOwl_OverrideLimbDraw.s")

#if 0
void EnOwl_PostLimbUpdate(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    Vec3f vec;
    u8* self = (u8*)thisx;

    vec.z = 0.0f;
    vec.x = (*(u16*)(self + 0x135C) & 2) ? 700.0f : 1400.0f;
    vec.y = (*(u16*)(self + 0x135C) & 2) ? 400.0f : 0.0f;
    if (limbIndex == 2) {
        VEC3TransformAsm((Vector3f*)(self + 0x3C), mtx, (Vector3f*)&vec);
    }
}
#endif
GLOBAL_ASM("asm/EnOwl_PostLimbUpdate.s")

GLOBAL_ASM("asm/EnOwl_WaitDeathMountainShortcut.s")

GLOBAL_ASM("asm/EnOwl_WaitDefault.s")

GLOBAL_ASM("asm/EnOwl_WaitDesertColossus.s")

GLOBAL_ASM("asm/EnOwl_WaitGerudo.s")

GLOBAL_ASM("asm/EnOwl_WaitHyliaShortcut.s")

GLOBAL_ASM("asm/EnOwl_WaitHyruleCastle.s")

GLOBAL_ASM("asm/EnOwl_WaitKakariko.s")

GLOBAL_ASM("asm/EnOwl_WaitLWPostSaria.s")

GLOBAL_ASM("asm/EnOwl_WaitLWPreSaria.s")

GLOBAL_ASM("asm/EnOwl_WaitLakeHylia.s")

GLOBAL_ASM("asm/EnOwl_WaitOutsideKokiri.s")

GLOBAL_ASM("asm/EnOwl_WaitZoraRiver.s")

GLOBAL_ASM("asm/FUN_0015d054.s")

GLOBAL_ASM("asm/FUN_00160f44.s")

}
