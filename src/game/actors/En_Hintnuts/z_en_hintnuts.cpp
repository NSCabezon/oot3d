extern "C" {
#include "z_en_hintnuts.hpp"
#include "model_manager.hpp"

#define FLAGS 0x00000005

void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);
void Animation_PlayOnce(SkelAnime* skelAnime, s32 idx);
void Animation_PlayOnceSetSpeed(SkelAnime* skelAnime, s32 idx, f32 speed);
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);
void Animation_MorphToPlayOnce(SkelAnime* skelAnime, s32 idx, f32 morph);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
void FUN_0036c174(MTX34* dst, MTX34* a, MTX34* b);
void FUN_003725e0(PlayState* play); // Message_CloseTextbox
s32 Camera_GetCamDirYaw(void* cam);
void Math_ScaledStepToS(s16* pValue, s16 target, s16 step);
s32 EnHintnuts_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);

void EnHintnuts_Init(Actor* actor, PlayState* state);
void EnHintnuts_Destroy(Actor* actor, PlayState* state);
void EnHintnuts_Update(Actor* actor, PlayState* state);
void EnHintnuts_Draw(Actor* actor, PlayState* state);
void EnHintnuts_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* rot);
void EnHintnuts_Wait(EnHintnuts* self, PlayState* play);
void EnHintnuts_LookAround(EnHintnuts* self, PlayState* play);
void EnHintnuts_Burrow(EnHintnuts* self, PlayState* play);
void EnHintnuts_BeginRun(EnHintnuts* self, PlayState* play);
void EnHintnuts_Run(EnHintnuts* self, PlayState* play);
void EnHintnuts_Freeze(EnHintnuts* self, PlayState* play);
void EnHintnuts_ThrowNut(EnHintnuts* self, PlayState* play);
void EnHintnuts_Stand(EnHintnuts* self, PlayState* play);
void EnHintnuts_Talk(EnHintnuts* self, PlayState* play);
void EnHintnuts_Leave(EnHintnuts* self, PlayState* play);

GLOBAL_ASM("data/z_En_Hintnuts.data.s")

GLOBAL_ASM("../src/game/actors/En_Hintnuts/data_syms.s")

extern EnHintnutsData EnHintnuts_sData;

ActorInit En_Hintnuts_InitVars = {
    ACTOR_EN_HINTNUTS,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_HINTNUTS,
    sizeof(EnHintnuts),
    (ActorFunc)EnHintnuts_Init,
    (ActorFunc)EnHintnuts_Destroy,
    (ActorFunc)EnHintnuts_Update,
    (ActorFunc)EnHintnuts_Draw,
};


extern __pure float _fabsf(float);
static __inline __pure float EnHintnuts_fabsf(float f) {
    return _fabsf(f);
}
// DRAFT EnHintnuts_Init (43 words off: reg allocation play r7/zero r8/zar sb vs ROM r5/r7/r6; see STATUS.md)
#if 0
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void Actor_SetTextWithPrefix(PlayState* play, Actor* actor, u16 prefix);
void FUN_00358ea8(void* zar, PlayState* play, SkelAnime* skel, void* cmb, void* p, s32 anim, void* jointTable,
                  void* morphTable, s32 limbCount);
extern InitChainEntry EnHintnuts_sInitChain[];
extern EnHintnutsAllData EnHintnuts_sAll;

static inline u8* EnHintnuts_GetObjBank(PlayState* play, u32 idx) {
    if (idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

static inline void EnHintnuts_SetupWait2(EnHintnuts* self, f32 zero) {
    Animation_PlayOnceSetSpeed(&self->skelAnime, EnHintnuts_sAll.data.upAnim, zero);
    self->animFlagAndTimer = Rand_S16Offset_003702c8(100, 50);
    self->collider.dim.height = zero;
    self->actor.world.pos = self->actor.home.pos;
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = EnHintnuts_Wait;
}

void EnHintnuts_Init(Actor* thisx, PlayState* play) {
    EnHintnuts* self = (EnHintnuts*)thisx;
    u8* zar;

    Actor_ProcessInitChain(&self->actor, EnHintnuts_sInitChain);
    zar = EnHintnuts_GetObjBank(play, (u8)self->actor.objBankIndex) + 0x10;
    if (self->actor.params == 0xA) {
        self->actor.flags &= ~5;
        {
            ModelAllocator* alloc = GetModelManager().allocator;

            alloc->owner = self->actor.unk_178;
            self->model = alloc->Create(ZAR_GetCMBByIndex(zar, 2), 1);
            alloc->owner = NULL;
        }
    } else {
        f32 zero = 0.0f;

        ActorShape_Init(&self->actor.shape, zero, ActorShadow_DrawCircle, 35.0f);
        self->model = NULL;
        FUN_00358ea8(zar, play, &self->skelAnime, ZAR_GetCMBByIndex(zar, 0), self->actor.unk_178,
                     EnHintnuts_sAll.data.standAnim, self->jointTable, self->morphTable, 9);
        Collider_InitCylinder(play, &self->collider);
        Collider_SetCylinder(play, &self->collider, &self->actor, &EnHintnuts_sAll.cylinderInit);
        CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, &EnHintnuts_sAll.colChkInfoInit);
        Actor_SetTextWithPrefix(play, &self->actor, (self->actor.params >> 8) & 0xFF);
        self->textIdCopy = self->actor.textId;
        self->actor.params &= 0xFF;
        EnHintnuts_sAll.data.puzzleCounter = 0;
        if (self->actor.textId == 0x109B) {
            if (Flags_GetClear(play, 9) != 0) {
                Actor_Kill(&self->actor);
                return;
            }
        }
        EnHintnuts_SetupWait2(self, zero);
        Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, 0x192, self->actor.world.pos.x,
                           self->actor.world.pos.y, self->actor.world.pos.z, 0, self->actor.world.rot.y, 0, 0xA);
        self->skelAnime.unk_75[0] = 0;
    }
    self->actor.unk_19A = 1;
    self->actor.unk_19B = 2;
}
#endif
GLOBAL_ASM("asm/EnHintnuts_Init.s")

// Model virtual class (vtable slot 1 = Destroy). _ZTI15EnHintnutsModel is an EQU in shared_syms.s (VFE key).
extern "C++" {
struct EnHintnutsModel {
    virtual void unk_vf0();
    virtual void Destroy();
};
}

void EnHintnuts_Destroy(Actor* thisx, PlayState* play) {
    EnHintnuts* self = (EnHintnuts*)thisx;

    if (thisx->params == 0xA) {
        if (self->model != NULL) {
            ((EnHintnutsModel*)self->model)->Destroy();
        }
    } else {
        SkelAnime_Free2(&self->skelAnime);
        Collider_DestroyCylinder(play, &self->collider);
    }
}


void EnHintnuts_Draw(Actor* thisx, PlayState* play) {
    s16 params = thisx->params;
    EnHintnuts* self = (EnHintnuts*)thisx;

    if (params == 0xA) {
        self->model->SetUnkAC(1);
        self->model->FUN_003721e0(&thisx->modelMtx);
        self->model->FUN_00372170(0);
    } else {
        SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, (OverrideLimbDrawOpa)EnHintnuts_OverrideLimbDraw,
                          EnHintnuts_PostLimbDraw, &self->actor, 0);
    }
}

void EnHintnuts_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* rot) {
}

static inline void EnHintnuts_SetupBurrow(EnHintnuts* self) {
    f32 morph = -5.0f;

    Animation_MorphToPlayOnce(&self->skelAnime, EnHintnuts_sData.burrowAnim, morph);
    Audio_PlayActorSound2(&self->actor, 0x010002ED);
    self->actionFunc = EnHintnuts_Burrow;
}

static inline void EnHintnuts_SetupRun(EnHintnuts* self) {
    Animation_PlayLoop(&self->skelAnime, EnHintnuts_sData.runAnim);
    self->animFlagAndTimer = 8;
    self->actionFunc = EnHintnuts_Run;
}

static inline void EnHintnuts_SetupFreeze(EnHintnuts* self) {
    Animation_PlayLoop(&self->skelAnime, EnHintnuts_sData.freezeAnim);
    self->actor.flags &= ~1;
    Actor_SetColorFilter(&self->actor, 0, 255, 0, 1);
    *(s16*)&self->actor.unk_118[0] = 1;
    self->animFlagAndTimer = 0;
    Audio_PlayActorSound2(&self->actor, 0x010002F2);
    if (EnHintnuts_sData.puzzleCounter == -3) {
        Audio_PlaySoundGeneral(0x0100048B, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, (s8*)&DAT_0054ac24);
        EnHintnuts_sData.puzzleCounter = -4;
    }
    self->actionFunc = EnHintnuts_Freeze;
}

void EnHintnuts_BeginFreeze(EnHintnuts* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime)) {
        EnHintnuts_SetupFreeze(self);
    }
}

void EnHintnuts_BeginRun(EnHintnuts* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime)) {
        self->unk_196 = self->actor.yawTowardsPlayer + 0x8000;
        EnHintnuts_SetupRun(self);
    }
    FUN_00370084(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0xE38);
}

void EnHintnuts_LookAround(EnHintnuts* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) && self->animFlagAndTimer != 0) {
        self->animFlagAndTimer--;
    }
    if ((self->actor.xzDistToPlayer < 120.0f) || (self->animFlagAndTimer == 0)) {
        EnHintnuts_SetupBurrow(self);
    }
}

void EnHintnuts_Burrow(EnHintnuts* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime)) {
        FUN_001e8b34(self);
    } else {
        self->collider.dim.height = ((3.0f - CLAMP(*EnHintnuts_CurFramePtr(&self->skelAnime), 1.0f, 3.0f)) * 12.0f);
    }
    if (Animation_OnFrameImpl(&self->skelAnime, 4.0f, 1.0f)) {
        self->collider.base.acFlags &= ~AC_ON;
    }

    Math_ApproachF(&self->actor.world.pos.x, self->actor.home.pos.x, 0.5f, 3.0f);
    Math_ApproachF(&self->actor.world.pos.z, self->actor.home.pos.z, 0.5f, 3.0f);
}

// N64 SetupWait, inlined in EnHintnuts_Freeze (the out-of-line copy is FUN_001e8b34).
static inline void EnHintnuts_SetupWait(EnHintnuts* self) {
    f32 zero = 0.0f;

    Animation_PlayOnceSetSpeed(&self->skelAnime, EnHintnuts_sData.upAnim, zero);
    self->animFlagAndTimer = Rand_S16Offset_003702c8(100, 50);
    self->collider.dim.height = zero;
    self->actor.world.pos = self->actor.home.pos;
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = EnHintnuts_Wait;
}

void EnHintnuts_Freeze(EnHintnuts* self, PlayState* play) {
    *(s16*)&self->actor.unk_118[0] = 1;
    FUN_003731e0(&self->skelAnime);
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, 0x010002F2);
    }
    if (self->animFlagAndTimer == 0) {
        if (EnHintnuts_sData.puzzleCounter == 3) {
            if (self->actor.child != NULL) {
                Actor_ChangeType(play, &play->actorCtx, self->actor.child, 6);
            }
            self->animFlagAndTimer = 1;
        } else if (EnHintnuts_sData.puzzleCounter == -4) {
            self->animFlagAndTimer = 2;
        }
    } else if (Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y - 35.0f, 7.0f) != 0) {
        if (self->animFlagAndTimer == 1) {
            Actor_Kill(&self->actor);
        } else {
            self->actor.flags |= 1;
            self->actor.flags &= ~0x10;
            self->actor.colChkInfo.health = EnHintnuts_sColChkInfoInit.health;
            *(s16*)&self->actor.unk_118[0] = 0;
            EnHintnuts_SetupWait(self);
        }
    }
}

// GET_ACTIVE_CAM(play): play->cameraPtrs[play->activeCamId] (raw offsets, as in the ROM)
static inline void* EnHintnuts_GetActiveCam(PlayState* play) {
    return *(void**)((u8*)play + 0xA54 + *(s16*)((u8*)play + 0xA64) * 4);
}

void EnHintnuts_Leave(EnHintnuts* self, PlayState* play) {
    s16 temp_a1;

    FUN_003731e0(&self->skelAnime);
    if (self->animFlagAndTimer != 0) {
        self->animFlagAndTimer--;
    }
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) || Animation_OnFrameImpl(&self->skelAnime, 6.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, 0x010002EF);
    }
    if (self->actor.bgCheckFlags & 0x8) {
        temp_a1 = self->actor.wallYaw;
    } else {
        temp_a1 = self->actor.yawTowardsPlayer - Camera_GetCamDirYaw(EnHintnuts_GetActiveCam(play)) - 0x8000;
        if (ABS(temp_a1) >= 0x4001) {
            temp_a1 = Camera_GetCamDirYaw(EnHintnuts_GetActiveCam(play)) + 0x8000;
        } else {
            temp_a1 = Camera_GetCamDirYaw(EnHintnuts_GetActiveCam(play)) - (temp_a1 >> 1) + 0x8000;
        }
    }
    Math_ScaledStepToS(&self->actor.shape.rot.y, temp_a1, 0x800);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    if ((self->animFlagAndTimer == 0) || (*(f32*)((u8*)&self->actor + 0xF4) < 0.0f)) {
        FUN_003725e0(play);
        if (self->actor.params == 3) {
            Flags_SetClear(play, self->actor.room);
            EnHintnuts_sData.puzzleCounter = 3;
        }
        if (self->actor.child != NULL) {
            Actor_ChangeType(play, &play->actorCtx, self->actor.child, 6);
        }
        Actor_Kill(&self->actor);
    }
}

// Scale MTX34: zeros stored first, diagonal last (ROM order). Matrix_Scale(x, y, z, APPLY) = FUN_0036c174(m, m, &scale).
static inline void EnHintnuts_MakeScale(MTX34* m, f32 x, f32 y, f32 z) {
    m->data[0][1] = 0.0f;
    m->data[0][2] = 0.0f;
    m->data[0][3] = 0.0f;
    m->data[1][0] = 0.0f;
    m->data[1][2] = 0.0f;
    m->data[1][3] = 0.0f;
    m->data[2][0] = 0.0f;
    m->data[2][1] = 0.0f;
    m->data[2][3] = 0.0f;
    m->data[0][0] = x;
    m->data[1][1] = y;
    m->data[2][2] = z;
}

s32 EnHintnuts_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnHintnuts* self = (EnHintnuts*)thisx;
    Vec3f vec;
    MTX34 scale;
    f32 curFrame;

    if (limbIndex == 4 && self->actionFunc == EnHintnuts_ThrowNut) {
        curFrame = *EnHintnuts_CurFramePtr(&self->skelAnime);
        if (curFrame <= 6.0f) {
            vec.y = 1.0f - (curFrame * 0.0833f);
            vec.x = vec.z = 1.0f + (curFrame * 0.1167f);
        } else if (curFrame <= 7.0f) {
            curFrame -= 6.0f;
            vec.y = 0.5f + curFrame;
            vec.x = vec.z = 1.7f - (curFrame * 0.7f);
        } else if (curFrame <= 10.0f) {
            vec.y = 1.5f - ((curFrame - 7.0f) * 0.1667f);
            vec.x = vec.z = 1.0f;
        } else {
            return false;
        }
        EnHintnuts_MakeScale(&scale, vec.x, vec.y, vec.z);
        FUN_0036c174(mtx, mtx, &scale);
    }
    return false;
}


s16 Actor_WorldYawTowardPoint(Actor* actor, Vec3f* point);
f32 Actor_WorldDistXZToPoint(Actor* actor, Vec3f* point);
void FUN_00363cb8(Actor* actor, PlayState* play); // Actor_OfferTalkNearColChkInfoCylinder

static inline void EnHintnuts_CheckProximity(EnHintnuts* self, PlayState* play) {
    if (self->actor.type != 5) {
        if ((self->collider.base.ocFlags1 & OC1_HIT) || self->actor.isTargeted) {
            self->actor.flags |= 0x10000;
        } else {
            self->actor.flags &= ~0x10000;
        }
        if (self->actor.xzDistToPlayer < 130.0f) {
            self->actor.textId = self->textIdCopy;
            FUN_00363cb8(&self->actor, play);
        }
    }
}

static inline void EnHintnuts_SetupTalk(EnHintnuts* self) {
    f32 morph = -5.0f;

    Animation_MorphToLoop(&self->skelAnime, EnHintnuts_sData.talkAnim, morph);
    self->actor.speedXZ = 0.0f;
    self->actionFunc = EnHintnuts_Talk;
}

void EnHintnuts_Run(EnHintnuts* self, PlayState* play) {
    s16 diffRotInit;
    s16 diffRot;
    f32 phi_f0;

    FUN_003731e0(&self->skelAnime);
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) && self->animFlagAndTimer != 0) {
        self->animFlagAndTimer--;
    }
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) || Animation_OnFrameImpl(&self->skelAnime, 6.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, 0x010002EF);
    }

    Math_StepToF(&self->actor.speedXZ, 7.5f, 1.0f);
    if (Math_SmoothStepToS(&self->actor.world.rot.y, self->unk_196, 1, 0xE38, 0xB6) == 0) {
        if (self->actor.bgCheckFlags & 0x20) {
            self->unk_196 = Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos);
        } else if (self->actor.bgCheckFlags & 8) {
            self->unk_196 = self->actor.wallYaw;
        } else if (self->animFlagAndTimer == 0) {
            diffRotInit = Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos);
            diffRot = diffRotInit - self->actor.yawTowardsPlayer;
            if (ABS(diffRot) >= 0x2001) {
                self->unk_196 = diffRotInit;
            } else {
                phi_f0 = (diffRot >= 0) ? 1.0f : -1.0f;
                self->unk_196 = (s16)((phi_f0 * -8192.0f) + (f32)self->actor.yawTowardsPlayer);
            }
        } else {
            self->unk_196 = (s16)(self->actor.yawTowardsPlayer + 0x8000);
        }
    }

    self->actor.shape.rot.y = self->actor.world.rot.y + 0x8000;
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        EnHintnuts_SetupTalk(self);
    } else if (self->animFlagAndTimer == 0 && Actor_WorldDistXZToPoint(&self->actor, &self->actor.home.pos) < 20.0f &&
               EnHintnuts_fabsf(self->actor.world.pos.y - self->actor.home.pos.y) < 2.0f) {
        self->actor.speedXZ = 0.0f;
        if (self->actor.type == 1) {
            self->actor.flags &= ~(0x10000 | 9);
            self->actor.flags |= 5;
            Actor_ChangeType(play, &play->actorCtx, &self->actor, 5);
        }
        EnHintnuts_SetupBurrow(self);
    } else {
        EnHintnuts_CheckProximity(self, play);
    }
}

// DRAFT EnHintnuts_Update (36 words off: r0/r1 choice in the inlined HitByScrubProjectile2; needs (EnHintnuts* self, PlayState*) signature)
#if 0
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 anim);
void FUN_00375fd0(Actor* actor, ColliderInfo* info, s32 flag); // Actor_SetDropFlag

static inline void EnHintnuts_HitByScrubProjectile1(EnHintnuts* self, PlayState* play) {
    if (self->actor.textId != 0 && self->actor.type == 5 &&
        ((self->actor.params == 0) || (EnHintnuts_sData.puzzleCounter == 2))) {
        self->actor.flags &= ~5;
        self->actor.flags |= 9;
        Actor_ChangeType(play, &play->actorCtx, &self->actor, 1);
    }
}

static inline void EnHintnuts_HitByScrubProjectile2(EnHintnuts* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, EnHintnuts_sData.unk_0C, -3.0f);
    self->collider.dim.height = 37;
    Audio_PlayActorSound2(&self->actor, 0x010002F0);
    self->collider.base.acFlags &= ~AC_ON;

    s16 prm = self->actor.params;
    if (prm > 0 && prm <= 3 && self->actor.type == 5) {
        if (EnHintnuts_sData.puzzleCounter == -4) {
            EnHintnuts_sData.puzzleCounter = 0;
        }
        if (prm == EnHintnuts_sData.puzzleCounter + 1) {
            EnHintnuts_sData.puzzleCounter++;
        } else {
            if (EnHintnuts_sData.puzzleCounter > 0) {
                EnHintnuts_sData.puzzleCounter = -EnHintnuts_sData.puzzleCounter;
            }
            EnHintnuts_sData.puzzleCounter--;
        }
        self->actor.flags |= 0x10;
        self->actionFunc = EnHintnuts_BeginFreeze;
    } else {
        self->actionFunc = EnHintnuts_BeginRun;
    }
}

static inline void EnHintnuts_ColliderCheck(EnHintnuts* self, PlayState* play) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        FUN_00375fd0(&self->actor, &self->collider.info, 1);
        if (self->collider.base.ac->id != 0x193) {
            EnHintnuts_SetupBurrow(self);
        } else {
            EnHintnuts_HitByScrubProjectile1(self, play);
            EnHintnuts_HitByScrubProjectile2(self);
        }
    } else if (play->actorCtx.unk_02 != 0) {
        EnHintnuts_HitByScrubProjectile1(self, play);
        EnHintnuts_HitByScrubProjectile2(self);
    }
}

void EnHintnuts_Update(EnHintnuts* self, PlayState* play) {

    if (self->actor.params != 0xA) {
        EnHintnuts_ColliderCheck(self, play);
        self->actionFunc(self, play);
        if (self->actionFunc != EnHintnuts_Freeze && self->actionFunc != EnHintnuts_BeginFreeze) {
            Actor_MoveForward(&self->actor);
            Actor_UpdateBgCheckInfo(play, &self->actor, 20.0f, self->collider.dim.radius, self->collider.dim.height,
                                    0x1D);
        }
        Collider_UpdateCylinder(&self->actor, &self->collider);
        if (self->collider.base.acFlags & AC_ON) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
        }
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
        if (self->actionFunc == EnHintnuts_Wait) {
            Actor_SetFocus(&self->actor, *EnHintnuts_CurFramePtr(&self->skelAnime));
        } else if (self->actionFunc == EnHintnuts_Burrow) {
            Actor_SetFocus(&self->actor, 20.0f - ((*EnHintnuts_CurFramePtr(&self->skelAnime) * 20.0f) /
                                                  Animation_GetLastFrame(&self->skelAnime, EnHintnuts_sData.burrowAnim)));
        } else {
            Actor_SetFocus(&self->actor, 20.0f);
        }
    }
}
#endif
GLOBAL_ASM("asm/EnHintnuts_Update.s")

void EnHintnuts_Stand(EnHintnuts* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) && self->animFlagAndTimer != 0) {
        self->animFlagAndTimer--;
    }
    if (!(self->animFlagAndTimer & 0x1000)) {
        FUN_00370084(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0xE38);
    }
    if (self->actor.xzDistToPlayer < 120.0f || self->animFlagAndTimer == 0x1000) {
        EnHintnuts_SetupBurrow(self);
    } else if (self->animFlagAndTimer == 0) {
        Animation_PlayOnce(&self->skelAnime, EnHintnuts_sData.unk_04);
        self->actionFunc = EnHintnuts_ThrowNut;
    }
}

static inline void EnHintnuts_SetupLeave(EnHintnuts* self, PlayState* play) {
    f32 morph = -5.0f;

    Animation_MorphToLoop(&self->skelAnime, EnHintnuts_sData.runAnim, morph);
    self->actor.speedXZ = 3.0f;
    self->animFlagAndTimer = 150;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->collider.base.ocFlags1 &= ~OC1_ON;
    self->actor.flags |= 0x10;
    Audio_PlayActorSound2(&self->actor, 0x010002F0);
    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ITEM00, self->actor.world.pos.x, self->actor.world.pos.y,
                self->actor.world.pos.z, 0x0, 0x0, 0x0, 0x3, 1);
    self->actionFunc = EnHintnuts_Leave;
}

void EnHintnuts_Talk(EnHintnuts* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 0x3, 0x400, 0x100);
    if (Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) {
        EnHintnuts_SetupLeave(self, play);
    }
}

void EnHintnuts_ThrowNut(EnHintnuts* self, PlayState* play) {
    Vec3f nutPos;

    FUN_00370084(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0xE38);
    if (self->actor.xzDistToPlayer < 120.0f) {
        EnHintnuts_SetupBurrow(self);
    } else if (FUN_003731e0(&self->skelAnime)) {
        FUN_00258bb0(self);
    } else if (Animation_OnFrameImpl(&self->skelAnime, 6.0f, 1.0f)) {
        nutPos.x = self->actor.world.pos.x + (Math_SinS(self->actor.shape.rot.y) * 35.0f);
        nutPos.y = self->actor.world.pos.y + 12.0f;
        nutPos.z = self->actor.world.pos.z + (Math_CosS(self->actor.shape.rot.y) * 35.0f);
        if (Actor_Spawn(&play->actorCtx, play, 0x193, nutPos.x, nutPos.y, nutPos.z, self->actor.shape.rot.x,
                        self->actor.shape.rot.y, self->actor.shape.rot.z, 1, 1) != NULL) {
            Audio_PlayActorSound2(&self->actor, 0x010002EE);
        }
    }
}

// DRAFT EnHintnuts_Wait (3 words off: s0 vs s1 for the inner xzDistToPlayer load)
#if 0
void SkelAnime_SetPlaySpeed(SkelAnime* skelAnime, f32 speed);
void EnHintnuts_Wait(EnHintnuts* self, PlayState* play) {
    s32 hasSlowPlaybackSpeed = (*EnHintnuts_PlaySpeedPtr(&self->skelAnime) < 0.5f) ? 1 : 0;

    if (hasSlowPlaybackSpeed && (self->animFlagAndTimer != 0)) {
        self->animFlagAndTimer--;
    }
    if (Animation_OnFrameImpl(&self->skelAnime, 9.0f, 1.0f)) {
        self->collider.base.acFlags |= AC_ON;
    } else if (Animation_OnFrameImpl(&self->skelAnime, 8.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, 0x010002EC);
    }

    self->collider.dim.height = ((CLAMP(*EnHintnuts_CurFramePtr(&self->skelAnime), 9.0f, 12.0f) - 9.0f) * 9.0f);
    if (!hasSlowPlaybackSpeed && (self->actor.xzDistToPlayer < 120.0f)) {
        EnHintnuts_SetupBurrow(self);
    } else if (FUN_003731e0(&self->skelAnime)) {
        if (self->actor.xzDistToPlayer < 120.0f) {
            EnHintnuts_SetupBurrow(self);
        } else if ((self->animFlagAndTimer == 0) && (self->actor.xzDistToPlayer > 320.0f)) {
            Animation_PlayLoop(&self->skelAnime, EnHintnuts_sData.spitAnim);
            self->animFlagAndTimer = 3;
            self->actionFunc = EnHintnuts_LookAround;
        } else {
            f32 morph = -3.0f;
            Animation_MorphToLoop(&self->skelAnime, EnHintnuts_sData.standAnim, morph);
            if (self->actionFunc == EnHintnuts_ThrowNut) {
                self->animFlagAndTimer = 0x1003;
            } else {
                self->animFlagAndTimer = 3;
            }
            self->actionFunc = EnHintnuts_Stand;
        }
    }
    if (hasSlowPlaybackSpeed && 160.0f < self->actor.xzDistToPlayer && EnHintnuts_fabsf(self->actor.yDistToPlayer) < 120.0f &&
        ((self->animFlagAndTimer == 0) || (self->actor.xzDistToPlayer < 480.0f))) {
        SkelAnime_SetPlaySpeed(&self->skelAnime, 1.0f);
    }
}
#endif
GLOBAL_ASM("asm/EnHintnuts_Wait.s")

}
