extern "C" {
#include "z_en_vali.hpp"

#define FLAGS 0x00001015
#define ACTOR_FLAG_ATTENTION_ENABLED (1 << 0)

void EnVali_Init(Actor* actor, GameState* state);
void EnVali_Destroy(Actor* actor, PlayState* play);
void EnVali_Update(Actor* actor, GameState* state);
void EnVali_Draw(Actor* actor, PlayState* play);

GLOBAL_ASM("data/z_En_Vali.data.s")

GLOBAL_ASM("../src/game/actors/En_Vali/data_syms.s")

extern CollisionCheckInfoInit EnVali_sColChkInfoInit;
extern char EnVali_sColliderQuadInit[];
extern ColliderCylinderInit EnVali_sCylinderInit;
extern InitChainEntry EnVali_sInitChain[];
extern Vec3f EnVali_sShockScaleTable[];
#define sColChkInfoInit EnVali_sColChkInfoInit
#define sColliderQuadInit EnVali_sColliderQuadInit
#define sCylinderInit EnVali_sCylinderInit
#define sInitChain EnVali_sInitChain

ActorInit En_Vali_InitVars = {
    ACTOR_EN_VALI,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_VALI,
    sizeof(EnVali),
    (ActorFunc)EnVali_Init,
    (ActorFunc)EnVali_Destroy,
    (ActorFunc)EnVali_Update,
    (ActorFunc)EnVali_Draw,
};

void Collider_InitQuad(PlayState* play, void* collider);
void Collider_SetQuad(PlayState* play, void* collider, Actor* actor, void* src);
f32 BgCheck_EntityRaycastFloor4(CollisionContext* colCtx, CollisionPoly** outPoly, s32* outBgId, Actor* actor,
                                Vec3f* pos);
void FUN_003e4744(EnVali* self, PlayState* play);
void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);

static inline void EnVali_SetupWaitPlayerNear(EnVali* self) {
    Animation_PlayLoop(&self->skelAnime, 0);
    self->actor.draw = NULL;
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = FUN_003e4744;
}

void EnVali_SetMat(SkeletonAnimationModel_unk_0C* a, AnimatedMaterial* m);

void EnVali_Init(Actor* thisx, GameState* state) {
    EnVali* self = (EnVali*)thisx;
    PlayState* play = (PlayState*)state;
    s32 bgId;
    f32 floorY;
    ZARInfo* zar;
    SkeletonAnimationModel* model;
    AnimatedMaterial* mat;
    void* cmab;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 27.0f);
    thisx->shape.shadowAlpha = 155;
    self->unk_8BC.x = 1.0f;
    self->unk_8BC.y = 1.0f;
    self->unk_8BC.z = 1.0f;
    zar = FUN_00372f38(&self->actor, play, &self->unk_808[0], 0, &self->unk_808[1], 2, &self->unk_808[2], 1,
                       &self->unk_808[3], 3, &self->unk_808[4], 3, &self->unk_808[5], 3, 0);
    FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 11);
    mat = ((SkeletonAnimationModel*)self->unk_808[1])->unk_10;
    cmab = zar->GetCMABByIndex(0);
    EnVali_SetMat(&self->anim, mat);
    self->anim.FUN_00372d94(cmab);
    self->anim.SetAnimType(1);
    self->anim.SetAnimSpeed(1.0f);
    self->unk_8B8 = 0;
    Collider_InitQuad(play, &self->leftTentacleCollider);
    Collider_SetQuad(play, &self->leftTentacleCollider, &self->actor, sColliderQuadInit);
    Collider_InitQuad(play, &self->rightTentacleCollider);
    Collider_SetQuad(play, &self->rightTentacleCollider, &self->actor, sColliderQuadInit);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, (DamageTable*)((u8*)&sColChkInfoInit + 0x90), &sColChkInfoInit);
    EnVali_SetupWaitPlayerNear(self);
    self->actor.flags &= ~1;
    floorY = BgCheck_EntityRaycastFloor4(&play->colCtx, &self->actor.floorPoly, &bgId, &self->actor, &self->actor.world.pos);
    self->actor.floorHeight = floorY;
    self->actor.params = 0;
    if (floorY == -32000.0f) {
        Actor_Kill(&self->actor);
    }
}

// Collider_DestroyQuad = FUN_0034eaf8 (C stub in En_GeldB TU, inlined by armlink)
s32 FUN_0034eaf8(PlayState* play, void* quad);
void FUN_003685a0(void* arg);

void EnVali_Destroy(Actor* thisx, PlayState* play) {
    EnVali* self = (EnVali*)thisx;

    FUN_0034eaf8(play, &self->leftTentacleCollider);
    FUN_0034eaf8(play, &self->rightTentacleCollider);
    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(thisx, self->unk_808[0], self->unk_808[1], self->unk_808[2], self->unk_808[3], self->unk_808[4],
                 self->unk_808[5], 0);
    SkelAnime_Free2(&self->skelAnime);
    FUN_003685a0(&self->anim);
}

u8 Actor_ApplyDamage(Actor* actor);
void FUN_00375fd0(Actor* actor, void* elem, s32 flag);
void FUN_00375b70(PlayState* play, Actor* actor);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
void Animation_MorphToPlayOnce(SkelAnime* skelAnime, s32 idx, f32 morph);
void Animation_PlayOnce(SkelAnime* skelAnime, s32 idx);
void FUN_001094c0(EnVali* self, PlayState* play);
void FUN_003e1448(EnVali* self);
void FUN_00120ad0(EnVali* self, PlayState* play);
void FUN_001565fc(EnVali* self, PlayState* play);
void FUN_001748c4(EnVali* self, PlayState* play);
void FUN_003d53a0(EnVali* self, PlayState* play);
void FUN_0011ca08(EnVali* self, PlayState* play);
void FUN_003e4744(EnVali* self, PlayState* play);

// EnVali_Update draft, NOT applied: 154/287 words (size 1084 vs ROM 1148). Structure is right (N64 Setup*/CheckCollide
// as static inlines, inlined 3DS SetupElectrify timer 30, unk_8B8 shock-scale table copy). Remaining: armcc
// cross-jumps the two SetupDamaged copies into one block (ROM keeps two + hoists FUN_00120ad0 into fp), bic RMW
// result reg (ROM `bic r1,r0`), per-case actionFunc store scheduling (ROM loads the func ptr before the strh/
// timer store). ~12 variants, left per budget.
#if 0
static inline void EnVali_SetupElectrify(EnVali* self) {
    self->electrifyTimer = 30;
    self->actor.flags &= ~1;
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = (EnValiActionFunc)FUN_001094c0;
}

static inline void EnVali_SetupDamaged(EnVali* self) {
    Animation_PlayOnce(&self->skelAnime, 2);
    Actor_SetColorFilter(&self->actor, 0x400000, 150, 0x200000, 30);
    self->actor.params = 0;
    self->unk_8B8 = 10;
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = FUN_00120ad0;
}

static inline void EnVali_SetupBurnt(EnVali* self) {
    self->timer = 3;
    self->collider.base.acFlags &= ~AC_ON;
    Actor_SetColorFilter(&self->actor, 0x400000, 150, 0x200000, 30);
    self->actionFunc = FUN_001748c4;
}

static inline void EnVali_SetupStunned(EnVali* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, 1, 10.0f);
    self->timer = 120;
    self->actor.velocity.y = 0.0f;
    Actor_SetColorFilter(&self->actor, 0, 255, 0x200000, 80);
    self->collider.info.bumper.effect = 0;
    Audio_PlayActorSound2(&self->actor, 0x0100030E);
    self->actionFunc = (EnValiActionFunc)FUN_003e1448;
    self->actor.velocity.y = 1.0f;
}

static inline void EnVali_SetupFrozen(EnVali* self) {
    self->actor.velocity.y = 0.0f;
    Actor_SetColorFilter(&self->actor, 0, 255, 0x200000, 36);
    self->collider.base.acFlags &= ~AC_ON;
    self->timer = 54;
    self->actionFunc = FUN_001565fc;
}

static inline void EnVali_CheckCollide(EnVali* self, PlayState* play) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        FUN_00375fd0(&self->actor, &self->collider.info, 1);
        if ((self->actor.colChkInfo.damageEffect != 0) || (self->actor.colChkInfo.damage != 0)) {
            if (Actor_ApplyDamage(&self->actor) == 0) {
                Audio_PlayActorSound2(&self->actor, 0x0100030F);
                FUN_00375b70(play, &self->actor);
                self->actor.flags &= ~1;
            } else {
                if ((self->actor.colChkInfo.damageEffect != 1) && (self->actor.colChkInfo.damageEffect != 0xE)) {
                    Audio_PlayActorSound2(&self->actor, 0x0100034C);
                }
            }
            if (self->actor.colChkInfo.damageEffect == 1) {
                if (self->actionFunc != (EnValiActionFunc)FUN_003e1448) {
                    EnVali_SetupStunned(self);
                }
            } else {
                if (self->actor.colChkInfo.damageEffect == 0xF) {
                    if (self->actionFunc != (EnValiActionFunc)FUN_003e1448) {
                        Actor_SetColorFilter(&self->actor, 0x400000, 150, 0x200000, 30);
                        self->actor.params = 1;
                        EnVali_SetupElectrify(self);
                    } else {
                        EnVali_SetupDamaged(self);
                    }
                } else if (self->actor.colChkInfo.damageEffect == 2) {
                    EnVali_SetupBurnt(self);
                } else if (self->actor.colChkInfo.damageEffect == 3) {
                    EnVali_SetupFrozen(self);
                } else if (self->actor.colChkInfo.damageEffect == 0xE) {
                    if (self->spinTimer == 0) {
                        self->spinTimer = 30;
                    }
                } else {
                    EnVali_SetupDamaged(self);
                }
            }
        }
    }
}

void EnVali_Update(Actor* thisx, GameState* state) {
    EnVali* self = (EnVali*)thisx;
    PlayState* play = (PlayState*)state;

    if ((self->collider.base.atFlags & AT_HIT) || (self->leftTentacleCollider.base.atFlags & AT_HIT) ||
        (self->rightTentacleCollider.base.atFlags & AT_HIT)) {
        self->leftTentacleCollider.base.atFlags &= ~AT_HIT;
        self->rightTentacleCollider.base.atFlags &= ~AT_HIT;
        self->collider.base.atFlags &= ~AT_HIT;
        EnVali_SetupElectrify(self);
    }
    EnVali_CheckCollide(self, play);
    self->actionFunc(self, play);
    if ((self->actionFunc != FUN_003d53a0) && (self->actionFunc != FUN_003e4744)) {
        Collider_UpdateCylinder(&self->actor, &self->collider);
        if (self->actionFunc == FUN_0011ca08) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &self->leftTentacleCollider.base);
            CollisionCheck_SetAT(play, &play->colChkCtx, &self->rightTentacleCollider.base);
            CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
        }
        if (self->collider.base.acFlags & AC_ON) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
        }
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
        Actor_SetFocus(&self->actor, 0.0f);
        self->unk_8BC.x = self->unk_8BC.y = self->unk_8BC.z = 1.0f;
        if (self->unk_8B8 != 0) {
            self->unk_8BC.x = EnVali_sShockScaleTable[10 - self->unk_8B8].x;
            self->unk_8BC.y = EnVali_sShockScaleTable[10 - self->unk_8B8].y;
            self->unk_8BC.z = EnVali_sShockScaleTable[10 - self->unk_8B8].z;
            self->unk_8B8--;
        }
    }
}
#endif

GLOBAL_ASM("asm/EnVali_Update.s")

void FUN_00187df0(EnVali* self, PlayState* play);
s32 FUN_001c4b18(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void FUN_001c4c88(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);

void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx); // N64 Gfx_SetupDL_37Opa (empty on 3DS)

void EnVali_Draw(Actor* thisx, PlayState* play) {
    EnVali* self = (EnVali*)thisx;
    s32 pad;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    FUN_00187df0(self, play);
    SkelAnime_DrawOpa(&self->skelAnime, &thisx->modelMtx, (OverrideLimbDrawOpa)FUN_001c4b18,
                      (PostLimbDrawOpa)FUN_001c4c88, thisx, 0);
}

void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);
void Animation_PlayOnce(SkelAnime* skelAnime, s32 idx);
void FUN_00370634(EnVali* self);
void FUN_00364538(EnVali* self, PlayState* play);
void FUN_00105e30(EnVali* self, PlayState* play);
void FUN_00180868(EnVali* self);
void FUN_003e4744(EnVali* self, PlayState* play);
void FUN_003da0b0(EnVali* self, PlayState* play);

void FUN_00120ad0(EnVali* self, PlayState* play) {
    if (FUN_00370734(&self->skelAnime)) {
        if (self->actor.colChkInfo.health != 0) {
            Animation_PlayOnce(&self->skelAnime, 3);
            self->actionFunc = FUN_00105e30;
        } else {
            FUN_00364538(self, play);
        }
    }
}

void FUN_00105e30(EnVali* self, PlayState* play) {
    if (FUN_00370734(&self->skelAnime)) {
        FUN_00370634(self);
    }
}

void FUN_003e4744(EnVali* self, PlayState* play) {
    if (self->actor.xzDistToPlayer < 150.0f) {
        self->actor.draw = (ActorFunc)EnVali_Draw;
        self->actor.velocity.y = 1.0f;
        self->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        self->actionFunc = FUN_003da0b0;
    }
}

void FUN_003da0b0(EnVali* self, PlayState* play) {
    FUN_00370734(&self->skelAnime);
    self->actor.velocity.y *= 1.5f;
    self->actor.velocity.y = CLAMP_MAX(self->actor.velocity.y, 40.0f);
    if (Math_StepToF(&self->actor.world.pos.y, self->actor.floorHeight, self->actor.velocity.y)) {
        Audio_PlayActorSound2(&self->actor, 0x010002EB);
        FUN_00370634(self);
    }
}

void FUN_003b97bc(EnVali* self, PlayState* play) {
    FUN_00370734(&self->skelAnime);
    if (Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.home.pos.y, 0.5f, 15.0f, 0.1f) < 0.01f) {
        FUN_00180868(self);
    }
}

void FUN_00370634(EnVali* self);

void FUN_003e1448(EnVali* self) {
    FUN_00370734(&self->skelAnime);
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->actor.velocity.y != 0.0f) {
        if (Math_StepToF(&self->actor.world.pos.y, self->actor.floorHeight, self->actor.velocity.y)) {
            self->actor.velocity.y = 0.0f;
            Audio_PlayActorSound2(&self->actor, 0x010002EB);
        } else {
            self->actor.velocity.y += 1.0f;
        }
    }
    if (self->timer == 0) {
        self->collider.info.bumper.effect = 1;
        FUN_00370634(self);
    }
}

// EnVali_Burnt (cfg-split tail at 0x001748C4, tail-calls EnVali_SetupDie)
void FUN_001748c4(EnVali* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        FUN_00364538(self, play);
    }
}

// FUN_00187df0 (EnVali_DrawBody) draft, NOT applied: ~309/367 words (size 1500 vs ROM 1468). N64 DrawBody + GetCoreScale/
// GetHoodScale inlined, 3DS Matrix_Scale/FUN_003713fc/FUN_00371fac billboard for the 3 nuclei. Far: VFP numbering
// (ROM keeps scale x/y/z in s17/s16/s18 live across the chain), `in`/`out` Vec3f stack order swapped, billboard K=0.01.
#if 0
void FUN_001094c0(EnVali* self, PlayState* play);
void FUN_00105e30(EnVali* self, PlayState* play);
void Matrix_Scale(MTX34* mtx, s32 mode, f32 x, f32 y, f32 z);
void FUN_003713fc(MTX34* mtx, s32 mode, f32 x, f32 y, f32 z);
void FUN_00371fac(MTX34* mtx, void* billboard);
void VEC3TransformAsm(Vector3f* dest, MTX34* mtx, Vector3f* src);
f32 __hardfp_sinf(f32 x);
f32 __hardfp_cosf(f32 x);
f32* EnVali_CurFramePtr(SkelAnime* skelAnime);
void FUN_00373bec(void* obj);

void FUN_00187df0(EnVali* self, PlayState* play) {
    MTX34 mf;
    MTX34 bb;
    Vec3f out;
    Vec3f in;
    f32 cosYaw;
    f32 sinYaw;
    f32 animCurFrame;
    Vec3f scale;
    Vec3f* shock = &self->unk_8BC;
    SkeletonAnimationModel* m;
    f32 f;
    s32 n;

    scale.x = scale.y = scale.z = 1.0f;
    animCurFrame = *EnVali_CurFramePtr(&self->skelAnime);
    if (self->actionFunc == (EnValiActionFunc)FUN_001094c0) {
        n = 30 - (self->electrifyTimer % 20);
        if (n >= 15) {
            n -= 15;
        }
        scale.y -= 0.13f * __hardfp_sinf(n * 0.31415927f);
    } else if (self->actionFunc == FUN_00120ad0) {
        f = __hardfp_sinf(0.31415927f * animCurFrame);
        scale.y -= 0.18f * f;
        scale.x -= 0.1f * f;
        scale.z = scale.x;
    } else if (self->actionFunc == FUN_00105e30) {
        f = __hardfp_cosf(0.06283185f * animCurFrame);
        scale.y -= 0.18f * f;
        scale.x -= 0.1f * f;
        scale.z = scale.x;
    } else if (self->actionFunc == (EnValiActionFunc)FUN_003e1448) {
        f = __hardfp_sinf(self->timer * 0.31415927f) * 0.08f;
        scale.x -= f;
        scale.y += f;
        scale.z -= f;
    } else {
        if (animCurFrame >= 40.0f) {
            animCurFrame -= 40.0f;
        }
        scale.y -= 0.13f * __hardfp_sinf(0.07853982f * animCurFrame);
    }
    MTX34CopyAsm(&mf, &self->actor.modelMtx);
    Matrix_Scale(&mf, 1, scale.x * shock->x, scale.y * shock->y, scale.z * shock->z);
    m = (SkeletonAnimationModel*)self->unk_808[1];
    m->SetUnkAC(1);
    m->FUN_003721e0(&mf);
    m->FUN_00372170(0);
    mf = self->actor.modelMtx;
    cosYaw = Math_CosS(self->actor.shape.rot.y);
    sinYaw = Math_SinS(self->actor.shape.rot.y);
    in.x = in.y = in.z = 0.0f;
    VEC3TransformAsm((Vector3f*)&out, &mf, (Vector3f*)&in);
    FUN_003713fc(&bb, 0, out.x, out.y, out.z);
    FUN_00371fac(&bb, (u8*)play + 0x2FC);
    Matrix_Scale(&bb, 1, shock->x * 0.01f, shock->y * 0.01f, shock->z * 0.01f);
    m = (SkeletonAnimationModel*)self->unk_808[3];
    m->SetUnkAC(1);
    m->FUN_003721e0(&bb);
    m->FUN_00372170(0);
    FUN_003713fc(&mf, 1, (506.0f * cosYaw) + (372.0f * sinYaw), 1114.0f, (372.0f * cosYaw) - (506.0f * sinYaw));
    VEC3TransformAsm((Vector3f*)&out, &mf, (Vector3f*)&in);
    FUN_003713fc(&bb, 0, out.x, out.y, out.z);
    FUN_00371fac(&bb, (u8*)play + 0x2FC);
    Matrix_Scale(&bb, 1, shock->x * 0.01f, shock->y * 0.01f, shock->z * 0.01f);
    m = (SkeletonAnimationModel*)self->unk_808[4];
    m->SetUnkAC(1);
    m->FUN_003721e0(&bb);
    m->FUN_00372170(0);
    FUN_003713fc(&mf, 1, (-964.0f * cosYaw) - (804.0f * sinYaw), -108.0f, (-804.0f * cosYaw) + (964.0f * sinYaw));
    VEC3TransformAsm((Vector3f*)&out, &mf, (Vector3f*)&in);
    FUN_003713fc(&bb, 0, out.x, out.y, out.z);
    FUN_00371fac(&bb, (u8*)play + 0x2FC);
    Matrix_Scale(&bb, 1, shock->x * 0.01f, shock->y * 0.01f, shock->z * 0.01f);
    m = (SkeletonAnimationModel*)self->unk_808[5];
    m->SetUnkAC(1);
    m->FUN_003721e0(&bb);
    m->FUN_00372170(0);
    mf = self->actor.modelMtx;
    scale.x = scale.y = scale.z = 1.0f;
    animCurFrame = *EnVali_CurFramePtr(&self->skelAnime);
    if (self->actionFunc == (EnValiActionFunc)FUN_001094c0) {
        n = 30 - (self->electrifyTimer % 20);
        if (n >= 15) {
            n -= 15;
        }
        scale.y -= 0.2f * __hardfp_sinf(n * 0.31415927f);
    } else if (self->actionFunc == FUN_00120ad0) {
        f = __hardfp_sinf(0.31415927f * animCurFrame);
        scale.y -= 0.24f * f;
        scale.x -= 0.13f * f;
        scale.z = scale.x;
    } else if (self->actionFunc == FUN_00105e30) {
        f = __hardfp_cosf(0.06283185f * animCurFrame);
        scale.y -= 0.24f * f;
        scale.x -= 0.13f * f;
        scale.z = scale.x;
    } else if (self->actionFunc == (EnValiActionFunc)FUN_003e1448) {
        f = __hardfp_sinf(self->timer * 0.31415927f) * 0.08f;
        scale.x += f;
        scale.y -= f;
        scale.z += f;
    } else {
        if (animCurFrame >= 40.0f) {
            animCurFrame -= 40.0f;
        }
        scale.y -= 0.2f * __hardfp_sinf(0.07853982f * animCurFrame);
    }
    Matrix_Scale(&mf, 1, scale.x * shock->x, scale.y * shock->y, scale.z * shock->z);
    m = (SkeletonAnimationModel*)self->unk_808[2];
    m->SetUnkAC(1);
    m->FUN_003721e0(&mf);
    m->FUN_00372170(0);
    self->anim.SetAnimSpeed(PauseContext_GetState() == 0 ? 1.0f : 0.0f);
    FUN_00373bec(&self->anim);
}
#endif

GLOBAL_ASM("asm/FUN_00187df0.s")


}
