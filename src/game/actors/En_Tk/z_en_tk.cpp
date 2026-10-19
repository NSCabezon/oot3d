extern "C" {
#include "z_en_tk.hpp"

#define FLAGS 0x00000009

void EnTk_Init(Actor* thisx, GameState* state);
void EnTk_Destroy(Actor* thisx, PlayState* play);
void EnTk_Update(EnTk* self, PlayState* play);
void EnTk_Draw(Actor* actor, GameState* state);
s32 EnTk_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void FUN_00273eac(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);

GLOBAL_ASM("data/z_En_Tk.data.s")

GLOBAL_ASM("../src/game/actors/En_Tk/data_syms.s")

ActorInit En_Tk_InitVars = {
    ACTOR_EN_TK,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_TK,
    sizeof(EnTk),
    (ActorFunc)EnTk_Init,
    (ActorFunc)EnTk_Destroy,
    (ActorFunc)EnTk_Update,
    (ActorFunc)EnTk_Draw,
};

void* SystemArena_Malloc_0035010c(s32 size);
void* FUN_003500c4(void* p);
void FUN_0034ff2c(void* obj, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void FUN_0034fea8(void* obj, s32 bank, GameState* state, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_00353c9c(Actor* actor, PlayState* play, SkelAnime* skelAnime, s32 cmbIndex, s32 csabIndex, void* jointTable,
                  void* morphTable, s32 limbCount);
void FUN_003ce19c(EnTk* self, PlayState* play);
extern ColliderCylinderInit EnTk_sCylinderInit;
extern CollisionCheckInfoInit2 EnTk_sColChkInfoInit;

void EnTk_Init(Actor* thisx, GameState* state) {
    EnTk* self = (EnTk*)thisx;
    PlayState* play = (PlayState*)state;
    void* obj;
    s32 objIdx;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 24.0f);
    FUN_00372f38(&self->actor, play, &self->skelAnimModel, 0, 0);
    FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 1, self->jointTable, self->morphTable, 22);
    SkelAnime_SetFaceAnimations((FaceAnimations*)&self->unk_1030, &self->skelAnime, 0, -1, -1);
    objIdx = Object_GetIndex(&play->objectCtx, 1);
    obj = SystemArena_Malloc_0035010c(0x28);
    if (obj != NULL) {
        obj = FUN_003500c4(obj);
    }
    self->unk_11FC = obj;
    FUN_0034ff2c(self->unk_11FC, 4, 4, 0x14, 1, 0);
    FUN_0034fea8(self->unk_11FC, objIdx, state, 0x10, 0x2601, 0x2601, 0x812F, 0x812F);
    Animation_Change_00375c08(&self->skelAnime, 1, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 1), 0, 0.0f);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &EnTk_sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, NULL, &EnTk_sColChkInfoInit);
    if ((u32)(gSaveContext.dayTime - 0xC001) >= 0x1FFF || gSaveContext.linkAge == 0 ||
        play->sceneNum != 0x53) {
        Actor_Kill(&self->actor);
        return;
    }
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.targetMode = 6;
    self->actor.gravity = -0.1f;
    self->currentReward = -1;
    self->currentSpot = NULL;
    self->actionFunc = FUN_003ce19c;
}

void FUN_0034fc7c(void* arg);
void SystemArena_Free(void* ptr);

void EnTk_Destroy(Actor* thisx, PlayState* play) {
    EnTk* self = (EnTk*)thisx;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(thisx, self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
    if (self->unk_11FC != NULL) {
        FUN_0034fc7c(self->unk_11FC);
        SystemArena_Free(self->unk_11FC);
    }
    self->unk_11FC = NULL;
    ((FaceAnimations*)&self->unk_1030)->Destroy();
}

static inline void EnTkEff_Update(EnTk* self) {
    s16 i;
    EnTkEff* eff;

    eff = self->eff;
    for (i = 0; i < 20; i++) {
        if (eff->active != 0) {
            eff->timeLeft--;
            if (eff->timeLeft == 0) {
                eff->active = 0;
            }
            eff->accel.x = Rand_ZeroOne() * 0.4f - 0.2f;
            eff->accel.z = Rand_ZeroOne() * 0.4f - 0.2f;
            eff->pos.x += eff->speed.x;
            eff->pos.y += eff->speed.y;
            eff->pos.z += eff->speed.z;
            eff->speed.x += eff->accel.x;
            eff->speed.y += eff->accel.y;
            eff->speed.z += eff->accel.z;
            eff->size += eff->growth;
        }
        eff++;
    }
}

static inline void EnTk_UpdateEyes(EnTk* self) {
    if (DECR(self->blinkCountdown) == 0) {
        self->eyeTextureIdx++;
        if (self->eyeTextureIdx >= 3) {
            self->blinkCycles--;
            if (self->blinkCycles < 0) {
                self->blinkCountdown = Rand_S16Offset_003702c8(30, 30);
                self->blinkCycles = 2;
                if (Rand_ZeroOne() > 0.5f) {
                    self->blinkCycles++;
                }
            }
            self->eyeTextureIdx = 0;
        }
    }
}

void EnTk_Update(EnTk* self, PlayState* play) {
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    FUN_003731e0(&self->skelAnime);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 40.0f, 10.0f, 0.0f, 5);
    self->actionFunc(self, play);
    EnTkEff_Update(self);
    EnTk_UpdateEyes(self);
}

s32 FUN_00369f3c(PlayState* play);
void FUN_00370778(PlayState* play);
void FUN_003725e0(PlayState* play);
void Rupees_ChangeBy(s16 rupeeChange);

// EnTk_UpdateTalkState
s16 FUN_00186540(PlayState* play, Actor* thisx) {
    s32 talkState = 1;

    switch (Message_GetState(&play->msgCtx)) {
        case 0:
        case 1:
            break;
        case 2:
            switch (thisx->textId) {
                case 0x5028:
                    gSaveContext.infTable[13] |= 0x100;
                    break;
            }
            talkState = 0;
            break;
        case 3:
            break;
        case 4:
            if (Message_ShouldAdvance(play)) {
                switch (thisx->textId) {
                    case 0x5018:
                    case 0x5019:
                        if (FUN_00369f3c(play) == 1) {
                            thisx->textId = 0x0084;
                        } else if (gSaveContext.rupees < 10) {
                            thisx->textId = 0x0085;
                        } else {
                            FUN_00370778(play);
                            Rupees_ChangeBy(-10);
                            gSaveContext.infTable[13] |= 0x200;
                            return 2;
                        }
                        Message_ContinueTextbox(play, thisx->textId);
                        gSaveContext.infTable[13] |= 0x200;
                        break;
                }
            }
            break;
        case 5:
            if (Message_ShouldAdvance(play) && (thisx->textId == 0x0084 || thisx->textId == 0x0085)) {
                FUN_003725e0(play);
                talkState = 0;
            }
            break;
        case 6:
        case 7:
        case 8:
        case 9:
            break;
    }

    return talkState;
}

// EnTk_PostLimbDraw (limb 0x14 = jaw -> focus.pos, 0x12 = neck -> v3f_304 at +0xBC0)
static inline void EnTk_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void FUN_00273eac(PlayState* play, s32 limbIndex, MTX34* mtx, void* self) {
    f32 zx = 0.0f, zy = 0.0f, big = 4600.0f;
    Vector3f sp28(zx, zy, big);
    Vector3f sp1C(0.0f, 0.0f, 0.0f);

    if (limbIndex == 0x14) {
        EnTk_MultVec3f(&sp1C, (Vector3f*)&((Actor*)self)->focus.pos, mtx);
    } else if (limbIndex == 0x12) {
        EnTk_MultVec3f(&sp28, (Vector3f*)((u8*)self + 0xBC0), mtx);
    }
}

// EnTk_Draw: ~78/134 words off (best draft; needs data_syms.s `EnTk_OverrideLimbDraw EQU 0x002882EC` when enabled).
// ROM keeps the 3 scale stores before the *=k multiplies and has play=r5/self=r7/base=r8 (ours: self=r8, base=r7).
#if 0
void FUN_0035619c(PlayState* play, f32* primOut, f32* envOut, s32 r, s32 g, s32 b, s32 a, s32 er, s32 eg, s32 eb);
void FUN_00342988(void* batch, f32* color, s32 arg);
void FUN_003693b4(void* batch, Vec3f* pos, s32 arg2, Vec3f* scale, f32* color, s32 imageIdx);
void FUN_00371eac(void* arg, s32 arg1);
void FUN_0036932c(struct SkeletonAnimationModel* model, s32 idx);

static inline void TkScale(Vec3f* v, f32 k) {
    v->x *= k;
    v->y *= k;
    v->z *= k;
}

void EnTk_Draw(Actor* actor, GameState* state) {
    EnTk* self = (EnTk*)actor;
    PlayState* play = (PlayState*)state;

    {
        EnTkEff* eff = self->eff;
        Vec3f scale;
        f32 primColor[4];
        f32 envColor[4];
        s16 gfxSetup;
        s16 i;

        FUN_0035619c(play, primColor, envColor, 170, 130, 90, 255, 100, 60, 20);
        FUN_00342988(self->unk_11FC, envColor, -1);
        gfxSetup = 0;
        for (i = 0; i < 20; i++, eff++) {
            if (eff->active == 0) {
                continue;
            }
            gfxSetup = i + 1;
            primColor[3] = (s16)(eff->timeLeft * (255.0f / eff->timeTotal)) * (1.0f / 255.0f);
            scale.x = eff->size;
            scale.y = eff->size;
            scale.z = eff->size;
            TkScale(&scale, 40.0f);
            FUN_003693b4(self->unk_11FC, &eff->pos, 0, &scale, primColor,
                         (s16)(15.0f - eff->timeLeft * (16.0f / eff->timeTotal)));
        }
        if (gfxSetup != 0) {
            FUN_00371eac(*(void**)((u8*)self->unk_11FC + 8), 0);
        }
    }
    ((FaceAnimations*)&self->unk_1030)->FUN_0035e3a4(0, self->eyeTextureIdx);
    FUN_0036932c(self->skelAnime.unk_28, 2);
    FUN_0036932c(self->skelAnime.unk_28, 3);
    FUN_0036932c(self->skelAnime.unk_28, 0);
    GraphicsContext_UnknownStub(play->state.gfxCtx);
    SkelAnime_DrawOpa(&self->skelAnime, &actor->modelMtx, (OverrideLimbDrawOpa)EnTk_OverrideLimbDraw,
                      (PostLimbDrawOpa)FUN_00273eac, actor, 0);
    ((FaceAnimations*)&self->unk_1030)->FUN_0035e330();
}
#endif

GLOBAL_ASM("asm/EnTk_Draw.s")
}
