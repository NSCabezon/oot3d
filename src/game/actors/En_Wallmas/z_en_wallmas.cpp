extern "C" {
#include "z_en_wallmas.hpp"

#define FLAGS 0x00000035

void EnWallmas_Init(Actor* actor, PlayState* play);
void EnWallmas_Destroy(Actor* actor, PlayState* play);
void EnWallmas_Update(Actor* actor, PlayState* play);
void EnWallmas_Draw(Actor* actor, PlayState* play);
__attribute__((noinline)) void EnWallmas_TimerInit(EnWallmas* self, PlayState* play);
void EnWallmas_Stand(EnWallmas* self, PlayState* play);
void EnWallmas_Walk(EnWallmas* self, PlayState* play);
void EnWallmas_WaitToDrop(EnWallmas* self, PlayState* play);
void EnWallmas_ReturnToCeiling(EnWallmas* self, PlayState* play);
void Animation_PlayOnce(SkelAnime* skelAnime, s32 idx);
void Animation_PlayOnceSetSpeed(SkelAnime* skelAnime, s32 idx, f32 speed);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* pos, s32 params);
s32 Math_ScaledStepToS(s16* value, s16 target, s16 step);
void EnWallmas_JumpToCeiling(EnWallmas* self, PlayState* play);
f32* EnWallmas_CurFramePtr(SkelAnime* skelAnime);
static inline void EnWallmas_ProximityOrSwitchInit(EnWallmas* self);
void FUN_00353c9c(Actor* actor, PlayState* play, SkelAnime* skelAnime, s32 idx, s32 limbCount, void* jointTable,
                  void* morphTable, s32 dofCount);
void EnWallmas_WaitForProximity(EnWallmas* self, PlayState* play);
void EnWallmas_WaitForSwitchFlag(EnWallmas* self, PlayState* play);
void EnWallmas_TakePlayer(EnWallmas* self, PlayState* play);
void EnWallmas_Stun(EnWallmas* self, PlayState* play);
void EnWallmas_TakeDamage(EnWallmas* self, PlayState* play);
void EnWallmas_WaitToDrop(EnWallmas* self, PlayState* play);
void EnWallmas_Drop(EnWallmas* self, PlayState* play);
void FUN_0048961c(s32 sfxId);
void Audio_PlaySoundGeneral(u32 sfxId, Vec3f* pos, u8 a2, f32* a3, f32* a4, s8* a5);
void FUN_0036f59c(Actor* actor, s32 sfxId);
Vec3f* FUN_0036f57c(Player* player, s32 idx);
void EffectSsDeadDb_Spawn(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, s32 scale, s32 scaleStep, s32 r, s32 g,
                          s32 b, s32 a, s32 r2, s32 g2, s32 b2, s32 a2, s32 unk, s32 flag);
s32 Animation_OnFrameImpl(SkelAnime* skelAnime, f32 frame, f32 updateRate);
void Animation_MorphToPlayOnce(SkelAnime* skelAnime, s32 idx, f32 morph);
void Audio_PlayActorSound2(Actor* actor, s32 sfxId);
s32 Player_InCsMode(PlayState* play);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 a, s32 bufFlag, s32 duration);
void FUN_00330d5c(PlayState* play, Actor* actor, s32 csAction);
void OnePointCutscene_Init(PlayState* play, s32 csStart, s32 csEnd, Actor* actor, s32 camId);

ActorInit En_Wallmas_InitVars = {
    ACTOR_EN_WALLMAS,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_WALLMASTER,
    sizeof(EnWallmas),
    (ActorFunc)EnWallmas_Init,
    (ActorFunc)EnWallmas_Destroy,
    (ActorFunc)EnWallmas_Update,
    (ActorFunc)EnWallmas_Draw,
};

static CollisionCheckInfoInit sColChkInfoInit = { 4, 30, 40, 150 };

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT0,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 30, 40, 0, { 0, 0, 0 } },
};

#define WM_DMG(damage, reaction) ((damage) | ((reaction) << 4))

static DamageTable sDamageTable = { {
    WM_DMG(0, 1), WM_DMG(2, 0), WM_DMG(1, 0), WM_DMG(2, 0), WM_DMG(0, 1), WM_DMG(2, 0), WM_DMG(2, 0), WM_DMG(0, 1),
    WM_DMG(1, 0), WM_DMG(2, 0), WM_DMG(4, 0), WM_DMG(4, 2), WM_DMG(2, 0), WM_DMG(4, 4), WM_DMG(4, 0), WM_DMG(2, 0),
    WM_DMG(2, 0), WM_DMG(4, 2), WM_DMG(0, 0), WM_DMG(4, 4), WM_DMG(0, 0), WM_DMG(0, 0), WM_DMG(1, 0), WM_DMG(4, 0),
    WM_DMG(2, 0), WM_DMG(2, 0), WM_DMG(8, 0), WM_DMG(4, 0), WM_DMG(0, 0), WM_DMG(0, 0), WM_DMG(4, 0), WM_DMG(0, 0),
} };

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x30, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -1500, ICHAIN_STOP),
};

static inline void EnWallmas_TimerInitInline(EnWallmas* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    self->actor.flags &= ~0x1;
    self->actor.flags |= 0x20;
    self->actor.velocity.y = 0.0f;
    self->timer = 0xC3;
    self->actor.world.rot.y = player->actor.shape.rot.y;
    self->actor.world.pos.y = player->actor.world.pos.y;
    self->actor.floorHeight = player->actor.floorHeight;
    self->actor.draw = (ActorFunc)EnWallmas_Draw;
    self->actionFunc = EnWallmas_WaitToDrop;
}

void EnWallmas_Init(Actor* thisx, PlayState* play) {
    EnWallmas* self = (EnWallmas*)thisx;

    Actor_ProcessInitChain(thisx, sInitChain);
    ActorShape_Init(&thisx->shape, 0.0f, NULL, 0.5f);
    FUN_00372f38(thisx, play, &self->skelAnimModel, 2, 0);
    FUN_00353c9c(thisx, play, &self->skelAnime, 1, 10, (u8*)self + 0x228, (u8*)self + 0x500, 14);

    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, thisx, &sCylinderInit);
    CollisionCheck_SetInfo(&thisx->colChkInfo, &sDamageTable, &sColChkInfoInit);

    self->switchFlag = (thisx->params >> 8) & 0xFF;
    thisx->params = thisx->params & 0xFF;

    if (thisx->params == 2) {
        if (Flags_GetSwitch(play, self->switchFlag) != 0) {
            Actor_Kill(thisx);
            return;
        }
        EnWallmas_ProximityOrSwitchInit(self);
    } else if (thisx->params == 1) {
        EnWallmas_ProximityOrSwitchInit(self);
    } else {
        EnWallmas_TimerInitInline(self, play);
    }
}

void EnWallmas_Destroy(Actor* thisx, PlayState* play) {
    EnWallmas* self = (EnWallmas*)thisx;

    SkelAnime_Free2(&self->skelAnime);
    FUN_00350f34(&self->actor, &self->skelAnimModel, 0);
    Collider_DestroyCylinder(play, &self->collider);
}

#if 0 // draft: Update ~75w (collider ptr regs r7/sb/r8, bic r1 alias, timer strh [r0,r4]); needs Land/Die prototypes
s32 Actor_ApplyDamage(Actor* actor);
void FUN_00375b70(PlayState* play, Actor* actor);
void FUN_00375fd0(Actor* actor, void* elem, s32 flag);
void FUN_00376864(Actor* actor);
void FUN_00330254(PlayState* play, Actor* actor, Vec3f* pos, s32 a, s32 b);
void FUN_0036f00c(PlayState* play, Actor* actor, Vec3f* pos, f32 a, s32 b, f32 c, s32 d, s32 e, s32 f);
s16 Actor_WorldYawTowardActor(Actor* actor, Actor* other);

static inline void EnWallmas_SetupStun(EnWallmas* self) {
    Animation_Change_00375c08(&self->skelAnime, 2, 1.5f, 0.0f, 20.0f, 2, -3.0f);
    self->actor.speedXZ = 0.0f;
    if (self->actor.colChkInfo.damageEffect == 4) {
        Actor_SetColorFilter(&self->actor, 0x800000, 0xFF, 0, 0x50);
    } else {
        Actor_SetColorFilter(&self->actor, 0, 0xFF, 0, 0x50);
        Audio_PlayActorSound2(&self->actor, 0x0100030E);
    }
    self->timer = 0x78;
    self->actionFunc = EnWallmas_Stun;
}

static inline void EnWallmas_SetupTakeDamage(EnWallmas* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, 0, -3.0f);
    {
        Actor* thisx = &self->actor;
        if (self->collider.info.acHitInfo->toucher.dmgFlags & 0x0001F824) {
            thisx->world.rot.y = self->collider.base.ac->world.rot.y;
        } else {
            thisx->world.rot.y = Actor_WorldYawTowardActor(thisx, self->collider.base.ac) + 0x8000;
        }
    }
    Actor_SetColorFilter(&self->actor, 0x400000, 0xFF, 0, 0x14);
    self->actor.speedXZ = 5.0f;
    self->actor.velocity.y = 10.0f;
    self->actionFunc = EnWallmas_TakeDamage;
}

static inline void EnWallmas_SetupLand(EnWallmas* self, PlayState* play) {
    Animation_Change_00375c08(&self->skelAnime, 2, 1.0f, 41.0f, Animation_GetLastFrame(&self->skelAnime, 2), 2, -3.0f);
    FUN_0036f00c(play, &self->actor, &self->actor.world.pos, 15.0f, 6, 20.0f, 300, 100, 1);
    Audio_PlayActorSound2(&self->actor, 0x01000303);
    self->actionFunc = EnWallmas_Land;
}

static inline void EnWallmas_ColUpdate(EnWallmas* self, PlayState* play) {
    if (self->collider.base.acFlags & 2) {
        self->collider.base.acFlags &= ~2;
        FUN_00375fd0(&self->actor, &self->collider.info, 1);
        if (self->actor.colChkInfo.damageEffect != 0 || self->actor.colChkInfo.damage != 0) {
            if (Actor_ApplyDamage(&self->actor) == 0) {
                FUN_00375b70(play, &self->actor);
                Audio_PlayActorSound2(&self->actor, 0x01000306);
                self->actor.flags &= ~1;
            } else if (self->actor.colChkInfo.damage != 0) {
                Audio_PlayActorSound2(&self->actor, 0x01000305);
            }

            if (self->actor.colChkInfo.damageEffect == 4 || self->actor.colChkInfo.damageEffect == 1) {
                if (self->actionFunc != EnWallmas_Stun) {
                    EnWallmas_SetupStun(self);
                }
            } else {
                if (self->actor.colChkInfo.damageEffect == 2) {
                    FUN_00330254(play, &self->actor, &self->actor.world.pos, 40, 40);
                }
                EnWallmas_SetupTakeDamage(self);
            }
        }
    }
}

void EnWallmas_Update(Actor* thisx, PlayState* play2) {
    EnWallmas* self = (EnWallmas*)thisx;
    PlayState* play = play2;

    EnWallmas_ColUpdate(self, play);
    self->actionFunc(self, play);

    if (self->actionFunc == EnWallmas_WaitToDrop || self->actionFunc == EnWallmas_WaitForProximity ||
        self->actionFunc == EnWallmas_TakePlayer || self->actionFunc == EnWallmas_WaitForSwitchFlag) {
        return;
    }

    if (self->actionFunc != EnWallmas_ReturnToCeiling) {
        FUN_00376864(&self->actor);
    }

    if (self->actionFunc != EnWallmas_Drop) {
        Actor_UpdateBgCheckInfo(play, &self->actor, 20.0f, 25.0f, 0.0f, 0x1D);
    } else if (self->actor.world.pos.y <= self->yTarget) {
        self->actor.world.pos.y = self->yTarget;
        self->actor.velocity.y = 0.0f;
        EnWallmas_SetupLand(self, play);
    }

    if (self->actionFunc != EnWallmas_Die && self->actionFunc != EnWallmas_Drop) {
        CollisionCheckContext* colChkCtx;

        Collider_UpdateCylinder(&self->actor, &self->collider);
        colChkCtx = &play->colChkCtx;
        CollisionCheck_SetOC(play, colChkCtx, &self->collider.base);

        if (self->actionFunc != EnWallmas_TakeDamage && (self->actor.bgCheckFlags & 0x1) &&
            self->actor.freezeTimer == 0) {
            CollisionCheck_SetAC(play, colChkCtx, &self->collider.base);
        }
    }

    Actor_SetFocus(&self->actor, 25.0f);

    if (self->actionFunc == EnWallmas_TakeDamage || self->actionFunc == EnWallmas_Die) {
        return;
    }

    self->actor.shape.rot.y = self->actor.world.rot.y;
}
#endif
GLOBAL_ASM("asm/EnWallmas_Update.s")

#if 0 // draft: Draw far (~140w): scale-apply per-multiply copies + SetUnkAC family, see STATUS.md
s32 EnWallmas_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnWallmas_Die(EnWallmas* self, PlayState* play);
void EnWallmas_TakeDamage(EnWallmas* self, PlayState* play);
void FUN_003687b4(CollisionPoly* poly, MTX34* mtx, f32 x, f32 y, f32 z);
void Matrix_RotateY(MTX34* mtx, f32 angle, s32 mode);
void FUN_00358778(SkeletonAnimationModel* model, s32 a1, s32 a2, void* color, s32 a4);

typedef struct {
    f32 r, g, b, a, e;
} EnWallmasShadowColor;

static inline void MtxScaleApply(MTX34* m, f32 x, f32 y, f32 z) {
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

void EnWallmas_Draw(Actor* thisx, PlayState* play) {
    EnWallmas* self = (EnWallmas*)thisx;
    MTX34 mtx;
    Vec3f vec;
    EnWallmasShadowColor color;
    f32 xzScale;
    f32 s;

    if (self->actionFunc != EnWallmas_WaitToDrop) {
        FUN_00357fd0(GET_PLAYER(play), self->actor.unk_178, &self->actor.world.pos);
        SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnWallmas_OverrideLimbDraw, NULL, &self->actor, 0);
    }

    if (self->actor.floorPoly == NULL) {
        return;
    }
    if (self->timer > 0x78 && self->actionFunc != EnWallmas_Stun) {
        return;
    }

    FUN_003687b4(self->actor.floorPoly, &mtx, self->actor.world.pos.x, self->actor.floorHeight, self->actor.world.pos.z);
    vec.x = 0.0f;
    vec.y = 0.4f;
    vec.z = 0.0f;
    Matrix_UpdatePosition(&mtx, &mtx, &vec);

    if (self->actionFunc != EnWallmas_WaitToDrop && self->actionFunc != EnWallmas_ReturnToCeiling &&
        self->actionFunc != EnWallmas_TakePlayer && self->actionFunc != EnWallmas_WaitForSwitchFlag) {
        xzScale = self->actor.scale.x * 50.0f;
    } else {
        s32 t = 0x50 - (s32)(self->timer * 2.0f * (1.0f / 3.0f) + 0.5f);
        if (t > 0x50) {
            t = 0x50;
        }
        xzScale = t;
    }
    s = xzScale * 0.00625f;
    MtxScaleApply(&mtx, s * 1.3f, 1.0f, s * 1.3f);

    if (self->actionFunc == EnWallmas_TakeDamage || self->actionFunc == EnWallmas_Die) {
        Matrix_RotateY(&mtx, 0.2f + self->actor.shape.rot.y * 9.58738e-05f + 3.14159f, 1);
    } else {
        Matrix_RotateY(&mtx, 0.2f + self->actor.world.rot.y * 9.58738e-05f + 3.14159f, 1);
    }
    vec.x = 5.0f;
    vec.y = 0.0f;
    vec.z = -30.0f;
    Matrix_UpdatePosition(&mtx, &mtx, &vec);

    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&mtx);
    color.r = 0.0f;
    color.g = 0.0f;
    color.b = 0.0f;
    color.a = 0.0f;
    color.e = 1.0f - s * 1.5f;
    FUN_00358778(self->skelAnimModel, 0, 4, &color, 2);
    self->skelAnimModel->FUN_00372170(0);
}
#endif
GLOBAL_ASM("asm/EnWallmas_Draw.s")

static inline void EnWallmas_SetupReturnToCeiling(EnWallmas* self) {
    self->actor.speedXZ = 0.0f;
    self->timer = 0;
    Animation_Change_00375c08(&self->skelAnime, 2, 3.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 2), 2, -3.0f);
    self->actionFunc = EnWallmas_ReturnToCeiling;
}

void EnWallmas_Cooldown(EnWallmas* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime)) {
        EnWallmas_SetupReturnToCeiling(self);
    }
}

void EnWallmas_Die(EnWallmas* self, PlayState* play) {
    if (Math_StepToF(&self->actor.scale.x, 0.0f, 0.0015f) != 0) {
        Actor_SetScale(&self->actor, 0.01f);
        Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos, 0xC0);
        Actor_Kill(&self->actor);
    }
    self->actor.scale.z = self->actor.scale.x;
    self->actor.scale.y = self->actor.scale.x;
}

static inline void EnWallmas_SetupTakePlayer(EnWallmas* self, PlayState* play) {
    Animation_MorphToPlayOnce(&self->skelAnime, 9, -5.0f);
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->timer = -0x45;
    self->actionFunc = EnWallmas_TakePlayer;
    self->yTarget = self->actor.yDistToPlayer;
    FUN_00330d5c(play, &self->actor, 0x25);
    OnePointCutscene_Init(play, 9500, 9999, &self->actor, 0);
}

void EnWallmas_Drop(EnWallmas* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (!Player_InCsMode(play) && !(player->stateFlags2 & 0x10) && (*(s8*)((u8*)player + 0x2488) >= 0) &&
        (self->actor.xzDistToPlayer < 30.0f) && (self->actor.yDistToPlayer < -5.0f) &&
        (self->actor.yDistToPlayer > -(*(f32*)((u8*)player + 0x1354) + 10.0f))) {
        EnWallmas_SetupTakePlayer(self, play);
    }
}


GLOBAL_ASM("../split/asm/EnWallmas_JumpToCeiling.s")
GLOBAL_ASM("../split/asm/FUN_001998f4.s")

void EnWallmas_Land(EnWallmas* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime)) {
        Animation_PlayOnce(&self->skelAnime, 0xB);
        self->actionFunc = EnWallmas_Stand;
    }
}

s32 EnWallmas_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    return false;
}

static inline void EnWallmas_ProximityOrSwitchInit(EnWallmas* self) {
    self->timer = 0;
    self->actor.draw = NULL;
    self->actor.flags &= ~0x1;
    if (self->actor.params == 1) {
        self->actionFunc = EnWallmas_WaitForProximity;
    } else {
        self->actionFunc = EnWallmas_WaitForSwitchFlag;
    }
}

void EnWallmas_ReturnToCeiling(EnWallmas* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    FUN_003731e0(&self->skelAnime);
    if (*EnWallmas_CurFramePtr(&self->skelAnime) > 20.0f) {
        self->actor.world.pos.y += 30.0f;
        self->timer += 14;
    }

    if (Animation_OnFrameImpl(&self->skelAnime, 20.0f, 1.0f) != 0) {
        Audio_PlayActorSound2(&self->actor, 0x01000301);
    }

    if (self->actor.yDistToPlayer < -900.0f) {
        if (self->actor.params == 2) {
            Actor_Kill(&self->actor);
            return;
        }

        if (self->actor.params == 0 || Math_Vec3f_DistXZ(&self->actor.home.pos, &player->actor.world.pos) < 200.0f) {
            EnWallmas_TimerInit(self, play);
        } else {
            EnWallmas_ProximityOrSwitchInit(self);
        }
    }
}

void EnWallmas_Stand(EnWallmas* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime)) {
        Animation_PlayOnceSetSpeed(&self->skelAnime, 7, 3.0f);
        self->actor.speedXZ = 3.0f;
        self->actionFunc = EnWallmas_Walk;
    }

    Math_ScaledStepToS(&self->actor.world.rot.y, (s16)((s32)self->actor.yawTowardsPlayer + 0x8000), 0xB6);
}

// Stun/TakeDamage share a guarded function-local static (zeroVec guard 0x53009c / obj 0x59b7f0, pinned via pin_static)
static inline void EnWallmas_SetupDie(EnWallmas* self, PlayState* play) {
    static Vector3f zeroVec(0.0f, 0.0f, 0.0f);

    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = 0.0f;
    EffectSsDeadDb_Spawn(play, &self->actor.world.pos, (Vec3f*)&zeroVec, (Vec3f*)&zeroVec, 250, -10, 255, 255, 255, 255,
                         0, 0, 255, 1, 11, true);
    Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos, 0xC0);
    self->actionFunc = EnWallmas_Die;
}

void EnWallmas_Stun(EnWallmas* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (self->timer != 0) {
        self->timer--;
        if (self->timer != 0) {
            return;
        }
    }

    if (self->timer == 0) {
        if (self->actor.colChkInfo.health == 0) {
            EnWallmas_SetupDie(self, play);
        } else {
            self->actor.speedXZ = 0.0f;
            Animation_Change_00375c08(&self->skelAnime, 2, 3.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 2), 2, -3.0f);
            self->actionFunc = EnWallmas_ReturnToCeiling;
        }
    }
}

void EnWallmas_TakeDamage(EnWallmas* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime)) {
        if (self->actor.colChkInfo.health == 0) {
            EnWallmas_SetupDie(self, play);
        } else {
            Animation_PlayOnce(&self->skelAnime, 1);
            self->actor.world.rot.y = self->actor.shape.rot.y;
            self->actor.speedXZ = 0.0f;
            self->actor.velocity.y = 0.0f;
            self->actionFunc = EnWallmas_Cooldown;
        }
    }

    if (Animation_OnFrameImpl(&self->skelAnime, 13.0f, 1.0f) != 0) {
        Audio_PlayActorSound2(&self->actor, 0x010002EB);
    }

    Math_StepToF(&self->actor.speedXZ, 0.0f, 0.2f);
}

GLOBAL_ASM("../split/asm/FUN_003e1a70.s")

void EnWallmas_TakePlayer(EnWallmas* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (Animation_OnFrameImpl(&self->skelAnime, 1.0f, 1.0f) != 0) {
        if (!LINK_IS_ADULT) {
            FUN_0036f59c(&self->actor, 0x01000510);
        } else {
            FUN_0036f59c(&self->actor, 0x010004F0);
        }
        Audio_PlayActorSound2(&self->actor, 0x01000302);
    }

    if (FUN_003731e0(&self->skelAnime)) {
        player->actor.world.pos.x = self->actor.world.pos.x;
        player->actor.world.pos.z = self->actor.world.pos.z;

        if (self->timer < 0) {
            self->actor.world.pos.y += 2.0f;
        } else {
            self->actor.world.pos.y += 10.0f;
        }
        if (!LINK_IS_ADULT) {
            player->actor.world.pos.y = self->actor.world.pos.y - self->unk_840;
        } else {
            player->actor.world.pos.y = self->actor.world.pos.y - self->unk_840;
        }

        if (self->timer == -0x45) {
            if (!LINK_IS_ADULT) {
                FUN_0036f59c(&self->actor, 0x01000517);
            } else {
                FUN_0036f59c(&self->actor, 0x010004F7);
            }
        }
        if (self->timer == 0) {
            Audio_PlayActorSound2(&self->actor, 0x01000301);
        }

        self->timer += 3;
    } else {
        Player* p2 = GET_PLAYER(play);
        Vec3f v = *FUN_0036f57c(p2, 0xB);

        self->unk_840 = v.y - p2->actor.world.pos.y;
        Math_StepToF(&self->actor.world.pos.y, v.y + 10.0f, 20.0f);
    }

    Math_StepToF(&self->actor.world.pos.x, player->actor.world.pos.x, 3.0f);
    Math_StepToF(&self->actor.world.pos.z, player->actor.world.pos.z, 3.0f);

    if (self->timer == 0x45) {
        Audio_PlaySoundGeneral(0x010004E1, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        FUN_0036f4f0(play);
    }
}

void EnWallmas_TimerInit(EnWallmas* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    self->actor.flags &= ~0x1;
    self->actor.flags |= 0x20;
    self->actor.velocity.y = 0.0f;
    self->timer = 0xC3;
    self->actor.world.rot.y = player->actor.shape.rot.y;
    self->actor.world.pos.y = player->actor.world.pos.y;
    self->actor.floorHeight = player->actor.floorHeight;
    self->actor.draw = (ActorFunc)EnWallmas_Draw;
    self->actionFunc = EnWallmas_WaitToDrop;
}

void EnWallmas_WaitForProximity(EnWallmas* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (Math_Vec3f_DistXZ(&self->actor.home.pos, &player->actor.world.pos) < 200.0f) {
        EnWallmas_TimerInit(self, play);
    }
}

void EnWallmas_WaitForSwitchFlag(EnWallmas* self, PlayState* play) {
    if (Flags_GetSwitch(play, self->switchFlag) != 0) {
        EnWallmas_TimerInit(self, play);
        self->timer = 0x79;
    }
}

static inline void EnWallmas_SetupDrop(EnWallmas* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    Animation_Change_00375c08(&self->skelAnime, 4, 0.0f, 20.0f, Animation_GetLastFrame(&self->skelAnime, 4), 2, 0.0f);
    self->yTarget = player->actor.world.pos.y;
    self->actor.world.pos.y = player->actor.world.pos.y + 300.0f;
    self->actor.floorHeight = player->actor.floorHeight;
    self->actor.flags |= 0x1;
    self->actionFunc = EnWallmas_Drop;
}

void EnWallmas_WaitToDrop(EnWallmas* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    self->actor.world.pos = player->actor.world.pos;
    self->actor.floorHeight = player->actor.floorHeight;
    self->actor.floorPoly = player->actor.floorPoly;

    if (self->timer != 0) {
        self->timer--;
    }

    if ((player->stateFlags1 & 0x100000) || (player->stateFlags1 & 0x8000000) ||
        !(player->actor.bgCheckFlags & 0x1) ||
        ((self->actor.params == 1) && (320.0f < Math_Vec3f_DistXZ(&self->actor.home.pos, &player->actor.world.pos)))) {
        FUN_0048961c(0x01000300);
        self->timer = 0xC3;
    }

    if (self->timer == 0x78) {
        Audio_PlayActorSound2(&self->actor, 0x01000300);
    }

    if (self->timer == 0) {
        EnWallmas_SetupDrop(self, play);
    }
}

static inline void EnWallmas_SetupJumpToCeiling(EnWallmas* self) {
    Animation_PlayOnce(&self->skelAnime, 8);
    self->actor.speedXZ = 0.0f;
    self->actionFunc = EnWallmas_JumpToCeiling;
}

void EnWallmas_Walk(EnWallmas* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime)) {
        EnWallmas_SetupJumpToCeiling(self);
    }

    Math_ScaledStepToS(&self->actor.world.rot.y, (s16)((s32)self->actor.yawTowardsPlayer + 0x8000), 0xB6);

    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) || Animation_OnFrameImpl(&self->skelAnime, 12.0f, 1.0f) ||
        Animation_OnFrameImpl(&self->skelAnime, 24.0f, 1.0f) || Animation_OnFrameImpl(&self->skelAnime, 36.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, 0x01000304);
    }
}


}
