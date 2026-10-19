extern "C" {
#include "z_en_dnt_nomal.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x00000030

void EnDntNomal_Init(EnDntNomal* self, PlayState* play);
void EnDntNomal_Destroy(Actor* thisx, PlayState* play);
void FUN_00259440(EnDntNomal* self, PlayState* play); // EnDntNomal_WaitForObject

s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 Collider_InitQuad(PlayState* play, EnDntNomalQuad* quad);
s32 Collider_SetQuad(PlayState* play, EnDntNomalQuad* dest, Actor* actor, void* src);
void FUN_0034eaf8(PlayState* play, EnDntNomalQuad* quad); // Collider_DestroyQuad

extern ColliderCylinderInit EnDntNomal_sBodyCylinderInit;
extern u8 EnDntNomal_sTargetQuadInit[];
extern s32 EnDntNomal_sAnim_3C8;
extern s32 EnDntNomal_sAnim_3CC;
extern s32 EnDntNomal_sAnim_3D0;
extern s32 EnDntNomal_sAnim_3E0;
extern s32 EnDntNomal_sAnim_3E8;

s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);

void FUN_001e7e70(EnDntNomal* self, PlayState* play); // StageReturn
void FUN_00174b4c(EnDntNomal* self, PlayState* play); // StageUnburrow
void FUN_0036ae48(Camera* camera); // Camera_SetFinishedFlag
void FUN_003725e0(PlayState* play); // Message_CloseTextbox
void FUN_0021cde4(EnDntNomal* self, PlayState* play); // SetupTargetTalk
extern __pure float _fabsf(float);
static __inline __pure float EnDntNomal_Fabsf(float f) {
    return _fabsf(f);
}
void FUN_0021c2a8(EnDntNomal* self, PlayState* play); // SetupStageHide
void FUN_0038639c(Actor* thisx, GameState* state); // DrawTargetScrub
void FUN_0028033c(Actor* thisx, GameState* state); // DrawStageScrub
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 bankIndex);
void FUN_001cb978(EnDntNomal* self, PlayState* play); // TargetReturn
void FUN_00238cd0(EnDntNomal* self, PlayState* play); // SetupStageDance
void FUN_001990c8(EnDntNomal* self, PlayState* play); // StageDance
f32* EnDntNomal_CurFramePtr(SkelAnime* skelAnime);

void FUN_0021c454(EnDntNomal* self, PlayState* play); // SetupStageWait
void FUN_0021ce64(EnDntNomal* self, PlayState* play); // SetupTargetWait
void FUN_0021ced4(EnDntNomal* self, PlayState* play); // SetupTargetWalk
void FUN_00177dd0(EnDntNomal* self, PlayState* play); // TargetWait
void FUN_00178148(EnDntNomal* self, PlayState* play); // TargetWalk
void FUN_00177d44(EnDntNomal* self, PlayState* play); // TargetTalk
void FUN_0019bfe4(EnDntNomal* self, PlayState* play); // TargetGivePrize
void FUN_00174efc(EnDntNomal* self, PlayState* play); // StageCelebrate
void EnDntNomal_Update(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Dnt_Nomal.data.s")
GLOBAL_ASM("../src/game/actors/En_Dnt_Nomal/data_syms.s")

ActorInit En_Dnt_Nomal_InitVars = {
    ACTOR_EN_DNT_NOMAL,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnDntNomal),
    (ActorFunc)EnDntNomal_Init,
    (ActorFunc)EnDntNomal_Destroy,
    (ActorFunc)EnDntNomal_Update,
    NULL,
};

void EnDntNomal_Init(EnDntNomal* self, PlayState* play) {
    self->type = self->actor.params;
    if (self->type < 0) {
        self->type = 0;
    }
    s16 neg = -1;
    self->actor.flags &= ~1;
    self->actor.colChkInfo.mass = 0xFF;
    self->objectId = neg;
    EnDntNomal* thisx2 = self;
    if (self->type == 0) {
        Collider_InitQuad(play, &thisx2->targetColliderQuad);
        Collider_SetQuad(play, &self->targetColliderQuad, &self->actor, EnDntNomal_sTargetQuadInit);
        self->actor.world.rot.y = self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
        self->objectId = OBJECT_HINTNUTS;
    } else {
        Collider_InitCylinder(play, &thisx2->bodyColliderCylinder);
        Collider_SetCylinder(play, &self->bodyColliderCylinder, &self->actor, &EnDntNomal_sBodyCylinderInit);
        self->objectId = OBJECT_DNK;
    }
    if (self->objectId >= 0) {
        self->requiredObjectSlot = Object_GetIndex(&play->objectCtx, self->objectId);
        if (self->requiredObjectSlot < 0) {
            Actor_Kill(&self->actor);
            return;
        }
    } else {
        Actor_Kill(&self->actor);
    }
    self->actionFunc = FUN_00259440;
}
GLOBAL_ASM("../split/asm/FUN_00249364.s")

void EnDntNomal_Destroy(Actor* thisx, PlayState* play) {
    EnDntNomal* self = (EnDntNomal*)thisx;

    FUN_003508b8(&self->actor, self->model, 0);
    SkelAnime_Free2(&self->skelAnime);
    if (self->type != 0) {
        ((FaceAnimations*)self->faceAnims)->Destroy();
    }
    if (self->type == 0) {
        FUN_0034eaf8(play, &self->targetColliderQuad);
    } else {
        Collider_DestroyCylinder(play, &self->bodyColliderCylinder);
    }
}

GLOBAL_ASM("asm/EnDntNomal_Update.s")

// EnDntNomal_SetFlower
void FUN_0019be80(EnDntNomal* self, PlayState* play) {
    if (self->actor.bgCheckFlags & 1) {
        self->flowerPos = self->actor.world.pos;
        if (self->type == 0) {
            self->actionFunc = FUN_0021ce64;
        } else {
            self->actionFunc = FUN_0021c454;
        }
    }
}

// EnDntNomal_SetupTargetWait
void FUN_0021ce64(EnDntNomal* self, PlayState* play) {
    s32 frames = Animation_GetLastFrame(&self->skelAnime, EnDntNomal_sAnim_3CC);

    self->endFrame = frames;
    Animation_Change_00375c08(&self->skelAnime, EnDntNomal_sAnim_3CC, 0.0f, 0.0f, frames, 2, -10.0f);
    self->skelAnime.curFrame = 8.0f;
    self->actionFunc = FUN_00177dd0;
}

// EnDntNomal_TargetBurrow
void FUN_00177ca4(EnDntNomal* self, PlayState* play) {
    f32 frame = *EnDntNomal_CurFramePtr(&self->skelAnime);

    FUN_003731e0(&self->skelAnime);
    if (frame >= self->endFrame) {
        self->actionFunc = FUN_0021ce64;
    }
}

// EnDntNomal_TargetUnburrow
void FUN_00177cf4(EnDntNomal* self, PlayState* play) {
    f32 frame = *EnDntNomal_CurFramePtr(&self->skelAnime);

    FUN_003731e0(&self->skelAnime);
    if (frame >= self->endFrame) {
        self->actionFunc = FUN_0021ced4;
    }
}

// EnDntNomal_SetupTargetWalk
void FUN_0021ced4(EnDntNomal* self, PlayState* play) {
    s32 frames = Animation_GetLastFrame(&self->skelAnime, EnDntNomal_sAnim_3E0);

    self->endFrame = frames;
    Animation_Change_00375c08(&self->skelAnime, EnDntNomal_sAnim_3E0, 1.0f, 0.0f, frames, 0, -10.0f);
    self->actor.speedXZ = 1.0f;
    self->actor.colChkInfo.mass = 0;
    self->actionFunc = FUN_00178148;
}

// EnDntNomal_SetupTargetTalk
void FUN_0021cde4(EnDntNomal* self, PlayState* play2) {
    PlayState* play = play2;
    s32 frames = Animation_GetLastFrame(&self->skelAnime, EnDntNomal_sAnim_3E8);

    self->endFrame = frames;
    Animation_Change_00375c08(&self->skelAnime, EnDntNomal_sAnim_3E8, 1.0f, 0.0f, frames, 0, -10.0f);
    self->actor.textId = 0x10AF;
    {
        PlayState* play3 = play;

        FUN_00367c7c(play3, 0x10AF, NULL);
    }
    self->actionFunc = FUN_00177d44;
}

// EnDntNomal_SetupTargetGivePrize
void FUN_002394cc(EnDntNomal* self, PlayState* play) {
    s32 frames = Animation_GetLastFrame(&self->skelAnime, EnDntNomal_sAnim_3C8);

    self->endFrame = frames;
    Animation_Change_00375c08(&self->skelAnime, EnDntNomal_sAnim_3C8, 1.0f, 0.0f, frames, 2, -10.0f);
    self->actionFunc = FUN_0019bfe4;
}

// EnDntNomal_SetupStageCelebrate
void FUN_0021c4e4(EnDntNomal* self, PlayState* play) {
    s32 frames = Animation_GetLastFrame(&self->skelAnime, 6);

    self->endFrame = frames;
    Animation_Change_00375c08(&self->skelAnime, 6, 1.0f, 0.0f, frames, 0, -10.0f);
    self->isSolid = true;
    self->bodyColliderCylinder.dim.height = 46.0f;
    self->actor.speedXZ = 3.0f;
    self->actionFunc = FUN_00174efc;
}

// EnDntNomal_PostLimbDraw
static inline void EnDntNomal_MultVec3f(Vec3f* src, Vec3f* dest, MTX34* mtx) {
    VEC3TransformAsm((Vector3f*)dest, mtx, (Vector3f*)src);
}
void FUN_00267c84(PlayState* play, s32 limbIndex, MTX34* mtx, EnDntNomal* self) {
    Vector3f zeroVec(0.0f, 0.0f, 0.0f);

    if (self->type == 0) {
        if (limbIndex == 8) {
            EnDntNomal_MultVec3f((Vec3f*)&zeroVec, &self->mouthPos, mtx);
        }
    } else if (limbIndex == 8) {
        EnDntNomal_MultVec3f((Vec3f*)&zeroVec, &self->mouthPos, mtx);
    }
}

// EnDntNomal_DrawTargetScrub: NOT MATCHED (recipe R9 MakeScale, 35/74 words, tag: zero-remat). Translate(flowerPos) *
// Scale(0.01) via the shared MTX34_MakeTrans/MTX34_MakeScale helpers; frame/stack layout, both word-store blocks and the
// SetUnkAC/draw tail match. Left: ROM loads pos.z for the trans d[11] into r0 and rematerializes `mov r0,#0` before the scale
// block (1 insn longer); armcc keeps the zero in r0 across both helpers (same zero-CSE issue as EnPart_Update /
// En_Horse_Normal FUN_003a5778). Tried: decl orders, Vec3f aggregate/struct copy, scale ctor after MakeTrans, helper store
// orders (row/col/rev/diag-first/chains + 250-step hill-climb: best 23w, no clean order), s32 pointers, static (late inline).
#if 0
s32 FUN_0037e448(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg); // returns 0
void FUN_0036c174(MTX34* dst, MTX34* a, MTX34* b); // MTX34 multiply
void FUN_0038639c(Actor* thisx, GameState* state) {
    EnDntNomal* self = (EnDntNomal*)thisx;
    PlayState* play = (PlayState*)state;
    MTX34 mtx;
    MTX34 transMtx;
    MTX34 scaleMtx;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, FUN_0037e448, (PostLimbDrawOpa)FUN_00267c84,
                      &self->actor, 0);
    Vector3f pos(self->flowerPos.x, self->flowerPos.y, self->flowerPos.z);
    Vector3f scale(0.01f, 0.01f, 0.01f);

    MTX34_MakeTrans(&transMtx, (Vec3f*)&pos);
    MTX34_MakeScale(&scaleMtx, (Vec3f*)&scale);
    FUN_0036c174(&mtx, &transMtx, &scaleMtx);
    self->model->SetUnkAC(1);
    self->model->FUN_003721e0(&mtx);
    self->model->FUN_00372170(0);
}
#endif

GLOBAL_ASM("asm/FUN_0038639c.s")

// EnDntNomal_StageSetupReturn
void FUN_0027433c(EnDntNomal* self, PlayState* play) {
    s32 frames = Animation_GetLastFrame(&self->skelAnime, 6);

    self->endFrame = frames;
    Animation_Change_00375c08(&self->skelAnime, 6, 1.5f, 0.0f, frames, 0, -10.0f);
    self->actor.speedXZ = 4.0f;
    self->isSolid = false;
    self->bodyColliderCylinder.dim.height = 0.0f;
    self->actionFunc = FUN_001e7e70;
}

// EnDntNomal_TargetTalk
void FUN_00177d44(EnDntNomal* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
        FUN_003725e0(play);
        FUN_0036ae48(play->cameraPtrs[play->activeCamera]);
        *(s16*)((u8*)play->cameraPtrs[play->activeCamera] + 0x1B0) = 0;
        FUN_0036e980(play, NULL, 8);
        self->actionFunc = FUN_002394cc;
    }
}

// EnDntNomal_TargetFacePlayer
void FUN_002746f0(EnDntNomal* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, 0x1388, 0);
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) || Animation_OnFrameImpl(&self->skelAnime, 6.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, 0x10002EF);
    }
    if (EnDntNomal_Fabsf(self->actor.shape.rot.y - self->actor.yawTowardsPlayer) < 30.0f) {
        self->actionFunc = FUN_0021cde4;
    }
}

// EnDntNomal_SetupStageUnburrow
void FUN_0021c380(EnDntNomal* self, PlayState* play) {
    if (self->timer3 == 0) {
        s32 frames = Animation_GetLastFrame(&self->skelAnime, 3);

        self->endFrame = frames;
        Animation_Change_00375c08(&self->skelAnime, 3, 1.0f, 0.0f, frames, 2, -10.0f);
        self->isSolid = false;
        self->bodyColliderCylinder.dim.height = 0.0f;
        EffectSsHahen_SpawnBurst(play, &self->actor.world.pos, 4.0f, 0, 10, 3, 15, -1, 10, NULL);
        Audio_PlayActorSound2(&self->actor, 0x10002EC);
        self->actionFunc = FUN_00174b4c;
    }
}

// EnDntNomal_SetupTargetUnburrow
void FUN_0021ccfc(EnDntNomal* self, PlayState* play) {
    Vec3f spawnPos;

    if (self->timer4 == 0) {
        s32 frames = Animation_GetLastFrame(&self->skelAnime, EnDntNomal_sAnim_3D0);

        self->endFrame = frames;
        Animation_Change_00375c08(&self->skelAnime, EnDntNomal_sAnim_3D0, 1.0f, 0.0f, frames, 2, -10.0f);
        spawnPos = self->actor.world.pos;
        spawnPos.y = self->actor.world.pos.y + 50.0f;
        EffectSsHahen_SpawnBurst(play, &spawnPos, 4.0f, 0, 10, 3, 15, -1, 10, NULL);
        Audio_PlayActorSound2(&self->actor, 0x10002EC);
        self->actionFunc = FUN_00177cf4;
    }
}

// EnDntNomal_TargetWalk
void FUN_00178148(EnDntNomal* self, PlayState* play) {
    f32 dx;
    f32 dz;

    FUN_003731e0(&self->skelAnime);
    dx = 1340.0f + 3.0f - self->actor.world.pos.x;
    dz = 0.0f - self->actor.world.pos.z;
    Math_SmoothStepToS(&self->actor.shape.rot.y, (s16)(Math_FAtan2F(dx, dz) * 10430.378f), 0x32, 0xBB8, 0);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) || Animation_OnFrameImpl(&self->skelAnime, 6.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, 0x10002EF);
    }
    if (self->actor.world.pos.z > -30.0f) {
        self->actor.speedXZ = 0.0f;
        self->actionFunc = FUN_002746f0;
    }
}

// EnDntNomal_StageReturn
void FUN_001e7e70(EnDntNomal* self, PlayState* play) {
    f32 sp2C;
    f32 sp28;

    FUN_003731e0(&self->skelAnime);
    sp2C = self->flowerPos.x - self->actor.world.pos.x;
    sp28 = self->flowerPos.z - self->actor.world.pos.z;
    Math_SmoothStepToS(&self->actor.shape.rot.y, (s16)(Math_FAtan2F(sp2C, sp28) * 10430.378f), 1, 0xBB8, 0);
    if (self->timer5 == 0) {
        self->timer5 = 15;
    } else if (!(self->timer5 & 1)) {
        Audio_PlayActorSound2(&self->actor, 0x10002EF);
    }
    if ((EnDntNomal_Fabsf(sp2C) < 7.0f) && (EnDntNomal_Fabsf(sp28) < 7.0f)) {
        self->actor.world.pos.x = self->flowerPos.x;
        self->actor.world.pos.z = self->flowerPos.z;
        self->actor.speedXZ = 0.0f;
        self->actionFunc = FUN_0021c2a8;
    }
}

// EnDntNomal_WaitForObject
void FUN_00259440(EnDntNomal* self, PlayState* play) {
    EnDntNomal* s2 = self;
    if (Object_IsLoaded(&play->objectCtx, self->requiredObjectSlot)) {
        self->actor.objBankIndex = self->requiredObjectSlot;
        ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
        self->actor.gravity = -2.0f;
        Actor_SetScale(&self->actor, 0.01f);
        if (self->type == 0) {
            FUN_00372f38(&s2->actor, play, &self->model, 2, 0);
            FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 2, self->jointTable, self->morphTable, 9);
            self->skelAnime.unk_75[0] = 0;
            self->flowerPos = self->actor.world.pos;
            self->actor.draw = FUN_0038639c;
        } else {
            FUN_00372f38(&self->actor, play, &self->model, 2, 0);
            FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 5, self->jointTable, self->morphTable, 9);
            self->skelAnime.unk_75[0] = 0;
            SkelAnime_SetFaceAnimations((FaceAnimations*)self->faceAnims, &self->skelAnime, 0, -1, -1);
            self->actor.draw = FUN_0028033c;
        }
        self->actionFunc = FUN_0019be80;
    }
}

// EnDntNomal_TargetGivePrize
void FUN_0019bfe4(EnDntNomal* self, PlayState* play) {
    f32 frame = *EnDntNomal_CurFramePtr(&self->skelAnime);

    FUN_003731e0(&self->skelAnime);
    if ((frame >= 8.0f) && !self->spawnedItem) {
        f32 itemX = self->actor.world.pos.x;
        f32 itemY = self->actor.world.pos.y + 21.0f;
        f32 itemZ = self->actor.world.pos.z;

        if (Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, 0x168, itemX, itemY, itemZ, 0, 0, 0, 0x13) ==
            NULL) {
            FUN_0036e980(play, NULL, 7);
            Actor_Kill(&self->actor);
        }
        self->spawnedItem = true;
        Audio_PlayActorSound2(&self->actor, 0x10002EE);
    }
    if (frame >= self->endFrame) {
        s32 frames = Animation_GetLastFrame(&self->skelAnime, EnDntNomal_sAnim_3E0);

        self->endFrame = frames;
        Animation_Change_00375c08(&self->skelAnime, EnDntNomal_sAnim_3E0, 1.0f, 0.0f, frames, 0, -10.0f);
        self->actionFunc = FUN_001cb978;
    }
}

// EnDntNomal_StageUnburrow
void FUN_00174b4c(EnDntNomal* self, PlayState* play) {
    f32 frame = *EnDntNomal_CurFramePtr(&self->skelAnime);

    FUN_003731e0(&self->skelAnime);
    if (frame >= self->endFrame) {
        if (self->action != 1) {
            self->timer3 = (((s16)Rand_ZeroFloat(2.0f) + (s16)(self->type * 0.5f)) > 0) ? (s32)(0.5f + (((s16)Rand_ZeroFloat(2.0f) + (s16)(self->type * 0.5f)) * 3.0f) * 0.5f)
                                   : (s32)((((s16)Rand_ZeroFloat(2.0f) + (s16)(self->type * 0.5f)) * 3.0f) * 0.5f - 0.5f);
            self->actionFunc = FUN_0021c4e4;
        } else {
            self->timer2 = 450;
            self->actionFunc = FUN_00238cd0;
        }
    }
}

// EnDntNomal_SetupStageDance
void FUN_00238cd0(EnDntNomal* self, PlayState* play) {
    s32 frames = Animation_GetLastFrame(&self->skelAnime, 2);

    self->endFrame = frames;
    Animation_Change_00375c08(&self->skelAnime, 2, 1.0f, 0.0f, frames, 0, -10.0f);
    self->isSolid = true;
    self->bodyColliderCylinder.dim.height = 46.0f;
    self->timer3 = (((s16)Rand_ZeroFloat(20.0f) + 20.0f) > 0.0f)
                       ? (s32)(0.5f + (((s16)Rand_ZeroFloat(20.0f) + 20.0f) * 3.0f) * 0.5f)
                       : (s32)((((s16)Rand_ZeroFloat(20.0f) + 20.0f) * 3.0f) * 0.5f - 0.5f);
    self->rotDirection = -1;
    if (Rand_ZeroFloat(1.99f) < 1.0f) {
        self->rotDirection = 1;
    }
    self->actionFunc = FUN_001990c8;
}

// EnDntNomal_TargetReturn
void FUN_001cb978(EnDntNomal* self, PlayState* play) {
    f32 dx;
    f32 dz;

    FUN_003731e0(&self->skelAnime);
    dx = self->flowerPos.x - self->actor.world.pos.x;
    dz = -180.0f - self->actor.world.pos.z;

    Math_SmoothStepToS(&self->actor.shape.rot.y, (s16)(Math_FAtan2F(dx, dz) * 10430.378f), 3, 0x1388, 0);
    if (EnDntNomal_Fabsf(self->actor.shape.rot.y - (s16)(Math_FAtan2F(dx, dz) * 10430.378f)) < 20.0f) {
        self->actor.speedXZ = 1.0f;
    }
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) || Animation_OnFrameImpl(&self->skelAnime, 6.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, 0x10002EF);
    }
    self->actor.world.rot.y = self->actor.shape.rot.y;
    if (self->actor.world.pos.z < -172.0f) {
        s32 frames = Animation_GetLastFrame(&self->skelAnime, EnDntNomal_sAnim_3CC);

        self->endFrame = frames;
        Animation_Change_00375c08(&self->skelAnime, EnDntNomal_sAnim_3CC, 1.0f, 0.0f, frames, 2, -10.0f);
        self->actor.world.pos.z = -173.0f;
        self->actor.speedXZ = 0.0f;
        self->actionFunc = FUN_00177ca4;
    }
}
}
