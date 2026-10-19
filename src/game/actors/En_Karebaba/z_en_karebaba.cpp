extern "C" {
#include "z_en_karebaba.hpp"

#define FLAGS 0x00000005

#define ACTOR_FLAG_UPDATE_CULLING_DISABLED 0x10

extern __pure float _fabsf(float);
static __inline __pure float fabsf(float f) {
    return _fabsf(f);
}

void EnKarebaba_Init(Actor* thisx, PlayState* play);
void EnKarebaba_Destroy(Actor* thisx, PlayState* play);
void EnKarebaba_Update(Actor* thisx, PlayState* play);
void EnKarebaba_Draw(Actor* actor, GameState* state);

void EnKarebaba_Grow(EnKarebaba* self, PlayState* play);
void EnKarebaba_Idle(EnKarebaba* self, PlayState* play);
void EnKarebaba_Awaken(EnKarebaba* self, PlayState* play);
void EnKarebaba_Spin(EnKarebaba* self, PlayState* play);
void EnKarebaba_Upright(EnKarebaba* self, PlayState* play);
void EnKarebaba_Regrow(EnKarebaba* self, PlayState* play);
void EnKarebaba_Dying(EnKarebaba* self, PlayState* play);
void EnKarebaba_DeadItemDrop(EnKarebaba* self, PlayState* play);
void EnKarebaba_Dead(EnKarebaba* self, PlayState* play);
__attribute__((noinline)) void EnKarebaba_SetupIdle(EnKarebaba* self);
static inline void EnKarebaba_SetupIdleInline(EnKarebaba* self);

extern ColliderCylinderInit EnKarebaba_sHeadColliderInit;
extern ColliderCylinderInit EnKarebaba_sBodyColliderInit;
extern CollisionCheckInfoInit EnKarebaba_sColCheckInfoInit;
extern InitChainEntry EnKarebaba_sInitChain;
DamageTable* DamageTable_Get(s32 index);

// 3DS engine helpers (ROM names; prototypes from the call sites)
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_001804d8(EnKarebaba* self);

GLOBAL_ASM("data/z_En_Karebaba.data.s")
GLOBAL_ASM("../src/game/actors/En_Karebaba/data_syms.s")

ActorInit En_Karebaba_InitVars = {
    ACTOR_EN_KAREBABA,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_DEKUBABA,
    sizeof(EnKarebaba),
    (ActorFunc)EnKarebaba_Init,
    (ActorFunc)EnKarebaba_Destroy,
    (ActorFunc)EnKarebaba_Update,
    (ActorFunc)EnKarebaba_Draw,
};

// NOT MATCHED (6 of 116 words): everything matches with the (EnKarebaba* self, PlayState* play) signature except the
// VFP numbering of the two hoisted consts: ROM 0.0f -> s16, 14.0f -> s17; ours swapped (~35 single-site variants:
// f32 locals in both decl orders, dead thisx/s2 aliases on every site, arm swap, SetupGrow helper).
#if 0
void EnKarebaba_Init(EnKarebaba* self, PlayState* play) {

    self->actor.unk_19A = 1;
    Actor_ProcessInitChain(&self->actor, &EnKarebaba_sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 22.0f);
    FUN_00372f38(&self->actor, play, &self->skelAnimModel0, 1, &self->skelAnimModel1, 2, &self->skelAnimModel2, 3,
                 &self->skelAnimModel3, 4, &self->skelAnimModel4, 5, 0);
    FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 3);
    Collider_InitCylinder(play, &self->bodyCollider);
    Collider_SetCylinder(play, &self->bodyCollider, &self->actor, &EnKarebaba_sBodyColliderInit);
    Collider_UpdateCylinder(&self->actor, &self->bodyCollider);
    Collider_InitCylinder(play, &self->headCollider);
    Collider_SetCylinder(play, &self->headCollider, &self->actor, &EnKarebaba_sHeadColliderInit);
    Collider_UpdateCylinder(&self->actor, &self->headCollider);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, DamageTable_Get(1), &EnKarebaba_sColCheckInfoInit);
    self->boundFloor = NULL;
    if (self->actor.params == 0) {
        Actor_SetScale(&self->actor, 0.0f);
        self->actor.shape.rot.x = -0x4000;
        self->actor.world.pos.y = self->actor.home.pos.y + 14.0f;
        self->actionFunc = EnKarebaba_Grow;
    } else {
        EnKarebaba_SetupIdleInline(self);
    }
}
#endif

GLOBAL_ASM("asm/EnKarebaba_Init.s")

void EnKarebaba_Destroy(Actor* thisx, PlayState* play) {
    EnKarebaba* self = (EnKarebaba*)thisx;

    Collider_DestroyCylinder(play, &self->bodyCollider);
    Collider_DestroyCylinder(play, &self->headCollider);
    SkelAnime_Free2(&self->skelAnime);
    FUN_00350f34(thisx, &self->skelAnimModel0, &self->skelAnimModel1, &self->skelAnimModel2, &self->skelAnimModel3,
                 &self->skelAnimModel4, 0);
}

#if 0
extern char EnKarebaba_sBlack[];
void FUN_0032d184(PlayState* play, Actor* actor, void* color, s32 a, s32 b); // func_80026230 (fog)
void FUN_0036e88c(MTX34* m, s16 x, s16 y, s16 z, s32 mode);
void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);
void EnKarebaba_ResetFog(PlayState* play, Actor* actor); // func_80026608 (empty stub)

static inline void EnKarebaba_ScaleMtx(MTX34* m, f32 sc) {
    f32* d = (f32*)m;
    d[0] = d[0] * sc;
    d[4] = d[4] * sc;
    d[8] = d[8] * sc;
    d[1] = d[1] * sc;
    d[5] = d[5] * sc;
    d[9] = d[9] * sc;
    d[2] = d[2] * sc;
    d[6] = d[6] * sc;
    d[10] = d[10] * sc;
}

void EnKarebaba_Draw(Actor* actor, GameState* state) {
    static Vector3f zeroVec(0.0f, 0.0f, 0.0f);
    EnKarebaba* self = (EnKarebaba*)actor;
    PlayState* play = (PlayState*)state;
    SkeletonAnimationModel* models[3];
    MTX34 mtx;
    f32 scale = 0.0f;
    s32 i;
    s32 numDLists;

    models[0] = (SkeletonAnimationModel*)self->skelAnimModel0;
    models[1] = (SkeletonAnimationModel*)self->skelAnimModel1;
    models[2] = (SkeletonAnimationModel*)self->skelAnimModel2;

    if (self->actionFunc == EnKarebaba_DeadItemDrop) {
        if (self->actor.params > 40 || (self->actor.params & 1)) {
            MTX34CopyAsm(&mtx, &self->actor.modelMtx);
            Vector3f t(0.0f, 0.0f, 200.0f);
            Matrix_UpdatePosition(&mtx, &mtx, (Vec3f*)&t);
            ((SkeletonAnimationModel*)self->skelAnimModel4)->SetUnkAC(1);
            ((SkeletonAnimationModel*)self->skelAnimModel4)->FUN_003721e0(&mtx);
            ((SkeletonAnimationModel*)self->skelAnimModel4)->FUN_00372170(0);
        }
    } else if (self->actionFunc != EnKarebaba_Dead) {
        FUN_0032d184(play, &self->actor, EnKarebaba_sBlack, 1, 2);
        SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, NULL, NULL, &self->actor, 0);
        Vector3f pos(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z);
        MTX34_MakeTrans(&mtx, (Vec3f*)&pos);
        if (self->actionFunc == EnKarebaba_Regrow || self->actionFunc == EnKarebaba_Grow) {
            scale = self->actor.params * 0.0005f;
        } else {
            scale = 0.01f;
        }
        EnKarebaba_ScaleMtx(&mtx, scale);
        FUN_0036e88c(&mtx, self->actor.shape.rot.x, self->actor.shape.rot.y, 0, 1);
        if (self->actionFunc == EnKarebaba_Dying) {
            numDLists = 2;
        } else {
            numDLists = 3;
        }
        for (i = 0; i < numDLists; i++) {
            Vector3f t(0.0f, 0.0f, -2000.0f);
            Matrix_UpdatePosition(&mtx, &mtx, (Vec3f*)&t);
            models[i]->SetUnkAC(1);
            models[i]->FUN_003721e0(&mtx);
            models[i]->FUN_00372170(0);
            if (i == 0 && self->actionFunc == EnKarebaba_Dying) {
                VEC3TransformAsm((Vector3f*)&self->actor.focus.pos, &mtx, &zeroVec);
            }
        }
        EnKarebaba_ResetFog(play, &self->actor);
    }

    FUN_0032d184(play, &self->actor, EnKarebaba_sBlack, 1, 2);
    Vector3f hpos(self->actor.home.pos.x, self->actor.home.pos.y, self->actor.home.pos.z);
    MTX34_MakeTrans(&mtx, (Vec3f*)&hpos);
    if (self->actionFunc != EnKarebaba_Grow) {
        scale = 0.01f;
    }
    EnKarebaba_ScaleMtx(&mtx, scale);
    Matrix_RotateY(&mtx, 1, (s16)self->actor.home.rot.y * 0.0000958738f);
    ((SkeletonAnimationModel*)self->skelAnimModel3)->SetUnkAC(1);
    ((SkeletonAnimationModel*)self->skelAnimModel3)->FUN_003721e0(&mtx);
    ((SkeletonAnimationModel*)self->skelAnimModel3)->FUN_00372170(0);
    if (self->actionFunc == EnKarebaba_Dying) {
        FUN_0036e88c(&mtx, -0x4000, (s16)(self->actor.shape.rot.y - self->actor.home.rot.y), 0, 1);
        ((SkeletonAnimationModel*)self->skelAnimModel2)->SetUnkAC(1);
        ((SkeletonAnimationModel*)self->skelAnimModel2)->FUN_003721e0(&mtx);
        ((SkeletonAnimationModel*)self->skelAnimModel2)->FUN_00372170(0);
    }
    EnKarebaba_ResetFog(play, &self->actor);
}
#endif

GLOBAL_ASM("asm/EnKarebaba_Draw.s")

// NOT MATCHED (6 of 112 words, size 448 = ROM): (1) flag clear: ROM `bic r1,r0,#2; mov r0,#0x1e; strb r1` vs ours
// `bic r0; strb r0; mov` (second-alias / local / pointer variants all identical); (2) literal pool order: ROM has
// EnKarebaba_Dying before 10.0f, ours 10.0f first (the block-level `f32 ten` is what makes armcc hoist 10.0f into s16;
// `ten` declared in the Dying branch loses the hoist); 3 words of (1) + pool swap (2). The boundFloor real branch
// needs the dead `s2` alias on the store (`s2->boundFloor = ...`).
#if 0
void EnKarebaba_Update(Actor* thisx, PlayState* play) {
    EnKarebaba* self = (EnKarebaba*)thisx;
    f32 height;

    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0 && (self->headCollider.base.atFlags & AT_HIT)) {
        EnKarebaba* s2 = self;
        s2->headCollider.base.atFlags &= ~AT_HIT;
        self->timer = 30;
    }

    self->actionFunc(self, play);

    if (self->actionFunc != EnKarebaba_Dead) {
        f32 ten = 10.0f;
        if (self->actionFunc == EnKarebaba_Dying) {
            Actor_MoveForward(&self->actor);
            Actor_UpdateBgCheckInfo(play, &self->actor, ten, 15.0f, 10.0f, 5);
        } else {
            Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);
            EnKarebaba* s2 = self;
            if (self->boundFloor == NULL) {
                s2->boundFloor = self->actor.floorPoly;
            }
        }
        if (self->actionFunc != EnKarebaba_Dying && self->actionFunc != EnKarebaba_DeadItemDrop) {
            if (self->actionFunc != EnKarebaba_Regrow && self->actionFunc != EnKarebaba_Grow) {
                if (self->timer == 0) {
                    CollisionCheck_SetAT(play, &play->colChkCtx, &self->headCollider.base);
                }
                CollisionCheck_SetAC(play, &play->colChkCtx, &self->bodyCollider.base);
            }
            CollisionCheck_SetOC(play, &play->colChkCtx, &self->headCollider.base);
            Actor_SetFocus(&self->actor, self->actor.scale.x * 1000.0f);
            height = self->actor.home.pos.y + 40.0f;
            self->actor.focus.pos.x = self->actor.home.pos.x;
            self->actor.focus.pos.y = CLAMP_MAX(self->actor.focus.pos.y, height);
            self->actor.focus.pos.z = self->actor.home.pos.z;
        }
    }
}
#endif

GLOBAL_ASM("asm/EnKarebaba_Update.s")

void EnKarebaba_Awaken(EnKarebaba* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    Math_StepToF(&self->actor.scale.x, 0.01f, 0.0005f);
    self->actor.scale.y = self->actor.scale.z = self->actor.scale.x;
    if (Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y + 60.0f, 5.0f)) {
        FUN_001804d8(self);
    }
    self->actor.shape.rot.y += 0x1999;
    EffectSsHahen_SpawnBurst(play, &self->actor.home.pos, 3.0f, 0, 12, 5, 1, -1, 10, NULL);
}


static inline void EnKarebaba_SetupRegrow(EnKarebaba* self) {
    self->actor.shape.yOffset = 0.0f;
    self->actor.shape.shadowScale = 22.0f;
    self->headCollider.dim.radius = EnKarebaba_sHeadColliderInit.dim.radius;
    Actor_SetScale(&self->actor, 0.0f);
    self->actionFunc = EnKarebaba_Regrow;
}

void EnKarebaba_Dead(EnKarebaba* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);

    if (self->actor.params != 0) {
        self->actor.params--;
    }
    if (self->actor.params == 0) {
        EnKarebaba_SetupRegrow(self);
    }
}

static inline void EnKarebaba_ResetCollider(EnKarebaba* self) {
    self->bodyCollider.dim.radius = 7.0f;
    self->bodyCollider.dim.height = 25.0f;
    self->bodyCollider.base.colType = 0xC;
    self->bodyCollider.base.acFlags |= AC_HARD;
    self->bodyCollider.info.bumper.dmgFlags = 0xFFCFFFFF;
    self->headCollider.dim.height = 25.0f;
}

static inline void EnKarebaba_SetupDead(EnKarebaba* self) {
    Animation_Change_00375c08(&self->skelAnime, 0, 0.0f, 0.0f, 0.0f, 2, 0.0f);
    EnKarebaba_ResetCollider(self);
    self->actor.shape.rot.x = -0x4000;
    self->actor.params = 200;
    self->actor.parent = NULL;
    self->actor.shape.shadowScale = 0.0f;
    Math_Vec3f_Copy(&self->actor.world.pos, &self->actor.home.pos);
    self->actionFunc = EnKarebaba_Dead;
}

void FUN_00346778(Actor* actor, PlayState* play, s32 getItemId);

void EnKarebaba_DeadItemDrop(EnKarebaba* self, PlayState* play) {
    if (self->actor.params != 0) {
        self->actor.params--;
    }
    if (Actor_HasParent(&self->actor, play) || self->actor.params == 0) {
        EnKarebaba_SetupDead(self);
    } else {
        FUN_00346778(&self->actor, play, 7);
    }
}

s32 Math_ScaledStepToS(s16* pValue, s16 target, s16 scale);
void FUN_00363ec4(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 duration);

void EnKarebaba_Dying(EnKarebaba* self, PlayState* play) {
    static Vector3f zeroVec(0.0f, 0.0f, 0.0f);
    s32 i;
    Vec3f position;
    Vec3f rotation;

    Math_StepToF(&self->actor.speedXZ, 0.0f, 0.1f);

    if (self->actor.params == 0) {
        Math_ScaledStepToS(&self->actor.shape.rot.x, 0x4800, 0x71C);
        EffectSsHahen_SpawnBurst(play, &self->actor.world.pos, 3.0f, 0, 12, 5, 1, -1, 10, NULL);

        if (self->actor.scale.x > 0.005f && ((self->actor.bgCheckFlags & 8) || (self->actor.bgCheckFlags & 2))) {
            self->actor.scale.x = self->actor.scale.y = self->actor.scale.z = 0.0f;
            self->actor.speedXZ = 0.0f;
            self->actor.flags &= ~5;
            EffectSsHahen_SpawnBurst(play, &self->actor.world.pos, 3.0f, 0, 12, 5, 15, -1, 10, NULL);
        }

        if (self->actor.bgCheckFlags & 2) {
            Audio_PlayActorSound2(&self->actor, 0x010002EB);
            self->actor.params = 1;
        }
    } else if (self->actor.params == 1) {
        Math_Vec3f_Copy(&position, &self->actor.world.pos);
        rotation.z = Math_SinS(self->actor.shape.rot.x) * 20.0f;
        rotation.x = -20.0f * Math_CosS(self->actor.shape.rot.x) * Math_SinS(self->actor.shape.rot.y);
        rotation.y = -20.0f * Math_CosS(self->actor.shape.rot.x) * Math_CosS(self->actor.shape.rot.y);

        for (i = 0; i < 4; i++) {
            FUN_00363ec4(play, &position, (Vec3f*)&zeroVec, (Vec3f*)&zeroVec, 500, 50);
            position.x += rotation.x;
            position.y += rotation.z;
            position.z += rotation.y;
        }

        FUN_00363ec4(play, &self->actor.home.pos, (Vec3f*)&zeroVec, (Vec3f*)&zeroVec, 500, 100);
        Actor_SetScale(&self->actor, 0.03f);
        self->actor.shape.yOffset = 1000.0f;
        self->actor.shape.shadowScale = 3.0f;
        self->actor.shape.rot.x -= 0x4000;
        self->actor.gravity = 0.0f;
        self->actor.velocity.y = 0.0f;
        Actor_ChangeType(play, &play->actorCtx, &self->actor, 8);
        self->actor.params = 200;
        self->actor.flags &= ~0x20;
        self->actionFunc = EnKarebaba_DeadItemDrop;
    }
}

void EnKarebaba_Grow(EnKarebaba* self, PlayState* play) {
    f32 scale;

    self->actor.params++;
    scale = self->actor.params * 0.05f;
    Actor_SetScale(&self->actor, 0.005f * scale);
    self->actor.world.pos.y = self->actor.home.pos.y + (14.0f * scale);
    if (self->actor.params == 20) {
        EnKarebaba_SetupIdleInline(self);
    }
}

static inline void EnKarebaba_SetupAwaken(EnKarebaba* self) {
    Animation_Change_00375c08(&self->skelAnime, 0, 4.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 0), 0, -3.0f);
    Audio_PlayActorSound2(&self->actor, 0x0100036F);
    self->actionFunc = EnKarebaba_Awaken;
}

void EnKarebaba_Idle(EnKarebaba* self, PlayState* play) {
    if (self->actor.xzDistToPlayer < 200.0f && fabsf(self->actor.yDistToPlayer) < 30.0f) {
        EnKarebaba_SetupAwaken(self);
    }
}

void EnKarebaba_Regrow(EnKarebaba* self, PlayState* play) {
    f32 scale;

    self->actor.params++;
    scale = self->actor.params * 0.05f;
    Actor_SetScale(&self->actor, 0.005f * scale);
    self->actor.world.pos.y = self->actor.home.pos.y + (14.0f * scale);

    if (self->actor.params == 20) {
        self->actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        self->actor.flags |= 0x5; // ATTENTION_ENABLED | HOSTILE
        Actor_ChangeType(play, &play->actorCtx, &self->actor, ACTORCAT_ENEMY);
        EnKarebaba_SetupIdle(self);
    }
}

void EnKarebaba_Retract(EnKarebaba* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    Math_StepToF(&self->actor.scale.x, 0.005f, 0.0005f);
    self->actor.scale.y = self->actor.scale.z = self->actor.scale.x;

    if (Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y + 14.0f, 5.0f)) {
        EnKarebaba_SetupIdle(self);
    }

    self->actor.shape.rot.y += 0x1999;
    EffectSsHahen_SpawnBurst(play, &self->actor.home.pos, 3.0f, 0, 12, 5, 1, -1, 10, NULL);
}

static inline void EnKarebaba_SetupIdleInline(EnKarebaba* self) {
    Actor_SetScale(&self->actor, 0.005f);
    self->actor.shape.rot.x = -0x4000;
    self->actor.world.pos.y = self->actor.home.pos.y + 14.0f;
    self->actionFunc = EnKarebaba_Idle;
}

void EnKarebaba_SetupIdle(EnKarebaba* self) {
    EnKarebaba_SetupIdleInline(self);
}

void FUN_001804d8(EnKarebaba* self) {
    if (self->actionFunc != EnKarebaba_Spin) {
        Actor_SetScale(&self->actor, 0.01f);
        self->bodyCollider.base.colType = COLTYPE_HIT6;
        self->bodyCollider.base.acFlags &= ~AC_HARD;
        // adult: (DMG_SWORD | DMG_BOOMERANG); child: same & ~DMG_JUMP_MASTER
        self->bodyCollider.info.bumper.dmgFlags = !LINK_IS_ADULT ? 0x07C00710 : 0x0FC00710;
        self->bodyCollider.dim.radius = 15.0f;
        self->bodyCollider.dim.height = 80.0f;
        self->headCollider.dim.height = 80.0f;
    }

    self->actor.params = 40;
    self->actionFunc = EnKarebaba_Upright;
}

void EnKarebaba_Dying(EnKarebaba* self, PlayState* play);
void FUN_00375b70(PlayState* play, Actor* actor);

static inline void EnKarebaba_SetupDying(EnKarebaba* self) {
    self->actor.params = 0;
    self->actor.gravity = -0.8f;
    self->actor.velocity.y = 4.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y + 0x8000;
    self->actor.speedXZ = 3.0f;
    Audio_PlayActorSound2(&self->actor, 0x010002D2);
    self->actor.flags |= 0x30;
    self->actionFunc = EnKarebaba_Dying;
}

void EnKarebaba_Spin(EnKarebaba* self, PlayState* play) {
    s32 value;
    f32 cos60;

    if (self->actor.params != 0) {
        self->actor.params--;
    }
    FUN_003731e0(&self->skelAnime);
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) || Animation_OnFrameImpl(&self->skelAnime, 12.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, 0x010002D0);
    }
    value = 20 - self->actor.params;
    value = 20 - (value < 0 ? -value : value);
    if (value > 10) {
        value = 10;
    }
    self->headCollider.dim.radius = EnKarebaba_sHeadColliderInit.dim.radius + (value * 2);
    self->actor.shape.rot.x = 0xC000 - (value * 0x100);
    self->actor.shape.rot.y += value * 0x2C0;
    self->actor.world.pos.y = (Math_SinS(self->actor.shape.rot.x) * -60.0f) + self->actor.home.pos.y;
    cos60 = Math_CosS(self->actor.shape.rot.x) * 60.0f;
    self->actor.world.pos.x = (cos60 * Math_SinS(self->actor.shape.rot.y)) + self->actor.home.pos.x;
    self->actor.world.pos.z = (cos60 * Math_CosS(self->actor.shape.rot.y)) + self->actor.home.pos.z;
    if (self->bodyCollider.base.acFlags & AC_HIT) {
        EnKarebaba_SetupDying(self);
        FUN_00375b70(play, &self->actor);
    } else if (self->actor.params == 0) {
        FUN_001804d8(self);
    }
}

static inline void EnKarebaba_SetupRetract(EnKarebaba* self) {
    Animation_Change_00375c08(&self->skelAnime, 0, -3.0f, Animation_GetLastFrame(&self->skelAnime, 0), 0.0f, 2, -3.0f);
    EnKarebaba_ResetCollider(self);
    self->actionFunc = EnKarebaba_Retract;
}

static inline void EnKarebaba_SetupSpin(EnKarebaba* self) {
    self->actor.params = 40;
    self->actionFunc = EnKarebaba_Spin;
}

void EnKarebaba_Upright(EnKarebaba* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    FUN_003731e0(&self->skelAnime);
    if (self->actor.params != 0) {
        self->actor.params--;
    }
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) || Animation_OnFrameImpl(&self->skelAnime, 12.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, 0x010002D0);
    }
    if (self->bodyCollider.base.acFlags & AC_HIT) {
        EnKarebaba_SetupDying(self);
        FUN_00375b70(play, &self->actor);
    } else if (Math_Vec3f_DistXZ(&self->actor.home.pos, &player->actor.world.pos) > 240.0f) {
        EnKarebaba_SetupRetract(self);
    } else if (self->actor.params == 0) {
        EnKarebaba_SetupSpin(self);
    }
}

}
