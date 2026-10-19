extern "C" {
#include "z_en_heishi2.hpp"
#include "../Bg_Spot15_Saku/z_bg_spot15_saku.hpp"
#include "../Bg_Gate_Shutter/z_bg_gate_shutter.hpp"

#define FLAGS 0x00000009

void EnHeishi2_Init(Actor* actor, GameState* state);
void EnHeishi2_Destroy(Actor* actor, GameState* state);
void EnHeishi2_Update(EnHeishi2* self, PlayState* play);
void EnHeishi2_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Heishi2.data.s")

ActorInit En_Heishi2_InitVars = {
    ACTOR_EN_HEISHI2,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_SD,
    sizeof(EnHeishi2),
    (ActorFunc)EnHeishi2_Init,
    (ActorFunc)EnHeishi2_Destroy,
    (ActorFunc)EnHeishi2_Update,
    (ActorFunc)EnHeishi2_Draw,
};

// Blocked (infra): inlines a guarded function-local static identity MTX34 (guard 0x0055A210 in the .data dump,
// object 0x005BE29C .bss) -> needs pinned COMDAT symbols in data/oot3d.ld + data chunk split (LOADER_RECIPE).
GLOBAL_ASM("asm/EnHeishi2_Init.s")

void FUN_0036bcc8(PlayState* play, Actor* actor, Vec3s* headRot, Vec3s* torsoRot, Vec3f focusPos, s32 arg5);

void EnHeishi2_Update(EnHeishi2* self, PlayState* play) {
    s32 i;

    Actor_SetFocus(&self->actor, self->unk_92C);
    if ((self->unk_94A == 2) || (self->unk_94A == 5)) {
        self->actor.focus.pos.y = 70.0f;
        Actor_SetFocus(&self->actor, 70.0f);
        FUN_0036bcc8(play, &self->actor, &self->unk_8AC, &self->unk_8B8, self->actor.focus.pos, 0x4300);
    }

    self->unk_948++;
    for (i = 0; i != 5; i++) {
        DECR(self->unk_93E[i]);
    }
    self->actionFunc(self, play);
    Actor_MoveForward(&self->actor);
    switch (self->unk_94A) {
        case 6:
            break;
        case 9:
            break;
        default:
            Actor_UpdateBgCheckInfo(play, &self->actor, 10.0f, 10.0f, 30.0f, 0x1D);
            Collider_UpdateCylinder(&self->actor, &self->collider);
            CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
            break;
    }
}

s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_0034e01c(MTX34* mtx, Vec3s* rot);
void FUN_001cbb1c(EnHeishi2* self, PlayState* play);
void FUN_0038a670(EnHeishi2* self, PlayState* play);
void FUN_0038a918(EnHeishi2* self, PlayState* play);
void FUN_00259ef4(EnHeishi2* self, PlayState* play);

void FUN_0013f448(EnHeishi2* self, PlayState* play) {
}

void FUN_00150888(EnHeishi2* self, PlayState* play) {
}

s32 FUN_002110e4(s32 unused, s32 which, MTX34* mtx, EnHeishi2* self) {
    if ((self->unk_94A != 1) && (self->unk_94A != 7)) {
        if (which == 8) {
            FUN_0034e01c(mtx, &self->unk_8B8);
        } else if (which == 0xf) {
            FUN_0034e01c(mtx, &self->unk_8AC);
        }
    }
    return 0;
}

void FUN_0024ba60(s32 unused, s32 which, EnHeishi2Block* src, EnHeishi2* self) {
    if (which == 0xf) {
        self->unk_A04 = *src;
    }
}

void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx);
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);

void EnHeishi2_Draw(Actor* thisx, GameState* state) {
    EnHeishi2* self = (EnHeishi2*)thisx;
    PlayState* play = (PlayState*)state;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    SkelAnime_DrawOpa(&self->skelAnime, &thisx->modelMtx, (OverrideLimbDrawOpa)FUN_002110e4,
                      (PostLimbDrawOpa)FUN_0024ba60, thisx, 0);
    if ((self->unk_94A == 5) && (gSaveContext.infTable[7] & 0x80)) {
        if (Object_GetIndex(&play->objectCtx, 0x15) >= 0) {
            self->unk_A00->SetUnkAC(1);
            self->unk_A00->FUN_003721e0((MTX34*)&self->unk_A04);
            self->unk_A00->FUN_00372170(0);
        }
    }
}

void FUN_00269aa0(Actor* thisx) {
    EnHeishi2* self = (EnHeishi2*)thisx;

    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&thisx->modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}

void FUN_0025a17c(EnHeishi2* self, PlayState* play) {
    f32 frames = Animation_GetLastFrame(&self->skelAnime, 0);

    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, frames, 0, -10.0f);
    self->actionFunc = FUN_001cbb1c;
}

void FUN_002742e8(EnHeishi2* self, PlayState* play) {
    f32 frames = Animation_GetLastFrame(&self->skelAnime, 0);

    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, frames, 0, -10.0f);
    self->actionFunc = FUN_0038a670;
}

void FUN_00298ef4(EnHeishi2* self, PlayState* play) {
    f32 frames = Animation_GetLastFrame(&self->skelAnime, 0);

    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, frames, 0, -10.0f);
    self->actionFunc = FUN_0038a918;
}

void FUN_001ca460(EnHeishi2* self, PlayState* play);
void FUN_002054f8(EnHeishi2* self, PlayState* play);
void FUN_00156988(EnHeishi2* self, PlayState* play);
void FUN_00288594(EnHeishi2* self, PlayState* play);
void FUN_00259e98(EnHeishi2* self, PlayState* play);
void FUN_00177c24(EnHeishi2* self, PlayState* play);
void FUN_0035021c(void* p);
s32 FUN_00370778(PlayState* play);
void Rupees_ChangeBy(s16 value);

void EnHeishi2_Destroy(Actor* thisx, GameState* state) {
    EnHeishi2* self = (EnHeishi2*)thisx;

    if ((self->collider.dim.radius != 0) || (self->collider.dim.height != 0)) {
        Collider_DestroyCylinder((PlayState*)state, &self->collider);
    }
    FUN_0035021c(self->unk_A00);
    FUN_003508b8(thisx, self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
}

void FUN_00259e98(EnHeishi2* self, PlayState* play) {
    self->unk_938 = Animation_GetLastFrame(&self->skelAnime, 2);
    Animation_Change_00375c08(&self->skelAnime, 2, 1.0f, 0.0f, self->unk_938, 2, -10.0f);
    self->actionFunc = FUN_001ca460;
}

void FUN_00288594(EnHeishi2* self, PlayState* play) {
    self->unk_938 = Animation_GetLastFrame(&self->skelAnime, 2);
    Animation_Change_00375c08(&self->skelAnime, 2, 1.0f, 0.0f, self->unk_938, 2, -10.0f);
    self->actionFunc = FUN_002054f8;
}

void FUN_002063c4(EnHeishi2* self, PlayState* play) {
    self->unk_938 = Animation_GetLastFrame(&self->skelAnime, 2);
    Animation_Change_00375c08(&self->skelAnime, 2, 1.0f, 0.0f, self->unk_938, 2, -10.0f);
    self->unk_954 = 0;
    self->actionFunc = FUN_00156988;
}

void FUN_002055b4(EnHeishi2* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if ((Message_GetState(&play->msgCtx) == self->unk_94C) && Message_ShouldAdvance(play)) {
        FUN_0036e980(play, NULL, 8);
        FUN_00370778(play);
        self->actionFunc = FUN_00288594;
    }
}

void FUN_0021c8c4(EnHeishi2* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if ((Message_GetState(&play->msgCtx) == self->unk_94C) && Message_ShouldAdvance(play)) {
        FUN_0036e980(play, NULL, 8);
        FUN_00370778(play);
        self->actionFunc = FUN_00259e98;
    }
}

void FUN_001cb90c(EnHeishi2* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
        self->actor.textId = 0x2021;
        Rupees_ChangeBy(15);
        Message_ContinueTextbox(play, self->actor.textId);
        self->actionFunc = FUN_00177c24;
    }
}

void FUN_001cd6f4(EnHeishi2* self, PlayState* play) {
    Math_SmoothStepToS(&self->actor.shape.rot.z, 200, 5, self->unk_930, 0);
    Math_ApproachF(&self->unk_930, 3000.0f, 1.0f, 500.0f);
    self->actor.world.rot.z = self->actor.shape.rot.z;
    if (self->actor.shape.rot.z > 0) {
        Actor_Kill(&self->actor);
    }
}

f32* EnHeishi2_CurFramePtr(SkelAnime* skelAnime);
void FUN_002743c0(EnHeishi2* self, PlayState* play);

void FUN_001ca460(EnHeishi2* self, PlayState* play) {
    f32 frameCount = *EnHeishi2_CurFramePtr(&self->skelAnime);
    Actor* actor = play->actorCtx.actorLists[ACTORCAT_ITEMACTION].head;

    FUN_003731e0(&self->skelAnime);
    if ((frameCount >= 12.0f) && (!self->unk_954)) {
        Audio_PlayActorSound2(&self->actor, 0x0100017a);
        self->unk_954 = 1;
    }
    if (self->unk_938 <= frameCount) {
        while (actor != NULL) {
            if (actor->id != ACTOR_BG_SPOT15_SAKU) {
                actor = actor->next;
            } else {
                self->gate = actor;
                ((BgSpot15Saku*)actor)->unk_168 = 1;
                break;
            }
        }
        self->actionFunc = FUN_002743c0;
    }
}

f32* EnHeishi2_CurFramePtr(SkelAnime* skelAnime);
void FUN_003725e0(PlayState* play);
void FUN_001e883c(EnHeishi2* self, PlayState* play);

void FUN_002aa134(EnHeishi2* self, PlayState* play);

void FUN_00156988(EnHeishi2* self, PlayState* play) {
    f32 frameCount = *EnHeishi2_CurFramePtr(&self->skelAnime);
    Actor* actor = play->actorCtx.actorLists[ACTORCAT_ITEMACTION].head;

    FUN_003731e0(&self->skelAnime);
    if ((frameCount >= 12.0f) && (!self->unk_954)) {
        Audio_PlayActorSound2(&self->actor, 0x0100017a);
        self->unk_954 = 1;
    }
    if (self->unk_938 <= frameCount) {
        while (actor != NULL) {
            if (actor->id != ACTOR_BG_GATE_SHUTTER) {
                actor = actor->next;
            } else {
                self->gate = actor;
                if (self->unk_956 != 2) {
                    ((BgGateShutter*)actor)->openingState = -1;
                } else {
                    ((BgGateShutter*)actor)->openingState = 2;
                }
                break;
            }
        }
        if (self->unk_956 == 0) {
            self->unk_956 = 1;
        }
        self->actionFunc = FUN_002aa134;
    }
}

void FUN_002744b8(EnHeishi2* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_CHOICE) && Message_ShouldAdvance(play)) {
        self->unk_94C = TEXT_STATE_EVENT;
        switch (FUN_00369f3c(play)) {
            case 0:
                if (gSaveContext.rupees >= 10) {
                    Rupees_ChangeBy(-10);
                    self->actor.textId = 0x7098;
                    self->actionFunc = FUN_0021c8c4;
                } else {
                    self->actor.textId = 0x7097;
                    self->actionFunc = FUN_0025a17c;
                }
                break;
            case 1:
                self->actor.textId = 0x7096;
                self->actionFunc = FUN_0025a17c;
                break;
            default:
                break;
        }
        Message_ContinueTextbox(play, self->actor.textId);
    }
}

void FUN_002743c0(EnHeishi2* self, PlayState* play) {
    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 0), 0, -10.0f);
    self->unk_93E[0] = 300;
    self->subCamId = Gameplay_CreateSubCamera(play);
    Gameplay_ChangeCameraStatus(play, 0, 1);
    Gameplay_ChangeCameraStatus(play, self->subCamId, 7);
    self->subCamAt.x = 947.0f;
    self->subCamAt.y = 1195.0f;
    self->subCamAt.z = 2682.0f;
    self->subCamEye.x = 1164.0f;
    self->subCamEye.y = 1145.0f;
    self->subCamEye.z = 3014.0f;
    Gameplay_CameraSetAtEye(play, self->subCamId, &self->subCamAt, &self->subCamEye);
    self->actionFunc = FUN_001e883c;
}

void FUN_00238abc(EnHeishi2* self, PlayState* play);

// = N64 func_80A53DF8 (unk_908 = subCamEyeInit, unk_8E4 = subCamAtInit). VFP numbering of the six pool constants:
// f32 locals declared in the order (az, ax, ex, ey, ez, ay) and assigned chained with the Init-field stores.
void FUN_002aa134(EnHeishi2* self, PlayState* play) {
    f32 az, ax, ex, ey, ez, ay;

    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 0), 0, -10.0f);
    self->unk_93E[0] = 300;
    self->subCamId = Gameplay_CreateSubCamera(play);
    Gameplay_ChangeCameraStatus(play, 0, 1);
    Gameplay_ChangeCameraStatus(play, self->subCamId, 7);
    self->unk_908.x = ax = -71.0f;
    self->unk_908.y = ay = 571.0f;
    self->unk_908.z = az = -1487.0f;
    self->unk_8E4.x = ex = 181.0f;
    self->unk_8E4.y = ey = 417.0f;
    self->unk_8E4.z = ez = -1079.0f;
    self->subCamAt.x = ax;
    self->subCamAt.y = ay;
    self->subCamAt.z = az;
    self->subCamEye.x = ex;
    self->subCamEye.y = ey;
    self->subCamEye.z = ez;
    Gameplay_CameraSetAtEye(play, self->subCamId, &self->subCamAt, &self->subCamEye);
    self->actionFunc = FUN_00238abc;
}

void FUN_001e883c(EnHeishi2* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    Gameplay_CameraSetAtEye(play, self->subCamId, &self->subCamAt, &self->subCamEye);
    BgSpot15Saku* gate = (BgSpot15Saku*)self->gate;
    if ((self->unk_93E[0] == 0) || (gate->unk_168 == 0)) {
        Gameplay_ClearCamera(play, self->subCamId);
        Gameplay_ChangeCameraStatus(play, 0, 7);
        FUN_003725e0(play);
        self->unk_958 = 1;
        FUN_0036e980(play, NULL, 7);
        self->actionFunc = FUN_00298ef4;
    }
}

void FUN_0036bbd0(Actor* actor, PlayState* play, s32 exchangeItem, f32 range);

void FUN_001e74ec(EnHeishi2* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s8 exchangeItem;

    FUN_003731e0(&self->skelAnime);
    if (Text_GetFaceReaction(play, ((u16)self->unk_94A == 5) ? 0x3c : 5) == 0) {
        self->actor.textId = 0x200E;
    } else {
        self->actor.textId = Text_GetFaceReaction(play, ((u16)self->unk_94A == 5) ? 0x3c : 5);
    }
    self->unk_94C = 6;
    if (Actor_ProcessTalkRequest(&self->actor, play) != 0) {
        exchangeItem = FUN_0036bc84(play);
        if (exchangeItem == 1) {
            FUN_00372244(&play->unk_5FCC, 0x1E, 0x01000487);
            player->actor.textId = 0x2010;
            self->unk_94C = 5;
            self->actionFunc = FUN_002055b4;
        } else if (exchangeItem != 0) {
            player->actor.textId = 0x200F;
        }
        return;
    }

    {
        s16 yawDiffTemp = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
        s16 yawDiff = ABS(yawDiffTemp);

        if ((self->actor.xzDistToPlayer > 120.0f) || (yawDiff >= 0x4300)) {
            return;
        }
    }
    FUN_0036bbd0(&self->actor, play, 1, 100.0f);
}

void FUN_00363cb8(Actor* actor, PlayState* play);
void FUN_0019bec4(EnHeishi2* self, PlayState* play);

void FUN_00259ef4(EnHeishi2* self, PlayState* play) {
    s16 yawDiff;

    FUN_003731e0(&self->skelAnime);
    if (Text_GetFaceReaction(play, ((u16)self->unk_94A == 5) ? 0x3c : 5) != 0) {
        if (self->unk_957 == 0) {
            if (self->unk_94A == 2) {
                self->actionFunc = (EnHeishi2ActionFunc)FUN_0038a918;
                return;
            }
            if (self->unk_94A == 5) {
                self->actionFunc = FUN_0038a670;
                return;
            }
        }
    } else if (self->unk_957 != 0) {
        if (self->unk_94A == 2) {
            self->actionFunc = (EnHeishi2ActionFunc)FUN_0038a918;
            return;
        }
        if (self->unk_94A == 5) {
            self->actionFunc = FUN_0038a670;
            return;
        }
    }

    if (Actor_ProcessTalkRequest(&self->actor, play) != 0) {
        if (self->unk_94A == 2) {
            if (self->unk_95A == 1) {
                self->actionFunc = FUN_002744b8;
                return;
            } else {
                self->actionFunc = (EnHeishi2ActionFunc)FUN_0038a918;
                return;
            }
        } else if (self->unk_94A == 5) {
            if (self->unk_94C == 6) {
                self->actionFunc = FUN_0038a670;
            }
            if (self->unk_94C == 5) {
                self->actionFunc = FUN_0025a17c;
            }
            if (self->unk_94C == 4) {
                self->unk_955 = 1;
                FUN_00372244(&play->unk_5FCC, 0x1E, 0x0100048C);
                self->actionFunc = FUN_0019bec4;
            }
            return;
        }
    }

    if ((self->unk_94A == 2) || (self->unk_94A == 5)) {
        s16 yawDiffTemp = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

        yawDiff = ABS(yawDiffTemp);
        if ((self->actor.xzDistToPlayer > 120.0f) || (yawDiff >= 0x4300)) {
            return;
        }
    }
    FUN_00363cb8(&self->actor, play);
}

s32 Player_GetMask(PlayState* play);

void FUN_0038a670(EnHeishi2* self, PlayState* play) {
    s32 var;
    u16 faceReaction;

    self->unk_957 = 0;
    var = 0;
    if (!(gSaveContext.infTable[7] & 0x40)) {
        self->actionFunc = FUN_001e74ec;
        return;
    }
    if (!(gSaveContext.infTable[7] & 0x80)) {
        if (Player_GetMask(play) == 1) {
            if (self->unk_955 == 0) {
                self->actor.textId = 0x200a;
            } else {
                self->actor.textId = 0x200b;
            }
            self->unk_94C = 4;
            self->unk_957 = 1;
        } else {
            self->actor.textId = 0x2016;
            self->unk_94C = 6;
        }
        var = 1;
    } else {
        self->actor.textId = 0x2020;
        self->unk_94C = 5;
        self->unk_95A = 0;
    }
    faceReaction = Text_GetFaceReaction(play, ((u16)self->unk_94A == 5) ? 0x3c : 5);
    if ((faceReaction != 0) && (var == 0)) {
        self->actor.textId = Text_GetFaceReaction(play, ((u16)self->unk_94A == 5) ? 0x3c : 5);
        self->unk_957 = 1;
        self->unk_94C = 6;
        self->unk_95A = 0;
    }
    self->actionFunc = FUN_00259ef4;
}

// N64 func_80A53278: actionFunc set in every branch (N64 shape); 7006/7002 end with an explicit return.
void FUN_0038a918(EnHeishi2* self, PlayState* play) {
    self->unk_957 = 0;
    self->unk_95A = 0;
    if (Text_GetFaceReaction(play, ((u16)self->unk_94A == 5) ? 0x3c : 5) != 0) {
        self->actor.textId = Text_GetFaceReaction(play, ((u16)self->unk_94A == 5) ? 0x3c : 5);
        self->unk_957 = 1;
        self->unk_94C = 6;
        self->actionFunc = FUN_00259ef4;
    } else if ((gSaveContext.eventChkInf[0] & 0x200) && (gSaveContext.eventChkInf[2] & 0x20) &&
               (gSaveContext.eventChkInf[3] & 0x80)) {
        self->unk_94C = 6;
        self->actor.textId = 0x7006;
        self->actionFunc = FUN_00259ef4;
        return;
    } else if (gSaveContext.nightFlag != 0) {
        self->unk_94C = 6;
        self->actor.textId = 0x7002;
        self->actionFunc = FUN_00259ef4;
        return;
    } else if (self->unk_958 != 0) {
        self->unk_94C = 6;
        self->actor.textId = 0x7099;
        self->actionFunc = FUN_00259ef4;
    } else if (gSaveContext.eventChkInf[1] & 4) {
        if (self->unk_95A == 0) {
            self->actor.textId = 0x7071;
            self->unk_95A = 1;
        } else {
            self->actor.textId = 0x7072;
        }
        self->unk_94C = 4;
        self->actionFunc = FUN_00259ef4;
    } else {
        self->unk_94C = 6;
        self->actor.textId = 0x7029;
        self->actionFunc = FUN_00259ef4;
    }
}

void FUN_00345fcc(PlayState* play);
void Item_Give(PlayState* play, s32 itemId); // = FUN_00376a78

void FUN_0019bec4(EnHeishi2* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_CHOICE) && Message_ShouldAdvance(play)) {
        switch (FUN_00369f3c(play)) {
            case 0:
                self->actor.textId = 0x2020;
                Message_ContinueTextbox(play, self->actor.textId);
                FUN_00345fcc(play);
                gSaveContext.infTable[7] |= 0x80;
                gSaveContext.itemGetInf[3] |= 0x100;
                Item_Give(play, 0x2c);
                if (self->unk_956 != 0) {
                    self->unk_956 = 2;
                    self->unk_95A = 1;
                    self->actionFunc = FUN_00177c24;
                } else {
                    self->unk_95A = 0;
                    self->actionFunc = FUN_001cb90c;
                }
                break;
            case 1:
                self->unk_95A = 1;
                self->actor.textId = 0x200c;
                Message_ContinueTextbox(play, self->actor.textId);
                self->unk_94C = 5;
                if (self->unk_956 == 0) {
                    self->actionFunc = FUN_00177c24;
                } else {
                    self->actionFunc = FUN_0025a17c;
                }
        }
    }
}

void FUN_00238bc4(EnHeishi2* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
        gSaveContext.infTable[7] |= 0x40;
        FUN_003725e0(play);
        FUN_0036e980(play, NULL, 7);
        self->actionFunc = FUN_002742e8;
    }
}

void FUN_00238abc(EnHeishi2* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    Gameplay_CameraSetAtEye(play, self->subCamId, &self->subCamAt, &self->subCamEye);
    BgGateShutter* gate = (BgGateShutter*)self->gate;
    if ((self->unk_93E[0] == 0) || (gate->openingState == 0)) {
        Gameplay_ClearCamera(play, self->subCamId);
        Gameplay_ChangeCameraStatus(play, 0, 7);
        if (self->unk_956 != 2) {
            if (self->unk_956 == 0) {
                self->actor.textId = 0x2015;
                Message_ContinueTextbox(play, self->actor.textId);
                self->actionFunc = FUN_00238bc4;
            } else {
                FUN_003725e0(play);
                FUN_0036e980(play, NULL, 7);
                self->actionFunc = FUN_002742e8;
            }
        } else {
            self->unk_95A = 0;
            self->actor.textId = 0x2021;
            Rupees_ChangeBy(15);
            Message_ContinueTextbox(play, self->actor.textId);
            self->actionFunc = FUN_00177c24;
        }
    }
}

void FUN_00177c24(EnHeishi2* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
        if (self->unk_95A == 0) {
            self->unk_956 = 0;
            FUN_003725e0(play);
            self->actionFunc = FUN_002742e8;
        } else {
            FUN_00370778(play);
            self->actionFunc = FUN_002063c4;
        }
    }
}

void FUN_001cbb1c(EnHeishi2* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if ((Message_GetState(&play->msgCtx) == self->unk_94C) && Message_ShouldAdvance(play)) {
        FUN_003725e0(play);
        if (self->unk_94A == 2) {
            self->actionFunc = FUN_00298ef4;
        } else if (self->unk_94A == 5) {
            self->actionFunc = FUN_002742e8;
        }
    }
}

void FUN_0019f4d4(Actor* thisx, PlayState* play) {
    EnHeishi2* self = (EnHeishi2*)thisx;

    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
        f32 posX;
        f32 posY;
        f32 posZ;
        f32 rotY;
        Actor* bomb;

        FUN_0036e980(play, NULL, 7);
        FUN_003725e0(play);
        posX = Rand_CenteredFloat(20.0f) + self->unk_8C0.x;
        posY = Rand_CenteredFloat(20.0f) + (self->unk_8C0.y - 40.0f);
        posZ = Rand_CenteredFloat(20.0f) + (self->unk_8C0.z - 20.0f);
        rotY = Rand_CenteredFloat(7000.0f) + self->actor.yawTowardsPlayer;
        bomb = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOM, posX, posY, posZ, 0, (s16)rotY, 0, 0, 1);
        if (bomb != NULL) {
            bomb->speedXZ = Rand_CenteredFloat(5.0f) + 10.0f;
            bomb->velocity.y = Rand_CenteredFloat(5.0f) + 10.0f;
        }
        self->actionFunc = FUN_001cd6f4;
    }
}

void FUN_00180674(EnHeishi2* self, PlayState* play) {
    Math_SmoothStepToS(&self->actor.shape.rot.z, -6100, 5, (s16)self->unk_930, 0);
    Math_ApproachF(&self->unk_930, 3000.0f, 1.0f, 500.0f);
    self->actor.world.rot.z = self->actor.shape.rot.z;
    if (self->actor.shape.rot.z < -6000) {
        FUN_00367c7c(play, 0x708f, NULL);
        self->actor.flags |= 0x10000;
        self->unk_930 = 0.0f;
        self->actionFunc = (EnHeishi2ActionFunc)FUN_0019f4d4;
    }
}

void FUN_002054f8(EnHeishi2* self, PlayState* play) {
    f32 frameCount = *EnHeishi2_CurFramePtr(&self->skelAnime);
    Actor* actor = play->actorCtx.actorLists[ACTORCAT_ITEMACTION].head;

    FUN_003731e0(&self->skelAnime);
    if ((frameCount >= 12.0f) && (!self->unk_954)) {
        Audio_PlayActorSound2(&self->actor, 0x0100017a);
        self->unk_954 = 1;
    }
    if (self->unk_938 <= frameCount) {
        while (actor != NULL) {
            if (actor->id != ACTOR_BG_GATE_SHUTTER) {
                actor = actor->next;
            } else {
                self->gate = actor;
                ((BgGateShutter*)actor)->openingState = 1;
                break;
            }
        }
        self->actionFunc = FUN_002aa134;
    }
}
}
