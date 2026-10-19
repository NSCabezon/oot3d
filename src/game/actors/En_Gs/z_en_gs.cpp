extern "C" {
#include "z_en_gs.hpp"
#include "model_manager.hpp"

#define FLAGS 0x02000009

void EnGs_Init(Actor* thisx, PlayState* play);
void EnGs_Destroy(Actor* thisx, PlayState* play);
void EnGs_Update(Actor* thisx, PlayState* play);
void EnGs_Draw(Actor* thisx, PlayState* play);

void FUN_003e339c(EnGs* self, PlayState* play);
void FUN_0034f94c(void* model, PlayState* play, Actor* actor, s32 type);
void FUN_0035046c(void* skelAnimModel);

void FUN_001222e8(EnGs* self, PlayState* play);
void FUN_001223e4(EnGs* self, PlayState* play);
void FUN_001224fc(EnGs* self, PlayState* play);
void FUN_001226bc(EnGs* self, PlayState* play);
void FUN_00122b7c(EnGs* self, PlayState* play);
void FUN_003422e4(EnGs* self);
void Color_RGBA8_Copy(Color_RGBA8* dst, Color_RGBA8* src);
s32 Player_GetMask(PlayState* play);
extern Color_RGBA8 En_Gs_sColor;

extern InitChainEntry En_Gs_sInitChain[];
extern ColliderCylinderInit En_Gs_sCylinderInit;
extern u8 En_Gs_sColBase[];
#define sInitChain En_Gs_sInitChain
#define sCylinderInit En_Gs_sCylinderInit
#define sDamageTable (*(DamageTable*)(En_Gs_sColBase + 0xA))
#define sColChkInfoInit (*(CollisionCheckInfoInit2*)En_Gs_sColBase)

GLOBAL_ASM("data/z_En_Gs.data.s")

GLOBAL_ASM("../src/game/actors/En_Gs/data_syms.s")

ActorInit En_Gs_InitVars = {
    ACTOR_EN_GS,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GS,
    sizeof(EnGs),
    (ActorFunc)EnGs_Init,
    (ActorFunc)EnGs_Destroy,
    (ActorFunc)EnGs_Update,
    (ActorFunc)EnGs_Draw,
};

void EnGs_Init(Actor* thisx, PlayState* play) {
    EnGs* self = (EnGs*)thisx;

    Actor_ProcessInitChain(thisx, sInitChain);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, thisx, &sCylinderInit);
    CollisionCheck_SetInfo2(&thisx->colChkInfo, &sDamageTable, &sColChkInfoInit);

    thisx->targetMode = 6;
    self->unk_23C = thisx->world.pos;
    self->unk_218[0].x = 1.0f;
    self->unk_218[0].y = 1.0f;
    self->unk_218[0].z = 1.0f;
    self->unk_218[1].x = 1.0f;
    self->unk_218[1].y = 1.0f;
    self->unk_218[1].z = 1.0f;
    self->actionFunc = FUN_003e339c;
    self->unk_26C = 0;
    self->unk_274 = 0;
    FUN_00372f38(thisx, play, &self->skelAnimModel, 0, 0);
    GetModelManager().allocator->owner = NULL;
    FUN_0034f94c(&self->unk_278, play, thisx, 11);
    thisx->unk_19B = 3;
}

void EnGs_Destroy(Actor* thisx, PlayState* play) {
    EnGs* self = (EnGs*)thisx;

    FUN_00350f34(thisx, &self->skelAnimModel, 0);
    FUN_0035046c(&self->unk_278);
}

static inline s32 EnGs_HandleText(EnGs* self, PlayState* play) {
    s32 ret = 2;

    switch (Message_GetState(&play->msgCtx)) {
        case 6:
            if (Message_ShouldAdvance(play)) {
                switch (self->actor.textId) {
                    case 0x2054:
                        self->actor.textId = (self->actor.params & 0xFF) + 0x400;
                        ret = 1;
                        break;
                    default:
                        ret = 0;
                        break;
                }
            }
            break;
    }
    return ret;
}

void EnGs_Update(Actor* thisx, PlayState* play) {
    EnGs* self = (EnGs*)thisx;

    Actor_SetFocus(thisx, 33.0f);
    if (!(self->unk_19E & 0x10)) {
        if (self->collider.base.acFlags & 2) {
            self->collider.base.acFlags &= ~2;
            Color_RGBA8_Copy(&self->flashColor, &En_Gs_sColor);
            switch (thisx->colChkInfo.damageEffect) {
                case 15:
                    self->unk_19F = 0;
                    self->unk_19E |= 1;
                    FUN_003422e4(self);
                    self->actionFunc = FUN_001222e8;
                    break;
                case 14:
                    self->unk_19F = 0;
                    self->unk_19E |= 1;
                    FUN_003422e4(self);
                    self->actionFunc = FUN_001223e4;
                    break;
                case 13:
                    self->unk_19F = 0;
                    self->unk_19E |= 1;
                    FUN_003422e4(self);
                    self->actionFunc = FUN_001224fc;
                    break;
                case 12:
                    self->unk_19F = 0;
                    self->unk_19E |= 2;
                    FUN_003422e4(self);
                    self->actionFunc = FUN_001226bc;
                    break;
                case 11:
                    self->unk_19F = 0;
                    self->unk_19E |= 1;
                    FUN_003422e4(self);
                    self->actionFunc = FUN_00122b7c;
                    break;
            }
        }
        Collider_UpdateCylinder(thisx, &self->collider);
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    }
    self->actionFunc(self, play);
    if (self->unk_19C == 2) {
        self->unk_19C = EnGs_HandleText(self, play);
        if (self->unk_19C == 1) {
            goto cont;
        }
    } else if (self->unk_19C == 1) {
    cont:
        Message_ContinueTextbox(play, thisx->textId);
        self->unk_19C = 2;
    } else if (Actor_ProcessTalkRequest(thisx, play)) {
        self->unk_19C = 2;
    } else {
        s16 sp26;
        s16 sp24;

        Actor_GetScreenPos(play, thisx, &sp26, &sp24);
        if ((sp26 >= 0) && (sp26 <= 400) && (sp24 >= 0) && (sp24 <= 240) && (self->unk_19C != 3)) {
            if (FUN_0036bb28(thisx, play, 40.0f) == 1) {
                if (Player_GetMask(play) == 8) {
                    thisx->textId = 0x2054;
                } else {
                    thisx->textId = 0x1B1;
                }
            }
        }
    }
}

// EnGs_Draw: 148 words off. Structure/rotations/scales/colour call match; ROM hoists `mov r6,#1` (SetUnkAC arg) above the
// `flags & 1` branch (4 live callee-saved regs, play in r7) - ours sinks it after the join (same blocker as BgHakaGate_Draw).
// Index-helper tricks (EnGs_Rot(self,1,c)) hoist it but break immediate offsets.
#if 0
void FUN_00371fac(MTX34* mtx, void* arg1);
void FUN_00373bec(void* obj);
void Matrix_RotateX(MTX34* mtx, s32 mode, f32 angle);
void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);
void Matrix_RotateZ(MTX34* mtx, s32 mode, f32 angle);

static inline void EnGs_Scale(MTX34* m, f32 x, f32 y, f32 z) {
    m->data[0][0] *= x;
    m->data[1][0] *= x;
    m->data[2][0] *= x;
    m->data[0][1] *= y;
    m->data[1][1] *= y;
    m->data[2][1] *= y;
    m->data[0][2] *= z;
    m->data[1][2] *= z;
    m->data[2][2] *= z;
}

void EnGs_Draw(Actor* thisx, PlayState* play) {
    EnGs* self = (EnGs*)thisx;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &thisx->modelMtx);
    if (!(self->unk_19E & 8)) {
        if (self->unk_19E & 1) {
            Matrix_RotateY(&mtx, 1, self->unk_1A0[0].y * (2.0f * 3.14159274f / 65536.0f));
            Matrix_RotateX(&mtx, 1, self->unk_1A0[0].x * (2.0f * 3.14159274f / 65536.0f));
            Matrix_RotateZ(&mtx, 1, self->unk_1A0[0].z * (2.0f * 3.14159274f / 65536.0f));
            EnGs_Scale(&mtx, self->unk_218[0].x, self->unk_218[0].y, self->unk_218[0].z);
            Matrix_RotateY(&mtx, 1, self->unk_1A0[1].y * (2.0f * 3.14159274f / 65536.0f));
            Matrix_RotateX(&mtx, 1, self->unk_1A0[1].x * (2.0f * 3.14159274f / 65536.0f));
            Matrix_RotateZ(&mtx, 1, self->unk_1A0[1].z * (2.0f * 3.14159274f / 65536.0f));
        }
        if (self->unk_26C != NULL) {
            self->unk_26C->SetUnkAC(1);
            self->unk_26C->FUN_003721e0(&mtx);
            self->unk_26C->FUN_00372170(0);
        }
        if ((self->unk_19E & 4) && !(self->flashColor.r == 0xFF && self->flashColor.g == 0xFF && self->flashColor.b == 0xFF)) {
            FUN_003695cc(self->skelAnimModel, 0, 5, 0, self->flashColor.r * (1.0f / 255.0f), self->flashColor.g * (1.0f / 255.0f),
                         self->flashColor.b * (1.0f / 255.0f), 0.5f);
        } else {
            FUN_003695cc(self->skelAnimModel, 0, 5, 0, 0.0f, 0.0f, 0.0f, 1.0f);
        }
        if (self->skelAnimModel != NULL) {
            self->skelAnimModel->SetUnkAC(1);
            self->skelAnimModel->FUN_003721e0(&mtx);
            self->skelAnimModel->FUN_00372170(0);
        }
        if (self->unk_274 != NULL) {
            self->unk_274->SetUnkAC(1);
            self->unk_274->FUN_003721e0(&mtx);
            self->unk_274->FUN_00372170(0);
        }
        mtx = thisx->modelMtx;
        if (self->unk_19E & 2) {
            FUN_00371fac(&mtx, (u8*)play + 0x2FC);
            EnGs_Scale(&mtx, 0.05f, -0.05f, 1.0f);
            *(f32*)((u8*)self->unk_280 + 0xC) = (PauseContext_GetState() != 0) ? 0.0f : 1.0f;
            FUN_00373bec(self->unk_280);
            ((SkeletonAnimationModel*)self->unk_278)->SetUnkAC(1);
            ((SkeletonAnimationModel*)self->unk_278)->FUN_003721e0(&mtx);
            ((SkeletonAnimationModel*)self->unk_278)->FUN_00372170(0);
        }
    }
}
#endif

GLOBAL_ASM("asm/EnGs_Draw.s")
}
