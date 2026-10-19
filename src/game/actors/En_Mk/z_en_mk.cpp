extern "C" {
#include "z_en_mk.hpp"

#define FLAGS 0x00000019

void EnMk_Init(Actor* actor, GameState* state);
void EnMk_Destroy(Actor* actor, GameState* state);
void EnMk_Update(Actor* actor, GameState* state);
void FUN_0036bcc8(PlayState* play, Actor* actor, Vec3s* headRot, Vec3s* torsoRot, Vec3f focusPos, s32 yawLimit);
void EnMk_Draw(Actor* actor, GameState* state);
s32 EnMk_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnMk_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);

void FUN_0034e01c(MTX34* mtx, Vec3s* rot);

ActorInit En_Mk_InitVars = {
    ACTOR_EN_MK,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_MK,
    sizeof(EnMk),
    (ActorFunc)EnMk_Init,
    (ActorFunc)EnMk_Destroy,
    (ActorFunc)EnMk_Update,
    (ActorFunc)EnMk_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    { 0x0A, 0x00, 0x11, 0x39, 0x10, 0x01 },
    { 0, { 0x00000000, 0, 0 }, { 0xFFCFFFFF, 0, 0 }, 0, 1, 1 },
    { 30.0f, 40.0f, 0.0f, { 0.0f, 0.0f, 0.0f } },
};

void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);
void EnMk_Wait(EnMk* self, PlayState* play);

void EnMk_Init(Actor* actor, GameState* state) {
    EnMk* self = (EnMk*)actor;
    PlayState* play = (PlayState*)state;

    self->actor.minVelocityY = -4.0f;
    self->actor.gravity = -1.0f;
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
    FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
    FUN_00353c9c(actor, play, &self->skelAnime, 0, 3, self->jointTable, self->morphTable, 14);
    Animation_PlayLoop(&self->skelAnime, 3);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, actor, &sCylinderInit);
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(actor, 0.01f);
    self->actionFunc = EnMk_Wait;
    self->flags = 0;
    self->swimFlag = 0;
    self->actor.targetMode = 6;

    if (gSaveContext.itemGetInf[1] & 1) {
        self->flags = 4;
    }
}


void EnMk_Destroy(Actor* actor, GameState* state) {
    EnMk* self = (EnMk*)actor;
    PlayState* play = (PlayState*)state;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(actor, self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
}

void EnMk_Update(Actor* actor, GameState* state) {
    EnMk* self = (EnMk*)actor;
    PlayState* play = (PlayState*)state;
    Player* player;
    s16 swimFlag;

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);

    if (!(self->flags & 2) && FUN_00370734(&self->skelAnime)) {
        self->flags |= 2;
    }

    self->actionFunc(self, play);

    if (self->flags & 1) {
        Vec3s vec;
        FUN_0036bcc8(play, &self->actor, &self->headRotation, &vec, self->actor.focus.pos, 0x4300);
    } else {
        Math_SmoothStepToS(&self->headRotation.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->headRotation.y, 0, 6, 6200, 100);
    }

    player = GET_PLAYER(play);

    if (self->flags & 8) {
        if (!(player->stateFlags2 & 0x400)) {
            self->flags &= ~8;
        }
    } else {
        if (((u8*)player)[0x1A7] == 1) {
            self->flags |= 8;
        } else if (player->stateFlags2 & 0x400) {
            swimFlag = (s32)player->actor.yDistToWater;

            if (swimFlag > 0) {
                if (swimFlag >= 320) {
                    if (swimFlag >= 355) {
                        swimFlag = 8;
                    } else {
                        swimFlag = 7;
                    }
                } else if (swimFlag < 80) {
                    swimFlag = 1;
                } else {
                    swimFlag *= 0.025f;
                }

                if (self->swimFlag < swimFlag) {
                    self->swimFlag = swimFlag;

                    if (!(self->flags & 4) && (self->swimFlag >= 8)) {
                        self->flags |= 4;
                        FUN_00372244(&play->unk_5FCC, 0x1E, 0x01000487);
                    }
                }
            }
        }
    }
}

void EnMk_Draw(Actor* actor, GameState* state) {
    EnMk* self = (EnMk*)actor;
    PlayState* play = (PlayState*)state;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnMk_OverrideLimbDraw, EnMk_PostLimbDraw, actor, 0);
}

s32 EnMk_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnMk* self = (EnMk*)thisx;

    if (limbIndex == 4) {
        FUN_0034e01c(mtx, &self->headRotation);
    }
    return 0;
}

void EnMk_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    static Vector3f D_80AAD64C(1000.0f, -100.0f, 0.0f);
    EnMk* self = (EnMk*)thisx;
    Vector3f* dst;

    if (limbIndex == 4) {
        dst = (Vector3f*)&self->actor.focus.pos;
        VEC3TransformAsm(dst, mtx, &D_80AAD64C);
    }
}

// N64 names in brackets (func_80AAxxxx). 3DS: anim mode LOOP = 0, ONCE = 2.
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
s32 FUN_00369a48(Actor* actor, PlayState* play);
void FUN_003725e0(PlayState* play);
void FUN_00370778(PlayState* play);
void FUN_00371e6c(s16 seconds);
void FUN_003f9354(EnMk* self, PlayState* play);
void FUN_003f9588(EnMk* self, PlayState* play);
void FUN_0010d5e8(EnMk* self, PlayState* play);
void FUN_0010d6b8(EnMk* self, PlayState* play);
void FUN_0010d7d4(EnMk* self, PlayState* play);
void FUN_001459a8(EnMk* self, PlayState* play);
void FUN_001459f4(EnMk* self, PlayState* play);
void FUN_00111fac(EnMk* self, PlayState* play);
void FUN_00112050(EnMk* self, PlayState* play);
void FUN_001120f8(EnMk* self, PlayState* play);
void FUN_00112414(EnMk* self, PlayState* play);
void FUN_00141ebc(EnMk* self, PlayState* play);
void FUN_0036bbd0(Actor* actor, PlayState* play, s32 exchangeItem, f32 range);

// func_80AACB6C
void FUN_003f9354(EnMk* self, PlayState* play) {
    if (Actor_ProcessTalkRequest(&self->actor, play) != 0) {
        self->actionFunc = FUN_001459a8;
    }
    self->flags |= 1;
}

// func_80AACA40
void FUN_003f9588(EnMk* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        self->actor.flags &= ~0x10000;
        self->actionFunc = EnMk_Wait;
    }
    self->flags |= 1;
}

// func_80AACBAC
void FUN_0010d7d4(EnMk* self, PlayState* play) {
    if (self->timer > 0) {
        self->timer--;
        self->actor.shape.rot.y -= 0x800;
    } else {
        self->actionFunc = FUN_003f9354;
        Message_ContinueTextbox(play, 0x4030);
    }
}

// func_80AACB14
void FUN_001459a8(EnMk* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        self->actionFunc = FUN_00112414;
        GiveItem(&self->actor, play, 0x25, 10000.0f, 50.0f);
    }
}

// func_80AAD014
void FUN_001459f4(EnMk* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        self->actionFunc = FUN_00141ebc;
        GiveItem(&self->actor, play, 0x3E, 10000.0f, 50.0f);
    }
    self->flags |= 1;
}

// func_80AACFA0
void FUN_00141ebc(EnMk* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        self->actor.parent = NULL;
        self->actionFunc = FUN_003f9588;
        gSaveContext.itemGetInf[1] |= 1;
    } else {
        GiveItem(&self->actor, play, 0x3E, 10000.0f, 50.0f);
    }
}

// func_80AACA94
void FUN_00112414(EnMk* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        self->actor.parent = NULL;
        self->actionFunc = FUN_003f9588;
        FUN_00371e6c(240);
        gSaveContext.eventInf[1] &= ~1;
    } else {
        GiveItem(&self->actor, play, 0x25, 10000.0f, 50.0f);
    }
}

// func_80AACD48
void FUN_001120f8(EnMk* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
        FUN_003725e0(play);
        self->actionFunc = FUN_0010d5e8;
        FUN_00370778(play);
        player->exchangeItemId = 0;
        self->timer = 16;
        Animation_Change_00375c08(&self->skelAnime, 3, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 3), 0, -4.0f);
        self->flags &= ~2;
    }
    self->flags |= 1;
}

// func_80AACEE8
void FUN_00111fac(EnMk* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
        Message_ContinueTextbox(play, 0x4000);
        Animation_Change_00375c08(&self->skelAnime, 2, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 2), 0, -4.0f);
        self->flags &= ~2;
        self->actionFunc = FUN_00112050;
    }
    self->flags |= 1;
}

// func_80AACE2C
void FUN_00112050(EnMk* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
        Message_ContinueTextbox(play, 0x4001);
        Animation_Change_00375c08(&self->skelAnime, 2, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 2), 2, -4.0f);
        self->flags &= ~2;
        self->actionFunc = FUN_001120f8;
    }
    self->flags |= 1;
}

// func_80AACCA0
void FUN_0010d5e8(EnMk* self, PlayState* play) {
    if (self->timer > 0) {
        self->timer--;
        self->actor.shape.rot.y += 0x800;
    } else {
        self->timer = 120;
        self->actionFunc = FUN_0010d6b8;
        Animation_Change_00375c08(&self->skelAnime, 1, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 1), 0, -4.0f);
        self->flags &= ~2;
    }
}

// func_80AACC04
void FUN_0010d6b8(EnMk* self, PlayState* play) {
    if (self->timer > 0) {
        self->timer--;
    } else {
        self->timer = 16;
        self->actionFunc = FUN_0010d7d4;
        Animation_Change_00375c08(&self->skelAnime, 3, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 3), 0, -4.0f);
        self->flags &= ~2;
    }
}

// Draft: 27 words off (mushroom arm if-converted; case-0 tails duplicated vs ROM shared `bne` tail). With `u8 item = INV_CONTENT(..)` local + default placed between case 0 and case 13 the head matches but tails dup (+16 B).
#if 0
void EnMk_Wait(EnMk* self, PlayState* play) {
    s16 angle;
    Player* player = GET_PLAYER(play);
    s32 playerExchangeItem;

    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        playerExchangeItem = FUN_0036bc84(play);

        if (self->actor.textId != 0x4018) {
            player->actor.textId = self->actor.textId;
            self->actionFunc = FUN_003f9588;
        } else {
            if (INV_CONTENT(ITEM_ODD_MUSHROOM) == 0x36) {
                player->actor.textId = 0x4032;
                self->actionFunc = FUN_003f9588;
            } else {
                switch (playerExchangeItem) {
                    case 0:
                        if (self->swimFlag >= 8) {
                            if (gSaveContext.itemGetInf[1] & 1) {
                                player->actor.textId = 0x4075;
                                self->actionFunc = FUN_003f9588;
                            } else {
                                player->actor.textId = 0x4074;
                                self->actionFunc = FUN_001459f4;
                                self->swimFlag = 0;
                            }
                        } else {
                            if (self->swimFlag == 0) {
                                player->actor.textId = 0x4018;
                                self->actionFunc = FUN_003f9588;
                            } else {
                                player->actor.textId = 0x406C + self->swimFlag;
                                self->actionFunc = FUN_003f9588;
                            }
                        }
                        break;
                    case 13:
                        player->actor.textId = 0x4019;
                        self->actionFunc = FUN_00111fac;
                        Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 0), 2, -4.0f);
                        self->flags &= ~2;
                        gSaveContext.timer2State = 0;
                        FUN_00372244(&play->unk_5FCC, 0x1E, 0x0100048C);
                        break;
                    default:
                        player->actor.textId = 0x4018;
                        self->actionFunc = FUN_003f9588;
                        break;
                }
            }
        }
    } else {
        self->actor.textId = Text_GetFaceReaction(play, 0x1A);

        if (self->actor.textId == 0) {
            self->actor.textId = 0x4018;
        }

        angle = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

        if ((ABS(angle) < 0x2151) && (self->actor.xzDistToPlayer < 100.0f)) {
            FUN_0036bbd0(&self->actor, play, 0xD, 100.0f);
            self->flags |= 1;
        }
    }
}
#endif
GLOBAL_ASM("asm/EnMk_Wait.s")

}
