extern "C" {
#include "z_en_bili.hpp"

#define FLAGS 0x00005005

#define ACTOR_FLAG_ATTACHED_TO_ARROW_BILI 0x8000
#define ACTOR_FLAG_ATTENTION_ENABLED_BILI 0x1

void EnBili_Init(Actor* thisx, PlayState* play);
void EnBili_Destroy(EnBili* self, PlayState* play);
void EnBili_Update(Actor* thisx, PlayState* play2);
void EnBili_Draw(Actor* actor, PlayState* play);
void EnBili_FloatIdle(EnBili* self, PlayState* play);
void EnBili_Die(EnBili* self, PlayState* play);
void EnBili_UpdateFloating(EnBili* self);
void EnBili_Burnt(EnBili* self);
void EnBili_Stunned(EnBili* self);
void EnBili_Frozen(EnBili* self, PlayState* play);
void EnBili_Recoil(EnBili* self);
void EnBili_DischargeLightning(EnBili* self, PlayState* play);
void EnBili_SetNewHomeHeight(EnBili* self);
void EnBili_ApproachPlayer(EnBili* self, PlayState* play);
f32* EnBili_CurFramePtr(SkelAnime* skelAnime);
f32 __hardfp_sinf(f32 x);
void FUN_0036f1b4(EnBili* self);
s32 FUN_00272930(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
s32 Math_ScaledStepToS(s16* value, s16 target, s16 step);
void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);

GLOBAL_ASM("../src/game/actors/En_Bili/z_En_Bili_data_tail.s")
GLOBAL_ASM("../src/game/actors/En_Bili/data_syms.s")

extern InitChainEntry EnBili_sInitChain[]; // data_syms.s
extern ColliderCylinderInit EnBili_sCylinderInit; // data_syms.s
extern CollisionCheckInfoInit2 EnBili_sColChkInfoInit; // data_syms.s
void Animation_PlayOnce(SkelAnime* skelAnime, s32 idx);
void EffectSsLightning_Spawn(PlayState* play, Vec3f* pos, Color_RGBA8* envColor, Color_RGBA8* primColor, s32 arg4, s32 yaw, s32 arg6, s32 arg7);

ActorInit En_Bili_InitVars = {
    ACTOR_EN_BILI,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_BL,
    sizeof(EnBili),
    (ActorFunc)EnBili_Init,
    (ActorFunc)EnBili_Destroy,
    (ActorFunc)EnBili_Update,
    (ActorFunc)EnBili_Draw,
};

static Color_RGBA8 EnBili_sEffEnvColor = { 255, 255, 255, 255 };
static Color_RGBA8 EnBili_sEffPrimColor = { 200, 255, 255, 255 };

void EnBili_SpawnedFlyApart(EnBili* self);
void EnBili_Climb(EnBili* self);
static inline void EnBili_SetupFloatIdleInline(EnBili* self);

void EnBili_Init(Actor* thisx, PlayState* play) {
    EnBili* self = (EnBili*)thisx;

    Actor_ProcessInitChain(&self->actor, EnBili_sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 17.0f);
    self->actor.shape.shadowAlpha = 155;
    FUN_00372f38(&self->actor, play, 0);
    FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 0, self->unk_228, (u8*)self + 0x360, 6);
    SkelAnime_SetFaceAnimations((FaceAnimations*)&self->unk_498, &self->skelAnime, 0, -1, -1);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, thisx, &EnBili_sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, (DamageTable*)((u8*)&EnBili_sColChkInfoInit + 0xA), &EnBili_sColChkInfoInit);
    self->playFlySfx = 0;
    if (self->actor.params == -1) {
        thisx->speedXZ = 0.7f;
        self->collider.info.bumper.effect = 1;
        self->timer = 48;
        self->actor.home.pos.y = self->actor.world.pos.y;
        self->actor.gravity = 0.0f;
        self->actor.velocity.y = 0.0f;
        self->collider.base.atFlags |= AT_ON;
        self->collider.base.acFlags |= AC_ON;
        self->actionFunc = (EnBiliActionFunc)EnBili_FloatIdle;
    } else {
        Animation_PlayLoop(&self->skelAnime, 0);
        self->timer = 38;
        self->actor.velocity.y = 6.0f;
        self->actor.gravity = -0.3f;
        self->actor.speedXZ = 3.0f;
        self->collider.base.atFlags &= ~AT_ON;
        self->actionFunc = (EnBiliActionFunc)EnBili_SpawnedFlyApart;
    }
}

void EnBili_Destroy(EnBili* self, PlayState* play) {
    Collider_DestroyCylinder(play, &self->collider);
    ((FaceAnimations*)&self->unk_498)->Destroy();
    SkelAnime_Free2(&self->skelAnime);
    FUN_00350f34(&self->actor, 0);
}

GLOBAL_ASM("asm/EnBili_Update.s")

void EnBili_Draw(Actor* thisx, PlayState* play) {
    EnBili* self = (EnBili*)thisx;
    FaceAnimations* face;

    self->tentaclesTexIndex = CLAMP_MAX(self->tentaclesTexIndex, 7);
    face = (FaceAnimations*)&self->unk_498;
    face->FUN_0035e3a4(0, self->tentaclesTexIndex);
    face = (FaceAnimations*)&self->unk_498;
    face->FUN_0035e330();
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, FUN_00272930, NULL, thisx, 0);
}

void EnBili_SetNewHomeHeight(EnBili* self) {
    FUN_003731e0(&self->skelAnime);
    if (self->timer != 0) {
        self->timer--;
    }
    Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer + 0x8000, 0x38E);
    EnBili_UpdateFloating(self);
    if (self->timer == 0) {
        FUN_0036f1b4(self);
    }
}

void EnBili_ApproachPlayer(EnBili* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    FUN_00370084(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 2, 0x71C);
    if (self->timer != 0) {
        self->timer--;
    }
    EnBili_UpdateFloating(self);
    if (self->timer == 0) {
        self->timer = 48;
    }
    if (self->actor.xzDistToPlayer > 200.0f) {
        FUN_0036f1b4(self);
    }
}

void EnBili_Burnt(EnBili* self) {
    FUN_003731e0(&self->skelAnime);
    if (self->actor.flags & ACTOR_FLAG_ATTACHED_TO_ARROW_BILI) {
        *(u16*)((u8*)self + 0x11A) = 30;
    } else {
        if (self->timer != 0) {
            self->timer--;
            if (self->timer != 0) {
                return;
            }
        }
        self->timer = 27;
        self->actor.speedXZ = 0.0f;
        Animation_PlayLoop(&self->skelAnime, 3);
        self->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED_BILI;
        self->actionFunc = EnBili_Die;
    }
}

static inline void EnBili_SetupFloatIdleInline(EnBili* self) {
    self->actor.speedXZ = 0.7f;
    self->collider.info.bumper.effect = 1;
    self->timer = 48;
    self->actor.home.pos.y = self->actor.world.pos.y;
    self->actor.gravity = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->collider.base.atFlags |= AT_ON;
    self->collider.base.acFlags |= AC_ON;
    self->actionFunc = EnBili_FloatIdle;
}

void EnBili_SpawnedFlyApart(EnBili* self) {
    FUN_003731e0(&self->skelAnime);
    if (self->timer != 0) {
        self->timer--;
        if (self->timer != 0) {
            return;
        }
    }
    EnBili_SetupFloatIdleInline(self);
}

void EnBili_Stunned(EnBili* self) {
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->actor.bgCheckFlags & 2) {
        Audio_PlayActorSound2(&self->actor, 0x010002EB);
    }
    if (self->timer == 0) {
        EnBili_SetupFloatIdleInline(self);
    }
}

void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* pos, s32 params);
s16 Rand_S16Offset(s16 base, s16 range);
void EffectSsDtBubble_SpawnColorProfile(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, s16 scale, s32 a,
                                        s32 b, s32 c);

void EnBili_Die(EnBili* self, PlayState* play) {
    static Vector3f sEffVel(0.0f, 0.0f, 0.0f);
    static Vector3f sEffAccel(0.0f, 0.0f, 0.0f);
    s16 effScale;
    Vec3f effPos;
    s32 i;

    FUN_003731e0(&self->skelAnime);
    if (self->actor.draw != NULL) {
        if (!(self->actor.flags & ACTOR_FLAG_ATTACHED_TO_ARROW_BILI)) {
            self->actor.draw = NULL;
            Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos, 0x50);
        } else {
            return;
        }
    }
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer != 0) {
        for (i = 0; i < 2; i++) {
            effPos.x = (Rand_ZeroOne() * 10.0f) + self->actor.world.pos.x - 5.0f;
            effPos.y = (Rand_ZeroOne() * 5.0f) + self->actor.world.pos.y - 2.5f;
            effPos.z = (Rand_ZeroOne() * 10.0f) + self->actor.world.pos.z - 5.0f;
            sEffVel.y = Rand_ZeroOne() + 1.0f;
            effScale = Rand_S16Offset(40, 40);
            if (Rand_ZeroOne() < 0.7f) {
                EffectSsDtBubble_SpawnColorProfile(play, &effPos, (Vec3f*)&sEffVel, (Vec3f*)&sEffAccel, effScale, 25, 2, 1);
            } else {
                EffectSsDtBubble_SpawnColorProfile(play, &effPos, (Vec3f*)&sEffVel, (Vec3f*)&sEffAccel, effScale, 25, 0, 1);
            }
        }
    } else {
        Actor_Kill(&self->actor);
    }
    if (self->timer == 21) {
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 40, 0x0100030C);
    }
}

void EnBili_DischargeLightning(EnBili* self, PlayState* play) {
    s32 i;

    for (i = 0; i < 4; i++) {
        s32 r;
        s32 t;

        if (self->timer > 0) {
            r = (s32)(((f32)self->timer * 2.0f / 3.0f) + 0.5f);
        } else {
            r = (s32)(((f32)self->timer * 2.0f / 3.0f) - 0.5f);
        }
        t = r + (i << 1);
        if (!(t % 4)) {
            Vec3f effPos;
            s16 effYaw;

            effYaw = (s16)Rand_CenteredFloat(12288.0f) + (i * 0x4000) + 0x2000;
            effPos.x = Rand_CenteredFloat(5.0f) + self->actor.world.pos.x;
            effPos.y = (2.5f + (Rand_ZeroOne() * 5.0f)) + self->actor.world.pos.y;
            effPos.z = Rand_CenteredFloat(5.0f) + self->actor.world.pos.z;
            EffectSsLightning_Spawn(play, &effPos, &EnBili_sEffEnvColor, &EnBili_sEffPrimColor, 15, effYaw, 6, 2);
        }
    }
    FUN_003731e0(&self->skelAnime);
    FUN_00373264(&self->actor, 0x0100030A);
    if (self->timer != 0) {
        self->timer--;
    }
    self->actor.velocity.y *= -1.0f;
    if (self->timer == 0 && Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f)) {
        if (self->actor.params == 1) {
            self->timer = 27;
            self->actor.speedXZ = 0.0f;
            Animation_PlayLoop(&self->skelAnime, 3);
            self->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED_BILI;
            self->actionFunc = EnBili_Die;
        } else {
            Animation_PlayOnce(&self->skelAnime, 1);
            self->actor.speedXZ = 0.0f;
            self->actor.velocity.y = 0.0f;
            self->collider.base.atFlags &= ~AT_ON;
            self->actionFunc = (EnBiliActionFunc)EnBili_Climb;
        }
    }
}

extern __pure float _fabsf(float);
static __inline __pure float EnBili_Fabsf(float f) {
    return _fabsf(f);
}

void EnBili_FloatIdle(EnBili* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer % 6 == 0) {
        self->actor.world.rot.y += Rand_CenteredFloat(1820.0f);
    }
    EnBili_UpdateFloating(self);
    if (self->timer == 0) {
        self->timer = 48;
    }
    if (self->actor.xzDistToPlayer < 160.0f && EnBili_Fabsf(self->actor.yDistToPlayer) < 45.0f) {
        self->actor.speedXZ = 1.2f;
        self->actionFunc = (EnBiliActionFunc)EnBili_ApproachPlayer;
    }
}

void EnBili_Frozen(EnBili* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
    }
    if (!(self->actor.flags & ACTOR_FLAG_ATTACHED_TO_ARROW_BILI)) {
        self->actor.gravity = -1.0f;
    }
    if ((self->actor.bgCheckFlags & 1) || (self->actor.floorHeight == -32000.0f)) {
        *(u16*)((u8*)self + 0x11A) = 0;
        self->timer = 27;
        self->actor.speedXZ = 0.0f;
        Animation_PlayLoop(&self->skelAnime, 3);
        self->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED_BILI;
        self->actionFunc = EnBili_Die;
    } else {
        *(u16*)((u8*)self + 0x11A) = 15;
    }
}

void EnBili_Recoil(EnBili* self) {
    FUN_003731e0(&self->skelAnime);
    if (Math_StepToF(&self->actor.speedXZ, 0.0f, 0.3f)) {
        self->actor.world.rot.y += 0x8000;
        EnBili_SetupFloatIdleInline(self);
    }
}

void EnBili_Climb(EnBili* self) {
    s32 r5 = FUN_003731e0(&self->skelAnime);
    f32 frame = self->skelAnime.curFrame;

    if (Animation_OnFrameImpl(&self->skelAnime, 9.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, 0x01000309);
    }
    if (frame > 9.0f) {
        Math_ApproachF(&self->actor.world.pos.y, self->actor.world.pos.y + self->actor.yDistToPlayer + 100.0f, 0.5f, 5.0f);
    }
    if (r5 != 0) {
        Animation_PlayLoop(&self->skelAnime, 0);
        self->timer = 144;
        self->actor.speedXZ = 0.9f;
        self->collider.base.atFlags |= AT_ON;
        self->actor.home.pos.y = self->actor.world.pos.y;
        self->actionFunc = (EnBiliActionFunc)EnBili_SetNewHomeHeight;
    }
}

void EnBili_UpdateFloating(EnBili* self) {
    f32 offs;
    f32 targetY;
    f32 ang;
    s32 r;
    f32 playerY = self->actor.world.pos.y + self->actor.yDistToPlayer;
    void* fn = (void*)self->actionFunc;
    f32 floorY = self->actor.floorHeight;
    offs = (fn == (void*)EnBili_SetNewHomeHeight) ? 100.0f : 40.0f;
    targetY = (floorY < playerY) ? playerY : floorY;
    Math_StepToF(&self->actor.home.pos.y, offs + targetY, 1.0f);
    if (self->timer > 0) {
        r = (s32)(((f32)self->timer * 2.0f / 3.0f) + 0.5f);
    } else {
        r = (s32)(((f32)self->timer * 2.0f / 3.0f) - 0.5f);
    }
    ang = (f32)r * 0.19634955f;
    self->actor.world.pos.y = __hardfp_sinf(ang) * 3.0f + self->actor.home.pos.y;
    if (self->actor.bgCheckFlags & 8) {
        self->actor.world.rot.y = self->actor.wallYaw;
    }
}


// Near-miss draft (Update, 404 insns): ~12 real word diffs, rest is pool-offset noise from 3 extra insns. Open: ROM tests
// `actionFunc != Die` with Die loaded first (r0) and puts the `frame >> 1` (not Climb) arm FIRST as a predicated
// `asrne` + shared strb, while the <19 arm keeps a private strb. Needs z_en_bili_anim.cpp (EnBili_CurFramePtr accessor,
// own TU so armlink inlines it to `add r0,r0,#0x3c`); not enabled because the function is unmatched.
#if 0
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
s32 Actor_ApplyDamage(Actor* actor);
void FUN_00375b70(PlayState* play, Actor* actor);
void FUN_00375fd0(Actor* actor, void* elem, s32 flag);
s16 Actor_WorldYawTowardPoint(Actor* actor, Vec3f* point);
s16 Actor_WorldPitchTowardPoint(Actor* actor, Vec3f* point);
void FUN_0033bd9c(Actor* actor);
void EffectSsEnIce_SpawnFlyingVec3f(PlayState* play, Actor* actor, Vec3f* pos, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, f32 scale);

static inline void EnBili_SetupElectrifyInline(EnBili* self) {
    Animation_PlayLoop(&self->skelAnime, 2);
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = -1.0f;
    self->timer = 15;
    self->unk_66C = 45;
    self->actionFunc = (EnBiliActionFunc)EnBili_DischargeLightning;
}

static inline void EnBili_SetupDamagedInline(EnBili* self) {
    Animation_PlayLoop(&self->skelAnime, 3);
    self->timer = 30;
    self->collider.base.atFlags &= ~AT_ON;
    self->collider.base.acFlags &= ~AC_ON;
    self->actor.speedXZ = 0.0f;
    self->actor.flags |= 0x10;
    Actor_SetColorFilter(&self->actor, 0x400000, 200, 0x200000, 20);
    self->actionFunc = (EnBiliActionFunc)EnBili_Burnt;
}

void EnBili_Update(Actor* thisx, PlayState* play2) {
    EnBili* self = (EnBili*)thisx;
    PlayState* play = play2;
    s32 i;

    if (self->unk_66C != 0) {
        self->unk_66C--;
    }
    if (self->collider.base.atFlags & 2) {
        self->collider.base.atFlags &= ~2;
        EnBili_SetupElectrifyInline(self);
    }
    if (self->actor.colChkInfo.health != 0 && (self->collider.base.acFlags & 2)) {
        self->collider.base.acFlags &= ~2;
        FUN_00375fd0(&self->actor, &self->collider.info, 1);
        if (self->actor.colChkInfo.damageEffect != 0 || self->actor.colChkInfo.damage != 0) {
            if (Actor_ApplyDamage(&self->actor) == 0) {
                Audio_PlayActorSound2(&self->actor, 0x0100030B);
                FUN_00375b70(play, &self->actor);
                self->actor.flags &= ~1;
            }
            if (self->actor.colChkInfo.damageEffect == 1) {
                if ((void*)self->actionFunc != (void*)EnBili_Stunned) {
                    self->actor.gravity = -1.0f;
                    self->timer = 120;
                    self->actor.speedXZ = 0.0f;
                    self->collider.info.bumper.effect = 0;
                    Actor_SetColorFilter(&self->actor, 0, 150, 0x200000, 80);
                    Audio_PlayActorSound2(&self->actor, 0x0100030E);
                    self->collider.base.atFlags &= ~AT_ON;
                    self->actionFunc = (EnBiliActionFunc)EnBili_Stunned;
                }
            } else if (self->actor.colChkInfo.damageEffect == 15) {
                if ((void*)self->actionFunc != (void*)EnBili_Stunned) {
                    Actor_SetColorFilter(&self->actor, 0x400000, 200, 0x200000, 10);
                    if (self->actor.colChkInfo.health == 0) {
                        self->actor.params = 1;
                    }
                    EnBili_SetupElectrifyInline(self);
                } else {
                    EnBili_SetupDamagedInline(self);
                }
            } else if (self->actor.colChkInfo.damageEffect == 2) {
                EnBili_SetupDamagedInline(self);
                self->timer = 3;
            } else if (self->actor.colChkInfo.damageEffect == 3) {
                Vec3f effPos;

                if (!(self->actor.flags & ACTOR_FLAG_ATTACHED_TO_ARROW_BILI)) {
                    self->actor.gravity = -1.0f;
                }
                self->actor.velocity.y = 0.0f;
                effPos.y = self->actor.world.pos.y - 15.0f;
                for (i = 0; i < 8; i++) {
                    { f32 d = (i & 1) ? 7.0f : -7.0f; effPos.x = self->actor.world.pos.x + d; }
                    effPos.y += 2.5f;
                    effPos.z = self->actor.world.pos.z + ((i & 4) ? 7.0f : -7.0f);
                    EffectSsEnIce_SpawnFlyingVec3f(play, &self->actor, &effPos, 150, 150, 150, 250, 235, 245, 255,
                                                   (Rand_ZeroOne() * 0.2f) + 0.7f);
                }
                self->actor.speedXZ = 0.0f;
                Actor_SetColorFilter(&self->actor, 0, 150, 0x200000, 10);
                self->collider.base.atFlags &= ~AT_ON;
                self->collider.base.acFlags &= ~AC_ON;
                self->timer = 450;
                self->actionFunc = (EnBiliActionFunc)EnBili_Frozen;
            } else if (self->actor.colChkInfo.damageEffect == 14) {
                if ((s32)self->skelAnime.curFrame != 0) {
                    Animation_PlayLoop(&self->skelAnime, 0);
                }
                self->actor.world.rot.y = Actor_WorldYawTowardPoint(&self->actor, &self->collider.base.ac->prevPos) + 0x8000;
                self->actor.world.rot.x = Actor_WorldPitchTowardPoint(&self->actor, &self->collider.base.ac->prevPos);
                self->actor.speedXZ = 5.0f;
                self->actionFunc = (EnBiliActionFunc)EnBili_Recoil;
            } else {
                EnBili_SetupDamagedInline(self);
            }
            if (self->collider.info.acHitInfo->toucher.dmgFlags & 0x0001F820) {
                self->actor.flags |= 0x10;
            }
        }
    }
    self->actionFunc(self, play);
    if ((void*)self->actionFunc != (void*)EnBili_Die) {
        s16 frame = (s16)(s32)*EnBili_CurFramePtr(&self->skelAnime);
        s16 n;

        if ((void*)self->actionFunc == (void*)EnBili_DischargeLightning) {
            n = 3 - frame;
            self->tentaclesTexIndex = (ABS(n) + 5) % 8;
        } else if ((void*)self->actionFunc != (void*)EnBili_Climb) {
            self->tentaclesTexIndex = frame >> 1;
        } else {
            if (frame < 10) {
                n = frame >> 1;
                self->tentaclesTexIndex = CLAMP_MAX(n, 3);
            } else if (frame < 19) {
                n = 17 - frame;
                self->tentaclesTexIndex = CLAMP_MIN(n, 0) >> 1;
            } else if (frame < 37) {
                self->tentaclesTexIndex = ((36 - frame) / 3) + 2;
            } else {
                self->tentaclesTexIndex = (40 - frame) >> 1;
            }
        }
        if (Animation_OnFrameImpl(&self->skelAnime, 9.0f, 1.0f) &&
            ((void*)self->actionFunc == (void*)EnBili_FloatIdle || (void*)self->actionFunc == (void*)EnBili_SetNewHomeHeight ||
             (void*)self->actionFunc == (void*)EnBili_ApproachPlayer || (void*)self->actionFunc == (void*)EnBili_Recoil)) {
            if (self->playFlySfx) {
                Audio_PlayActorSound2(&self->actor, 0x01000308);
                self->playFlySfx = 0;
            } else {
                self->playFlySfx = 1;
            }
        }
        if ((void*)self->actionFunc == (void*)EnBili_Recoil) {
            FUN_0033bd9c(thisx);
        } else {
            Actor_MoveForward(thisx);
        }
        Actor_UpdateBgCheckInfo(play, thisx, 5.0f, self->collider.dim.radius, self->collider.dim.height, 7);
        Collider_UpdateCylinder(thisx, &self->collider);
        if ((self->collider.base.atFlags & AT_ON) && self->unk_66C == 0) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
        }
        if (self->collider.base.acFlags & AC_ON) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
        }
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
        Actor_SetFocus(thisx, 0.0f);
    }
}
#endif

}
