extern "C" {
#include "z_en_heishi4.hpp"

#define FLAGS 0x00000009
#define PARAMS_GET_U(params, shift, count) ((((u16)(params)) >> (shift)) & ((1 << (count)) - 1))

void EnHeishi4_Init(Actor* thisx, PlayState* play);
void EnHeishi4_Destroy(Actor* thisx, PlayState* play);
void EnHeishi4_Update(Actor* thisx, GameState* state);
void EnHeishi4_Draw(Actor* thisx, PlayState* play);

void FUN_00180b24(EnHeishi4* self, PlayState* play); // N64: func_80A56328
void FUN_002057dc(EnHeishi4* self, PlayState* play); // N64: func_80A56544
void FUN_0021c1d0(EnHeishi4* self, PlayState* play); // N64: func_80A5673C
void FUN_00299518(EnHeishi4* self, PlayState* play); // N64: func_80A563BC
void FUN_0038a814(EnHeishi4* self, PlayState* play); // N64: func_80A56614
void FUN_0025a074(EnHeishi4* self, PlayState* play); // N64: func_80A56B40 (asm)
void FUN_001748f4(EnHeishi4* self, PlayState* play); // N64: func_80A56874 (asm)
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_0034e01c(MTX34* mtx, Vec3s* rot);           // rotates mtx by rot (limb helper)
s32 FUN_00211524(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void FUN_0034c664(Actor* actor, EnHeishi4InteractInfo* info, s32 arg2, s32 arg3); // Npc_TrackPoint
void FUN_00353c9c(Actor* actor, PlayState* play, SkelAnime* skelAnime, s32 cmbIndex, s32 csabIndex,
                  void* jointTable, void* morphTable, s32 limbCount);

s32 Collider_DestroyCylinder(PlayState* play, ColliderCylinder* collider);
void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx);
void __ARM_common_memcpy2_6(void* dst, void* src); // N64: Gfx_SetupDL_25Opa (empty on 3DS)

// 0x00588758: 3DS global read by the dying-guard Init path (name TBD, see syms.s).
extern u8 gSaveContext_E00[];
extern u32 EnHeishi4_sMaskReactionSets[]; // 0x00526364

// Data in the asm blob: sCylinderInit lives at 0x0052636C (see syms.s).
extern ColliderCylinderInit EnHeishi4_sCylinderInit;

GLOBAL_ASM("data/z_En_Heishi4.data.s")
GLOBAL_ASM("../src/game/actors/En_Heishi4/syms.s")

ActorInit En_Heishi4_InitVars = {
    ACTOR_EN_HEISHI4,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_SD,
    sizeof(EnHeishi4),
    (ActorFunc)EnHeishi4_Init,
    (ActorFunc)EnHeishi4_Destroy,
    (ActorFunc)EnHeishi4_Update,
    (ActorFunc)EnHeishi4_Draw,
};

void EnHeishi4_Init(Actor* thisx, PlayState* play) {
    EnHeishi4* self = (EnHeishi4*)thisx;

    Actor_SetScale(thisx, 0.01f);
    self->type = PARAMS_GET_U(thisx->params, 0, 8);
    thisx->colChkInfo.mass = MASS_IMMOVABLE;
    self->pos = thisx->world.pos;
    thisx->targetMode = 6;
    if (self->type == HEISHI4_AT_MARKET_DYING) {
        self->height = 30.0f;
        ActorShape_Init(&thisx->shape, 0.0f, NULL, 30.0f);
        FUN_00372f38(thisx, play, &self->skelAnimModel, 0, 0);
        FUN_00353c9c(thisx, play, &self->skelAnime, 0, 3, self->jointTable, self->morphTable, 16);
    } else {
        self->height = 60.0f;
        ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
        FUN_00372f38(thisx, play, &self->skelAnimModel, 0, 0);
        FUN_00353c9c(thisx, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 16);
    }
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, thisx, &EnHeishi4_sCylinderInit);
    self->collider.dim.yShift = 0;
    self->collider.dim.radius = 15;
    self->collider.dim.height = 70;
    {
        EnHeishi4ActionFunc fn;
        if (self->type == 0 || self->type == 4) {
            fn = FUN_00180b24;
        } else {
            switch (self->type) {
            case 7: {
                self->collider.dim.radius = 28;
                self->collider.dim.height = 30;
                u8* flags = gSaveContext_E00;
                if ((*(u16*)&flags[0xF4] & 0x20) || !(*(u16*)&flags[0xFC] & 1)) {
                    Actor_Kill(thisx);
                    return;
                }
                fn = FUN_0021c1d0;
                break;
            }
            case 8:
                fn = FUN_002057dc;
                self->actionFunc = fn;
            default:
                goto done;
            }
        }
        self->actionFunc = fn;
    }
done:
    self->unk_27C = (u8)(thisx->params >> 8);
}

// N64: func_80A56328
void FUN_00180b24(EnHeishi4* self, PlayState* play) {
    f32 frames = Animation_GetLastFrame(&self->skelAnime, 0);

    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, frames, 0, -10.0f);
    self->actionFunc = FUN_00299518;
}

// N64: func_80A56544
void FUN_002057dc(EnHeishi4* self, PlayState* play) {
    f32 frames = Animation_GetLastFrame(&self->skelAnime, 0);

    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, frames, 0, -10.0f);
    if (gSaveContext.linkAge == 0) {
        Actor_Kill(&self->actor);
    } else {
        self->actionFunc = FUN_0038a814;
    }
}

// N64: func_80A5673C
void FUN_0021c1d0(EnHeishi4* self, PlayState* play) {
    f32 frames;

    if (gSaveContext.eventChkInf[4] & 0x20) {
        Actor_Kill(&self->actor);
        return;
    }
    self->unk_284 = 0;
    if (gSaveContext.eventChkInf[8] & 1) {
        if (!(gSaveContext.infTable[6] & 0x1000)) {
            frames = Animation_GetLastFrame(&self->skelAnime, 3);
            Animation_Change_00375c08(&self->skelAnime, 3, 1.0f, 0.0f, frames, 0, -10.0f);
            self->actor.textId = 0x7007;
            self->unk_282 = 5;
            self->unk_284 = 1;
        } else {
            self->actor.textId = 0x7008;
            self->unk_282 = 6;
        }
        self->actionFunc = FUN_001748f4;
    } else {
        Actor_Kill(&self->actor);
    }
}

// N64: func_80A563BC
void FUN_00299518(EnHeishi4* self, PlayState* play) {
    s16 reactionOffset;

    self->unk_2B4 = 0;
    reactionOffset = self->type - 4;
    if (reactionOffset < 0) {
        reactionOffset = 0;
    }
    if (reactionOffset > 2) {
        reactionOffset = 1;
    }
    if (Text_GetFaceReaction(play, EnHeishi4_sMaskReactionSets[reactionOffset]) != 0) {
        self->actor.textId = Text_GetFaceReaction(play, EnHeishi4_sMaskReactionSets[reactionOffset]);
        self->unk_2B4 = 1;
        self->actionFunc = FUN_0025a074;
    } else {
        if (gSaveContext.eventChkInf[8] & 1) {
            self->actor.textId = 0x5065;
            self->actionFunc = FUN_0025a074;
            return;
        }
        if (gSaveContext.eventChkInf[4] & 0x20) {
            self->actor.textId = 0x5068;
            self->actionFunc = FUN_0025a074;
            return;
        }
        if (self->type == HEISHI4_AT_IMPAS_HOUSE) {
            if (self->unk_284 == 0) {
                self->actor.textId = 0x5079;
            } else {
                self->actor.textId = 0x507A;
            }
        } else if (gSaveContext.nightFlag == 0) {
            if (self->unk_284 == 0) {
                self->actor.textId = 0x5063;
            } else {
                self->actor.textId = 0x5064;
            }
        } else if (self->unk_284 == 0) {
            self->actor.textId = 0x5066;
        } else {
            self->actor.textId = 0x5067;
        }
        self->actionFunc = FUN_0025a074;
    }
}

// N64: func_80A56614
void FUN_0038a814(EnHeishi4* self, PlayState* play) {
    s16 reactionOffset;

    self->unk_2B4 = 0;
    reactionOffset = self->type - 4;
    if (reactionOffset < 0) {
        reactionOffset = 0;
    }
    if (reactionOffset > 2) {
        reactionOffset = 1;
    }
    if (Text_GetFaceReaction(play, EnHeishi4_sMaskReactionSets[reactionOffset]) != 0) {
        self->actor.textId = Text_GetFaceReaction(play, EnHeishi4_sMaskReactionSets[reactionOffset]);
        self->unk_2B4 = 1;
        self->actionFunc = FUN_0025a074;
        return;
    }
    if (play->sceneNum == 0x4D) {
        if (gSaveContext.nightFlag == 0) {
            self->actor.textId = 0x7004;
        } else {
            self->actor.textId = 0x709A;
        }
    } else if (play->sceneNum != 0x21) {
        if (gSaveContext.nightFlag == 0) {
            if (gSaveContext.eventChkInf[8] & 1) {
                self->actor.textId = 0x7206;
            } else {
                self->actor.textId = 0x7002;
            }
        } else {
            self->actor.textId = 0x7003;
        }
    } else {
        self->actor.textId = 0x7003;
    }
    self->actionFunc = FUN_0025a074;
}


void EnHeishi4_Destroy(Actor* thisx, PlayState* play) {
    EnHeishi4* self = (EnHeishi4*)thisx;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(thisx, self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
}

s32 FUN_00211524(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnHeishi4* self = (EnHeishi4*)thisx;

    if (limbIndex == 8) {
        FUN_0034e01c(mtx, &self->torsoRot);
    } else if (limbIndex == 15) {
        FUN_0034e01c(mtx, &self->headRot);
    }
    return 0;
}

void EnHeishi4_Update(Actor* thisx, GameState* state) {
    EnHeishi4* self = (EnHeishi4*)thisx;
    PlayState* play = (PlayState*)state;
    Player* player = GET_PLAYER(play);

    thisx->world.pos.x = self->pos.x;
    thisx->world.pos.y = self->pos.y;
    thisx->world.pos.z = self->pos.z;
    Actor_SetFocus(thisx, self->height);
    if (self->type != HEISHI4_AT_MARKET_DYING) {
        self->interactInfo.trackPos = player->actor.world.pos;
        if (!LINK_IS_ADULT) {
            self->interactInfo.trackPos.y = player->actor.world.pos.y - 10.0f;
        }
        FUN_0034c664(thisx, &self->interactInfo, 2, 4);
        self->headRot = self->interactInfo.headRot;
        __ARM_common_memcpy2_6(&self->torsoRot, &self->interactInfo.torsoRot);
    }
    self->unk_27E++;
    self->actionFunc(self, play);
    Actor_MoveForward(thisx);
    Actor_UpdateBgCheckInfo(play, thisx, 10.0f, 10.0f, 30.0f, 0x1D);
    Collider_UpdateCylinder(thisx, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
}

void EnHeishi4_Draw(Actor* thisx, PlayState* play) {
    EnHeishi4* self = (EnHeishi4*)thisx;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, FUN_00211524, NULL, thisx, 0);
}

}
