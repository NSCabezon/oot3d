extern "C" {
#include "z_en_am.hpp"

#define FLAGS 0x84000015

#define ANIMMODE_LOOP 0
#define BGCHECKFLAG_GROUND (1 << 0)
#define BGCHECKFLAG_GROUND_TOUCH (1 << 1)
#define BGCHECKFLAG_WALL (1 << 3)

typedef enum ArmosBehavior {
    /*  0 */ AM_BEHAVIOR_NONE,
    /*  1 */ AM_BEHAVIOR_DAMAGED,
    /*  3 */ AM_BEHAVIOR_DO_NOTHING = 3,
    /*  5 */ AM_BEHAVIOR_5 = 5,
    /*  6 */ AM_BEHAVIOR_STUNNED,
    /*  7 */ AM_BEHAVIOR_GO_HOME,
    /*  8 */ AM_BEHAVIOR_RICOCHET,
    /* 10 */ AM_BEHAVIOR_AGGRO = 10
} ArmosBehavior;

void EnAm_Init(Actor* thisx, PlayState* play);
void EnAm_Destroy(Actor* thisx, PlayState* play);
void EnAm_Update(Actor* thisx, PlayState* play);
void EnAm_Draw(Actor* thisx, PlayState* play);

GLOBAL_ASM("data/z_En_Am.data.s")

GLOBAL_ASM("../src/game/actors/En_Am/data_syms.s")

ActorInit En_Am_InitVars = {
    ACTOR_EN_AM,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_AM,
    sizeof(EnAm),
    (ActorFunc)EnAm_Init,
    (ActorFunc)EnAm_Destroy,
    (ActorFunc)EnAm_Update,
    (ActorFunc)EnAm_Draw,
};

void FUN_003e8f48(EnAm* self, PlayState* play);
void FUN_003e3020(EnAm* self, PlayState* play);
void Animation_PlayLoopSetSpeed(SkelAnime* skelAnime, s32 idx, f32 speed);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);

void FUN_00180724(EnAm* self) {
    f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 1);

    Animation_Change_00375c08(&self->skelAnime, 1, 0.0f, lastFrame, lastFrame, ANIMMODE_LOOP, 0.0f);
    self->dyna.actor.speedXZ = 0.0f;
    self->behavior = AM_BEHAVIOR_DO_NOTHING;
    if (self->textureBlend == 255) {
        self->unk_258 = 0;
    } else {
        self->unk_258 = 1;
    }
    self->actionFunc = FUN_003e3020;
}

void FUN_001814b8(EnAm* self) {
    Animation_PlayLoopSetSpeed(&self->skelAnime, 0, 4.0f);
    self->dyna.actor.speedXZ = 0.0f;
    self->unk_258 = 3;
    self->behavior = AM_BEHAVIOR_AGGRO;
    self->dyna.actor.world.rot.y = self->dyna.actor.shape.rot.y;
    self->actionFunc = FUN_003e8f48;
}

s32 FUN_00363108(Actor* thisx, PlayState* play, f32 distance, s16 yaw); // EnAm_CanMove, own TU z_en_am_canmove.cpp

void FUN_003d4e70(EnAm* self, PlayState* play) {
    if (self->dyna.actor.speedXZ < 0.0f) {
        self->dyna.actor.speedXZ += 0.5f;
    }

    if ((self->dyna.actor.velocity.y <= 0.0f) &&
        !FUN_00363108(&self->dyna.actor, play, -8.0f, self->dyna.actor.world.rot.y)) {
        self->dyna.actor.speedXZ = 0.0f;
    }

    if (FUN_003731e0(&self->skelAnime)) {
        Animation_PlayLoopSetSpeed(&self->skelAnime, 0, 4.0f);
        self->dyna.actor.speedXZ = 0.0f;
        self->unk_258 = 3;
        self->behavior = AM_BEHAVIOR_AGGRO;
        self->dyna.actor.world.rot.y = self->dyna.actor.shape.rot.y;
        self->actionFunc = FUN_003e8f48;
        self->deathTimer = 96;
        self->panicSpinRot = 0;
    }
}

void FUN_00375b70(PlayState* play, Actor* actor); // Enemy_StartFinishingBlow

void FUN_00296a7c(EnAm* self, PlayState* play) {
    Math_SmoothStepToS(&self->dyna.actor.shape.rot.y, self->dyna.actor.world.rot.y, 1, 0xFA0, 0);

    if (self->dyna.actor.speedXZ < 0.0f) {
        self->dyna.actor.speedXZ += 0.5f;
    }

    if ((self->dyna.actor.velocity.y <= 0.0f) &&
        !FUN_00363108(&self->dyna.actor, play, -9.0f, self->dyna.actor.world.rot.y)) {
        self->dyna.actor.speedXZ = 0.0f;
    }

    u8* cfBase = (u8*)self + 0x100;

    if (*(u16*)(cfBase + 0x1A) == 0) {
        if (self->dyna.actor.colChkInfo.health != 0) {
            FUN_001814b8(self);
        } else {
            Animation_Change_00375c08(&self->skelAnime, 2, 1.0f, 4.0f,
                                      Animation_GetLastFrame(&self->skelAnime, 2) - 6.0f, 2, 0.0f);
            self->behavior = AM_BEHAVIOR_DAMAGED;
            self->dyna.actor.world.rot.y = self->dyna.actor.yawTowardsPlayer;
            Audio_PlayActorSound2(&self->dyna.actor, 0x010002B7);

            if (FUN_00363108(&self->dyna.actor, play, -6.0f, self->dyna.actor.world.rot.y)) {
                self->dyna.actor.speedXZ = -6.0f;
            }

            *(u16*)(cfBase + 0x1A) = 0;
            FUN_00375b70(play, &self->dyna.actor);
            self->actionFunc = FUN_003d4e70;
        }
    }
}

#if 0 // FUN_00185a60 draft (EnAm_Cooldown): 60/106 words off. Same inlined-SmoothStepToS shape as FUN_001e88ec (scale local keeps idiv); here ours inlines FUN_001814b8 (ROM bl) and hoists 0.0f into s16 (ROM vldr s0 per use); GLOBAL_ASM kept
void FUN_0035a534(EnAm* self, PlayState* play, s32 flag);
void FUN_001814b8(EnAm* self);
static inline s16 SmoothStepToS_(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep) {
    s16 stepSize = 0;
    s16 diff = target - *pValue;

    if (*pValue != target) {
        stepSize = diff / scale;

        if ((stepSize > minStep) || (stepSize < -minStep)) {
            if (stepSize > step) {
                stepSize = step;
            }

            if (stepSize < -step) {
                stepSize = -step;
            }

            *pValue += stepSize;
        } else {
            if (diff >= 0) {
                *pValue += minStep;

                if ((s16)(target - *pValue) <= 0) {
                    *pValue = target;
                }
            } else {
                *pValue -= minStep;

                if ((s16)(target - *pValue) >= 0) {
                    *pValue = target;
                }
            }
        }
    }

    return diff;
}

void FUN_00185a60(EnAm* self, PlayState* play) {
    s16 yawDiff = self->dyna.actor.yawTowardsPlayer - self->dyna.actor.world.rot.y;
    s16 scale = 1;

    yawDiff = ABS(yawDiff);

    if (self->cooldownTimer != 0) {
        self->cooldownTimer--;
    } else {
        f32 frame = self->skelAnime.curFrame;

        if (frame > 5.2333336f) {
            if (frame < 5.4333333f) {
                SmoothStepToS_(&self->dyna.actor.world.rot.y, self->dyna.actor.yawTowardsPlayer, scale, 0x1F40, 0);
                self->dyna.actor.velocity.y = 12.0f;
                goto done;
            }
        }
        if (frame > 10.6666667f) {
            if (!(self->dyna.actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
                self->skelAnime.curFrame = 10.6666667f;
            } else {
                if (yawDiff < 3500) {
                    self->unk_258 = 0;
                }
                self->dyna.actor.velocity.y = 0.0f;
                self->dyna.actor.world.pos.y = self->dyna.actor.floorHeight;
                if (self->unk_462 != 0) {
                    FUN_0035a534(self, play, 0);
                } else {
                    u16 t;
                    FUN_0035a534(self, play, 1);
                    if (self->unk_460 == 0) {
                        self->unk_460 = 1;
                        t = 2;
                    } else {
                        self->unk_460 = 0;
                        t = 1;
                    }
                    self->unk_462 = t;
                }
            }
        }
done:
        FUN_003731e0(&self->skelAnime);

        if (self->unk_258 == 0) {
            FUN_001814b8(self);
            Audio_PlayActorSound2(&self->dyna.actor, 0x010002B8);
        }

        self->dyna.actor.shape.rot.y = self->dyna.actor.world.rot.y;
    }
}
#endif

#if 0 // FUN_0035a534 draft (EnAm_SpawnEffects-like, guarded statics velocity 0x598E58 guard 0x5203E8 / accel 0x598E64 guard 0x5203E4): 3/112 words off (accel ctor store order: ROM vmov s1,s2; vstr s1[x]; ours vstr s2[x] first), GLOBAL_ASM kept
void FUN_0036f00c(PlayState* play, Actor* actor, Vec3f* pos, f32 a, s32 b, f32 c, s32 d, s32 e, s32 f);
void EffectSsKiraKira_SpawnSmall(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor,
                                 Color_RGBA8* envColor);
struct VA { f32 x; f32 y; f32 z; VA(f32 x_, f32 y_, f32 z_) { x = x_; y = y_; z = z_; } };
struct AA { f32 x; f32 y; f32 z; AA(f32 y_, f32 z_, f32 x_) { x = x_; y = y_; z = z_; } };
void FUN_0035a534(EnAm* self, PlayState* play, s32 flag) {
    static VA velocity(0.0f, -1.5f, 0.0f);
    static AA accel(-0.2f, 0.0f, 0.0f);
    Vec3f pos;
    Color_RGBA8 primColor = { 100, 100, 100, 0 };
    Color_RGBA8 envColor = { 40, 40, 40, 0 };
    s32 i;
    PlayState* play2 = play;

    for (i = 4; i != 0; i--) {
        pos.x = self->dyna.actor.world.pos.x + ((Rand_ZeroOne() - 0.5f) * 65.0f);
        pos.y = (40.0f + ((Rand_ZeroOne() - 0.5f) * 10.0f)) + self->dyna.actor.world.pos.y;
        pos.z = self->dyna.actor.world.pos.z + ((Rand_ZeroOne() - 0.5f) * 65.0f);

        EffectSsKiraKira_SpawnSmall(play2, &pos, (Vec3f*)&velocity, (Vec3f*)&accel, &primColor, &envColor);
    }

    if (flag != 0) {
        Audio_PlayActorSound2(&self->dyna.actor, 0x010002B4);
    }

    FUN_0036f00c(play, &self->dyna.actor, &self->dyna.actor.world.pos, 4.0f, 3, 8.0f, 300, 15, 0);
}
#endif

void FUN_0013e138(EnAm* self, PlayState* play);
void FUN_0035a534(EnAm* self, PlayState* play, s32 flag); // EnAm_SpawnEffects-like (asm, functions_350000s.cpp)

#if 0 // FUN_001e88ec draft (EnAm_RotateToHome): 84/117 words off. Structure matches ROM (inlined Math_SmoothStepToS without 3DS time scaling, minStep 0; `s16 scale = 1` local keeps the idiv call FUN_00368d94); open: ROM hoists mov r8,#0 / mov sb,#1 to entry and rematerialises mov r1,#1 for the idiv arg, ldr r0,=-0x1f40 before cmn/movlt. GLOBAL_ASM kept
static inline s16 SmoothStepToS_(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep) {
    s16 stepSize = 0;
    s16 diff = target - *pValue;

    if (*pValue != target) {
        stepSize = diff / scale;

        if ((stepSize > minStep) || (stepSize < -minStep)) {
            if (stepSize > step) {
                stepSize = step;
            }

            if (stepSize < -step) {
                stepSize = -step;
            }

            *pValue += stepSize;
        } else {
            if (diff >= 0) {
                *pValue += minStep;

                if ((s16)(target - *pValue) <= 0) {
                    *pValue = target;
                }
            } else {
                *pValue -= minStep;

                if ((s16)(target - *pValue) >= 0) {
                    *pValue = target;
                }
            }
        }
    }

    return diff;
}

void FUN_001e88ec(EnAm* self, PlayState* play) {
    s16 yawToHome = Math_Vec3f_Yaw(&self->dyna.actor.world.pos, &self->homePos);
    f32 frame = self->skelAnime.curFrame;
    s16 scale = 1;

    if (frame > 5.2333336f) {
        if (frame < 5.4333333f) {
            SmoothStepToS_(&self->dyna.actor.world.rot.y, yawToHome, scale, 0x1F40, 0);
            self->dyna.actor.velocity.y = 12.0f;
            goto done;
        }
    }
    if (frame > 10.6666667f) {
        if (!(self->dyna.actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
            self->skelAnime.curFrame = 10.6666667f;
        } else {
            if (self->unk_462 != 0) {
                FUN_0035a534(self, play, 0);
            } else {
                u16 t;
                FUN_0035a534(self, play, 1);
                if (self->unk_460 == 0) {
                    self->unk_460 = 1;
                    t = 2;
                } else {
                    self->unk_460 = 0;
                    t = 1;
                }
                self->unk_462 = t;
            }
            if (self->dyna.actor.world.rot.y == yawToHome) {
                self->unk_258 = 0;
            }
            self->dyna.actor.velocity.y = 0.0f;
            self->dyna.actor.world.pos.y = self->dyna.actor.floorHeight;
        }
    }
done:
    FUN_003731e0(&self->skelAnime);

    if (self->unk_258 == 0) {
        Animation_PlayLoopSetSpeed(&self->skelAnime, 0, 4.0f);
        self->behavior = AM_BEHAVIOR_GO_HOME;
        self->dyna.actor.speedXZ = 0.0f;
        self->unk_258 = 1;
        self->actionFunc = FUN_0013e138;
    }

    self->dyna.actor.shape.rot.y = self->dyna.actor.world.rot.y;
}
#endif

#if 0 // FUN_00342af8 draft (shield helper): 6/58 words off (prologue scheduling: ROM mov r4,r2; ldr r2,[sp,#0x18]; tst; ours tst first + ldr ip), GLOBAL_ASM kept in functions_340000s.cpp
void CollisionCheck_SpawnShieldParticles(PlayState* play, Vec3f* pos, s32 a);
void FUN_0034e568(PlayState* play, Vec3f* pos, s32 a);
void FUN_003661a8(PlayState* play, Vec3f* pos, s32 a);
extern f32 FLOAT_0054ac20;
s32 FUN_00342af8(PlayState* play, Vec3f* hitPos, Vec3f* pos, u32 flags, s32 type) {
    Vec3f* p = pos;

    if (!(flags & 0x18)) {
        u32 sfx = 0x010000FA;

        switch (type) {
            case 0:
                CollisionCheck_SpawnShieldParticles(play, hitPos, 8);
                Audio_PlaySoundGeneral(sfx, p, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                break;
            case 1:
                FUN_0034e568(play, hitPos, 8);
                Audio_PlaySoundGeneral(sfx, p, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                break;
            case 2:
                FUN_003661a8(play, hitPos, 8);
                Audio_PlaySoundGeneral(sfx, p, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                break;
        }
        return 1;
    }
    return 0;
}
#endif

void FUN_00125f98(EnAm* self, PlayState* play) {
    if (self->dyna.actor.speedXZ < 0.0f) {
        self->dyna.actor.speedXZ += 0.5f;
    }

    if ((self->dyna.actor.velocity.y <= 0.0f) &&
        !FUN_00363108(&self->dyna.actor, play, self->dyna.actor.speedXZ * 1.5f, self->dyna.actor.world.rot.y)) {
        self->dyna.actor.speedXZ = 0.0f;
    }

    if (FUN_003731e0(&self->skelAnime)) {
        self->dyna.actor.speedXZ = 0.0f;
        FUN_001814b8(self);
    }
}

// EnAm_Statue-like slide action (3DS splits N64 unk_258 into angle 0x450 + counter 0x452)
s32 FUN_0036d288(PlayState* play, DynaPolyActor* dyna, s16 arg2, s16 arg3, s32 arg4);

void FUN_0010b3b4(EnAm* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 temp158f = self->dyna.unk_1B0;
    s16 moveDir = 0;

    if (self->unk_258 == 0) {
        if (self->dyna.unk_1A8 != 0.0f) {
            Audio_PlayActorSound2(&self->dyna.actor, 0x01000161);
            self->unk_450 = 0x8000;
            self->unk_258 = 0x18;
        }
    } else {
        self->unk_450 -= 0x555;
        self->unk_258--;
        Audio_PlayActorSound2(&self->dyna.actor, 0x01000161);

        if (self->dyna.unk_1A8 < 0.0f) {
            temp158f = self->dyna.unk_1B0 + 0x8000;
        }

        if (self->hurtCollider.base.ocFlags1 & 2) {
            moveDir = Math_Vec3f_Yaw(&self->dyna.actor.world.pos, &self->hurtCollider.base.oc->world.pos) - temp158f;
        }

        if ((self->dyna.unk_1A8 == 0.0f) || (self->unk_258 == 0) || !(self->dyna.actor.bgCheckFlags & 1) ||
            !FUN_0036d288(play, &self->dyna, 0x14, 40.0f + self->dyna.unk_1A8 * (Math_SinS(self->unk_450) * 0.5f), 10) ||
            ((self->hurtCollider.base.ocFlags1 & 2) && (ABS(moveDir) <= 0x2000))) {
            self->unk_258 = 0;
            player->actor.speedXZ = 0.0f;
            *(u32*)((u8*)player + 0x1714) &= ~0x151;
            self->dyna.unk_1A8 = self->dyna.unk_1AC = 0.0f;
        }

        self->dyna.actor.world.rot.y = self->dyna.unk_1B0;
        self->dyna.actor.speedXZ = Math_SinS(self->unk_450) * 0.5f * self->dyna.unk_1A8;
    }

    if (self->dyna.actor.bgCheckFlags & 2) {
        if (*((u8*)self + 0x5A4) == 0) {
            Audio_PlayActorSound2(&self->dyna.actor, 0x0100018C);
        }
    }

    self->dyna.unk_1A8 = self->dyna.unk_1AC = 0.0f;
}

#if 0 // EnAm_Init draft: 6/164 words off (statue branch: ROM loads FUN_0010b3b4 pool word before the flags store; sleep branch: ROM ldr r1,=FUN_003e3020 after the cmp #0xff, ours r2 before it). Tables via EQUs in data_syms.s (EnAm_s*). Dead `play2` alias on the FUN_00353c9c call fixes the r8/sb swap. GLOBAL_ASM kept
extern InitChainEntry EnAm_sInitChain[];
extern ColliderCylinderInit EnAm_sHurtCylinderInit;
extern ColliderCylinderInit EnAm_sBlockCylinderInit;
extern u8 EnAm_sQuadInit[];
extern DamageTable EnAm_sDamageTable;
s32 Collider_InitQuad(PlayState* play, void* collider);
s32 Collider_SetQuad(PlayState* play, void* collider, Actor* actor, void* src);

void EnAm_Init(Actor* thisx, PlayState* play) {
    EnAm* self = (EnAm*)thisx;
    CollisionHeader* colHeader = NULL;
    PlayState* play2 = play;

    Actor_ProcessInitChain(thisx, EnAm_sInitChain);
    ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, 48.0f);
    FUN_00372f38(thisx, play, 0);
    FUN_00353c9c(thisx, play2, &self->skelAnime, 0, 1, self->jointTable, (Vec3s*)((u8*)self + 0x344), 5);
    Actor_SetScale(thisx, 0.01f);
    DynaPolyActor_Init(&self->dyna, 0);
    Collider_InitCylinder(play, &self->hurtCollider);
    Collider_InitCylinder(play, &self->blockCollider);
    Collider_SetCylinder(play, &self->hurtCollider, thisx, &EnAm_sHurtCylinderInit);

    if (thisx->params == 0) {
        f32 lastFrame;

        thisx->colChkInfo.health = 1;
        Collider_SetCylinder(play, &self->blockCollider, thisx, &EnAm_sHurtCylinderInit);
        self->hurtCollider.base.ocFlags1 = 0x35;
        self->blockCollider.base.ocFlags1 = 0xD;
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0), &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
        Actor_ChangeType(play, &play->actorCtx, thisx, 1);

        lastFrame = Animation_GetLastFrame(&self->skelAnime, 1);
        Animation_Change_00375c08(&self->skelAnime, 1, 0.0f, lastFrame, lastFrame, ANIMMODE_LOOP, 0.0f);
        thisx->flags &= ~1;
        thisx->speedXZ = 0.0f;
        self->actionFunc = FUN_0010b3b4;
        self->behavior = AM_BEHAVIOR_DO_NOTHING;
    } else {
        Collider_SetCylinder(play, &self->blockCollider, thisx, &EnAm_sBlockCylinderInit);
        Collider_InitQuad(play, (u8*)self + 0x524);
        Collider_SetQuad(play, (u8*)self + 0x524, thisx, EnAm_sQuadInit);
        thisx->colChkInfo.health = 1;
        thisx->colChkInfo.damageTable = &EnAm_sDamageTable;
        FUN_00180724(self);
        self->unk_258 = 0;
        thisx->unk_19B = 4;
    }

    thisx->colChkInfo.mass = 0xFE;
    *((u8*)self + 0x5A4) = 0x1E;
}

#endif

GLOBAL_ASM("asm/EnAm_Init.s")

void EnAm_Destroy(Actor* thisx, PlayState* play) {
    EnAm* self = (EnAm*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyCylinder(play, &self->hurtCollider);
    Collider_DestroyCylinder(play, &self->blockCollider);
    SkelAnime_Free2(&self->skelAnime);
    FUN_003508b8(&self->dyna.actor, 0);
}

#if 0 // EnAm_Update draft: 172/585 words off, size 2336 vs ROM 2340 (statues' EnAm_UpdateDamage + Setup* helpers as separate static fns = ROM shape; open: reg alloc (ROM play=r5, sl=play copy for the inlined damage code, r7=self+0x100, r8=self+0x400), hit-effect temps). EQUs EnAm_sDust{Prim,Env}Color in data_syms.s. GLOBAL_ASM kept
void FUN_003741e4(PlayState* play, u32 dmgFlags, s32 idx, Vec3f* pos, s32 zero);
void FUN_00375fd0(Actor* actor, void* elem, s32 flag);
void FUN_00375f90(PlayState* play, Vec3f* pos, s32 a);
void FUN_00365d20(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* acc, Color_RGBA8* c1, Color_RGBA8* c2, s16 scale, s32 a, s32 b);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* pos, s32 params);
s32 FUN_00342af8(PlayState* play, Vec3f* hitPos, Vec3f* pos, u32 flags, s32 type);
void FUN_00125f98(EnAm* self, PlayState* play);
void FUN_003d4e70(EnAm* self, PlayState* play);
void FUN_00296a7c(EnAm* self, PlayState* play);
extern "C" float sinf(float);
extern "C" float cosf(float);
extern Color_RGBA8 EnAm_sDustPrimColor;
extern Color_RGBA8 EnAm_sDustEnvColor;
#define EAM_U8(self, off) (*((u8*)(self) + (off)))
#define EAM_S16P(self, off) ((s16*)((u8*)(self) + (off)))

static void EnAm_SetupRicochet(EnAm* self, PlayState* play) {
    Animation_Change_00375c08(&self->skelAnime, 1, 1.0f, 0.0f, 8.0f, 2, 0.0f);
    self->dyna.actor.world.rot.y = self->dyna.actor.yawTowardsPlayer;
    if (FUN_00363108(&self->dyna.actor, play, -6.0f, self->dyna.actor.world.rot.y)) {
        self->dyna.actor.speedXZ = -6.0f;
    }
    self->unk_264 = 0;
    self->unk_258 = 0;
    self->cooldownTimer = 8;
    self->behavior = AM_BEHAVIOR_RICOCHET;
    self->actionFunc = FUN_00125f98;
}

static void EnAm_SetupStunned(EnAm* self, PlayState* play) {
    Animation_Change_00375c08(&self->skelAnime, 2, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 2), 2, 0.0f);
    self->dyna.actor.world.rot.y = self->dyna.actor.yawTowardsPlayer;
    if (FUN_00363108(&self->dyna.actor, play, -6.0f, self->dyna.actor.world.rot.y)) {
        self->dyna.actor.speedXZ = -6.0f;
    }
    Actor_SetColorFilter(&self->dyna.actor, 0, 120, 0, 100);
    if (self->damageReaction == 0xD) {
        self->iceTimer = 48;
    }
    self->behavior = AM_BEHAVIOR_STUNNED;
    Audio_PlayActorSound2(&self->dyna.actor, 0x0100030E);
    self->actionFunc = FUN_00296a7c;
}

static void EnAm_SetupRecoilFromDamage(EnAm* self, PlayState* play) {
    Animation_Change_00375c08(&self->skelAnime, 2, 1.0f, 4.0f, Animation_GetLastFrame(&self->skelAnime, 2) - 6.0f, 2, 0.0f);
    self->behavior = AM_BEHAVIOR_DAMAGED;
    self->dyna.actor.world.rot.y = self->dyna.actor.yawTowardsPlayer;
    Audio_PlayActorSound2(&self->dyna.actor, 0x010002B7);
    if (FUN_00363108(&self->dyna.actor, play, -6.0f, self->dyna.actor.world.rot.y)) {
        self->dyna.actor.speedXZ = -6.0f;
    }
    *(u16*)((u8*)self + 0x11A) = 0;
    FUN_00375b70(play, &self->dyna.actor);
    self->actionFunc = FUN_003d4e70;
}

static void EnAm_UpdateDamage(EnAm* self, PlayState* play) {
    Actor* thisx = &self->dyna.actor;
    s16* hitPos = EAM_S16P(self, 0x49A);
    Vec3f pos;

    if (self->deathTimer != 0) {
        if (self->hurtCollider.base.acFlags & 2) {
            u32* acHitInfo = *(u32**)((u8*)self + 0x4B0);

            self->hurtCollider.base.acFlags &= ~2;
            pos.x = hitPos[0];
            pos.y = hitPos[1];
            pos.z = hitPos[2];
            FUN_003741e4(play, *acHitInfo, 1, &pos, 0);
        }
    } else if (self->blockCollider.base.acFlags & 0x80) {
        if (self->behavior != 3) {
            s16* bp = EAM_S16P(self, 0x4F2);
            u8* acHitInfo = *(u8**)((u8*)self + 0x508);

            self->blockCollider.base.acFlags &= ~0x82;
            self->hurtCollider.base.acFlags &= ~2;
            pos.x = bp[0];
            pos.y = bp[1];
            pos.z = bp[2];
            FUN_003741e4(play, *(u32*)acHitInfo, 1, &pos, 0);
            FUN_00342af8(play, &pos, &thisx->world.pos, acHitInfo[0x15], 1);

            if (self->behavior > 4) {
                EnAm_SetupRicochet(self, play);
            }
        }
    } else if (self->hurtCollider.base.acFlags & 2) {
        if (self->behavior > 4) {
            s32 flag = 1;

            self->hurtCollider.base.acFlags &= ~2;
            if (EAM_U8(self, 0xB9) != 0xE) {
                u8* acHitInfo;

                self->unk_264 = 0;
                self->damageReaction = EAM_U8(self, 0xB9);
                FUN_00375fd0(thisx, (u8*)self + 0x48C, 0);

                if ((EAM_U8(self, 0xB9) == 1) || (EAM_U8(self, 0xB9) == 6) || (EAM_U8(self, 0xB9) == 0xD)) {
                    if (self->behavior != AM_BEHAVIOR_STUNNED) {
                        EnAm_SetupStunned(self, play);

                        if (EAM_U8(self, 0xB8) != 0) {
                            EAM_U8(self, 0xB7) = 0;
                            flag = 0;
                        }
                    } else if (EAM_U8(self, 0xB9) == 6) {
                        Vec3f sparkPos = thisx->world.pos;

                        sparkPos.y += 50.0f;
                        FUN_00375f90(play, &sparkPos, 8);
                    }
                } else if ((EAM_U8(self, 0xB9) == 0xF) || (self->behavior == AM_BEHAVIOR_STUNNED)) {
                    EAM_U8(self, 0xB7) = 0;
                    EnAm_SetupRecoilFromDamage(self, play);
                    flag = 0;
                } else {
                    EnAm_SetupRicochet(self, play);
                }

                acHitInfo = *(u8**)((u8*)self + 0x4B0);
                pos.x = hitPos[0];
                pos.y = hitPos[1];
                pos.z = hitPos[2];
                FUN_003741e4(play, *(u32*)acHitInfo, flag, &pos, 0);
                if (*(u32*)acHitInfo & 0x100) {
                    Audio_PlaySoundGeneral(0x01000103, (Vec3f*)(*(u8**)((u8*)self + 0x47C) + 0x28), 4, &FLOAT_0054ac20,
                                           &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                }
            }
        }
    }
}

void EnAm_Update(Actor* thisx, PlayState* play) {
    EnAm* self = (EnAm*)thisx;
    s32 i;

    if (EAM_U8(self, 0x5A4) != 0) {
        EAM_U8(self, 0x5A4)--;
    }

    if (thisx->params != 0) {
        EnAm_UpdateDamage(self, play);
    }

    if (self->unk_462 != 0) {
        self->unk_462--;
    }

    if (EAM_U8(self, 0xB9) != 0xE) {
        if (self->attackTimer != 0) {
            self->attackTimer--;
        }

        self->actionFunc(self, play);

        if (self->deathTimer != 0) {
            self->deathTimer--;

            if (self->deathTimer == 0) {
                static Vector3f zeroVec(0.0f, 0.0f, 0.0f);
                Vec3f pos;
                f32 dustPosScale = *(u32*)((u8*)play + 0x5BF4) * 10;
                Actor* bomb;

                if (self->unk_462 != 0) {
                    FUN_0035a534(self, play, 0);
                } else {
                    FUN_0035a534(self, play, 1);
                    if (self->unk_460 == 0) {
                        self->unk_460 = 1;
                        self->unk_462 = 2;
                    } else {
                        self->unk_460 = 0;
                        self->unk_462 = 1;
                    }
                }
                bomb = Actor_Spawn(&play->actorCtx, play, 0x10, thisx->world.pos.x, thisx->world.pos.y,
                                   thisx->world.pos.z, 0, 0, 2, 0, 1);
                if (bomb != NULL) {
                    *(s16*)((u8*)bomb + 0x26C) = 0;
                }
                Audio_PlayActorSound2(thisx, 0x010002B6);
                Item_DropCollectibleRandom(play, thisx, &thisx->world.pos, 0xA0);

                for (i = 9; i >= 0; i--) {
                    pos.x = sinf(dustPosScale) * 7.0f + thisx->world.pos.x;
                    pos.y = 40.0f + Rand_CenteredFloat(10.0f) * 6.0f + thisx->world.pos.y;
                    pos.z = cosf(dustPosScale) * 7.0f + thisx->world.pos.z;
                    FUN_00365d20(play, &pos, (Vec3f*)&zeroVec, (Vec3f*)&zeroVec, &EnAm_sDustPrimColor,
                                 &EnAm_sDustEnvColor, 200, 45, 12);
                    dustPosScale += 60.0f;
                }

                Actor_Kill(thisx);
                return;
            }

            if ((self->deathTimer & 5) == 0) {
                Actor_SetColorFilter(thisx, 0x400000, 0xFF, 0, 4);
            }
        }

        Actor_MoveForward(thisx);
        Actor_UpdateBgCheckInfo(play, thisx, 20.0f, 28.0f, 80.0f, 0x1D);
    }

    Collider_UpdateCylinder(thisx, &self->hurtCollider);
    Collider_UpdateCylinder(thisx, &self->blockCollider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->hurtCollider.base);

    if (thisx->params != 0) {
        Actor_SetFocus(thisx, thisx->scale.x * 4500.0f);

        if (*(u16*)((u8*)self + 0x11A) == 0) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &self->blockCollider.base);
        }

        CollisionCheck_SetAC(play, &play->colChkCtx, &self->hurtCollider.base);

        if ((self->behavior >= 4) && (self->unk_264 > 0)) {
            u8 atFlags = EAM_U8(self, 0x534);

            if (!(atFlags & 4)) {
                if (atFlags & 2) {
                    Player* player = GET_PLAYER(play);

                    if (*(Actor**)((u8*)self + 0x528) == &player->actor) {
                        Audio_PlayActorSound2(&player->actor, 0x0100003F);
                    }
                }
                CollisionCheck_SetAT(play, &play->colChkCtx, (Collider*)((u8*)self + 0x524));
            } else {
                EAM_U8(self, 0x534) &= ~6;
                *(Actor**)((u8*)self + 0x528) = NULL;
                EnAm_SetupRicochet(self, play);
            }
        }
    } else {
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->blockCollider.base);
    }
}

#endif

GLOBAL_ASM("asm/EnAm_Update.s")

#if 0 // FUN_002336e0 draft (EnAm_TransformSwordHitbox limb callback; guards 0x5203F8/F4/F0/EC -> objects 0x598E70/7C/88/94, needs FUN_0035a534 statics before it in the TU): 20/126 words off (tail reg alloc: ROM push {r4-r8}, mtx r7, p1 r8/p2 r5/p3 r6, sub sp,#8), GLOBAL_ASM kept
void FUN_0035479c(void* quad, Vec3f* a, Vec3f* b, Vec3f* c, Vec3f* d); // Collider_SetQuadVertices

void FUN_002336e0(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg) {
    EnAm* self = (EnAm*)arg;

    if ((limbIndex == 0) && (self->unk_264 != 0)) {
        static Vector3f sSwordTipSrc1(2500.0f, 7000.0f, 0.0f);
        static Vector3f sSwordTipSrc0(-2000.0f, 0.0f, 0.0f);
        static Vector3f sSwordTipSrc3(2500.0f, 7000.0f, 4000.0f);
        static Vector3f sSwordTipSrc2(-2500.0f, 0.0f, 4000.0f);

        VEC3TransformAsm((Vector3f*)((u8*)self + 0x570), mtx, &sSwordTipSrc1);
        VEC3TransformAsm((Vector3f*)((u8*)self + 0x564), mtx, &sSwordTipSrc0);
        VEC3TransformAsm((Vector3f*)((u8*)self + 0x588), mtx, &sSwordTipSrc3);
        VEC3TransformAsm((Vector3f*)((u8*)self + 0x57C), mtx, &sSwordTipSrc2);
        FUN_0035479c((u8*)self + 0x524, (Vec3f*)((u8*)self + 0x564), (Vec3f*)((u8*)self + 0x570), (Vec3f*)((u8*)self + 0x57C),
                     (Vec3f*)((u8*)self + 0x588));
    }
}
#endif

void FUN_001587b0(EnAm* self, PlayState* play);
f32 Actor_WorldDistXYZToPoint(Actor* actor, Vec3f* point); // 3DS: returns distance as f32 in s0

void FUN_0013e138(EnAm* self, PlayState* play) {
    s16 yawToHome = Math_Vec3f_Yaw(&self->dyna.actor.world.pos, &self->homePos);

    f32 frame = self->skelAnime.curFrame;
    if (frame > 5.2333336f) {
        if (self->skelAnime.curFrame < 5.4333333f) {
            self->dyna.actor.velocity.y = 12.0f;
            self->dyna.actor.speedXZ = 6.0f;
            goto done;
        }
    }
    if (self->skelAnime.curFrame > 10.6666667f) {
        if (!(self->dyna.actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
            self->skelAnime.curFrame = 10.6666667f;
        } else {
            Math_SmoothStepToS(&self->dyna.actor.world.rot.y, yawToHome, 1, 0xBB8, 0);

            if (self->dyna.actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
                self->unk_258--;
            }
            self->dyna.actor.velocity.y = 0.0f;
            self->dyna.actor.speedXZ = 0.0f;
            self->dyna.actor.world.pos.y = self->dyna.actor.floorHeight;

            if (self->unk_462 != 0) {
                FUN_0035a534(self, play, 0);
            } else {
                FUN_0035a534(self, play, 1);
                u16 t;
                if (self->unk_460 != 0) {
                    self->unk_460 = 0;
                    t = 1;
                } else {
                    self->unk_460 = 1;
                    t = 2;
                }
                self->unk_462 = t;
            }

            if (Actor_WorldDistXYZToPoint(&self->dyna.actor, &self->homePos) < 80.0f) {
                Animation_PlayLoopSetSpeed(&self->skelAnime, 0, 4.0f);
                self->behavior = AM_BEHAVIOR_GO_HOME;
                self->dyna.actor.speedXZ = 0.0f;
                self->unk_258 = 1;
                self->actionFunc = FUN_001587b0;
            }
        }
    }

done:
    if ((self->dyna.actor.speedXZ != 0.0f) && (self->dyna.actor.bgCheckFlags & BGCHECKFLAG_WALL)) {
        self->dyna.actor.world.rot.y = self->dyna.actor.wallYaw;
        Actor_MoveForward(&self->dyna.actor);
    }

    FUN_003731e0(&self->skelAnime);
    self->dyna.actor.shape.rot.y = self->dyna.actor.world.rot.y;
}

#if 0 // FUN_001587b0 draft (EnAm_RotateToInit): 103/126 words off. Same shape as FUN_001e88ec; ours inlines FUN_00180724 (ROM bl) and uses extra regs (fp/ip); GLOBAL_ASM kept
void FUN_0035a534(EnAm* self, PlayState* play, s32 flag);
static inline s16 SmoothStepToS_(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep) {
    s16 stepSize = 0;
    s16 diff = target - *pValue;

    if (*pValue != target) {
        stepSize = diff / scale;

        if ((stepSize > minStep) || (stepSize < -minStep)) {
            if (stepSize > step) {
                stepSize = step;
            }

            if (stepSize < -step) {
                stepSize = -step;
            }

            *pValue += stepSize;
        } else {
            if (diff >= 0) {
                *pValue += minStep;

                if ((s16)(target - *pValue) <= 0) {
                    *pValue = target;
                }
            } else {
                *pValue -= minStep;

                if ((s16)(target - *pValue) >= 0) {
                    *pValue = target;
                }
            }
        }
    }

    return diff;
}

void FUN_001587b0(EnAm* self, PlayState* play) {
    f32 frame = self->skelAnime.curFrame;
    s16 scale = 1;

    if (frame > 5.2333336f) {
        if (frame < 5.4333333f) {
            if ((self->dyna.actor.world.pos.x == self->homePos.x) && (self->dyna.actor.world.pos.z == self->homePos.z)) {
                SmoothStepToS_(&self->dyna.actor.world.rot.y, self->dyna.actor.home.rot.y, scale, 0x1F40, 0);
            }
            self->unk_258 = 2;
            self->dyna.actor.velocity.y = 12.0f;
            goto done;
        }
    }
    if (frame > 10.6666667f) {
        if (!(self->dyna.actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
            self->skelAnime.curFrame = 10.6666667f;
        } else {
            self->unk_258 = 1;
            if (self->unk_462 != 0) {
                FUN_0035a534(self, play, 0);
            } else {
                u16 t;
                FUN_0035a534(self, play, 1);
                if (self->unk_460 == 0) {
                    self->unk_460 = 1;
                    t = 2;
                } else {
                    self->unk_460 = 0;
                    t = 1;
                }
                self->unk_462 = t;
            }
            if (self->dyna.actor.home.rot.y == self->dyna.actor.world.rot.y) {
                self->unk_258 = 0;
            }
            self->dyna.actor.velocity.y = 0.0f;
            self->dyna.actor.world.pos.y = self->dyna.actor.floorHeight;
        }
    }
done:
    if (self->unk_258 == 2) {
        Math_SmoothStepToF(&self->dyna.actor.world.pos.x, self->homePos.x, 1.0f, 8.0f, 0.0f);
        Math_SmoothStepToF(&self->dyna.actor.world.pos.z, self->homePos.z, 1.0f, 8.0f, 0.0f);
    }

    FUN_003731e0(&self->skelAnime);

    if (self->unk_258 == 0) {
        FUN_00180724(self);
    }

    self->dyna.actor.shape.rot.y = self->dyna.actor.world.rot.y;
}
#endif

#if 0 // EnAm_Draw draft: 5/87 words off (ROM base r1 / iceTimer r2, ours swapped; tst #5 is ROM), GLOBAL_ASM kept
void FUN_002336e0(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg);
void FUN_00342be0(EnAm* self, s32 a, s32 b, f32 c, f32 d, f32 e, f32 f);
void EffectSsEnIce_SpawnFlyingVec3f(PlayState* play, Actor* actor, Vec3f* pos, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, f32 scale);
extern Vec3f sIcePosOffsets[];

void EnAm_Draw(Actor* thisx, PlayState* play) {
    EnAm* self = (EnAm*)thisx;
    Vec3f sp68;
    s32 index;

    FUN_00357fd0(GET_PLAYER(play), self->dyna.actor.unk_178, &self->dyna.actor.world.pos);
    {
        u8* unk178 = (u8*)self->dyna.actor.unk_178;

        unk178[0x1B7] = unk178[0x1B6];
        unk178[0x1B6] = 0;
    }
    FUN_00342be0(self, 4, 2, 0.0f, 0.0f, 0.0f, 1.0f - (self->textureBlend * (1.0f / 255.0f)));
    SkelAnime_DrawOpa(&self->skelAnime, (MTX34*)((u8*)self + 0x148), NULL, FUN_002336e0, &self->dyna.actor, 0);

    s16 iceTimer = self->iceTimer;

    if (iceTimer != 0) {
        {
            u8* cfBase = (u8*)self + 0x100;

            *(u16*)(cfBase + 0x1A) = *(u16*)(cfBase + 0x1A) + 1;
        }
        iceTimer--;
        self->iceTimer = iceTimer;

        if ((iceTimer & 5) == 0) {
            index = iceTimer >> 2;

            sp68.x = self->dyna.actor.world.pos.x + sIcePosOffsets[index].x;
            sp68.y = self->dyna.actor.world.pos.y + sIcePosOffsets[index].y;
            sp68.z = self->dyna.actor.world.pos.z + sIcePosOffsets[index].z;

            EffectSsEnIce_SpawnFlyingVec3f(play, &self->dyna.actor, &sp68, 150, 150, 150, 250, 235, 245, 255, 1.4f);
        }
    }
}
#endif

GLOBAL_ASM("asm/EnAm_Draw.s")


}
