extern "C" {
#include "z_en_weiyer.hpp"

#define FLAGS 0x00000005

void FUN_0019fce0(EnWeiyer* self, PlayState* play);
void FUN_00126840(EnWeiyer* self, PlayState* play);
void FUN_0019b798(EnWeiyer* self, PlayState* play);
void FUN_001236dc(EnWeiyer* self, PlayState* play);
void FUN_003e2240(EnWeiyer* self, PlayState* play);
f32 __hardfp_cosf(f32);
f32 __hardfp_sinf(f32);
f32* EnWeiyer_CurFramePtr(SkelAnime* skelAnime); // z_en_weiyer_anim.cpp
s32 FUN_0036e5e0(SkelAnime* skelAnime, f32 frame, f32 updateRate);
s32 Math_ScaledStepToS(s16* value, s16 target, s16 step);
void FUN_001cca7c(EnWeiyer* self);
void FUN_001c8264(EnWeiyer* self);
void FUN_00364394(EnWeiyer* self);
f32 BgCheck_EntityRaycastFloor4(CollisionContext* colCtx, CollisionPoly** outPoly, s32* outBgId, Actor* actor, Vec3f* pos);
s32 WaterBox_GetSurfaceImpl(PlayState* play, CollisionContext* colCtx, f32 x, f32 z, f32* ySurface, WaterBox** waterBox);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_003dda10(EnWeiyer* self, PlayState* play);
void EffectSsGSplash_Spawn(PlayState* play, Vec3f* pos, Vec3f* a, Vec3f* b, s16 c, s16 d);
void Audio_PlayActorSound2(Actor* actor, s32 arg1);
void FUN_003d59b8(EnWeiyer* self, PlayState* play);
void FUN_0013f804(EnWeiyer* self, PlayState* play);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morphFrames);
void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* pos, s16 params);
void SkelAnime_SetPlaySpeed(SkelAnime* skelAnime, f32 speed);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);
void EnWeiyer_Init(Actor* thisx, PlayState* play);
void EnWeiyer_Destroy(Actor* thisx, PlayState* play);
void EnWeiyer_Update(Actor* thisx, PlayState* play);
void EnWeiyer_Draw(Actor* thisx, PlayState* play);
s32 EnWeiyer_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg);

GLOBAL_ASM("data/z_En_Weiyer.data.s")

GLOBAL_ASM("../src/game/actors/En_Weiyer/data_syms.s")

extern CollisionCheckInfoInit EnWeiyer_sColChkInfoInit;
extern ColliderCylinderInit EnWeiyer_sCylinderInit;
extern DamageTable EnWeiyer_sDamageTable;
extern InitChainEntry EnWeiyer_sInitChain[];
#define sColChkInfoInit EnWeiyer_sColChkInfoInit
#define sCylinderInit EnWeiyer_sCylinderInit
#define sDamageTable EnWeiyer_sDamageTable
#define sInitChain EnWeiyer_sInitChain

ActorInit En_Weiyer_InitVars = {
    ACTOR_EN_WEIYER,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_EI,
    sizeof(EnWeiyer),
    (ActorFunc)EnWeiyer_Init,
    (ActorFunc)EnWeiyer_Destroy,
    (ActorFunc)EnWeiyer_Update,
    (ActorFunc)EnWeiyer_Draw,
};

void EnWeiyer_Init(Actor* thisx, PlayState* play) {
    EnWeiyer* self = (EnWeiyer*)thisx;

    Actor_ProcessInitChain(thisx, sInitChain);
    ActorShape_Init(&self->actor.shape, 1000.0f, ActorShadow_DrawCircle, 65.0f);
    FUN_00372f38(thisx, play, 0);
    FUN_00353c9c(thisx, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 14);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, (DamageTable*)((u8*)&sColChkInfoInit + 0x40), &sColChkInfoInit);
    self->actionFunc = (EnWeiyerActionFunc)FUN_0019fce0;
}

void EnWeiyer_Destroy(Actor* thisx, PlayState* play) {
    EnWeiyer* self = (EnWeiyer*)thisx;

    FUN_003508b8(thisx, NULL);
    SkelAnime_Free2(&self->skelAnime);
    Collider_DestroyCylinder(play, &self->collider);
}


void FUN_003d59b8(EnWeiyer* self, PlayState* play) {
    s32 alpha;

    FUN_003731e0(&self->skelAnime);
    alpha = self->actor.shape.shadowAlpha - 5;
    if (alpha < 0) {
        alpha = 0;
    }
    self->actor.shape.shadowAlpha = alpha;
    self->actor.world.pos.y -= 2.0f;

    if (self->timer != 0) {
        self->timer--;
        if (self->timer != 0) {
            return;
        }
    }

    Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos, 0xE0);
    Actor_Kill(&self->actor);
}

void FUN_0013f804(EnWeiyer* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    Math_ScaledStepToS(&self->actor.shape.rot.x, -0x4000, 0x400);
    self->actor.shape.rot.z += 0x1000;

    if (self->timer != 0) {
        self->timer--;
    }

    if ((self->timer == 0) || (self->actor.bgCheckFlags & 0x10)) {
        Animation_MorphToLoop(&self->skelAnime, 5, -5.0f);

        {
            s32 t;

            if (self->skelAnime.animLength > 0.0f) {
                t = (s32)(0.5f + self->skelAnime.animLength * 3.0f / 2.0f);
            } else {
                t = (s32)(self->skelAnime.animLength * 3.0f / 2.0f - 0.5f);
            }
            self->timer = t;
        }

        self->actor.speedXZ = 0.0f;
        self->actor.velocity.y = 0.0f;
        *(u32*)((u8*)&self->actor + 0x11C) |= 0x200000;
        self->actionFunc = FUN_003d59b8;
    }
}

void FUN_0019fce0(EnWeiyer* self, PlayState* play) {
    WaterBox* waterBox;
    s32 bgId;

    self->actor.world.pos.y += 0.5f;
    self->actor.floorHeight = BgCheck_EntityRaycastFloor4(&play->colCtx, &self->actor.floorPoly, &bgId, &self->actor, &self->actor.world.pos);

    if (!WaterBox_GetSurfaceImpl(play, &play->colCtx, self->actor.world.pos.x, self->actor.world.pos.z, &self->actor.home.pos.y, &waterBox) ||
        (self->actor.floorHeight >= (self->actor.home.pos.y - 5.0f))) {
        Actor_Kill(&self->actor);
    } else {
        self->actor.home.pos.y -= 5.0f;
        self->actor.world.pos.y = (self->actor.home.pos.y + self->actor.floorHeight) / 2.0f;
        FUN_00364394(self);
    }
}

void FUN_00126840(EnWeiyer* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);

    if (self->timer == 0) {
        if (Math_ScaledStepToS(&self->actor.shape.rot.x, -0x4000, 0x800)) {
            self->actor.shape.rot.y += 0x8000;
            self->actor.shape.rot.z = 0;
            self->timer = 1;
        } else {
            self->actor.shape.rot.z = self->actor.shape.rot.x * 2;
        }
    } else {
        Math_ScaledStepToS(&self->actor.shape.rot.x, 0x1800, 0x800);

        if (self->actor.world.pos.y < self->actor.home.pos.y) {
            if (self->actor.shape.rot.x > 0) {
                EffectSsGSplash_Spawn(play, &self->actor.world.pos, NULL, NULL, 1, 400);
                Audio_PlayActorSound2(&self->actor, 0x01000334);
            }

            FUN_001c8264(self);
        } else if (self->actor.bgCheckFlags & 1) {
            Animation_Change_00375c08(&self->skelAnime, 4, 2.0f, 0.0f, 0.0f, 0, -8.0f);
            self->timer = 60;
            self->collider.base.atFlags |= AT_ON;
            self->actionFunc = FUN_003dda10;
        }
    }
}

void FUN_00125798(EnWeiyer* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);

    if (self->timer != 0) {
        self->timer--;
    }

    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.05f, 0.2f, 0.0f);

    if (self->timer == 0) {
        Animation_MorphToLoop(&self->skelAnime, 2, -5.0f);
        self->actor.speedXZ = 5.0f;
        self->collider.base.atFlags |= AT_ON;
        self->timer = 0;
        self->actionFunc = FUN_00126840;
    }
}

void FUN_003461f0(EnWeiyer* self) {
    Animation_Change_00375c08(&self->skelAnime, 7, 1.0f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 7), 3, 0.0f);
    {
        s32 t = (self->skelAnime.animLength > 0.0f) ? (s32)(0.5f + self->skelAnime.animLength * 3.0f / 2.0f)
                                                    : (s32)(self->skelAnime.animLength * 3.0f / 2.0f - 0.5f);
        self->timer = t;
    }
    self->actor.shape.rot.x = 0;
    self->actor.shape.rot.z = 0;
    self->actor.speedXZ = -1.5f;
    self->actor.gravity = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actionFunc = FUN_00125798;
}

void FUN_0019b798(EnWeiyer* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);

    if (self->timer != 0) {
        self->timer--;
    }

    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.1f, 0.2f, 0.0f);

    if (self->timer == 0) {
        self->timer = 300;
        self->targetYaw = self->actor.yawTowardsPlayer + 0x8000;
        self->swimHeight = self->actor.world.pos.y;
        self->actor.speedXZ = CLAMP_MAX(self->actor.speedXZ, 4.0f);
        self->collider.base.atFlags &= ~AT_ON;
        SkelAnime_SetPlaySpeed(&self->skelAnime, 1.0f);
        self->actionFunc = FUN_001236dc;
    }
}

void FUN_0034613c(EnWeiyer* self) {
    Animation_Change_00375c08(&self->skelAnime, 6, 1.0f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 6), 3, 0.0f);

    {
        s32 t;

        if (self->skelAnime.animLength > 0.0f) {
            t = (s32)(0.5f + self->skelAnime.animLength * 1.5f);
        } else {
            t = (s32)(self->skelAnime.animLength * 1.5f - 0.5f);
        }
        self->timer = t;
    }
    self->actor.shape.rot.x = 0;
    self->actor.shape.rot.z = 0;
    self->actor.speedXZ = -1.5f;
    self->actor.gravity = 0.0f;
    self->actor.velocity.y = 0.5f;
    self->actionFunc = FUN_0019b798;
}

void FUN_003dda10(EnWeiyer* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);

    if (FUN_0036e5e0(&self->skelAnime, 0.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, 0x010003BE);
    }

    Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x800);
    Math_StepToF(&self->actor.speedXZ, 0.0f, 1.0f);

    if (self->timer != 0) {
        self->timer--;
    }

    if (self->timer == 0) {
        FUN_001cca7c(self);
    } else if (self->actor.home.pos.y > self->actor.world.pos.y) {
        FUN_00364394(self);
    }
}

void FUN_003e1608(EnWeiyer* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
    }

    Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x200);
    Math_ScaledStepToS(&self->actor.shape.rot.z, 0, 0x200);
    FUN_003731e0(&self->skelAnime);

    if (self->actor.floorHeight > self->actor.home.pos.y) {
        if (FUN_0036e5e0(&self->skelAnime, 0.0f, 1.0f)) {
            Audio_PlayActorSound2(&self->actor, 0x010003BE);
        }

        if (self->actor.bgCheckFlags & 2) {
            Audio_PlayActorSound2(&self->actor, 0x010002EB);
        }
    }

    if (self->timer == 0) {
        self->actor.gravity = 0.0f;
        self->actor.velocity.y = 0.0f;
        self->collider.dim.height = EnWeiyer_sCylinderInit.dim.height;
        FUN_00364394(self);
    }
}

void FUN_001236dc(EnWeiyer* self, PlayState* play) {
    f32 curFrame;

    FUN_003731e0(&self->skelAnime);
    Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x800);
    curFrame = *EnWeiyer_CurFramePtr(&self->skelAnime);
    Math_StepToF(&self->swimHeight, (self->actor.home.pos.y - self->actor.floorHeight) * 0.25f + self->actor.floorHeight, 1.0f);
    self->actor.world.pos.y = self->swimHeight - __hardfp_cosf((curFrame - 5.0f) * (M_PI / 40)) * 3.0f;

    if (curFrame <= 45.0f) {
        Math_StepToF(&self->actor.speedXZ, 1.0f, 0.03f);
    } else {
        Math_StepToF(&self->actor.speedXZ, 1.3f, 0.03f);
    }

    if (self->timer != 0) {
        self->timer--;
    }

    if (self->actor.bgCheckFlags & 8) {
        self->targetYaw = self->actor.wallYaw;
    }

    if (Math_SmoothStepToS(&self->actor.shape.rot.y, self->targetYaw, 2, 0x200, 0x80) == 0) {
        self->targetYaw = self->actor.yawTowardsPlayer + 0x8000;
    }

    if (self->actor.world.pos.y > self->actor.home.pos.y) {
        if (self->actor.floorHeight > self->actor.home.pos.y) {
            Animation_MorphToLoop(&self->skelAnime, 2, -5.0f);
            self->actor.speedXZ = 5.0f;
            self->collider.base.atFlags |= AT_ON;
            self->timer = 0;
            self->actionFunc = FUN_00126840;
        } else {
            self->actor.world.pos.y = self->actor.home.pos.y;
        }
    }

    if (self->timer == 0) {
        FUN_00364394(self);
    }
}

GLOBAL_ASM("asm/EnWeiyer_Update.s")

void FUN_0033dd8c(SkelAnime* skelAnime, s32 arg1, s32 arg2, s32 arg3, f32 f0, f32 f1, f32 f2, f32 f3);
void* FUN_003687a8(SkeletonAnimationModel* model);
void FUN_003589cc(void* p, s32 n);
void FUN_00358964(void* p, s32 n, void* color);

void EnWeiyer_Draw(Actor* thisx, PlayState* play) {
    EnWeiyer* self = (EnWeiyer*)thisx;

    if (self->actionFunc != FUN_003d59b8) {
        FUN_0033dd8c(&self->skelAnime, 0, 0, 0, 0.0f, 1.0f, 5.0f, 2.0f);
        SkelAnime_DrawOpa(&self->skelAnime, (MTX34*)&self->actor.modelMtx, EnWeiyer_OverrideLimbDraw, NULL, &self->actor, 0);
    } else {
        f32 color[4] = { 0.0f, 0.0f, 0.0f, (f32)self->actor.shape.shadowAlpha * (1.0f / 255.0f) };
        void* p;

        p = FUN_003687a8(*(SkeletonAnimationModel**)((u8*)self + 0x1CC));
        FUN_003589cc(p, 5);
        FUN_00358964(p, 5, color);
        SkelAnime_DrawOpa(&self->skelAnime, (MTX34*)&self->actor.modelMtx, EnWeiyer_OverrideLimbDraw, NULL, &self->actor, 0);
    }
}

// VFP numbering of the translate vector: declaration order of the f32 locals (y, x, z) sets the registers
// (ROM: zero in s0 for y, copy in s1 for x), assignment order sets the load order.
s32 EnWeiyer_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg) {
    if (limbIndex == 1) {
        f32 y, x, z;
        Vec3f pos;

        x = 0.0f;
        y = 0.0f;
        z = 2000.0f;
        pos.x = x;
        pos.y = y;
        pos.z = z;
        Matrix_UpdatePosition(mtx, mtx, &pos);
    }

    return 0;
}


}
