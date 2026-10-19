extern "C" {
#include "z_en_ta.hpp"

#define FLAGS 0x00000009
void FUN_00145c0c(EnTa* self, PlayState* play);
void FUN_00145c3c(EnTa* self, PlayState* play);
void FUN_00145c6c(EnTa* self, PlayState* play);
void FUN_0012cfac(EnTa* self, PlayState* play);
void FUN_0012cfec(EnTa* self, PlayState* play);
s32 FUN_002336b0(s32 unused, s32 idx, void* val, u8* self);
void FUN_001a3638(EnTa* self);
void FUN_003c15c4(EnTa* self, PlayState* play);
void FUN_003c14fc(EnTa* self, PlayState* play);
void FUN_001cf750(EnTa* self);
void FUN_0014594c(EnTa* self, PlayState* play);
void FUN_003af690(EnTa* self);
void FUN_003cd668(EnTa* self, PlayState* play);
void FUN_003af700(EnTa* self);
void FUN_003af754(EnTa* self);
void FUN_00112594(EnTa* self);
void FUN_001043ec(EnTa* self, PlayState* play);
void FUN_0021734c(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void FUN_003b04b8(EnTa* self, PlayState* play);
void FUN_001167cc(void);
void FUN_003cd4b8(void);
s32 Inventory_HasEmptyBottle(void);

void EnTa_Init(Actor* thisx, PlayState* play);
void EnTa_Destroy(EnTa* self, PlayState* play);
void FUN_0034ec14(void);
void FUN_003b963c(void);
void FUN_00114ba4(void);
void FUN_003b021c(EnTa* self, PlayState* play);
void FUN_003c5258(void);
void FUN_00202678(void);
void FUN_003ae6b8(void);
void FUN_003685f4(Actor* actor);
void* ZAR_GetCMBByIndex(ZARInfo* zar, s32 index);
void FUN_00353e78(ZARInfo* zar, PlayState* play, SkelAnime* skel, void* cmb, void* arg4, s32 arg5, Vec3s* jointTable,
                  Vec3s* morphTable, s32 limbCount);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, s32 mode, f32 speed, f32 start, f32 end, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
s32 FUN_0036bcc8(PlayState* play, Actor* actor, Vec3s* headRot, Vec3s* torsoRot, Vec3f pos, s32 arg);
void EnTa_Update(Actor* thisx, PlayState* play);
void EnTa_Draw(Actor* actor, GameState* state);

s32 FUN_00369a48(Actor* actor, PlayState* play); // Actor_TextboxIsClosing
void Animation_PlayOnce(SkelAnime* skelAnime, void* anim);
void FUN_001cf7d0(void);
void FUN_003ae868(void);
void FUN_00145a6c(void);
void FUN_003725e0(PlayState* play);
s32 GiveItem(Actor* actor, PlayState* play, s32 getItemId, f32 xyRange, f32 yRange);
void FUN_003af83c(void);
void FUN_003ae93c(void);
void FUN_003ae784(void);
void FUN_0036bbd0(Actor* actor, PlayState* play, s32 exchangeItem, f32 range);
void FUN_003c1620(void);
void FUN_003b0360(void);
void FUN_001eb2b4(void);
s32 FUN_003731e0(SkelAnime* skelAnime);
void FUN_0034e01c(void* a, void* b);

GLOBAL_ASM("data/z_En_Ta.data.s")

ActorInit En_Ta_InitVars = {
    ACTOR_EN_TA,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_TA,
    sizeof(EnTa),
    (ActorFunc)EnTa_Init,
    (ActorFunc)EnTa_Destroy,
    (ActorFunc)EnTa_Update,
    (ActorFunc)EnTa_Draw,
};

// Function-local static: guard 0x0052F58C (TU .data, cut from the data chunk), object 0x0059B668 pinned in
// data/oot3d.ld (scripts/pin_static.py).
void FUN_0021734c(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    static Vector3f sMultVec(1100.0f, 1000.0f, 0.0f);
    EnTa* self = (EnTa*)thisx;
    Vector3f* dst;

    if (limbIndex == 9) {
        dst = (Vector3f*)&self->actor.focus.pos;
        VEC3TransformAsm(dst, mtx, &sMultVec);
    }
}

static inline void EnTa_SetupAction(EnTa* self, EnTaFunc actionFunc, EnTaFunc animFunc) {
    self->actionFunc = actionFunc;
    self->animFunc = animFunc;
}

static inline u8* EnTa_GetObjBank(PlayState* play, u32 idx) {
    if (idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

// far (layout): body matches modulo literal-pool placement (ROM dumps the pool mid-way through the Actor_Spawn block, ~91 insns
// shifted) and the day-branch store schedule; kept as asm.
#if 0
void EnTa_Init(Actor* thisx, PlayState* play) {
    EnTa* self = (EnTa*)thisx;
    u8* bank;
    ZARInfo* zar;
    void* cmb;

    ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
    bank = EnTa_GetObjBank(play, (u8)thisx->objBankIndex);
    zar = (ZARInfo*)(bank + 0x10);
    cmb = ZAR_GetCMBByIndex(zar, 0);
    FUN_00353e78(zar, play, &self->skelAnime, cmb, thisx->unk_178, 0, self->jointTable, self->morphTable, 0x13);
    SkelAnime_SetFaceAnimations((FaceAnimations*)self->unk_9E0, &self->skelAnime, 0, -1, -1);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, thisx, (ColliderCylinderInit*)0x0052F590);

    thisx->colChkInfo.mass = 0xFF;
    self->flags_C3C = 0;
    self->unk_C2A = 0;
    self->unk_C3E = 0;
    self->blinkTimer = 20;
    self->blinkFunc = (EnTaFunc)FUN_00114ba4;
    Actor_SetScale(thisx, 0.01f);
    thisx->velocity.y = -4.0f;
    thisx->minVelocityY = -4.0f;
    thisx->gravity = -1.0f;
    thisx->targetMode = 6;

    switch (thisx->params) {
        case 1:
            if (gSaveContext.eventChkInf[6] & 0x800) {
                Actor_Kill(thisx);
            } else if (gSaveContext.linkAge != 0) {
                Actor_Kill(thisx);
            } else if (gSaveContext.eventChkInf[6] & 0x400) {
                EnTa_SetupAction(self, (EnTaFunc)FUN_003b0360, (EnTaFunc)FUN_001eb2b4);
                self->unk_C10 = 0;
                Animation_PlayOnce(&self->skelAnime, (void*)0);
                self->unk_C40 = (void*)0;
            } else {
                EnTa_SetupAction(self, (EnTaFunc)FUN_003c1620, (EnTaFunc)FUN_001cf750);
                self->unk_C10 = 2;
                Animation_PlayOnce(&self->skelAnime, (void*)1);
                self->unk_C40 = (void*)1;
                thisx->shape.shadowScale = 54.0f;
            }
            break;
        case 2:
            if (!(gSaveContext.eventChkInf[6] & 0x800)) {
                Actor_Kill(thisx);
            } else if (gSaveContext.linkAge != 0) {
                Actor_Kill(thisx);
            } else if (play->sceneNum == 0x36 && gSaveContext.nightFlag != 0) {
                Actor_Kill(thisx);
            } else {
                EnTa_SetupAction(self, (EnTaFunc)FUN_003b04b8, (EnTaFunc)FUN_001eb2b4);
                self->unk_C10 = 0;
                Animation_PlayOnce(&self->skelAnime, (void*)0);
                self->unk_C40 = (void*)0;
            }
            break;
        default:
            if (play->sceneNum == 0x5F) {
                if (gSaveContext.eventChkInf[1] & 0x10) {
                    Actor_Kill(thisx);
                } else if (gSaveContext.eventChkInf[1] & 8) {
                    EnTa_SetupAction(self, (EnTaFunc)FUN_003ae6b8, (EnTaFunc)FUN_001eb2b4);
                    self->unk_C10 = 0;
                    Animation_PlayOnce(&self->skelAnime, (void*)0);
                    self->unk_C40 = (void*)0;
                } else {
                    EnTa_SetupAction(self, (EnTaFunc)FUN_003c14fc, (EnTaFunc)FUN_001cf750);
                    self->unk_C10 = 2;
                    Animation_PlayOnce(&self->skelAnime, (void*)1);
                    self->unk_C40 = (void*)1;
                    thisx->shape.shadowScale = 54.0f;
                }
            } else if (play->sceneNum == 0x4C) {
                if (!(gSaveContext.eventChkInf[1] & 0x10)) {
                    Actor_Kill(thisx);
                } else if (gSaveContext.linkAge == 0) {
                    Actor_Kill(thisx);
                } else if (gSaveContext.nightFlag == 0) {
                    thisx->flags |= 0x10;
                    self->superCuccoTimers[0] = self->superCuccoTimers[1] = self->superCuccoTimers[2] = 7;
                    self->superCuccos[0] = Actor_Spawn(&play->actorCtx, play, 0x19, thisx->world.pos.x + 5.0f,
                                                       thisx->world.pos.y + 3.0f, thisx->world.pos.z + 26.0f, 0, 0, 0, 0xD, 1);
                    self->superCuccos[1] = Actor_Spawn(&play->actorCtx, play, 0x19, thisx->world.pos.x - 20.0f,
                                                       thisx->world.pos.y + 40.0f, thisx->world.pos.z - 30.0f, 0, 0, 0, 0xD, 1);
                    self->superCuccos[2] = Actor_Spawn(&play->actorCtx, play, 0x19, thisx->world.pos.x + 20.0f,
                                                       thisx->world.pos.y + 40.0f, thisx->world.pos.z - 30.0f, 0, 0, 0, 0xD, 1);
                    FUN_003685f4(thisx);
                    if (gSaveContext.eventInf[0] & 0x400) {
                        EnTa_SetupAction(self, (EnTaFunc)FUN_003b021c, (EnTaFunc)FUN_001a3638);
                        Animation_Change_00375c08(&self->skelAnime, 6, 2, 1.0f,
                                                  Animation_GetLastFrame(&self->skelAnime, 6) - 1.0f,
                                                  Animation_GetLastFrame(&self->skelAnime, 6), 0.0f);
                        gSaveContext.eventInf[0] &= ~0x400;
                    } else {
                        EnTa_SetupAction(self, (EnTaFunc)FUN_003c5258, (EnTaFunc)FUN_00202678);
                        self->unk_C10 = 0;
                        Animation_PlayOnce(&self->skelAnime, (void*)5);
                        self->unk_C40 = (void*)5;
                    }
                } else {
                    EnTa_SetupAction(self, (EnTaFunc)FUN_003c15c4, (EnTaFunc)FUN_001cf750);
                    self->unk_C10 = 2;
                    Animation_PlayOnce(&self->skelAnime, (void*)1);
                    self->unk_C40 = (void*)1;
                    thisx->shape.shadowScale = 54.0f;
                }
            } else {
                EnTa_SetupAction(self, (EnTaFunc)FUN_003c14fc, (EnTaFunc)FUN_001cf750);
                self->unk_C10 = 2;
                Animation_PlayOnce(&self->skelAnime, (void*)1);
                self->unk_C40 = (void*)1;
                thisx->shape.shadowScale = 54.0f;
            }
            break;
    }
}
#endif
GLOBAL_ASM("asm/EnTa_Init.s")


void EnTa_Update(Actor* thisx, PlayState* play) {
    EnTa* self = (EnTa*)thisx;
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    ((void (*)(EnTa*))self->animFunc)(self);
    ((void (*)(EnTa*, PlayState*))self->actionFunc)(self, play);

    if (!(self->flags_C3C & 4)) {
        ((void (*)(EnTa*))self->blinkFunc)(self);
    }

    if (self->flags_C3C & 1) {
        if (self->actionFunc == (EnTaFunc)FUN_0012cfac || self->actionFunc == (EnTaFunc)FUN_0012cfec ||
            self->actionFunc == (EnTaFunc)FUN_003b963c) {
            FUN_0036bcc8(play, &self->actor, &self->headRot, &self->torsoRot, self->actor.focus.pos, 0x4E34);
        } else {
            FUN_0036bcc8(play, &self->actor, &self->headRot, &self->torsoRot, self->actor.focus.pos, 0x4300);
        }
    } else {
        Math_SmoothStepToS(&self->headRot.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->headRot.y, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->torsoRot.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->torsoRot.y, 0, 6, 6200, 100);
    }

    self->flags_C3C &= ~5;

    if (self->unk_C28 > 0) {
        self->unk_C28--;
    }
}


void FUN_00145c0c(EnTa* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        EnTaFunc a = (EnTaFunc)FUN_003c14fc;
        EnTaFunc b = (EnTaFunc)FUN_001cf750;
        self->actionFunc = a;
        self->animFunc = b;
    }
}

void FUN_00145c3c(EnTa* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        EnTaFunc a = (EnTaFunc)FUN_003c15c4;
        EnTaFunc b = (EnTaFunc)FUN_001cf750;
        self->actionFunc = a;
        self->animFunc = b;
    }
}

void FUN_00145c6c(EnTa* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        EnTaFunc a = (EnTaFunc)FUN_003c1620;
        EnTaFunc b = (EnTaFunc)FUN_001cf750;
        self->actionFunc = a;
        self->animFunc = b;
    }
}

void FUN_0012cfac(EnTa* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        EnTaFunc a = (EnTaFunc)FUN_003b0360;
        EnTaFunc b = (EnTaFunc)FUN_001eb2b4;
        self->actionFunc = a;
        self->animFunc = b;
    }
    self->flags_C3C |= 1;
}

void FUN_0012cfec(EnTa* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        EnTaFunc a = (EnTaFunc)FUN_003b04b8;
        EnTaFunc b = (EnTaFunc)FUN_001eb2b4;
        self->actionFunc = a;
        self->animFunc = b;
    }
    self->flags_C3C |= 1;
}

s32 FUN_002336b0(s32 unused, s32 idx, void* val, u8* self) {
    if (idx == 8) {
        FUN_0034e01c(val, self + 0xC36);
    } else if (idx == 9) {
        FUN_0034e01c(val, self + 0xC30);
    }
    return 0;
}

void FUN_001a3638(EnTa* self) {
    if (!(self->flags_C3C & 0x10)) {
        if (FUN_003731e0(&self->skelAnime)) {
            self->flags_C3C |= 0x10;
        }
        self->flags_C3C |= 8;
    }
}

void FUN_003c15c4(EnTa* self, PlayState* play) {
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        EnTaFunc b = (EnTaFunc)FUN_00145c3c;
        EnTaFunc a = (EnTaFunc)FUN_001cf750;
        self->animFunc = a;
        self->actionFunc = b;
    }
    self->actor.textId = 0x204B;
    FUN_0036bb28(&self->actor, play, 100.0f);
}

void FUN_003c14fc(EnTa* self, PlayState* play) {
    Actor* player = (Actor*)GET_PLAYER(play);
    s32 talk = Actor_ProcessTalkRequest(&self->actor, play);
    u16 text = 0x702A;
    if (talk) {
        s8 r = FUN_0036bc84(play);
        if (r == 3) {
            player->textId = text | (text >> 13);
            EnTaFunc a = (EnTaFunc)FUN_00112594;
            EnTaFunc b = (EnTaFunc)FUN_001eb2b4;
            self->actionFunc = a;
            self->animFunc = b;
            self->unk_C28 = 0x3C;
            return;
        }
        if (r != 0) {
            player->textId = text;
        }
        EnTaFunc d = (EnTaFunc)FUN_00145c0c;
        EnTaFunc c = (EnTaFunc)FUN_001cf750;
        self->actionFunc = d;
        self->animFunc = c;
        return;
    }
    self->actor.textId = text;
    { s32 three = 3; FUN_0036bbd0(&self->actor, play, three, 100.0f); }
}

void FUN_001cf750(EnTa* self) {
    if (FUN_003731e0(&self->skelAnime)) {
        Animation_PlayOnce(&self->skelAnime, self->unk_C40);
        Audio_PlayActorSound2(&self->actor, 0x0100053B);
    }
    self->flags_C3C |= 0xC;
}

void FUN_0014594c(EnTa* self, PlayState* play) {
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actionFunc = (EnTaFunc)FUN_001cf7d0;
        self->actor.flags &= ~0x10000;
    } else {
        FUN_0036bb28(&self->actor, play, 1000.0f);
    }
    self->flags_C3C |= 1;
}

void FUN_003af690(EnTa* self) {
    self->actor.world.rot.y -= 0x800;
    self->actor.shape.rot.y -= 0x800;
    if (self->unk_C28 == 0) {
        Audio_PlayActorSound2(&self->actor, 0x0100053E);
        EnTaFunc b = (EnTaFunc)FUN_003ae784;
        EnTaFunc a = (EnTaFunc)FUN_001eb2b4;
        self->animFunc = a;
        self->actionFunc = b;
        self->unk_C28 = 0x62;
        self->actor.flags |= 0x10;
    }
}

void FUN_003cd668(EnTa* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 4 && Message_ShouldAdvance(play)) {
        Message_ContinueTextbox(play, 0x2087);
        EnTaFunc a = (EnTaFunc)FUN_001043ec;
        EnTaFunc b = (EnTaFunc)FUN_001a3638;
        self->actionFunc = a;
        self->animFunc = b;
    }
    if (self->flags_C3C & 0x10) {
        self->flags_C3C |= 1;
    }
}

void FUN_003af700(EnTa* self) {
    self->actor.world.rot.y -= 0x8AA;
    self->actor.shape.rot.y -= 0x8AA;
    if (self->unk_C28 == 0) {
        EnTaFunc a = (EnTaFunc)FUN_003ae868;
        EnTaFunc b = (EnTaFunc)FUN_001eb2b4;
        self->actionFunc = a;
        self->animFunc = b;
        self->unk_C28 = 0x62;
    }
}

void FUN_003af754(EnTa* self) {
    self->actor.world.rot.y += 0x800;
    self->actor.shape.rot.y += 0x800;
    if (self->unk_C28 == 0) {
        EnTaFunc a = (EnTaFunc)FUN_003ae93c;
        EnTaFunc b = (EnTaFunc)FUN_001eb2b4;
        self->actionFunc = a;
        self->animFunc = b;
        self->unk_C28 = 0x5A;
    }
}

void FUN_00112594(EnTa* self) {
    self->flags_C3C |= 4;
    if (self->unk_C28 == 0) {
        EnTaFunc a = (EnTaFunc)FUN_00145a6c;
        EnTaFunc b = (EnTaFunc)FUN_001eb2b4;
        self->actionFunc = a;
        self->animFunc = b;
        self->unk_C2A = 3;
        self->unk_C28 = 0x5A;
        Animation_PlayOnce(&self->skelAnime, (void*)2);
        self->unk_C40 = NULL;
        Audio_PlayActorSound2(&self->actor, 0x0100053C);
    }
}

void FUN_001043ec(EnTa* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 5 && Message_ShouldAdvance(play)) {
        FUN_003725e0(play);
        self->flags_C3C &= ~2;
        EnTaFunc a = (EnTaFunc)FUN_003af83c;
        EnTaFunc b = (EnTaFunc)FUN_001a3638;
        self->actionFunc = a;
        self->animFunc = b;
        GiveItem(&self->actor, play, 0x14, 10000.0f, 50.0f);
    }
}

void FUN_003af8f4(EnTa* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 5 && Message_ShouldAdvance(play)) {
        if (Inventory_HasEmptyBottle()) {
            FUN_003725e0(play);
            self->flags_C3C |= 2;
            EnTaFunc a = (EnTaFunc)FUN_003af83c;
            EnTaFunc b = (EnTaFunc)FUN_001a3638;
            self->actionFunc = a;
            self->animFunc = b;
            GiveItem(&self->actor, play, 0x50, 10000.0f, 50.0f);
        } else {
            Message_ContinueTextbox(play, 0x208A);
            EnTaFunc a = (EnTaFunc)FUN_001167cc;
            EnTaFunc b = (EnTaFunc)FUN_001a3638;
            self->actionFunc = a;
            self->animFunc = b;
        }
    }
}

void FUN_003b021c(EnTa* self, PlayState* play) {
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        switch (self->actor.textId) {
            case 0x2085:
                self->actionFunc = (EnTaFunc)FUN_003cd4b8;
                break;
            case 0x2086:
                self->actionFunc = (EnTaFunc)FUN_003cd668;
                break;
            case 0x2088:
                self->actionFunc = (EnTaFunc)FUN_003af8f4;
                break;
        }
        self->actor.flags &= ~0x10000;
    } else {
        self->actor.flags |= 0x10000;
        FUN_0036bb28(&self->actor, play, 1000.0f);
    }
    self->flags_C3C |= 1;
}

static inline s32 EnTa_RequestTalk(EnTa* self, PlayState* play, u16 textId) {
    s16 yawDiff;
    Actor* thisx = &self->actor;

    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        return 1;
    }
    self->actor.textId = textId;
    yawDiff = thisx->yawTowardsPlayer - thisx->shape.rot.y;

    if ((ABS(yawDiff) < 0x4301) && (self->actor.xzDistToPlayer < 100.0f)) {
        self->flags_C3C |= 1;
        FUN_0036bb28(&self->actor, play, 100.0f);
    }
    return 0;
}

void FUN_003b04b8(EnTa* self, PlayState* play) {
    if (EnTa_RequestTalk(self, play, 0x2055)) {
        EnTaFunc a = (EnTaFunc)FUN_0012cfec;
        EnTaFunc b = (EnTaFunc)FUN_001eb2b4;
        self->actionFunc = a;
        self->animFunc = b;
    }
}

void EnTa_Destroy(EnTa* self, PlayState* play2) {
    PlayState* play = play2;
    Collider_DestroyCylinder(play2, &self->collider);
    if (self->actor.params != 1 && self->actor.params != 2) {
        if (play->sceneNum == 0x4C) {
            gSaveContext.timer1State = 0;
        }
    }
    if (self->flags_C3C & 0x200) {
        FUN_0034ec14();
    }
    SkelAnime_Free2(&self->skelAnime);
    ((FaceAnimations*)self->unk_9E0)->Destroy();
}

void EnTa_Draw(Actor* actor, GameState* state) {
    EnTa* self = (EnTa*)actor;

    FaceAnimations* fa = (FaceAnimations*)((EnTa*)actor)->unk_9E0;

    fa->FUN_0035e3a4(0, self->unk_C10);
    ((FaceAnimations*)self->unk_9E0)->FUN_0035e330();
    SkelAnime_DrawOpa(&self->skelAnime, (MTX34*)&self->actor.modelMtx, (OverrideLimbDrawOpa)FUN_002336b0,
                      (PostLimbDrawOpa)FUN_0021734c, &self->actor, 0);
}

}
