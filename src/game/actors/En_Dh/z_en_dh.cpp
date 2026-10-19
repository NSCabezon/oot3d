extern "C" {
#include "z_en_dh.hpp"

#define FLAGS 0x00000415
#define FALLTHROUGH
#define DMG_DEFAULT 0xFFCFFFFF

void EnDh_Init(Actor* thisx, PlayState* play);
void EnDh_Destroy(Actor* thisx, PlayState* play);
void EnDh_Update(Actor* thisx, PlayState* play);
void EnDh_Draw(Actor* thisx, PlayState* play);

void FUN_00373d0c(PlayState* play);
void FUN_00350f34(Actor* actor, ...);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Collider_DestroyJntSph(PlayState* play, void* collider);
void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);
void FUN_002b6748(EnDh* self);
void EffectSsHahen_Spawn(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, s16 arg4, s16 scale, s16 arg6, s16 arg7, void* dList);
void Animation_MorphToPlayOnce(SkelAnime* skelAnime, s32 idx, f32 morph);
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);
void Audio_PlayActorSound2(Actor* actor, s32 arg1);
f32 Math_SinF(f32 angle);
f32 Math_CosF(f32 angle);
void Actor_ChangeType(PlayState* play, ActorContext* actorCtx, Actor* actor, u8 actorCategory);
void Collider_UpdateSpheres(s32 index, void* collider, MTX34* mtx);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);
void FUN_003586ec(SkeletonAnimationModel_unk_0C* anim);
extern s32 gUnk_0054C8CC; // src/game/shared_syms.s
void EnDh_SetupWalk(EnDh* self);
void EnDh_Walk(EnDh* self, PlayState* play);
void EnDh_Attack(EnDh* self, PlayState* play);
void EnDh_Wait(EnDh* self, PlayState* play);
void EnDh_SpawnDebris(PlayState* play, EnDh* self, Vec3f* spawnPos, f32 spread, s32 arg4, f32 accelXZ, f32 scale);

void EnDh_Retreat(EnDh* self, PlayState* play);
void EnDh_Burrow(EnDh* self, PlayState* play);
void EnDh_Damage(EnDh* self, PlayState* play);
void EnDh_Death(EnDh* self, PlayState* play);

GLOBAL_ASM("../src/game/actors/En_Dh/data_syms.s")
extern ColliderCylinderInit EnDh_sCylinderInit;
extern char EnDh_sJntSphInit[];
extern DamageTable EnDh_sDamageTable;
extern InitChainEntry EnDh_sInitChain[];
void Collider_InitJntSph(PlayState* play, void* collider);
void Collider_SetJntSph(PlayState* play, void* collider, Actor* actor, void* init, void* elements);
void FUN_0034f724(PlayState* play);
s32 Actor_GetCollidedExplosive(PlayState* play, Collider* collider);
void FUN_003302a0(PlayState* play, Actor* actor);

void Animation_PlayOnce(SkelAnime* skelAnime, s32 idx);
GLOBAL_ASM("data/z_En_Dh.data.s")

ActorInit En_Dh_InitVars = {
    ACTOR_EN_DH,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_DH,
    sizeof(EnDh),
    (ActorFunc)EnDh_Init,
    (ActorFunc)EnDh_Destroy,
    (ActorFunc)EnDh_Update,
    (ActorFunc)EnDh_Draw,
};

static inline void EnDh_SetupWait(EnDh* self) {
    Animation_PlayLoop(&self->skelAnime, 3);
    {
        SkeletonAnimationModel_unk_0C* anim = self->unk_934;
        f32 zero = 0.0f;

        if (gUnk_0054C8CC == 0) {
            *(f32*)&anim->curFrame = zero;
            FUN_003586ec(anim);
        }
    }
    self->curAction = 0;
    self->actor.world.pos.x = Rand_CenteredFloat(600.0f) + self->actor.home.pos.x;
    self->actor.world.pos.z = Rand_CenteredFloat(600.0f) + self->actor.home.pos.z;
    self->actor.shape.yOffset = -15000.0f;
    self->actor.speedXZ = 0.0f;
    self->dirtWaveSpread = 0.0f;
    self->actor.params = 0;
    self->actionState = 0;
    self->dirtWavePhase = 0;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->actor.flags |= 0x80;
    self->actionFunc = EnDh_Wait;
}

void EnDh_Init(Actor* thisx, PlayState* play) {
    EnDh* self = (EnDh*)thisx;
    ZARInfo* zar;

    Actor_ProcessInitChain(&self->actor, EnDh_sInitChain);
    self->actor.colChkInfo.damageTable = &EnDh_sDamageTable;
    zar = FUN_00372f38(&self->actor, play, &self->skelAnimModel, 2, 0);
    self->unk_934 = self->skelAnimModel->unk_0C;
    self->unk_934->FUN_00372d94(zar->GetCMABByIndex(0));
    self->unk_934->SetAnimSpeed(2.0f);
    self->unk_934->SetAnimType(1);
    FUN_00353c9c(&self->actor, play, &self->skelAnime, 1, 1, &self->unk_228, (u8*)self + 0x534, 15);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 64.0f);
    thisx->params = 0;
    self->actor.colChkInfo.mass = 0xFE;
    if (gSaveContext.linkAge == 0) {
        thisx->colChkInfo.health = 14;
    } else {
        thisx->colChkInfo.health = 20;
    }
    self->alpha = self->unk_258 = 255;
    thisx->flags &= ~1;
    Collider_InitCylinder(play, &self->collider1);
    Collider_SetCylinder(play, &self->collider1, &self->actor, &EnDh_sCylinderInit);
    Collider_InitJntSph(play, &self->colliderJntSph);
    Collider_SetJntSph(play, &self->colliderJntSph, &self->actor, EnDh_sJntSphInit, self->colliderJntSphElements);
    EnDh_SetupWait(self);
}

void EnDh_Destroy(Actor* thisx, PlayState* play) {
    EnDh* self = (EnDh*)thisx;

    FUN_00373d0c(play);
    SkelAnime_Free2(&self->skelAnime);
    FUN_00350f34(thisx, &self->skelAnimModel, NULL);
    Collider_DestroyCylinder(play, &self->collider1);
    Collider_DestroyJntSph(play, &self->colliderJntSph);
}

void EnDh_SetupWalk(EnDh* self) {
    Animation_Change_00375c08(&self->skelAnime, 3, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 3) - 3.0f,
                              0, -6.0f);
    self->actor.speedXZ = 1.0f;
    self->curAction = 3;
    self->timer = 450;
    self->actionFunc = EnDh_Walk;
}

void FUN_00233b14(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    f32 x = 2000.0f;
    f32 y = 1000.0f;
    f32 z = 0.0f;
    Vector3f headOffset(x, y, z);
    EnDh* self = (EnDh*)thisx;

    if (limbIndex == 13) {
        MTX34 tmp;
        Vec3f t;

        {
            Vector3f* hp = (Vector3f*)&self->headPos;
            Vector3f* ho = &headOffset;

            VEC3TransformAsm(hp, mtx, ho);
        }
        MTX34CopyAsm(&tmp, mtx);
        {
            f32 x = headOffset.x;
            f32 y = headOffset.y;
            f32 z = headOffset.z;

            t.x = x;
            t.y = y;
            t.z = z;
        }
        Matrix_UpdatePosition(&tmp, &tmp, &t);
        Collider_UpdateSpheres(1, &self->colliderJntSph, &tmp);
    }
}

void EnDh_Wait(EnDh* self, PlayState* play) {
    EnDh* s2 = self;

    if ((s32)self->skelAnime.curFrame == 5) {
        FUN_0034f724(play);
    }
    if (Actor_GetCollidedExplosive(play, &self->collider1.base)) {
        self->actor.params = 5;
    }
    if ((self->actor.params > 0) || (self->actor.params <= -4)) {
        switch (self->actionState) {
            case 0:
                self->actor.flags |= 1;
                self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
                self->actor.flags &= ~0x80;
                self->actionState++;
                self->drawDirtWave++;
                Audio_PlayActorSound2(&self->actor, 0x01000418);
                FALLTHROUGH;
            case 1:
                s2->dirtWavePhase += 0x26F;
                Math_SmoothStepToF(&self->dirtWaveSpread, 300.0f, 1.0f, 5.0f, 0.0f);
                self->dirtWaveHeight = Math_SinS(self->dirtWavePhase) * 55.0f;
                self->dirtWaveAlpha = (s16)(Math_SinS(self->dirtWavePhase) * 255.0f);
                EnDh_SpawnDebris(play, self, &self->actor.world.pos, self->dirtWaveSpread, 4, 2.05f, 1.2f);
                if (self->actor.shape.yOffset == 0.0f) {
                    self->actionState++;
                    self->drawDirtWave = false;
                } else if (self->dirtWavePhase > 0x12C0) {
                    self->actor.shape.yOffset += 500.0f;
                }
                break;
            case 2:
                EnDh_SetupWalk(self);
                break;
        }
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0x7D0, 0);
        FUN_003731e0(&self->skelAnime);
        if (self->actor.params != 5) {
            FUN_003302a0(play, &self->actor);
        }
    }
}
void EnDh_Walk(EnDh* self, PlayState* play) {
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0xFA, 0);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    FUN_003731e0(&self->skelAnime);
    if (((s32)self->skelAnime.curFrame % 8) == 0) {
        Audio_PlayActorSound2(&self->actor, 0x01000371);
    }
    if ((*(u32*)((u8*)play + 0x5BF4) & 0x5F) == 0) {
        Audio_PlayActorSound2(&self->actor, 0x01000417);
    }
    if (self->actor.xzDistToPlayer <= 100.0f) {
        self->actor.speedXZ = 0.0f;
        if (Actor_IsFacingPlayer(&self->actor, 0x2AAA)) {
            Animation_MorphToPlayOnce(&self->skelAnime, 5, -6.0f);
            self->actionState = 0;
            self->timer = 0;
            self->actor.speedXZ = 0.0f;
            self->curAction = 4;
            self->actionFunc = EnDh_Attack;
        }
    } else if (--self->timer == 0) {
        Animation_MorphToPlayOnce(&self->skelAnime, 2, -6.0f);
        {
            SkeletonAnimationModel_unk_0C* anim = self->unk_934;
            f32 zero = 0.0f;

            if (gUnk_0054C8CC == 0) {
                *(f32*)&anim->curFrame = zero;
                FUN_003586ec(anim);
            }
        }
        self->curAction = 2;
        self->actor.speedXZ = 0.0f;
        self->dirtWaveSpread = 0.0f;
        self->dirtWavePhase = 0;
        self->actionState = 0;
        self->actor.world.rot.y = self->actor.shape.rot.y;
        self->actor.flags &= ~1;
        Audio_PlayActorSound2(&self->actor, 0x01000418);
        self->actionFunc = EnDh_Burrow;
    }
}

void EnDh_Retreat(EnDh* self, PlayState* play) {
    self->timer--;
    if (self->timer == 0) {
        self->retreat = false;
        Animation_MorphToPlayOnce(&self->skelAnime, 2, -6.0f);
        {
            SkeletonAnimationModel_unk_0C* anim = self->unk_934;
            f32 zero = 0.0f;

            if (gUnk_0054C8CC == 0) {
                *(f32*)&anim->curFrame = zero;
                FUN_003586ec(anim);
            }
        }
        self->curAction = 2;
        self->actor.speedXZ = 0.0f;
        self->dirtWaveSpread = 0.0f;
        self->dirtWavePhase = 0;
        self->actionState = 0;
        self->actor.world.rot.y = self->actor.shape.rot.y;
        self->actor.flags &= ~1;
        Audio_PlayActorSound2(&self->actor, 0x01000418);
        self->actionFunc = EnDh_Burrow;
    } else {
        Math_SmoothStepToS(&self->actor.shape.rot.y, (s16)(self->actor.yawTowardsPlayer + 0x8000), 1, 0xBB8, 0);
    }
    self->actor.world.rot.y = self->actor.shape.rot.y;
    FUN_003731e0(&self->skelAnime);
}

#define EL ((ColliderInfo*)self->colliderJntSph.elements)

void EnDh_Attack(EnDh* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime)) {
        self->actionState++;
    } else if ((self->actor.xzDistToPlayer > 100.0f) || !Actor_IsFacingPlayer(&self->actor, 0x2AAA)) {
        Animation_Change_00375c08(&self->skelAnime, 5, -1.0f, self->skelAnime.curFrame, 0.0f, 2, -4.0f);
        self->actionState = 4;
        self->colliderJntSph.base.atFlags = EL->toucherFlags = AT_NONE;
        EL->toucher.dmgFlags = EL->toucher.damage = 0;
    }
    switch (self->actionState) {
        case 1:
            Animation_PlayOnce(&self->skelAnime, 6);
            self->actionState++;
            Audio_PlayActorSound2(&self->actor, 0x01000370);
            FALLTHROUGH;
        case 0:
            Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0x5DC, 0);
            break;
        case 2:
            if (self->skelAnime.curFrame >= 4.0f) {
                self->colliderJntSph.base.atFlags = AT_ON | AT_TYPE_ENEMY;
                EL->toucherFlags = 1;
                EL->toucher.dmgFlags = DMG_DEFAULT;
                EL->toucher.damage = 8;
            }
            if (self->colliderJntSph.base.atFlags & AT_BOUNCED) {
                self->colliderJntSph.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
                self->colliderJntSph.base.atFlags = EL->toucherFlags = AT_NONE;
                EL->toucher.dmgFlags = EL->toucher.damage = 0;
                self->actionState++;
            } else if (self->colliderJntSph.base.atFlags & AT_HIT) {
                self->colliderJntSph.base.atFlags &= ~AT_HIT;
                FUN_00374bb8(play, &self->actor, 8.0f, self->actor.shape.rot.y, 8.0f);
            }
            break;
        case 3:
            if ((self->actor.xzDistToPlayer <= 100.0f) && Actor_IsFacingPlayer(&self->actor, 0x2AAA)) {
                Animation_Change_00375c08(&self->skelAnime, 5, 1.0f, 20.0f, Animation_GetLastFrame(&self->skelAnime, 5), 2, -6.0f);
                self->actionState = 0;
            } else {
                Animation_Change_00375c08(&self->skelAnime, 5, -1.0f, Animation_GetLastFrame(&self->skelAnime, 5), 0.0f, 2, -4.0f);
                self->actionState++;
                self->colliderJntSph.base.atFlags = EL->toucherFlags = AT_NONE;
                EL->toucher.dmgFlags = EL->toucher.damage = 0;
            }
            break;
        case 5:
            EnDh_SetupWalk(self);
            break;
        case 4:
            break;
    }
    self->actor.world.rot.y = self->actor.shape.rot.y;
}

void EnDh_Burrow(EnDh* self, PlayState* play) {
    switch (self->actionState) {
        case 0:
            self->actionState++;
            self->drawDirtWave++;
            self->collider1.base.atFlags = self->collider1.info.toucherFlags = AT_ON | AT_TYPE_ENEMY;
            self->collider1.info.toucher.dmgFlags = DMG_DEFAULT;
            self->collider1.info.toucher.damage = 4;
            FALLTHROUGH;
        case 1:
            self->dirtWavePhase += 0x2FF;
            Math_SmoothStepToF(&self->dirtWaveSpread, 300.0f, 1.0f, 8.0f, 0.0f);
            self->dirtWaveHeight = Math_SinS(self->dirtWavePhase) * 55.0f;
            self->dirtWaveAlpha = (s16)(Math_SinS(self->dirtWavePhase) * 255.0f);
            EnDh_SpawnDebris(play, self, &self->actor.world.pos, self->dirtWaveSpread, 4, 2.05f, 1.2f);
            self->collider1.dim.radius = (s16)(self->dirtWaveSpread * 0.6f);
            if (FUN_003731e0(&self->skelAnime)) {
                self->actionState++;
            }
            break;
        case 2:
            self->drawDirtWave = false;
            self->collider1.dim.radius = 35;
            self->collider1.base.atFlags = self->collider1.info.toucherFlags = AT_NONE;
            self->collider1.info.toucher.dmgFlags = self->collider1.info.toucher.damage = 0;
            FUN_002b6748(self);
            break;
    }
}

void EnDh_Damage(EnDh* self, PlayState* play) {
    if (self->actor.speedXZ < 0.0f) {
        self->actor.speedXZ += 0.15f;
    }
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    if (FUN_003731e0(&self->skelAnime)) {
        self->actor.world.rot.y = self->actor.shape.rot.y;
        if (self->retreat) {
            Animation_MorphToLoop(&self->skelAnime, 1, -4.0f);
            self->curAction = 1;
            self->timer = 105;
            self->actor.speedXZ = 1.0f;
            self->actionFunc = EnDh_Retreat;
        } else if ((self->actor.xzDistToPlayer <= 105.0f) && Actor_IsFacingPlayer(&self->actor, 0x2AAA)) {
            f32 frames = (f32)Animation_GetLastFrame(&self->skelAnime, 5);

            Animation_MorphToPlayOnce(&self->skelAnime, 5, -6.0f);
            self->actionState = 0;
            self->timer = 0;
            self->actor.speedXZ = 0.0f;
            self->curAction = 4;
            self->actionFunc = EnDh_Attack;
            Animation_Change_00375c08(&self->skelAnime, 5, 1.0f, 20.0f, frames, 2, -6.0f);
        } else {
            EnDh_SetupWalk(self);
        }
        self->unk_258 = 255;
    }
}

void EnDh_Death(EnDh* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime) || (self->timer != 450)) {
        if (self->timer == 450) {
            Animation_PlayLoop(&self->skelAnime, 8);
        }
        self->timer--;
        if (self->timer < 225) {
            if (self->alpha != 0) {
                self->actor.scale.y -= 0.000075f;
                self->actor.shape.shadowAlpha = self->alpha -= 5;
            } else {
                Actor_Kill(&self->actor);
                return;
            }
        }
    } else {
        if (((s32)self->skelAnime.curFrame == 53) || ((s32)self->skelAnime.curFrame == 56) ||
            ((s32)self->skelAnime.curFrame == 61)) {
            Audio_PlayActorSound2(&self->actor, 0x010002EA);
        }
        if ((s32)self->skelAnime.curFrame == 61) {
            Actor_ChangeType(play, &play->actorCtx, &self->actor, 6);
        }
    }
}

#if 0 // draft: 1 word off (0x48: accel.x takes the vel.x zero copy s1 instead of the vel.z copy s2; ~40 variants). Still asm.
void EnDh_SpawnDebris(PlayState* play, EnDh* self, Vec3f* spawnPos, f32 spread, s32 arg4, f32 accelXZ, f32 scale) {
    Vec3f pos;
    Vector3f vel(0.0f, 8.0f, 0.0f);
    Vec3f accel;
    f32 spreadAngle;
    f32 scaleMod;
    PlayState* pl = play;

    accel.x = 0.0f;
    accel.y = -1.5f;
    accel.z = 0.0f;
    spreadAngle = (Rand_ZeroOne() - 0.5f) * 6.28f;
    pos.y = self->actor.floorHeight;
    pos.x = (Math_SinF(spreadAngle) * spread) + spawnPos->x;
    pos.z = (Math_CosF(spreadAngle) * spread) + spawnPos->z;
    accel.x = (Rand_ZeroOne() - 0.5f) * accelXZ;
    accel.z = (Rand_ZeroOne() - 0.5f) * accelXZ;
    vel.y += (Rand_ZeroOne() - 0.5f) * 4.0f;
    scaleMod = (Rand_ZeroOne() * 5.0f) + 12.0f;
    EffectSsHahen_Spawn(pl, &pos, (Vec3f*)&vel, &accel, (s16)arg4, scaleMod * scale, -1, 10, NULL);
}
#endif

GLOBAL_ASM("asm/EnDh_SpawnDebris.s")
#if 0 // draft: all of EnDh_Update matches except the player->unk_844/845 reads (+0x2228/0x2229): ROM uses `ldr r2,=0x2228; ldrb [r2,r6]` (const reg, `orr r0,r2,r2,asr #13` for +1) while armcc folds `add r0,r6,#0x2000; ldrb [r0,#0x228]`. Cascades into r0/r1 zero-reg choice. ~15 variants. Still asm.
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
s32 Actor_ApplyDamage(Actor* actor);
void Item_DropCollectibleRandom(PlayState* play, Actor* fromActor, Vec3f* spawnPos, s16 params);
void FUN_00373bec(SkeletonAnimationModel_unk_0C* anim);
void EnDh_Update(Actor* thisx, PlayState* play) {
    EnDh* self = (EnDh*)thisx;
    u8* player = (u8*)GET_PLAYER(play);

    if ((self->colliderJntSph.base.acFlags & AC_HIT) && !self->retreat) {
        self->colliderJntSph.base.acFlags &= ~AC_HIT;
        if ((self->actor.colChkInfo.damageEffect != 0) && (self->actor.colChkInfo.damageEffect != 6) &&
            ((self->actionFunc != EnDh_Burrow) && (self->actionFunc != EnDh_Walk) && (self->actionFunc != EnDh_Wait)) &&
            !(self->actor.flags & 0x80)) {
            s32 lastHealth;

            self->colliderJntSph.base.atFlags = EL->toucherFlags = AT_NONE;
            EL->toucher.dmgFlags = EL->toucher.damage = 0;
            if (*(player + 0x2228) != 0) {
                self->unk_258 = *(player + 0x2229);
            }
            Actor_SetColorFilter(&self->actor, 0x400000, 255, 0, 16);
            lastHealth = self->actor.colChkInfo.health;
            if (Actor_ApplyDamage(&self->actor) == 0) {
                Animation_MorphToPlayOnce(&self->skelAnime, 7, -1.0f);
                self->actor.speedXZ = 0.0f;
                self->curAction = 5;
                self->timer = 450;
                self->actor.flags &= ~1;
                FUN_00373d0c(play);
                self->actor.params = 10;
                Audio_PlayActorSound2(&self->actor, 0x0100041B);
                self->actionFunc = EnDh_Death;
                Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos, 0x90);
            } else {
                if (((lastHealth > 14) && (self->actor.colChkInfo.health <= 14)) ||
                    ((lastHealth > 8) && (self->actor.colChkInfo.health <= 8)) ||
                    ((lastHealth > 2) && (self->actor.colChkInfo.health <= 2))) {
                    self->retreat++;
                }
                Animation_MorphToPlayOnce(&self->skelAnime, 4, -6.0f);
                if (self->actor.bgCheckFlags & 1) {
                    self->actor.speedXZ = -1.0f;
                }
                Audio_PlayActorSound2(&self->actor, 0x01000419);
                self->curAction = 6;
                self->actionFunc = EnDh_Damage;
            }
        }
    }
    if (self->unk_843 == 0 || --self->unk_843 == 0) {
        if (self->colliderJntSph.base.atFlags & AT_HIT) {
            self->colliderJntSph.base.atFlags &= ~AT_HIT;
            self->unk_843 = 29;
        }
    }
    self->actionFunc(self, play);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 20.0f, 45.0f, 45.0f, 0x1D);
    self->actor.focus.pos = *(Vec3f*)&self->headPos;
    Collider_UpdateCylinder(&self->actor, &self->collider1);
    if (self->actor.colChkInfo.health > 0) {
        if (self->curAction == 0) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider1.base);
        } else {
            CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider1.base);
        }
        if (((self->curAction != 6) && (self->actor.shape.yOffset == 0.0f)) ||
            ((*(player + 0x2228) != 0) && (*(player + 0x2229) != self->unk_258))) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &self->colliderJntSph.base);
            if (self->unk_843 == 0) {
                CollisionCheck_SetAT(play, &play->colChkCtx, &self->colliderJntSph.base);
            }
            CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider1.base);
        }
    } else {
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider1.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->colliderJntSph.base);
    }
    self->actor.targetArrowOffset = ((self->actor.focus.pos.y - 10.0f) * -16.67f) + 1000.0f;
    if (self->drawDirtWave) {
        FUN_00373bec(self->unk_934);
    }
}
#endif

GLOBAL_ASM("asm/EnDh_Update.s")

#if 0 // draft: SetUnkAC family (self must be r4 with `mov r5,#1` kept for SetUnkAC) + per-multiply scale copy (vmov s2,s0; vmul s2,s3,s2) not reproduced. ~77/106 words off. Still asm.
typedef struct { f32 r, g, b, a; } EnDhColorF;
void FUN_00357388(Actor* actor, EnDhColorF* color, s32 arg2, s32 arg3);
void EnDh_Draw(Actor* thisx, PlayState* play) {
    EnDh* self = (EnDh*)thisx;
    EnDhColorF color = { 0.0f, 0.0f, 0.0f, 0.0f };

    color.a = (f32)self->alpha * (1.0f / 255.0f);
    FUN_00357388(thisx, &color, 4, 0);
    SkelAnime_DrawOpa(&self->skelAnime, (MTX34*)&self->actor.modelMtx, NULL, FUN_00233b14, thisx, 0);
    if (self->drawDirtWave) {
        MTX34* mtx = &self->actor.modelMtx;
        Vec3f t;
        f32 sx;
        f32 sy;

        t.x = 0.0f;
        t.y = -self->actor.shape.yOffset;
        t.z = 0.0f;
        Matrix_UpdatePosition(mtx, mtx, &t);
        sx = self->dirtWaveSpread * 0.01f;
        sy = self->dirtWaveHeight * 0.01f;
        mtx->data[0][0] *= sx;
        mtx->data[1][0] *= sx;
        mtx->data[2][0] *= sx;
        mtx->data[0][1] *= sy;
        mtx->data[1][1] *= sy;
        mtx->data[2][1] *= sy;
        mtx->data[0][2] *= sx;
        mtx->data[1][2] *= sx;
        mtx->data[2][2] *= sx;
        FUN_003695cc(self->skelAnimModel, 0, 4, 2, 0.0f, 0.0f, 0.0f, self->dirtWaveAlpha * (1.0f / 255.0f));
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(mtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
#endif

GLOBAL_ASM("asm/EnDh_Draw.s")

}
