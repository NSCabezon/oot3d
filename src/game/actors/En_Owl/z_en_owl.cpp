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
extern u8 gSaveContext_E00[];
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
void FUN_0012c070(EnOwl* self, PlayState* play);
void FUN_0012c1b8(EnOwl* self, PlayState* play);
void FUN_0012c2d4(EnOwl* self, PlayState* play);
void FUN_0012c668(EnOwl* self, PlayState* play);
void FUN_0012c784(EnOwl* self, PlayState* play);
void FUN_0012c574(EnOwl* self, PlayState* play);
void FUN_0012c444(EnOwl* self, PlayState* play);
void FUN_0012c4e8(EnOwl* self, PlayState* play);
void FUN_0012c1b8(EnOwl* self, PlayState* play);
void FUN_0012bf54(EnOwl* self, PlayState* play);
void EnOwl_WaitOutsideKokiri(EnOwl* self, PlayState* play);
void FUN_0015d054(EnOwl* self);
void FUN_0015cc5c(EnOwl* self);
void FUN_001eb30c(EnOwl* self);
void FUN_00160f44(EnOwl* self);
void FUN_003c16ec(EnOwl* self, PlayState* play);
void EnOwl_ConfirmKokiriMessage(EnOwl* self, PlayState* play);
void FUN_003cc72c(EnOwl* self, PlayState* play);
s32 FUN_00369f3c(PlayState* play);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 EnOwl_CheckInitTalk(EnOwl* self, PlayState* play, s32 textId, f32 targetDist, s32 flags);
void EnOwl_ChangeMode(EnOwl* self, void* actionFunc, void* updateFunc, SkelAnime* skelAnime,
                      FaceAnimations* faceAnimations, s32 animationIndex, f32 morphFrames);
void FUN_00160e78(EnOwl* self);
void FUN_0035c528(u32 seqId);

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

#if 0
void FUN_0012bf54(EnOwl* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 4 && Message_ShouldAdvance(play)) {
        s32 choice = FUN_00369f3c(play);
        if (choice == 0) {
            Message_ContinueTextbox(play, 0x2069);
            self->actionFunc = (void*)FUN_0012bee0;
        } else if (choice == 1) {
            Message_ContinueTextbox(play, 0x206B);
            self->actionFunc = (void*)FUN_003c16ec;
        }
        self->actionFlags &= ~2;
        FUN_0035ff80(self);
    }
}
#endif

void FUN_0012bffc(EnOwl* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 5 && Message_ShouldAdvance(play)) {
        Message_ContinueTextbox(play, 0x206A);
        self->actionFunc = (void*)FUN_0012c070;
        self->actionFlags |= 2;
        FUN_0035ff80(self);
    }
}

void FUN_0012c144(EnOwl* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 5 && Message_ShouldAdvance(play)) {
        Message_ContinueTextbox(play, 0x206A);
        self->actionFunc = (void*)FUN_0012c1b8;
        self->actionFlags |= 2;
        FUN_0035ff80(self);
    }
}

void FUN_0012c260(EnOwl* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 5 && Message_ShouldAdvance(play)) {
        Message_ContinueTextbox(play, 0x206A);
        self->actionFunc = (void*)FUN_0012c2d4;
        self->actionFlags |= 2;
        FUN_0035ff80(self);
    }
}

void FUN_0012c5f4(EnOwl* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 5 && Message_ShouldAdvance(play)) {
        Message_ContinueTextbox(play, 0x10C2);
        self->actionFunc = (void*)FUN_0012c668;
        self->actionFlags |= 2;
        FUN_0035ff80(self);
    }
}

void FUN_0012c710(EnOwl* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 5 && Message_ShouldAdvance(play)) {
        Message_ContinueTextbox(play, 0x10C6);
        self->actionFunc = (void*)FUN_0012c784;
        self->actionFlags |= 2;
        FUN_0035ff80(self);
    }
}

#if 0
void FUN_0012c1b8(EnOwl* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 4 && Message_ShouldAdvance(play)) {
        s32 choice = FUN_00369f3c(play);
        if (choice == 0) {
            Message_ContinueTextbox(play, 0x206F);
            self->actionFunc = (void*)FUN_0012c144;
        } else if (choice == 1) {
            Message_ContinueTextbox(play, 0x2070);
            self->actionFunc = (void*)FUN_003c16ec;
        }
        self->actionFlags &= ~2;
        FUN_0035ff80(self);
    }
}
#endif

void FUN_0012c574(EnOwl* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 4 && Message_ShouldAdvance(play)) {
        s32 choice = FUN_00369f3c(play);
        if (choice == 0) {
            Message_ContinueTextbox(play, 0x607A);
        } else if (choice == 1) {
            Message_ContinueTextbox(play, 0x607C);
            self->actionFunc = (void*)FUN_003c16ec;
        }
    }
}

void FUN_0012c4e8(EnOwl* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        Audio_SetBGM(1, 0);
        EnOwl_ChangeMode(self, (void*)FUN_0015d054, (void*)FUN_001eb30c,
                         (SkelAnime*)((u8*)self + 0x1FC), (FaceAnimations*)((u8*)self + 0xAA0), 1, 0.0f);
        self->eyeTexIndex = 0;
        self->unk_1356 = Rand_S16Offset_003702c8(60, 60);
        self->actionFunc = (void*)FUN_00160f44;
    }
}

void FUN_0012c444(EnOwl* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        Audio_SetBGM(1, 0);
        EnOwl_ChangeMode(self, (void*)FUN_0015d054, (void*)FUN_001eb30c,
                         (SkelAnime*)((u8*)self + 0x1FC), (FaceAnimations*)((u8*)self + 0xAA0), 1, 0.0f);
        self->eyeTexIndex = 0;
        self->unk_1356 = Rand_S16Offset_003702c8(60, 60);
        self->actionFunc = (void*)FUN_00160f44;
        Flags_SetSwitch(play, 0x23);
    }
}

#if 0
void FUN_0015d054(EnOwl* self) {
    if (self->actionFlags & 1) {
        self->unk_135E = 3;
        EnOwl_ChangeMode(self, (void*)FUN_0015cc5c, (void*)FUN_001eb30c,
                         (SkelAnime*)((u8*)self + 0x1FC), (FaceAnimations*)((u8*)self + 0xAA0), 2, 0.0f);
        self->unk_1358 = self->actor.world.pos.y;
        self->actor.velocity.y = 2.0f;
        s32 yawOffset = 0x4000;
        if (*(s32*)((u8*)self + 0x1378) == 10) {
            yawOffset = 0x6400;
        }
        s32 yaw = self->actor.world.rot.y;
        if (self->actionFlags & 0x40) {
            yaw += yawOffset;
        } else {
            yaw -= yawOffset;
        }
        *(s16*)((u8*)self + 0x1360) = yaw;
    }
    self->actionFlags |= 8;
}
#endif

void FUN_001857ec(EnOwl* self, PlayState* play) {
    if ((*(u16*)((u8*)self + 0x134E) & 0x3F) == 0) {
        s32 switchFlag = self->actor.params & 0x3F;
        if (switchFlag < 0x20) {
            Flags_SetSwitch(play, switchFlag);
            Flags_GetSwitch(play, switchFlag);
        }
        EnOwl_ChangeMode(self, (void*)FUN_0015d054, (void*)FUN_001eb30c,
                         (SkelAnime*)((u8*)self + 0x1FC), (FaceAnimations*)((u8*)self + 0xAA0), 1, 0.0f);
        self->eyeTexIndex = 0;
        self->unk_1356 = Rand_S16Offset_003702c8(60, 60);
    }
}

void EnOwl_WaitOutsideKokiri(EnOwl* self, PlayState* play) {
    Player* player = *(Player**)((u8*)play + 0x20AC);
    self->actor.world.rot.y = Math_Atan2S(player->actor.world.pos.z - self->actor.world.pos.z,
                                          player->actor.world.pos.x - self->actor.world.pos.x);
    self->actor.shape.rot.y = self->actor.world.rot.y;
    if (EnOwl_CheckInitTalk(self, play, 0x2064, 360.0f, 0)) {
        FUN_0035c528(0x010005C7);
        self->actionFunc = (void*)EnOwl_ConfirmKokiriMessage;
        ((u16*)gSaveContext_E00)[0x7C] |= 0x8000; // gSaveContext.eventChkInf[6]
    }
}

void FUN_001eb30c(EnOwl* self) {
    if (FUN_003731e0(self->drawSkelAnime)) {
        Animation_Change_00375c08(self->drawSkelAnime, self->drawSkelAnime->animationType, 1.0f,
                                  0.0f,
                                  (f32)Animation_GetLastFrame(self->drawSkelAnime,
                                                              self->drawSkelAnime->animationType),
                                  2, 0.0f);
        self->actionFlags |= 1;
    } else {
        self->actionFlags &= ~1;
    }
}

#if 0
void EnOwl_ChangeMode(EnOwl* self, void* actionFunc, void* updateFunc, SkelAnime* skelAnime,
                      FaceAnimations* faceAnimations, s32 animationIndex, f32 morphFrames) {
    self->faceAnimations = faceAnimations;
    self->drawSkelAnime = skelAnime;
    Animation_Change_00375c08(skelAnime, animationIndex, 1.0f, 0.0f,
                              (f32)Animation_GetLastFrame(skelAnime, animationIndex), 2, morphFrames);
    self->actionFunc = actionFunc;
    self->updateFunc = updateFunc;
}
#endif
GLOBAL_ASM("asm/EnOwl_ChangeMode.s")

void EnOwl_ConfirmKokiriMessage(EnOwl* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 4 && Message_ShouldAdvance(play)) {
        s32 choice = FUN_00369f3c(play);

        if (choice == 0) {
            Message_ContinueTextbox(play, 0x2065);
        } else if (choice == 1) {
            Message_ContinueTextbox(play, 0x2067);
            self->actionFunc = (void*)FUN_003cc72c;
        }
    }
}

void EnOwl_WaitDesertColossus(EnOwl* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    self->actor.world.rot.y = Math_Atan2S(player->actor.world.pos.z - self->actor.world.pos.z,
                                          player->actor.world.pos.x - self->actor.world.pos.x);
    self->actor.shape.rot.y = self->actor.world.rot.y;
    if (EnOwl_CheckInitTalk(self, play, 0x6079, 360.0f, 2)) {
        FUN_0035c528(0x010005C7);
        self->actionFunc = (void*)FUN_0012c574;
    }
}

void EnOwl_WaitGerudo(EnOwl* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    self->actor.world.rot.y = Math_Atan2S(player->actor.world.pos.z - self->actor.world.pos.z,
                                          player->actor.world.pos.x - self->actor.world.pos.x);
    self->actor.shape.rot.y = self->actor.world.rot.y;
    if (EnOwl_CheckInitTalk(self, play, 0x206F, 360.0f, 0)) {
        FUN_0035c528(0x010005C7);
        self->actionFunc = (void*)FUN_0012c144;
    }
}

void EnOwl_WaitHyruleCastle(EnOwl* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    self->actor.world.rot.y = Math_Atan2S(player->actor.world.pos.z - self->actor.world.pos.z,
                                          player->actor.world.pos.x - self->actor.world.pos.x);
    self->actor.shape.rot.y = self->actor.world.rot.y;
    if (EnOwl_CheckInitTalk(self, play, 0x2068, 540.0f, 0)) {
        FUN_0035c528(0x010005C7);
        self->actionFunc = (void*)FUN_0012be6c;
    }
}

void EnOwl_WaitKakariko(EnOwl* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    self->actor.world.rot.y = Math_Atan2S(player->actor.world.pos.z - self->actor.world.pos.z,
                                          player->actor.world.pos.x - self->actor.world.pos.x);
    self->actor.shape.rot.y = self->actor.world.rot.y;
    if (EnOwl_CheckInitTalk(self, play, 0x206C, 480.0f, 0)) {
        FUN_0035c528(0x010005C7);
        self->actionFunc = (void*)FUN_0012bffc;
    }
}

void EnOwl_WaitLWPostSaria(EnOwl* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    self->actor.world.rot.y = Math_Atan2S(player->actor.world.pos.z - self->actor.world.pos.z,
                                          player->actor.world.pos.x - self->actor.world.pos.x);
    self->actor.shape.rot.y = self->actor.world.rot.y;
    if (EnOwl_CheckInitTalk(self, play, 0x10C4, 360.0f, 0)) {
        FUN_0035c528(0x010005C7);
        self->actionFunc = (void*)FUN_0012c710;
    }
}

void EnOwl_WaitLWPreSaria(EnOwl* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    self->actor.world.rot.y = Math_Atan2S(player->actor.world.pos.z - self->actor.world.pos.z,
                                          player->actor.world.pos.x - self->actor.world.pos.x);
    self->actor.shape.rot.y = self->actor.world.rot.y;
    if (EnOwl_CheckInitTalk(self, play, 0x10C0, 190.0f, 0)) {
        FUN_0035c528(0x010005C7);
        self->actionFunc = (void*)FUN_0012c5f4;
    }
}

void EnOwl_WaitLakeHylia(EnOwl* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    self->actor.world.rot.y = Math_Atan2S(player->actor.world.pos.z - self->actor.world.pos.z,
                                          player->actor.world.pos.x - self->actor.world.pos.x);
    self->actor.shape.rot.y = self->actor.world.rot.y;
    if (EnOwl_CheckInitTalk(self, play, 0x2071, 360.0f, 0)) {
        FUN_0035c528(0x010005C7);
        self->actionFunc = (void*)FUN_0012c260;
    }
}

void FUN_00160f44(EnOwl* self) {
    if (self->actionFlags & 1) {
        self->unk_135E = 3;
        EnOwl_ChangeMode(self, (void*)FUN_00160e78, (void*)FUN_001eb30c, (SkelAnime*)((u8*)self + 0x1FC),
                         (FaceAnimations*)((u8*)self + 0xAA0), 2, 0.0f);
        self->unk_1358 = self->actor.world.pos.y;
        self->actor.velocity.y = 0.2f;
    }
    self->actionFlags |= 8;
}

GLOBAL_ASM("data/z_En_Owl.data.s")

GLOBAL_ASM("asm/FUN_0015d054.s")

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

void EnOwl_Destroy(Actor* actor, PlayState* play) {
    u8* self = (u8*)actor;

    Collider_DestroyCylinder(play, (ColliderCylinder*)(self + 0x1A4));
    SkelAnime_Free2((SkelAnime*)(self + 0x1FC));
    SkelAnime_Free2((SkelAnime*)(self + 0xC6C));
    ((FaceAnimations*)(self + 0xAA0))->Destroy();
    ((FaceAnimations*)(self + 0x1168))->Destroy();
}


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

GLOBAL_ASM("asm/EnOwl_CheckInitTalk.s")

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

GLOBAL_ASM("asm/EnOwl_WaitHyliaShortcut.s")


GLOBAL_ASM("asm/EnOwl_WaitZoraRiver.s")


}
