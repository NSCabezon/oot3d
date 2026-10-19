extern "C" {
#include "z_bg_spot00_hanebasi.hpp"
#include "model_manager.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x00400010

void BgSpot00Hanebasi_Init(BgSpot00Hanebasi* self, PlayState* play);
void BgSpot00Hanebasi_Destroy(Actor* actor, GameState* state);
void BgSpot00Hanebasi_Update(Actor* actor, GameState* state);
void BgSpot00Hanebasi_Draw(Actor* actor, GameState* state);

s32 Math_ScaledStepToS(s16* value, s16 target, s16 step);
void FUN_00373264(Actor* actor, s32 sfxId);
void Lights_PointGlowSetInfo(void* light, f32 x, f32 y, f32 z, u8 r, u8 g, u8 b, s16 radius, s32 pad);
void FUN_0035046c(void* obj);
void LightContext_RemoveLight(PlayState* play, void* lightCtx, void* light);
void FUN_003e077c(BgSpot00Hanebasi* self, PlayState* play);
s32 Flags_GetEnv(PlayState* play, s32 flag);

#define IS_CUTSCENE_LAYER (gSaveContext.sceneSetupIndex >= 4)
#define EVENTCHKINF_ZELDA_FLED_CASTLE_SET (gSaveContext.eventChkInf[8] & 0x1)

extern f32 BgSpot00Hanebasi_sTorchFlameScale;
extern InitChainEntry BgSpot00Hanebasi_sInitChain[];
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_0034f94c(void* model, PlayState* play, Actor* actor, s32 type);
void* LightContext_InsertLight(PlayState* play, void* lightCtx, void* info);

GLOBAL_ASM("data/z_Bg_Spot00_Hanebasi.data.s")
GLOBAL_ASM("../src/game/actors/Bg_Spot00_Hanebasi/data_syms.s")

ActorInit Bg_Spot00_Hanebasi_InitVars = {
    ACTOR_BG_SPOT00_HANEBASI,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT00_OBJECTS,
    sizeof(BgSpot00Hanebasi),
    (ActorFunc)BgSpot00Hanebasi_Init,
    (ActorFunc)BgSpot00Hanebasi_Destroy,
    (ActorFunc)BgSpot00Hanebasi_Update,
    (ActorFunc)BgSpot00Hanebasi_Draw,
};

void FUN_003e207c(BgSpot00Hanebasi* self, PlayState* play) {
}

void FUN_00107a40(BgSpot00Hanebasi* self, PlayState* play) {
    u8 lightColor = (u8)(Rand_ZeroOne() * 127.0f) + 128;

    Lights_PointGlowSetInfo(self->lightInfo, (self->dyna.actor.params == 0) ? 260.0f : -260.0f,
                            (5000.0f * BgSpot00Hanebasi_sTorchFlameScale) + 128.0f, 690.0f, lightColor, lightColor, 0,
                            BgSpot00Hanebasi_sTorchFlameScale * 37500.0f, 0);
}

void FUN_003e8b68(BgSpot00Hanebasi* self, PlayState* play) {
    BgSpot00Hanebasi* child = (BgSpot00Hanebasi*)self->dyna.actor.child;

    if (!IS_CUTSCENE_LAYER && CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD) && CHECK_QUEST_ITEM(QUEST_GORON_RUBY) &&
        CHECK_QUEST_ITEM(QUEST_ZORA_SAPPHIRE) && !EVENTCHKINF_ZELDA_FLED_CASTLE_SET) {
        return;
    }

    if ((self->dyna.actor.shape.rot.x != 0) && (Flags_GetEnv(play, 0) || (!IS_CUTSCENE_LAYER && IS_DAY))) {
        self->actionFunc = FUN_003e077c;
        self->destAngle = 0;
        child->destAngle = 0;
    } else if ((self->dyna.actor.shape.rot.x == 0) && !IS_CUTSCENE_LAYER && !LINK_IS_ADULT && !IS_DAY) {
        self->actionFunc = FUN_003e077c;
        self->destAngle = -0x4000;
        child->destAngle = -0xFE0;
    }
}

void FUN_003e077c(BgSpot00Hanebasi* self, PlayState* play) {
    BgSpot00Hanebasi* child = (BgSpot00Hanebasi*)self->dyna.actor.child;
    Actor* childsChild = child->dyna.actor.child;
    s16 angle = 80;

    if (Math_ScaledStepToS(&self->dyna.actor.shape.rot.x, self->destAngle, 80)) {
        self->actionFunc = FUN_003e8b68;
    }

    if (self->dyna.actor.shape.rot.x >= -0x27D8) {
        angle *= 0.4f;
        Math_ScaledStepToS(&child->dyna.actor.shape.rot.x, child->destAngle, angle);
        Math_ScaledStepToS(&childsChild->shape.rot.x, child->destAngle, angle);
    }

    if (self->destAngle < 0) {
        if (self->actionFunc == FUN_003e8b68) {
            Audio_PlayActorSound2(&self->dyna.actor, 0x01000166);
        } else {
            FUN_00373264(&self->dyna.actor, 0x01000164);
        }
    } else {
        if (self->actionFunc == FUN_003e8b68) {
            Audio_PlayActorSound2(&self->dyna.actor, 0x01000165);
        } else {
            FUN_00373264(&self->dyna.actor, 0x01000163);
        }
    }
}

// Init: 63/274 words off, ALL from one register swap (ROM keeps self in r5 / alloc+loop index in r4, ours r4/r5);
// the rest (statics, VFP numbering, guard model loader) matches. ~40 variants, see STATUS.md. Draft kept under #if 0.
#if 0
void BgSpot00Hanebasi_Init(BgSpot00Hanebasi* self, PlayState* play2) {
    PlayState* play = play2;
    f32 cx;
    f32 cy;
    f32 cz;
    CollisionHeader* colHeader = NULL;
    ZARInfo* zar;
    void* cmb;
    s32 i;

    Actor_ProcessInitChain(&self->dyna.actor, BgSpot00Hanebasi_sInitChain);
    self->dyna.actor.unk_19A = 1;

    zar = &play->GetObjectStatus(self->dyna.actor.objBankIndex)->zarInfo;
    cmb = ZAR_GetCMBByIndex(zar, self->dyna.actor.params == -1);
    {
        ModelAllocator* alloc = GetModelManager().allocator;

        alloc->owner = self->dyna.actor.unk_178;
        self->model = (BgSpot00HanebasiModel*)alloc->Create(cmb, 1);
        alloc->owner = NULL;
    }
    for (i = 0; i < 2; i++) {
        FUN_0034f94c(&self->unk_1E0[i], play, &self->dyna.actor, 0xB);
    }

    DynaPolyActor_Init(&self->dyna, 1);
    ReadCollisionHeaderFromZsi(zar->FUN_003532c0(self->dyna.actor.params == -1), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);

    if (self->dyna.actor.params == -1) {
        if (LINK_IS_ADULT && gSaveContext.sceneSetupIndex <= 3) {
            Actor_Kill(&self->dyna.actor);
            return;
        }

        if ((gSaveContext.sceneSetupIndex != 6) &&
            ((gSaveContext.sceneSetupIndex == 4) || (gSaveContext.sceneSetupIndex == 5) ||
             (!LINK_IS_ADULT && !IS_DAY))) {
            self->dyna.actor.shape.rot.x = -0x4000;
        } else {
            self->dyna.actor.shape.rot.x = 0;
        }

        if (gSaveContext.sceneSetupIndex != 6) {
            if (CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD) && CHECK_QUEST_ITEM(QUEST_GORON_RUBY) &&
                CHECK_QUEST_ITEM(QUEST_ZORA_SAPPHIRE) && !EVENTCHKINF_ZELDA_FLED_CASTLE_SET) {
                self->dyna.actor.shape.rot.x = -0x4000;
            }
        }

        cy = (10.0f * Math_CosS(self->dyna.actor.shape.rot.x)) -
                     (Math_SinS(self->dyna.actor.shape.rot.x) * 400.0f);
        cz = (10.0f * Math_SinS(self->dyna.actor.shape.rot.x)) -
                     (Math_CosS(self->dyna.actor.shape.rot.x) * 400.0f);
        cx = (158.0f * Math_CosS(self->dyna.actor.shape.rot.y)) +
                     (cz * Math_SinS(self->dyna.actor.shape.rot.y));
        cz = (-158.0f * Math_SinS(self->dyna.actor.shape.rot.y)) +
                     (cz * Math_CosS(self->dyna.actor.shape.rot.y));

        if (Actor_SpawnAsChild(&play->actorCtx, &self->dyna.actor, play, ACTOR_BG_SPOT00_HANEBASI,
                               self->dyna.actor.world.pos.x + cx, self->dyna.actor.world.pos.y + cy,
                               self->dyna.actor.world.pos.z + cz,
                               ((self->dyna.actor.shape.rot.x == 0) ? 0 : 0xF020), self->dyna.actor.shape.rot.y, 0,
                               0) == NULL) {
            Actor_Kill(&self->dyna.actor);
        }

        self->actionFunc = FUN_003e8b68;
        {
            s16* da = &self->destAngle;
            *da = 40;
        }
    } else if (self->dyna.actor.params == 0) {
        if (Actor_SpawnAsChild(&play->actorCtx, &self->dyna.actor, play, ACTOR_BG_SPOT00_HANEBASI,
                               self->dyna.actor.world.pos.x - (Math_CosS(self->dyna.actor.shape.rot.y) * 316.0f),
                               self->dyna.actor.world.pos.y,
                               self->dyna.actor.world.pos.z + (Math_SinS(self->dyna.actor.shape.rot.y) * 316.0f),
                               self->dyna.actor.shape.rot.x, self->dyna.actor.shape.rot.y, 0, 1) == NULL) {
            Actor_Kill(&self->dyna.actor);
            Actor_Kill(self->dyna.actor.parent);
        }

        self->actionFunc = FUN_00107a40;
    } else {
        self->actionFunc = FUN_00107a40;
    }

    if (self->dyna.actor.params >= 0) {
        self->lightNode = LightContext_InsertLight(play, (u8*)play + 0xA70, &self->lightInfo);
        Lights_PointGlowSetInfo(&self->lightInfo, (self->dyna.actor.params == 0) ? 260.0f : -260.0f, 168.0f, 690.0f,
                                255, 255, 0, 0, 0);
    }
}

#endif

GLOBAL_ASM("asm/BgSpot00Hanebasi_Init.s")

// Destroy: 2 of 32 words off (scratch reg of the unk_1E0 loop index, ROM uses r0); draft kept under #if 0.
// Needs _ZTI21BgSpot00HanebasiModel EQU 0x7FFF00A0 in src/game/shared_syms.s (added in the ws, before END).
// Needs _ZTI21BgSpot00HanebasiModel EQU 0x7FFF00A0 in src/game/shared_syms.s (added in ws; add to main on merge).
void BgSpot00Hanebasi_Destroy(Actor* actor, GameState* state) {
    BgSpot00Hanebasi* self = (BgSpot00Hanebasi*)actor;
    PlayState* play = (PlayState*)state;
    s32 i;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    if (actor->params >= 0) {
        LightContext_RemoveLight(play, (u8*)play + 0xA70, self->lightNode);
    }
    if (self->model != NULL) {
        self->model->Destroy();
    }
    self->model = NULL;
    for (i = 0; i < 2; i++) {
        FUN_0035046c(&self->unk_1E0[i]);
    }
}


// Update: 10/251 words off. All of it is the gTimeSpeed block: ROM compares dayTime with SIGNED gt
// (`ldr r1,=0xD556; ldrh r0; cmp r0,r1; addgt`), ours is unsigned hi (u16 zero-extend is known to armcc), and the
// second block loads dayTime/0x554 into swapped regs. ~25 variants (s32/u16 locals, tmp compare, >= 0xD557...).
#if 0
s32 Gameplay_InCsMode(PlayState* play);
extern u16 gTimeSpeed;

void BgSpot00Hanebasi_Update(Actor* actor, GameState* state) {
    BgSpot00Hanebasi* self = (BgSpot00Hanebasi*)actor;
    PlayState* play = (PlayState*)state;
    Player* player;
    BgSpot00Hanebasi* child;
    Actor* childsChild;

    self->actionFunc(self, play);

    if (self->dyna.actor.params == -1) {
        if (play->sceneNum == 0x51) {
            if (CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD) && CHECK_QUEST_ITEM(QUEST_GORON_RUBY) &&
                CHECK_QUEST_ITEM(QUEST_ZORA_SAPPHIRE) && !EVENTCHKINF_ZELDA_FLED_CASTLE_SET && LINK_IS_CHILD) {
                player = GET_PLAYER(play);

                if ((player->actor.world.pos.x > -450.0f) && (player->actor.world.pos.x < 450.0f) &&
                    (player->actor.world.pos.z > 1080.0f) && (player->actor.world.pos.z < 1700.0f) &&
                    !Gameplay_InCsMode(play)) {
                    gSaveContext.eventChkInf[8] |= 1;
                    Flags_SetEventChkInf(0x82);
                    self->actionFunc = FUN_003e207c;
                    FUN_0036e980(play, &player->actor, 8);
                    FUN_003716f0(play, 0xCD, 0x14, 4);
                    gSaveContext.nextCutsceneIndex = 0xFFF1;
                } else if (Actor_IsFacingAndNearPlayer(&self->dyna.actor, 3000.0f, 0x7530)) {
                    *((u8*)play + 0x325D) = 1;
                }
            }
        }

        if (gSaveContext.sceneSetupIndex == 5) {
            u16 dayTime;
            s32 tmp;

            if (gTimeSpeed == 50) {
                tmp = 0xD556;

                if (gSaveContext.dayTime > 0xD556) {
                    tmp = 0xD556 + 0x10000;
                }

                gTimeSpeed = (tmp - gSaveContext.dayTime) * (1.0f / 350.0f);
            }

            dayTime = gSaveContext.dayTime;

            if ((dayTime > 0x2AAB) && (dayTime < 0x3000) && (gSaveContext.sceneSetupIndex == 5)) {
                gTimeSpeed = 0;
            }
        }
    }

    child = (BgSpot00Hanebasi*)self->dyna.actor.child;
    if (child != NULL && (childsChild = child->dyna.actor.child) != NULL) {
        if (self->dyna.actor.shape.rot.x > -5100) {
            FUN_003695cc((SkeletonAnimationModel*)child->model, 3, 4, 0, 1.0f, 1.0f, 1.0f, 1.0f);
            FUN_003695cc((SkeletonAnimationModel*)child->model, 2, 4, 0, 1.0f, 1.0f, 1.0f, 1.0f);
            FUN_003695cc((SkeletonAnimationModel*)((BgSpot00Hanebasi*)childsChild)->model, 3, 4, 0, 1.0f, 1.0f, 1.0f, 1.0f);
            FUN_003695cc((SkeletonAnimationModel*)((BgSpot00Hanebasi*)childsChild)->model, 2, 4, 0, 1.0f, 1.0f, 1.0f, 1.0f);
        } else if (self->dyna.actor.shape.rot.x > -9550) {
            FUN_003695cc((SkeletonAnimationModel*)child->model, 3, 4, 0, 1.0f, 1.0f, 1.0f, 0.0f);
            FUN_003695cc((SkeletonAnimationModel*)child->model, 2, 4, 0, 1.0f, 1.0f, 1.0f, 1.0f);
            FUN_003695cc((SkeletonAnimationModel*)((BgSpot00Hanebasi*)childsChild)->model, 3, 4, 0, 1.0f, 1.0f, 1.0f, 0.0f);
            FUN_003695cc((SkeletonAnimationModel*)((BgSpot00Hanebasi*)childsChild)->model, 2, 4, 0, 1.0f, 1.0f, 1.0f, 1.0f);
        } else {
            FUN_003695cc((SkeletonAnimationModel*)child->model, 3, 4, 0, 1.0f, 1.0f, 1.0f, 0.0f);
            FUN_003695cc((SkeletonAnimationModel*)child->model, 2, 4, 0, 1.0f, 1.0f, 1.0f, 0.0f);
            FUN_003695cc((SkeletonAnimationModel*)((BgSpot00Hanebasi*)childsChild)->model, 3, 4, 0, 1.0f, 1.0f, 1.0f, 0.0f);
            FUN_003695cc((SkeletonAnimationModel*)((BgSpot00Hanebasi*)childsChild)->model, 2, 4, 0, 1.0f, 1.0f, 1.0f, 0.0f);
        }
    }
}

#endif

GLOBAL_ASM("asm/BgSpot00Hanebasi_Update.s")

// Draw: far (~200/291 words). Draft: inlined 3DS Matrix_Translate/RotateY/Scale (see Bg_Mizu_Shutter helpers), the
// model-manager draw-queue call FUN_00330b98(&mm + 0x180, model, 0) x3 (ROM hoists the literal mm+0x180 into sl and
// derives &mm as sl-0x180 for the guard ctor; ours hoists &mm), basePos via Vector3f ctor (vldr, not ldm).
#if 0
f32 sinf(f32 x);
f32 cosf(f32 x);
void FUN_00330b98(void* mgr, SkeletonAnimationModel* model, s32 flag);
void FUN_00373bec(SkeletonAnimationModel_unk_0C* anim);
s16 Camera_GetCamDirYaw(Camera* camera);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);

static inline void BgSpot00Hanebasi_RotY(MTX34* m, f32 angle) {
    if (angle != 0.0f) {
        f32 sinA = sinf(angle);
        f32 cosA = cosf(angle);

        for (s32 r = 0; r < 3; r++) {
            f32 a = m->data[r][0];
            f32 b = m->data[r][2];

            m->data[r][0] = a * cosA - b * sinA;
            m->data[r][2] = a * sinA + b * cosA;
        }
    }
}

static inline void BgSpot00Hanebasi_Scale(MTX34* m, f32 x, f32 y, f32 z) {
    for (s32 r = 0; r < 3; r++) {
        m->data[r][0] *= x;
    }
    for (s32 r = 0; r < 3; r++) {
        m->data[r][1] *= y;
    }
    for (s32 r = 0; r < 3; r++) {
        m->data[r][2] *= z;
    }
}

void BgSpot00Hanebasi_Draw(Actor* actor, GameState* state) {
    BgSpot00Hanebasi* self = (BgSpot00Hanebasi*)actor;
    PlayState* play = (PlayState*)state;
    Vector3f basePos(158.0f, 10.0f, 400.0f);
    Vec3f newPos;
    MTX34 mtx;
    f32 angle;
    s32 i;

    if (actor->params == -1) {
        ((SkeletonAnimationModel*)self->model)->FUN_003721e0(&actor->modelMtx);
        ((SkeletonAnimationModel*)self->model)->SetUnkAC(1);
        FUN_00330b98(GetModelManager().unk_180, (SkeletonAnimationModel*)self->model, 0);

        VEC3TransformAsm((Vector3f*)&newPos, &actor->modelMtx, &basePos);
        actor->child->world.pos.x = newPos.x;
        actor->child->world.pos.y = newPos.y;
        actor->child->world.pos.z = newPos.z;
        basePos.x *= -1.0f;

        VEC3TransformAsm((Vector3f*)&newPos, &actor->modelMtx, &basePos);
        actor->child->child->world.pos.x = newPos.x;
        actor->child->child->world.pos.y = newPos.y;
        actor->child->child->world.pos.z = newPos.z;

        if (gSaveContext.sceneSetupIndex != 12) {
            if (gSaveContext.sceneSetupIndex >= 4 || (!LINK_IS_ADULT && actor->shape.rot.x < -0x2000)) {
                if (IS_CUTSCENE_LAYER) {
                    BgSpot00Hanebasi_sTorchFlameScale = 0.008f;
                } else {
                    BgSpot00Hanebasi_sTorchFlameScale = ((actor->shape.rot.x * -1) - 0x2000) * (1.0f / 1024000.0f);
                }

                angle = (s16)(Camera_GetCamDirYaw(play->cameraPtrs[play->activeCamera]) + 0x8000) *
                        (2.0f * 3.14159274f / 65536.0f);

                for (i = 0; i < 2; i++) {
                    Vector3f pos((i == 0) ? 260.0f : -260.0f, 128.0f, 690.0f);

                    MTX34_MakeTrans(&mtx, (Vec3f*)&pos);
                    BgSpot00Hanebasi_RotY(&mtx, angle);
                    BgSpot00Hanebasi_Scale(&mtx, BgSpot00Hanebasi_sTorchFlameScale, BgSpot00Hanebasi_sTorchFlameScale,
                                           BgSpot00Hanebasi_sTorchFlameScale);
                    self->unk_1E0[i].anim->SetAnimSpeed(PauseContext_GetState() != 0 ? 0.0f : 1.0f);
                    FUN_00373bec(self->unk_1E0[i].anim);
                    self->unk_1E0[i].model->FUN_003721e0(&mtx);
                    self->unk_1E0[i].model->SetUnkAC(1);
                    FUN_00330b98(GetModelManager().unk_180, self->unk_1E0[i].model, 0);
                }
            } else {
                BgSpot00Hanebasi_sTorchFlameScale = 0.0f;
            }
        }
    } else {
        ((SkeletonAnimationModel*)self->model)->FUN_003721e0(&actor->modelMtx);
        ((SkeletonAnimationModel*)self->model)->SetUnkAC(1);
        FUN_00330b98(GetModelManager().unk_180, (SkeletonAnimationModel*)self->model, 0);
    }
}
#endif

GLOBAL_ASM("asm/BgSpot00Hanebasi_Draw.s")
}
