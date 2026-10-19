extern "C" {
#include "z_en_owl.hpp"

#define FLAGS 0x00000019

void EnOwl_Init(EnOwl* self, PlayState* play);
void EnOwl_Destroy(Actor* actor, PlayState* state);
void EnOwl_Update(Actor* actor, GameState* state);
void EnOwl_Draw(EnOwl* self, PlayState* state);
void EnOwl_WaitDeathMountainShortcut(EnOwl* self, PlayState* play);
void EnOwl_WaitDefault(EnOwl* self, PlayState* play);
void EnOwl_WaitHyliaShortcut(EnOwl* self, PlayState* play);
void EnOwl_WaitZoraRiver(EnOwl* self, PlayState* play);
void FUN_00188650(EnOwl* self);
void FUN_0035ff80(EnOwl* self);

extern ColliderCylinderInit EnOwl_sCylinderInit;
extern InitChainEntry EnOwl_sInitChain[];
extern f32 EnOwl_sFlapFrames[2];
GLOBAL_ASM("../src/game/actors/En_Owl/data_syms.s")

extern u8 EnRiverSound_sState[];
extern u8 gSaveContext_E00[];
extern u8 gSaveContext_F00[];
s32 FUN_00369a48(Actor* actor, PlayState* play);
void FUN_00359220(Actor* actor, PlayState* play, s32 arg, f32 a, f32 b);
void FUN_003528dc(Actor* actor, PlayState* play);
void FUN_003525e0(Actor* actor, PlayState* play);

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
void FUN_0012c37c(EnOwl* self, PlayState* play);
void FUN_0012c574(EnOwl* self, PlayState* play);
void FUN_0012c444(EnOwl* self, PlayState* play);
void FUN_0012c4e8(EnOwl* self, PlayState* play);
void FUN_0012c1b8(EnOwl* self, PlayState* play);
void FUN_0012bf54(EnOwl* self, PlayState* play);
void EnOwl_WaitOutsideKokiri(EnOwl* self, PlayState* play);
void FUN_0015d054(EnOwl* self);
void FUN_001857ec(EnOwl* self, PlayState* play);
void FUN_0015cc5c(EnOwl* self);
void FUN_00188484(EnOwl* self, PlayState* play);
void FUN_0021a9e8(EnOwl* self, PlayState* play);
void FUN_003cb000(EnOwl* self, PlayState* play);
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
 s32 OnePointCutscene_Init(PlayState* play, s32 textId, s32 priority, Actor* actor, s32 camId);
extern u16 gTimeSpeed;
s32 Gameplay_InCsMode(PlayState* play);



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

void FUN_0012bf54(EnOwl* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 4 && Message_ShouldAdvance(play)) {
        switch (FUN_00369f3c(play)) {
        case 0:
            Message_ContinueTextbox(play, 0x2069);
            self->actionFunc = (void*)FUN_0012bee0;
            break;
        case 1:
            Message_ContinueTextbox(play, 0x206B);
            self->actionFunc = (void*)FUN_003c16ec;
            break;
        }
        self->actionFlags &= ~2;
        FUN_0035ff80(self);
    }
}

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

void FUN_0012c1b8(EnOwl* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 4 && Message_ShouldAdvance(play)) {
        switch (FUN_00369f3c(play)) {
        case 0:
            Message_ContinueTextbox(play, 0x206F);
            self->actionFunc = (void*)FUN_0012c144;
            break;
        case 1:
            Message_ContinueTextbox(play, 0x2070);
            self->actionFunc = (void*)FUN_003c16ec;
            break;
        }
        self->actionFlags &= ~2;
        FUN_0035ff80(self);
    }
}

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

void FUN_0012c2d4(EnOwl* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 4 && Message_ShouldAdvance(play)) {
        switch (FUN_00369f3c(play)) {
        case 0:
            Message_ContinueTextbox(play, 0x2071);
            self->actionFunc = (void*)FUN_0012c260;
            break;
        case 1:
            Message_ContinueTextbox(play, 0x2072);
            self->actionFunc = (void*)FUN_003c16ec;
            break;
        }
        self->actionFlags &= ~2;
        FUN_0035ff80(self);
    }
}

void FUN_0012c668(EnOwl* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 4 && Message_ShouldAdvance(play)) {
        switch (FUN_00369f3c(play)) {
        case 0:
            Message_ContinueTextbox(play, 0x10C1);
            self->actionFunc = (void*)FUN_0012c5f4;
            break;
        case 1:
            Message_ContinueTextbox(play, 0x10C3);
            self->actionFunc = (void*)FUN_003c16ec;
            break;
        }
        self->actionFlags &= ~2;
        FUN_0035ff80(self);
    }
}

void FUN_0012c784(EnOwl* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 4 && Message_ShouldAdvance(play)) {
        switch (FUN_00369f3c(play)) {
        case 0:
            Message_ContinueTextbox(play, 0x10C5);
            self->actionFunc = (void*)FUN_0012c710;
            break;
        case 1:
            Message_ContinueTextbox(play, 0x10C7);
            self->actionFunc = (void*)FUN_003c16ec;
            break;
        }
        self->actionFlags &= ~2;
        FUN_0035ff80(self);
    }
}

void FUN_0012c070(EnOwl* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 4 && Message_ShouldAdvance(play)) {
        switch (FUN_00369f3c(play)) {
        case 0:
            if (((u16*)gSaveContext_E00)[0x7A] & 1) {
                Message_ContinueTextbox(play, 0x206D);
            } else {
                Message_ContinueTextbox(play, 0x206C);
            }
            self->actionFunc = (void*)FUN_0012bffc;
            break;
        case 1:
            Message_ContinueTextbox(play, 0x206E);
            self->actionFunc = (void*)FUN_003c16ec;
            break;
        }
        self->actionFlags &= ~2;
        FUN_0035ff80(self);
    }
}

void FUN_001c5148(EnOwl* self) {
    FUN_003731e0(self->drawSkelAnime);
    if (self->unk_135E != 0) {
        self->unk_135E--;
        s16 angle = self->unk_135E * 0x333;
        *((s16*)((u8*)self + 0xC0)) = (s32)(Math_SinS(angle) * 1000.0f);
    } else {
        self->updateFunc = (void*)FUN_0021a9e8;
        self->unk_135E = 6;
        Animation_Change_00375c08(self->drawSkelAnime, 3, 1.0f, 0.0f,
                                  (f32)Animation_GetLastFrame(self->drawSkelAnime, 3), 2, 5.0f);
    }
}

void FUN_00188650(EnOwl* self) {
    Math_SmoothStepToS(&self->actor.world.rot.y, *(s16*)((u8*)self + 0x1360), 2, 0x384, 0x258);
    self->actor.shape.rot.y = self->actor.world.rot.y;
    if (self->actionFlags & 1) {
        EnOwl_ChangeMode(self, (void*)FUN_003cb000, (void*)FUN_0021a9e8,
        (SkelAnime*)((u8*)self + 0x1FC), (FaceAnimations*)((u8*)self + 0xAA0), 3, 0.0f);
        self->unk_135E = 6;
        self->actor.velocity.y = 2.0f;
        self->actor.gravity = 0.0f;
        self->actor.speedXZ = 4.0f;
    }
    self->actionFlags |= 8;
}

void FUN_00160e78(EnOwl* self) {
    if (self->unk_238 < 20.0f) {
        self->actor.speedXZ = 1.5f;
    } else {
        self->actor.speedXZ = 0.0f;
        Math_SmoothStepToS(&self->actor.world.rot.y, *(s16*)((u8*)self + 0x1360), 2, 0x384, 0x258);
        self->actor.shape.rot.y = self->actor.world.rot.y;
    }
    if (self->unk_238 >= 37.0f) {
        self->unk_238 = 21.0f;
        self->actionFunc = (void*)FUN_00188484;
        self->unk_135E = 5;
        self->actor.velocity.y = 0.0f;
        self->actor.gravity = 0.0f;
        self->actor.speedXZ = 0.0f;
    }
    self->actionFlags |= 8;
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

void FUN_0012c37c(EnOwl* self, PlayState* play) {
    FUN_0036e980(play, &self->actor, 8);
    if (FUN_00369a48(&self->actor, play)) {
        Audio_SetBGM(1, 0);
        s32 switchFlag = self->actor.params & 0x3F;
        if (switchFlag < 0x20) {
            Flags_SetSwitch(play, switchFlag);
            Flags_GetSwitch(play, switchFlag);
        }
        EnOwl_ChangeMode(self, (void*)FUN_0015d054, (void*)FUN_001eb30c,
                         (SkelAnime*)((u8*)self + 0x1FC), (FaceAnimations*)((u8*)self + 0xAA0), 1, 0.0f);
        self->eyeTexIndex = 0;
        self->unk_1356 = Rand_S16Offset_003702c8(60, 60);
        self->actor.flags &= ~0x10000;
    }
}

void FUN_003cc72c(EnOwl* self, PlayState* play) {
    FUN_0036e980(play, &self->actor, 8);
    if (FUN_00369a48(&self->actor, play)) {
        Audio_SetBGM(1, 0);
        s32 switchFlag = self->actor.params & 0x3F;
        if (switchFlag < 0x20) {
            Flags_SetSwitch(play, switchFlag);
            Flags_GetSwitch(play, switchFlag);
        }
        EnOwl_ChangeMode(self, (void*)FUN_0015d054, (void*)FUN_001eb30c,
                         (SkelAnime*)((u8*)self + 0x1FC), (FaceAnimations*)((u8*)self + 0xAA0), 1, 0.0f);
        self->eyeTexIndex = 0;
        self->unk_1356 = Rand_S16Offset_003702c8(60, 60);
        self->actor.flags &= ~0x10000;
    }
}

void FUN_003c16ec(EnOwl* self, PlayState* play) {
    FUN_0036e980(play, &self->actor, 8);
    if (FUN_00369a48(&self->actor, play)) {
        Audio_SetBGM(1, 0);
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
        } else {
            self->actionFlags &= ~2;
            FUN_0035ff80(self);
            self->actionFunc = (void*)FUN_001857ec;
        }
        self->actor.flags &= ~0x10000;
    }
}

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
        if (self->actionFlags & 0x40) {
            *(s16*)((u8*)self + 0x1360) = yawOffset + (u16)self->actor.world.rot.y;
        } else {
            *(s16*)((u8*)self + 0x1360) = (u16)self->actor.world.rot.y - yawOffset;
        }
    }
    self->actionFlags |= 8;
}

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

void EnOwl_ChangeMode(EnOwl* self, void* actionFunc, void* updateFunc, SkelAnime* skelAnime,
                      FaceAnimations* faceAnimations, s32 animationIndex, f32 morphFrames) {
    self->faceAnimations = faceAnimations;
    self->drawSkelAnime = skelAnime;
    Animation_Change_00375c08(self->drawSkelAnime, animationIndex, 1.0f, 0.0f,
                              (f32)Animation_GetLastFrame(skelAnime, animationIndex), 2, morphFrames);
    self->updateFunc = updateFunc;
    self->actionFunc = actionFunc;
}


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

#if 0 // Sol: full916-byte candidate,39 words differ; no split/pin.
void EnOwl_Init(EnOwl* self, PlayState* play) {
    s32 owlType;
    s32 switchFlag;
    Actor_ProcessInitChain(&self->actor, EnOwl_sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
    FUN_00353c9c(&self->actor, play, (SkelAnime*)((u8*)self + 0x1FC), 1, 3,
                  (u8*)self + 0x280, (u8*)self + 0x690, 20);
    SkelAnime_SetFaceAnimations((FaceAnimations*)((u8*)self + 0xAA0),
                                  (SkelAnime*)((u8*)self + 0x1FC), 0, -1, -1);
    FUN_00353c9c(&self->actor, play, (SkelAnime*)((u8*)self + 0xC6C), 0, 0,
                  (u8*)self + 0xCF0, (u8*)self + 0xF2C, 11);
    SkelAnime_SetFaceAnimations((FaceAnimations*)((u8*)self + 0x1168),
                                  (SkelAnime*)((u8*)self + 0xC6C), 1, -1, -1);
    Collider_InitCylinder(play, (ColliderCylinder*)((u8*)self + 0x1A4));
    Collider_SetCylinder(play, (ColliderCylinder*)((u8*)self + 0x1A4), &self->actor,
                          &EnOwl_sCylinderInit);
    self->actor.colChkInfo.mass = 0xFF;
    self->actor.minVelocityY = -10.0f;
    self->actor.targetArrowOffset = 500.0f;
    EnOwl_ChangeMode(self, (void*)EnOwl_WaitDefault, (void*)FUN_001eb30c,
                     (SkelAnime*)((u8*)self + 0xC6C),
                     (FaceAnimations*)((u8*)self + 0x1168), 0, 0.0f);
    self->actionFlags = self->unk_1366 = self->unk_136A = 0;
    self->unk_1364[1] = 4;
    self->unk_1364[0] = self->unk_1368[0] = 0;
    self->unk_1368[1] = 4;
    owlType = (self->actor.params & 0xFC0) >> 6;
    switchFlag = self->actor.params & 0x3F;
    if (self->actor.params == 0xFFF) {
        owlType = 1;
        switchFlag = 0x20;
    }
    if (owlType != 0 && switchFlag < 0x20 && Flags_GetSwitch(play, switchFlag)) {
        Actor_Kill(&self->actor);
        return;
    }
    self->unk_1378 = owlType;
    self->unk_134E = 0;
    self->targetYaw = self->actor.world.rot.y;
    switch (owlType) {
    case 0:
        self->actionFunc = (void*)EnOwl_WaitDefault;
        self->actor.uncullZoneForward = 4000.0f;
        self->cueId = 0;
        break;
    case 1:
        self->actionFunc = (void*)EnOwl_WaitOutsideKokiri;
        break;
    case 2:
        self->actionFlags |= 2;
        self->unk_134E = 0x20;
        self->actionFunc = (void*)EnOwl_WaitHyruleCastle;
        break;
    case 3:
        if (((u16*)gSaveContext_E00)[0x7A] & 1) {
            Actor_Kill(&self->actor);
            return;
        }
        self->actionFunc = (void*)EnOwl_WaitKakariko;
        break;
    case 4:
        if (((u16*)gSaveContext_E00)[0x7A] & 8) {
            Actor_Kill(&self->actor);
            return;
        }
        self->actionFunc = (void*)EnOwl_WaitGerudo;
        break;
    case 5:
        self->actionFunc = (void*)EnOwl_WaitLakeHylia;
        break;
    case 6:
        if ((((u16*)gSaveContext_E00)[0x79] & 0x200) || !(((u16*)gSaveContext_E00)[0x7A] & 1)) {
            Actor_Kill(&self->actor);
            return;
        }
        self->actionFunc = (void*)EnOwl_WaitZoraRiver;
        break;
    case 7:
        self->actionFunc = (void*)EnOwl_WaitHyliaShortcut;
        Flags_UnsetSwitch(play, 0x23);
        return;
    case 8:
    case 9:
        self->actionFunc = (void*)EnOwl_WaitDeathMountainShortcut;
        break;
    case 10:
        self->actionFunc = (void*)EnOwl_WaitDesertColossus;
        break;
    case 11:
        if (!CHECK_QUEST_ITEM(12)) {
            Actor_Kill(&self->actor);
            return;
        }
        self->actionFunc = (void*)EnOwl_WaitLWPreSaria;
        break;
    case 12:
        if (!CHECK_QUEST_ITEM(14)) {
            Actor_Kill(&self->actor);
            return;
        }
        self->actionFunc = (void*)EnOwl_WaitLWPostSaria;
        break;
    default:
        self->actionFlags |= 2;
        self->unk_134E = 0x20;
        self->actionFunc = (void*)EnOwl_WaitOutsideKokiri;
        break;
    }
}
#endif
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
void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);
void Matrix_RotateZ(MTX34* mtx, s32 mode, f32 angle);
f32 sinf(f32 angle);
f32 cosf(f32 angle);

#if 0 // Sol: full104-byte candidate,9 words differ; r4/r5 base allocation.
void EnOwl_Draw(EnOwl* self, PlayState* play) {
    {
        FaceAnimations* face = self->faceAnimations;
        s16 eyeIndex = self->eyeTexIndex;
        face->FUN_0035e3a4(0, eyeIndex);
    }
    {
        FaceAnimations* face = self->faceAnimations;
        face->FUN_0035e330();
    }
    SkelAnime_DrawOpa(self->drawSkelAnime, &self->actor.modelMtx, EnOwl_OverrideLimbDraw,
                     EnOwl_PostLimbUpdate, &self->actor, 0);
    self->drawFlag = 1;
}
#endif
GLOBAL_ASM("asm/EnOwl_Draw.s")


#if 0 // Sol: complete 2164-byte candidate,454 words differ; preserved best full-size draft.
static inline s32 EnOwl_FrameCount(s32 count) {
    return (s32)((f32)count * 3.0f / SREG(30) + 0.5f);
}

static inline s32 EnOwl_CanPlayFlap(EnOwl* self, PlayState* play) {
    s32 type = (self->actor.params & 0xFC0) >> 6;
    if (play->sceneNum != 0x5C) {
        return 1;
    } else if (type == 10) {
        return 1;
    } else if (play->csCtx.frames >= EnOwl_FrameCount(300) && play->csCtx.frames <= EnOwl_FrameCount(430)) {
        return 1;
    } else if (play->csCtx.frames >= EnOwl_FrameCount(1080) && play->csCtx.frames <= EnOwl_FrameCount(1170)) {
        return 1;
    } else {
        return 0;
    }
}

void EnOwl_Update(Actor* actor, GameState* state) {
    EnOwl* self = (EnOwl*)actor;
    PlayState* play = (PlayState*)state;
    s16 phi;
    Collider_UpdateCylinder(&self->actor, (ColliderCylinder*)((u8*)self + 0x1A4));
    CollisionCheck_SetOC(play, &play->colChkCtx, (Collider*)((u8*)self + 0x1A4));
    if (self->drawFlag) {
        if (self->hasMoved || !self->bgCheckInitialized) {
            Actor_UpdateBgCheckInfo(play, &self->actor, 10.0f, 10.0f, 10.0f, 5);
            self->bgCheckInitialized = 1;
        }
        ((void (*)(EnOwl*))self->updateFunc)(self);
    }
    self->actionFlags &= ~8;
    ((void (*)(EnOwl*, PlayState*))self->actionFunc)(self, play);
    if (self->actor.update == NULL) {
        return;
    }
    if (!(self->actionFlags & 0x80) && EnOwl_CanPlayFlap(self, play)) {
        SkelAnime* skel = (SkelAnime*)((u8*)self + 0x1FC);
        if ((skel->animationType == 2 &&
             (Animation_OnFrameImpl(skel, 2.0f, 2.0f / 3.0f) ||
              Animation_OnFrameImpl(skel, 12.0f, 2.0f / 3.0f) ||
              Animation_OnFrameImpl(skel, 23.0f, 2.0f / 3.0f) ||
              Animation_OnFrameImpl(skel, 40.0f, 2.0f / 3.0f) ||
              Animation_OnFrameImpl(skel, 58.0f, 2.0f / 3.0f))) ||
            (skel->animationType == 3 &&
             Animation_OnFrameImpl(skel, EnOwl_sFlapFrames[1], 2.0f / 3.0f))) {
            Audio_PlayActorSound2(&self->actor, 0x01000396);
        }
    }
    if (self->actor.draw != NULL) {
        Actor_MoveForward(&self->actor);
    }
    f32 dx = self->actor.world.pos.x - self->actor.prevPos.x;
    f32 dy = self->actor.world.pos.y - self->actor.prevPos.y;
    f32 dz = self->actor.world.pos.z - self->actor.prevPos.z;
    self->hasMoved = dx * dx + dy * dy + dz * dz > 1.0f;
    if (self->actionFlags & 2) {
        self->eyeTexIndex = 2;
    } else {
        if (self->unk_1356 != 0) {
            self->unk_1356--;
        }
        if (self->unk_1356 == 0) {
            self->unk_1356 = Rand_S16Offset_003702c8(60, 60);
        }
        self->eyeTexIndex = self->unk_1356;
        if (self->eyeTexIndex >= 3) {
            self->eyeTexIndex = 0;
        }
    }
    if (!(self->actionFlags & 8)) {
        phi = 0;
        if (self->actionFlags & 0x10) {
            switch (self->unk_1364[0]) {
            case 0:
                self->unk_1364[0] = 1;
                self->unk_1364[1] = 6;
                break;
            case 1:
                self->unk_1364[1]--;
                if (self->unk_1364[1] != 0) {
                    phi = Math_CosS(self->unk_1364[1] * 8192) * 4096.0f;
                } else {
                    if (self->actionFlags & 2) {
                        self->unk_134E = 0;
                    } else {
                        self->unk_134E = 0x20;
                    }
                    if (self->actionFlags & 0x20) {
                        self->unk_134E -= 4;
                    } else {
                        self->unk_134E += 4;
                    }
                    self->unk_1364[0]++;
                }
                if (self->actionFlags & 0x20) {
                    phi = -phi;
                }
                break;
            case 2:
                if ((self->unk_134E & 0x3F) == ((self->actionFlags & 2) ? 0x20 : 0)) {
                    self->actionFlags &= ~0x10;
                    self->unk_1366 = EnOwl_FrameCount((s32)Rand_ZeroFloat(20.0f) + 60);
                    self->unk_1364[0] = 0;
                    if (Rand_CenteredFloat(1.0f) < 0.0f) {
                        self->actionFlags |= 0x20;
                    } else {
                        self->actionFlags &= ~0x20;
                    }
                } else if (self->actionFlags & 0x20) {
                    self->unk_134E += 4;
                } else {
                    self->unk_134E -= 4;
                }
                break;
            default:
                break;
            }
        } else {
            if (self->unk_1366 > 0) {
                self->unk_1366--;
            } else {
                if (self->unk_1364[0] == 0) {
                    if (Rand_ZeroOne() < 0.3f) {
                        self->unk_1364[0] = 4;
                        self->unk_1364[1] = 12;
                    } else {
                        self->unk_1364[0] = 1;
                        self->unk_1364[1] = 4;
                    }
                }
                self->unk_1364[1]--;
                switch (self->unk_1364[0]) {
                case 1:
                    phi = Math_SinS((4 - self->unk_1364[1]) * 4096) * 5000.0f;
                    if (self->unk_1364[1] == 0) {
                        self->unk_1364[1] = (s32)(Rand_ZeroFloat(15.0f) + 5.0f);
                        self->unk_1364[0] = 2;
                    }
                    break;
                case 2:
                    phi = 5000;
                    if (self->unk_1364[1] == 0) {
                        self->unk_1364[0] = 3;
                        self->unk_1364[1] = 4;
                    }
                    break;
                case 3:
                    phi = Math_SinS(self->unk_1364[1] * 4096) * 5000.0f;
                    if (self->unk_1364[1] == 0) {
                        self->unk_1366 = EnOwl_FrameCount((s32)Rand_ZeroFloat(20.0f) + 60);
                        self->unk_1364[0] = 0;
                        if (Rand_CenteredFloat(1.0f) < 0.0f) {
                            self->actionFlags |= 0x20;
                        } else {
                            self->actionFlags &= ~0x20;
                        }
                    }
                    break;
                case 4:
                    phi = Math_SinS(self->unk_1364[1] * 8192) * 5000.0f;
                    if (self->unk_1364[1] == 0) {
                        self->unk_1366 = EnOwl_FrameCount((s32)Rand_ZeroFloat(20.0f) + 60);
                        self->unk_1364[0] = 0;
                        if (Rand_CenteredFloat(1.0f) < 0.0f) {
                            self->actionFlags |= 0x20;
                        } else {
                            self->actionFlags &= ~0x20;
                        }
                    }
                    break;
                default:
                    break;
                }
                if (self->actionFlags & 0x20) {
                    phi = -phi;
                }
            }
            if (self->unk_136A > 0) {
                self->unk_136A--;
            } else {
                self->unk_1368[1]--;
                switch (self->unk_1368[0]) {
                case 0:
                    self->unk_1352 = (4 - self->unk_1368[1]) * 1500;
                    if (self->unk_1368[1] == 0) {
                        self->unk_1368[0] = 1;
                        self->unk_1368[1] = (s32)(Rand_ZeroFloat(15.0f) + 5.0f);
                    }
                    break;
                case 1:
                    self->unk_1352 = 6000;
                    if (self->unk_1368[1] == 0) {
                        self->unk_1368[0] = 2;
                        self->unk_1368[1] = 4;
                    }
                    break;
                case 2:
                    self->unk_1352 = self->unk_1368[1] * 1500;
                    if (self->unk_1368[1] == 0) {
                        self->unk_1368[0] = 0;
                        self->unk_1368[1] = 4;
                        self->unk_136A = EnOwl_FrameCount((s16)((s32)Rand_ZeroFloat(40.0f) + 160));
                    }
                    break;
                default:
                    break;
                }
            }
        }
        self->unk_1350 = (u16)((self->unk_134E << 2) << 8) + phi;
        self->unk_134C = ABS(self->unk_1350) >> 3;
    } else {
        self->unk_1352 = 0;
        if (self->actionFlags & 2) {
            self->unk_1350 = -0x8000;
        } else {
            self->unk_1350 = 0;
        }
        self->unk_134C = ABS(self->unk_1350) >> 3;
    }
    self->drawFlag = 0;
}
#endif
GLOBAL_ASM("asm/EnOwl_Update.s")





s32 EnOwl_CheckInitTalk(EnOwl* self, PlayState* play, s32 textId, f32 targetDist, s32 flags) {
    s32 priority;
    f32 distCheck;

    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        if (self->actor.params == 0x0FFF) {
            priority = -100;
            self->actionFlags |= 0x40;
        } else if (Rand_ZeroOne() < 0.5f) {
            priority = (flags & 1) ? -97 : -99;
            self->actionFlags |= 0x40;
        } else {
            priority = (flags & 1) ? -96 : -98;
            self->actionFlags &= ~0x40;
        }
        s32 cutsceneTextId = 0x21FC;
        if (self->unk_1378 == 10) {
            cutsceneTextId |= cutsceneTextId >> 8;
        }
        *(s16*)((u8*)self + 0x1362) =
            OnePointCutscene_Init(play, (s16)cutsceneTextId, (s16)priority, &self->actor, 0);
        return 1;
    }

    *(u16*)((u8*)self + 0x116) = textId;
    distCheck = (flags & 2) ? 200.0f : 1000.0f;
    if (self->actor.xzDistToPlayer < targetDist) {
        self->actor.flags |= 0x10000;
        FUN_00359220(&self->actor, play, 0, targetDist, distCheck);
    }
    return 0;
}


static inline void EnOwl_ApplyXRotation(MTX34* mtx, f32 angle) {
    if (angle != 0.0f) {
        f32 sinAngle = sinf(angle);
        f32 cosAngle = cosf(angle);
        f32 col1;
        f32 col2;

        col1 = mtx->data[0][1];
        col2 = mtx->data[0][2];
        mtx->data[0][1] = col1 * cosAngle + col2 * sinAngle;
        mtx->data[0][2] = col2 * cosAngle - col1 * sinAngle;
        col1 = mtx->data[1][1];
        col2 = mtx->data[1][2];
        mtx->data[1][1] = col1 * cosAngle + col2 * sinAngle;
        mtx->data[1][2] = col2 * cosAngle - col1 * sinAngle;
        col1 = mtx->data[2][1];
        col2 = mtx->data[2][2];
        mtx->data[2][1] = col1 * cosAngle + col2 * sinAngle;
        mtx->data[2][2] = col2 * cosAngle - col1 * sinAngle;
    }
}

s32 EnOwl_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    u8* self = (u8*)thisx;
    f32 angle;

    switch (limbIndex) {
    case 2:
        Matrix_RotateZ(mtx, 1, (s16)(*(s16*)(self + 0x134C) - *(s16*)(self + 0x1352)) *
                                  0.0000958738f);
        angle = *(s16*)(self + 0x1350) * 0.0000958738f;
        EnOwl_ApplyXRotation(mtx, angle);
        break;
    case 1:
        Matrix_RotateZ(mtx, 1, *(s16*)(self + 0x1352) * 0.0000958738f);
        break;
    case 3:
        if (!(*(u16*)(self + 0x135C) & 8)) {
            Matrix_RotateY(mtx, 1, (s16)-(s32)(*(s16*)(self + 0x134C) * 1.5f) * 0.0000958738f);
        }
        break;
    case 4:
        if (!(*(u16*)(self + 0x135C) & 8)) {
            Matrix_RotateY(mtx, 1, (s16)(*(s16*)(self + 0x134C) * 1.5f) * 0.0000958738f);
        }
        break;
    default:
        break;
    }
    return 0;
}


void EnOwl_PostLimbUpdate(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    Vec3f vec;
    EnOwl* self = (EnOwl*)thisx;
    vec.z = 0.0f;
    if (self->actionFlags & 2) {
        vec.x = 700.0f;
        vec.y = 400.0f;
    } else {
        vec.x = 1400.0f;
        vec.y = 0.0f;
    }
    if (limbIndex == 2) {
        Vector3f* focus = (Vector3f*)&self->actor.focus.pos;
        VEC3TransformAsm(focus, mtx, (Vector3f*)&vec);
    }
}


static inline s32 EnOwl_CheckTalk(EnOwl* self, PlayState* play, u16 textId) {
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        return 1;
    } else {
        self->actor.textId = textId;
        if (self->actor.xzDistToPlayer < 120.0f) {
            FUN_00359220(&self->actor, play, 0, 350.0f, 1000.0f);
        }
        return 0;
    }
}

void EnOwl_WaitDeathMountainShortcut(EnOwl* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    self->actor.world.rot.y = Math_Atan2S(player->actor.world.pos.z - self->actor.world.pos.z,
                                         player->actor.world.pos.x - self->actor.world.pos.x);
    self->actor.shape.rot.y = self->actor.world.rot.y;
    if (gSaveContext.magicAcquired == 0) {
        if (EnOwl_CheckTalk(self, play, 0x3062)) {
            FUN_0035c528(0x010005C7);
            self->actionFunc = (void*)FUN_001eb37c;
        }
    } else {
        if (EnOwl_CheckTalk(self, play, 0x3063)) {
            FUN_0035c528(0x010005C7);
            self->actionFunc = (void*)FUN_0012c4e8;
        }
    }
}


#if 0 // Sol: source-correct212/224-byte candidate,43 words differ; not a near-match.
static void EnOwl_SetCuePosition(EnOwl* self, PlayState* play, s32 idx) {
    Vec3f pos;
    pos.x = play->csCtx.npcActions[idx]->startPos.x;
    pos.y = play->csCtx.npcActions[idx]->startPos.y;
    pos.z = play->csCtx.npcActions[idx]->startPos.z;
    self->actor.world.pos = pos;
    self->actor.world.rot.y = self->actor.shape.rot.y = play->csCtx.npcActions[idx]->rot.y;
    self->actor.shape.rot.z = play->csCtx.npcActions[idx]->rot.z;
}

void EnOwl_WaitDefault(EnOwl* self, PlayState* play) {
    if (play->GetCutsceneState() != 0 && play->csCtx.npcActions[7] != NULL) {
        if (*(u8*)((u8*)self + 0x136C) != play->csCtx.npcActions[7]->action) {
            self->actionFlags |= 4;
            EnOwl_SetCuePosition(self, play, 7);
            FUN_003528dc(&self->actor, play);
        } else {
            self->actor.world.rot.z = play->csCtx.npcActions[7]->rot.y;
        }
    }
    if (self->actionFlags & 0x80) {
        FUN_003525e0(&self->actor, play);
    }
}
#endif
GLOBAL_ASM("asm/EnOwl_WaitDefault.s")



void EnOwl_WaitHyliaShortcut(EnOwl* self, PlayState* play) {
    Player* player;
    u16 textId;
    textId = (((u16*)gSaveContext_F00)[0x21] & 0x20) ? 0x4004 : 0x4003;
    player = GET_PLAYER(play);
    self->actor.world.rot.y = Math_Atan2S(player->actor.world.pos.z - self->actor.world.pos.z,
                                          player->actor.world.pos.x - self->actor.world.pos.x);
    self->actor.shape.rot.y = self->actor.world.rot.y;
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        ((u16*)gSaveContext_F00)[0x21] |= 0x20;
        FUN_0035c528(0x010005C7);
        self->actionFunc = (void*)FUN_0012c444;
    } else {
        self->actor.textId = textId;
        if (self->actor.xzDistToPlayer < 120.0f) {
            FUN_00359220(&self->actor, play, 0, 350.0f, 1000.0f);
        }
    }
}


void FUN_0021a9e8(EnOwl* self, PlayState* play) {
    if (!FUN_003731e0(self->drawSkelAnime)) {
        return;
    }
    if (self->unk_135E != 0) {
        self->unk_135E--;
        Animation_Change_00375c08(self->drawSkelAnime, self->drawSkelAnime->animationType, 1.0f, 0.0f,
                                  (f32)Animation_GetLastFrame(self->drawSkelAnime,
                                                              self->drawSkelAnime->animationType),
                                  2, 0.0f);
    } else {
        self->unk_135E = 0xA0;
        self->updateFunc = (void*)FUN_001c5148;
        Animation_Change_00375c08(self->drawSkelAnime, 4, 1.0f, 0.0f,
                                  (f32)Animation_GetLastFrame(self->drawSkelAnime, 4), 0, 5.0f);
    }
}

void FUN_003cb000(EnOwl* self, PlayState* play) {
    self->actor.flags |= 0x20;
    if (self->actor.xzDistToPlayer > 6000.0f && !(self->actionFlags & 0x80)) {
        Actor_Kill(&self->actor);
    }
    Math_SmoothStepToS(&self->actor.world.rot.y, *(s16*)((u8*)self + 0x1360), 2, 0x80, 0x40);
    self->actor.shape.rot.y = self->actor.world.rot.y;
    if (self->actor.speedXZ < 16.0f) {
        self->actor.speedXZ += 0.5f;
    }
    if (self->actor.world.pos.y > self->unk_1358 + 1000.0f) {
        if (self->actor.velocity.y > 0.0f) {
            self->actor.velocity.y -= 0.4f;
        }
    } else if (self->actor.velocity.y < 4.0f) {
        self->actor.velocity.y += 0.2f;
    }
    self->actionFlags |= 8;
}

void FUN_0015cc5c(EnOwl* self) {
    if (self->unk_238 > 10.0f) {
        Math_SmoothStepToS(&self->actor.world.rot.y, *(s16*)((u8*)self + 0x1360), 2, 0x400, 0x40);
        self->actor.shape.rot.y = self->actor.world.rot.y;
    }
    if (self->unk_238 > 45.0f) {
        self->actor.velocity.y = 2.0f;
        self->actor.gravity = 0.0f;
        self->actor.speedXZ = 8.0f;
    } else if (self->unk_238 > 17.0f) {
        self->actor.velocity.y = 6.0f;
        self->actor.gravity = 0.0f;
        self->actor.speedXZ = 4.0f;
    }
    if (self->actionFlags & 1) {
        EnOwl_ChangeMode(self, (void*)FUN_003cb000, (void*)FUN_0021a9e8,
                         (SkelAnime*)((u8*)self + 0x1FC), (FaceAnimations*)((u8*)self + 0xAA0), 3, 0.0f);
        self->unk_135E = 6;
        if (self->actionFlags & 0x40) {
            *(s16*)((u8*)self + 0x1360) += 0x2000;
        } else {
            *(s16*)((u8*)self + 0x1360) -= 0x2000;
        }
    }
    self->actionFlags |= 8;
}


void EnOwl_WaitZoraRiver(EnOwl* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    self->actor.world.rot.y = Math_Atan2S(player->actor.world.pos.z - self->actor.world.pos.z,
                                          player->actor.world.pos.x - self->actor.world.pos.x);
    self->actor.shape.rot.y = self->actor.world.rot.y;
    s32 textId;
    if (CHECK_QUEST_ITEM(14)) {
        if (CHECK_QUEST_ITEM(12)) {
            textId = 0x4031;
        } else {
            textId = 0x4017;
        }
    } else {
        textId = 0x4002;
    }
    if (EnOwl_CheckInitTalk(self, play, textId, 360.0f, 0)) {
        FUN_0035c528(0x010005C7);
        self->actionFunc = (void*)FUN_0012c37c;
    }
}


}
