extern "C" {
#include "z_en_ny.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x00000005
#define BGCHECKFLAG_GROUND_TOUCH 0x02

f32 sinf(f32 x);
f32 cosf(f32 x);
static __inline __pure f32 EnNy_Fabsf(f32 x) {
    return __fabsf(x);
}

#define R_UPDATE_RATE SREG(30)
#define FRAMES(n) ((n) * 3.0f / R_UPDATE_RATE + 0.5f)

extern f32 EnNy_sWaterStep;
extern Vec3f EnNy_sFireOffsets[4];
extern InitChainEntry EnNy_sInitChain[];
extern DamageTable EnNy_sDamageTable;
extern char EnNy_sColliderInit[];

void Collider_InitJntSph(PlayState* play, EnNyColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, EnNyColliderJntSph* collider, Actor* actor, void* init, void* elements);

s32 Rand_S16Offset(s16 base, s16 range);
void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* pos, s32 params);
void EffectSsBubble_Spawn(PlayState* play, Vec3f* pos, f32 a, f32 b, f32 c, f32 scale);
void EffectSsDtBubble_SpawnColorProfile(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, s32 scale, s32 life,
                                        s32 colorProfile, s32 flag);
void EffectSsEnFire_SpawnVec3f(PlayState* play, Actor* actor, Vec3f* pos, s32 scale, s32 flags, s32 flags2,
                               s32 arg, s32 arg2);
void FUN_00371fac(MTX34* mtx, void* billboard);
void FUN_00375b70(PlayState* play, Actor* actor);
void FUN_003741e4(PlayState* play, u32 dmgFlags, s32 idx, Vec3f* pos, s32 zero);
u8 Actor_ApplyDamage(Actor* actor);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
void Collider_UpdateSpheres(s32 index, EnNyColliderJntSph* collider, MTX34* mtx);
void FUN_0016761c(EnNy* self, PlayState* play);
void FUN_00167204(Actor* thisx, PlayState* play);
void FUN_00130dc0(EnNy* self, PlayState* play);
void FUN_00114930(EnNy* self, PlayState* play);
s32 Collider_DestroyJntSph(PlayState* play, EnNyColliderJntSph* collider);
void FUN_003b3ea8(EnNy* self, PlayState* play);
void FUN_003cf52c(EnNy* self, PlayState* play);
void FUN_003cf564(EnNy* self, PlayState* play);
void FUN_003ce888(EnNy* self, PlayState* play);
void FUN_00103794(EnNy* self, PlayState* play);
void FUN_003af7a0(EnNy* self, PlayState* play);
void FUN_003ae2a0(EnNy* self, PlayState* play);
void FUN_001dc50c(EnNy* self, PlayState* play2);

static inline void EnNy_SetupAction(EnNy* self, EnNyActionFunc func) {
    self->actionFunc = func;
}

void EnNy_Init(Actor* actor, GameState* state);
void EnNy_Destroy(Actor* thisx, PlayState* play);
void EnNy_Update(Actor* actor, GameState* state);
void EnNy_Draw(Actor* actor, PlayState* play);

GLOBAL_ASM("../src/game/actors/En_Ny/data_syms.s")
GLOBAL_ASM("data/z_En_Ny.data.s")

ActorInit En_Ny_InitVars = {
    ACTOR_EN_NY,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_NY,
    sizeof(EnNy),
    (ActorFunc)EnNy_Init,
    (ActorFunc)EnNy_Destroy,
    (ActorFunc)EnNy_Update,
    (ActorFunc)EnNy_Draw,
};

void EnNy_Init(Actor* thisx, GameState* state) {
    EnNy* self = (EnNy*)thisx;
    PlayState* play = (PlayState*)state;
    ZARInfo* zar;
    void* cmab;
    s32 i;

    Actor_ProcessInitChain(thisx, EnNy_sInitChain);
    thisx->colChkInfo.damageTable = &EnNy_sDamageTable;
    thisx->colChkInfo.health = 2;
    Collider_InitJntSph(play, &self->collider);
    Collider_SetJntSph(play, &self->collider, thisx, EnNy_sColliderInit, self->unk_1C8);
    ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, 20.0f);
    zar = FUN_00372f38(thisx, play, &self->models[0], 0, &self->models[1], 1, &self->models[2], 2, &self->models[3], 0,
                       &self->models[4], 0, &self->models[5], 0, &self->models[6], 0, &self->models[7], 0,
                       &self->models[8], 0, &self->models[9], 0, &self->models[10], 0, &self->models[11], 3,
                       &self->models[12], 3, &self->models[13], 3, &self->models[14], 3, 0);
    cmab = zar->GetCMABByIndex(0);
    for (i = 0; i < 4; i++) {
        SkeletonAnimationModel* m = self->models[i + 11];
        m->unk_0C->FUN_00372d94(cmab);
        self->models[i + 11]->unk_0C->animType = 1;
        self->models[i + 11]->unk_0C->animSpeed = 2.0f;
    }
    self->unk_1CA = 0;
    self->unk_1D0 = 0;
    Actor_SetScale(thisx, 0.01f);
    thisx->shape.rot.y = 0;
    thisx->speedXZ = 0.0f;
    thisx->gravity = -0.4f;
    thisx->velocity.y = 0.0f;
    self->unk_1E8 = 0.0f;
    self->hitPlayer = 0;
    self->unk_1CE = 2;
    self->unk_1D4 = 0xFF;
    self->unk_1D8 = 0;
    self->unk_1E0 = 0.25f;
    if (thisx->params == 0) {
        self->actor.colChkInfo.mass = 0;
        self->unk_1D4 = 0;
        self->unk_1D8 = 0xFF;
        self->unk_1E0 = 1.0f;
        self->unk_1F4 = 0.0f;
        self->unk_1E8 = ((self->actor.yDistToWater > 0.0f) ? 0.7f : 1.0f) * 2.8f;
        self->stoneTimer = 0x10E;
        EnNy_SetupAction(self, FUN_003ae2a0);
    } else {
        self->actor.colChkInfo.mass = 0xFF;
        self->actor.update = (ActorFunc)FUN_001dc50c;
        self->collider.base.colType = 9;
    }
}

void EnNy_Destroy(Actor* thisx, PlayState* play) {
    EnNy* self = (EnNy*)thisx;

    Collider_DestroyJntSph(play, &self->collider);
    FUN_00350f34(thisx, &self->models[0], &self->models[1], &self->models[2], &self->models[3],
                 &self->models[4], &self->models[5], &self->models[6], &self->models[7], &self->models[8],
                 &self->models[9], &self->models[10], &self->models[11], &self->models[12],
                 &self->models[13], &self->models[14], 0);
}

void FUN_003b3ea8(EnNy* self, PlayState* play) {
    if (self->actor.xyzDistToPlayerSq <= SQ(160.0f)) {
        self->stoneTimer = 0x1E;
        self->actionFunc = FUN_003cf52c;
    }
}

void FUN_003cf52c(EnNy* self, PlayState* play) {
    s32 phi_v1 = self->unk_1D4 - 0x40;
    s32 phi_v0 = self->unk_1D8 + 0x40;

    if (phi_v0 >= 0xFF) {
        phi_v1 = 0;
        phi_v0 = 0xFF;
        self->actionFunc = FUN_003cf564;
    }
    self->unk_1D4 = phi_v1;
    self->unk_1D8 = phi_v0;
}

void FUN_003cf564(EnNy* self, PlayState* play) {
    f32 phi_f0 = self->unk_1E0;

    phi_f0 += 2.0f;
    if (phi_f0 >= 1.0f) {
        phi_f0 = 1.0f;
        FUN_003ce888(self, play);
    }
    self->unk_1E0 = phi_f0;
}

void FUN_00103794(EnNy* self, PlayState* play) {
    s32 phi_v0 = self->unk_1D4 + 0x40;
    s32 phi_v1 = self->unk_1D8 - 0x40;

    if (phi_v0 >= 0xFF) {
        phi_v0 = 0xFF;
        phi_v1 = 0;
        if (self->stoneTimer != 0) {
            self->stoneTimer--;
        } else {
            self->actionFunc = FUN_003b3ea8;
        }
    }
    self->unk_1D4 = phi_v0;
    self->unk_1D8 = phi_v1;
}

void FUN_003af7a0(EnNy* self, PlayState* play) {
    f32 phi_f0 = self->unk_1E0;

    phi_f0 -= 2.0f;
    if (phi_f0 <= 0.25f) {
        phi_f0 = 0.25f;
        if (self->actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
            if (self->actor.yDistToWater <= self->unk_1F0) {
                Audio_PlayActorSound2(&self->actor, 0x010002EB);
            }
            self->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND_TOUCH;
            self->actor.speedXZ = 0.0f;
            {
                s16 rotY = self->actor.shape.rot.y;

                self->actor.world.rot.y = rotY;
            }
            self->stoneTimer = 0x5A;
            self->actionFunc = FUN_00103794;
        }
    }
    self->unk_1E0 = phi_f0;
}

void FUN_003ae2a0(EnNy* self, PlayState* play) {
    f32 yawDiff;
    s32 stoneTimer;

    if (self->actor.yDistToWater <= self->unk_1F0) {
        FUN_00373264(&self->actor, 0x01000359);
    }
    self->unk_1E8 = ((self->actor.yDistToWater > 0.0f) ? 0.7f : 1.0f) * 2.8f;
    stoneTimer = self->stoneTimer;
    self->stoneTimer--;
    if ((stoneTimer <= 0) || self->hitPlayer) {
        Audio_PlayActorSound2(&self->actor, 0x0100035A);
        self->unk_1E8 = 0.0f;
        self->actionFunc = FUN_003af7a0;
    } else {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 0xA, (s16)self->unk_1F4, 0);
        Math_ApproachF(&self->unk_1F4, 2000.0f, 1.0f, 100.0f);
        self->actor.world.rot.y = self->actor.shape.rot.y;
        yawDiff = Math_FAtan2F(self->actor.yDistToPlayer, self->actor.xzDistToPlayer);
        self->actor.speedXZ = EnNy_Fabsf(cosf(yawDiff) * self->unk_1E8);
        if (self->actor.yDistToWater > self->unk_1F0) {
            self->unk_1EC = sinf(yawDiff) * self->unk_1E8;
        }
    }
}

void FUN_001dc50c(EnNy* self, PlayState* play2) {
    PlayState* play = play2;
    f32 sp3C = self->unk_1E0 - 0.25f;
    f32 temp_f0 = (24.0f * sp3C) + 12.0f;

    self->timer++;
    Actor_SetFocus(&self->actor, 0.0f);
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.prevPos.y -= temp_f0;
    self->actor.world.pos.y -= temp_f0;
    Actor_UpdateBgCheckInfo(play, &self->actor, 20.0f, 20.0f, 60.0f, 7);
    self->actor.world.pos.y += temp_f0;
    self->unk_1F0 = temp_f0;
    CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    Actor_MoveForward(&self->actor);
    Math_StepToF(&self->unk_1E4, self->unk_1E8, 0.1f);
}

void FUN_00130dc0(EnNy* self, PlayState* play) {
    s32 i;

    if (self->actor.yDistToWater > 0.0f) {
        for (i = 0; i < 8; i++) {
            self->unk_1F8[i].x += self->unk_1F8[i + 8].x;
            self->unk_1F8[i].y += self->unk_1F8[i + 8].y;
            self->unk_1F8[i].z += self->unk_1F8[i + 8].z;
            Math_StepToF(&self->unk_1F8[i + 8].x, 0.0f, 0.1f * EnNy_sWaterStep);
            Math_StepToF(&self->unk_1F8[i + 8].y, -1.0f, 0.4f * EnNy_sWaterStep);
            Math_StepToF(&self->unk_1F8[i + 8].z, 0.0f, 0.1f * EnNy_sWaterStep);
        }
        for (i = 0; i < 4; i++) {
            self->unk_320[i].x += self->unk_320[i + 4].x;
            self->unk_320[i].y += self->unk_320[i + 4].y;
            self->unk_320[i].z += self->unk_320[i + 4].z;
            Math_StepToF(&self->unk_320[i + 4].x, 0.0f, 0.1f * EnNy_sWaterStep);
            Math_StepToF(&self->unk_320[i + 4].y, 0.0f, 0.1f * EnNy_sWaterStep);
            Math_StepToF(&self->unk_320[i + 4].z, 0.0f, 0.1f * EnNy_sWaterStep);
        }
        if (self->timer > (s32)FRAMES(30)) {
            Actor_Kill(&self->actor);
            return;
        }
    } else {
        for (i = 0; i < 8; i++) {
            self->unk_1F8[i].x += self->unk_1F8[i + 8].x;
            self->unk_1F8[i].y += self->unk_1F8[i + 8].y;
            self->unk_1F8[i].z += self->unk_1F8[i + 8].z;
            Math_StepToF(&self->unk_1F8[i + 8].x, 0.0f, 0.15f);
            Math_StepToF(&self->unk_1F8[i + 8].y, -1.0f, 0.6f);
            Math_StepToF(&self->unk_1F8[i + 8].z, 0.0f, 0.15f);
        }
        if (self->timer > ((s32)FRAMES(30) >> 1)) {
            Actor_Kill(&self->actor);
            return;
        }
    }
}

void FUN_00114930(EnNy* self, PlayState* play) {
    s32 effectScale;
    s32 i;
    Vec3f effectPos;
    Vector3f effectVelocity(0.0f, 0.0f, 0.0f);
    Vector3f effectAccel(0.0f, 0.1f, 0.0f);

    if (self->timer > 1) {
        if (self->actor.yDistToWater > 0.0f) {
            for (i = 0; i < 10; i++) {
                effectPos.x = Rand_CenteredFloat(30.0f) + self->actor.world.pos.x;
                effectPos.y = Rand_CenteredFloat(30.0f) + self->actor.world.pos.y;
                effectPos.z = Rand_CenteredFloat(30.0f) + self->actor.world.pos.z;
                effectScale = Rand_S16Offset(0x50, 0x64);
                EffectSsDtBubble_SpawnColorProfile(play, &effectPos, (Vec3f*)&effectVelocity,
                                                   (Vec3f*)&effectAccel, effectScale, 25, 0, 1);
            }
            for (i = 0; i < 0x14; i++) {
                effectPos.x = Rand_CenteredFloat(30.0f) + self->actor.world.pos.x;
                effectPos.y = Rand_CenteredFloat(30.0f) + self->actor.world.pos.y;
                effectPos.z = Rand_CenteredFloat(30.0f) + self->actor.world.pos.z;
                EffectSsBubble_Spawn(play, &effectPos, 10.0f, 10.0f, 30.0f, 0.25f);
            }
        }
        for (i = 0; i < 8; i++) {
            self->unk_1F8[i + 8].x = Rand_CenteredFloat(10.0f);
            self->unk_1F8[i + 8].z = Rand_CenteredFloat(10.0f);
            self->unk_1F8[i + 8].y = Rand_ZeroFloat(4.0f) + 4.0f;
        }
        for (i = 0; i < 4; i++) {
            self->unk_320[i + 4].x = Rand_CenteredFloat(3.0f) - 1.5f;
            self->unk_320[i + 4].z = Rand_CenteredFloat(3.0f) - 1.5f;
            self->unk_320[i + 4].y = Rand_ZeroFloat(1.0f);
        }
        self->timer = 0;
        if (self->unk_1D0 == 0) {
            Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos, 0xA0);
        } else {
            Item_DropCollectible(play, &self->actor.world.pos, 8);
        }
        Audio_PlayActorSound2(&self->actor, 0x010003BB);
        self->actionFunc = FUN_00130dc0;
    }
}

void FUN_0016761c(EnNy* self, PlayState* play) {
    self->timer++;
    if (self->unk_1CA != 0) {
        self->unk_1CA--;
    }
    self->actionFunc(self, play);
}

// Drafts (still GLOBAL_ASM below). 3DS EnNy_Update/EnNy_Draw/FUN_00167204 (death draw):
// - EnNy_Update: 7 insns off (all instructions equal; armcc lays the two velocity-clamp blocks of the inlined gravity
//   helper out differently: ROM branch 1 jumps forward to the clamp placed after branch 2).
// - EnNy_Draw: ~21 insns off (ROM keeps copies `vmov s1,s0; vldr s2; vmul s1,s2,s1` for the x/y columns of the
//   inline Matrix_Scale; z column uses s0 directly; also `ldr r5,=sFireOffsets` is scheduled after the 1st Rand call).
// - FUN_00167204: far (MakeTrans + SetUnkAC 1 hoisting / register pressure; ROM hoists only `1`, no zero/1.0f).
#if 0
static inline void EnNy_ScaleMtx(MTX34* m, f32 x, f32 y, f32 z) {
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

void FUN_00167204(Actor* thisx, PlayState* play) {
    EnNy* self = (EnNy*)thisx;
    s32 i;
    MTX34 mtx;
    Vec3f posA;
    Vec3f posB;
    Vec3f tempVec;
    f32 scale;

    EnNy* s2 = self;
    GraphicsContext_UnknownStub(play->state.gfxCtx);
    for (i = 0; i < 8; i++) {
        if (self->timer < (s32)FRAMES(30) + i - 8) {
            posA.x = self->unk_1F8[i].x;
            posA.y = self->unk_1F8[i].y;
            posA.z = self->unk_1F8[i].z;
            MTX34_MakeTrans(&mtx, &posA);
            scale = self->actor.scale.x * ((1.0f + i * 0.04f) * 0.4f);
            EnNy_ScaleMtx(&mtx, scale, scale, scale);
            self->models[i + 3]->SetUnkAC(1);
            self->models[i + 3]->FUN_003721e0(&mtx);
            s2->models[i + 3]->FUN_00372170(0);
        }
    }
    if (self->actor.yDistToWater > 0.0f) {
        f32 t = (f32)self->timer / (f32)(s32)FRAMES(30);
        f32 ringScale = 100.0f + t * 200.0f;
        f32 alpha = 1.0f - t;

        for (i = 0; i < 4; i++) {
            posB.x = self->unk_320[i].x;
            posB.y = self->unk_320[i].y;
            posB.z = self->unk_320[i].z;
            MTX34_MakeTrans(&mtx, &posB);
            FUN_00371fac(&mtx, (u8*)play + 0x2FC);
            scale = thisx->scale.x * ringScale;
            EnNy_ScaleMtx(&mtx, scale, scale, scale);
            self->models[i + 11]->SetUnkAC(1);
            self->models[i + 11]->FUN_003721e0(&mtx);
            FUN_003695cc(self->models[i + 11], 0, 4, 0, 0.0f, 0.0f, 0.0f, alpha);
            self->models[i + 11]->FUN_00372170(0);
        }
    }
    if (self->unk_1CA != 0) {
        s16 idx = self->unk_1CA - 1;
        Vec3f* fireOffset;

        (*(u16*)((u8*)self + 0x11A))++;
        if ((idx & 1) == 0) {
            fireOffset = &EnNy_sFireOffsets[idx & 3];
            tempVec.x = Rand_CenteredFloat(5.0f) + (self->actor.world.pos.x + fireOffset->x);
            tempVec.y = Rand_CenteredFloat(5.0f) + (self->actor.world.pos.y + fireOffset->y);
            tempVec.z = Rand_CenteredFloat(5.0f) + (self->actor.world.pos.z + fireOffset->z);
            EffectSsEnFire_SpawnVec3f(play, &self->actor, &tempVec, 100, 0, 0, -1, 1);
        }
    }
}

static void EnNy_UpdateGravity(EnNy* self, f32 arg1, f32 arg2) {
    if (self->unk_1E8 == 0.0f) {
        self->actor.gravity = -0.4f;
    } else if (!(arg1 < self->actor.yDistToWater)) {
        self->actor.gravity = -0.4f;
    } else if (arg2 < self->actor.yDistToWater) {
        self->actor.gravity = 0.0f;
        if (self->actor.velocity.y > self->unk_1EC) {
            self->actor.velocity.y -= 0.4f;
            if (self->actor.velocity.y < self->unk_1EC) {
                self->actor.velocity.y = self->unk_1EC;
            }
        } else if (self->actor.velocity.y < self->unk_1EC) {
            self->actor.velocity.y += 0.4f;
            if (self->actor.velocity.y > self->unk_1EC) {
                self->actor.velocity.y = self->unk_1EC;
            }
        }
    }
}

static s32 EnNy_CollisionCheck(EnNy* self, PlayState* play) {
    u8 sp3F;
    Vec3f effectPos;
    u32* hitInfo;

    sp3F = 0;
    self->hitPlayer = 0;
    if (self->collider.base.atFlags & 4) {
        self->collider.base.atFlags &= ~4;
        self->hitPlayer = 1;
        self->actor.world.rot.y = self->actor.yawTowardsPlayer;
        self->actor.speedXZ = -4.0f;
        return 0;
    }
    if (self->collider.base.atFlags & 2) {
        self->collider.base.atFlags &= ~2;
        self->hitPlayer = 1;
        return 0;
    } else {
        if (self->collider.base.acFlags & 2) {
            self->collider.base.acFlags &= ~2;
            effectPos.x = *(s16*)((u8*)self->collider.elements + 0xE);
            effectPos.y = *(s16*)((u8*)self->collider.elements + 0x10);
            effectPos.z = *(s16*)((u8*)self->collider.elements + 0x12);
            if ((self->unk_1E0 == 0.25f) && (self->unk_1D4 == 0xFF)) {
                switch (self->actor.colChkInfo.damageEffect) {
                    case 0xE:
                        sp3F = 1;
                    case 0xF:
                        Actor_ApplyDamage(&self->actor);
                        Actor_SetColorFilter(&self->actor, 0x400000, 255, 0x200000, 80);
                        break;
                    case 1:
                        Actor_ApplyDamage(&self->actor);
                        Actor_SetColorFilter(&self->actor, 0x400000, 255, 0x200000, 80);
                        break;
                    case 2:
                        self->unk_1CA = 4;
                        Actor_ApplyDamage(&self->actor);
                        Actor_SetColorFilter(&self->actor, 0x400000, 255, 0x200000, 80);
                        break;
                }
            }
            self->stoneTimer = 0;
            hitInfo = *(u32**)((u8*)self->collider.elements + 0x24);
            if (self->actor.colChkInfo.health == 0) {
                self->actor.flags &= ~1;
                self->actor.shape.shadowAlpha = 0;
                self->unk_1D0 = sp3F;
                FUN_00375b70(play, &self->actor);
                FUN_003741e4(play, *hitInfo, 0, &effectPos, 0);
                return 1;
            }
            FUN_003741e4(play, *hitInfo, 1, &effectPos, 0);
            return 0;
        }
    }
    return 0;
}

void EnNy_Update(Actor* thisx, GameState* state) {
    EnNy* self = (EnNy*)thisx;
    PlayState* play = (PlayState*)state;
    f32 temp_f20;
    f32 temp_f22;
    f32 sphereScale;

    temp_f20 = self->unk_1E0 - 0.25f;
    self->timer++;
    if (self->unk_1CA != 0) {
        self->unk_1CA--;
    }
    temp_f22 = (24.0f * temp_f20) + 12.0f;
    sphereScale = 1.33f * temp_f20 + 1.0f;
    Actor_SetFocus(&self->actor, 0.0f);
    Actor_SetScale(&self->actor, 0.01f);
    *(f32*)((u8*)self->collider.elements + 0x48) = sphereScale;
    self->actor.shape.rot.x += (s16)(self->unk_1E8 * 1000.0f);
    EnNy_UpdateGravity(self, temp_f22 + 10.0f, temp_f22 - 10.0f);
    Actor_MoveForward(&self->actor);
    Math_StepToF(&self->unk_1E4, self->unk_1E8, 0.1f);
    self->actionFunc(self, play);
    thisx->prevPos.y -= temp_f22;
    self->actor.world.pos.y -= temp_f22;
    Actor_UpdateBgCheckInfo(play, &self->actor, 20.0f, 20.0f, 60.0f, 7);
    self->actor.world.pos.y += temp_f22;
    self->unk_1F0 = temp_f22;
    if (EnNy_CollisionCheck(self, play) != 0) {
        s32 i;

        for (i = 0; i < 8; i++) {
            self->unk_1F8[i].x = (Rand_CenteredFloat(20.0f) + self->actor.world.pos.x);
            self->unk_1F8[i].y = (Rand_CenteredFloat(20.0f) + self->actor.world.pos.y);
            self->unk_1F8[i].z = (Rand_CenteredFloat(20.0f) + self->actor.world.pos.z);
        }
        for (i = 0; i < 4; i++) {
            self->unk_320[i].x = (Rand_CenteredFloat(10.0f) + self->actor.world.pos.x);
            self->unk_320[i].y = (Rand_CenteredFloat(10.0f) + self->actor.world.pos.y);
            self->unk_320[i].z = (Rand_CenteredFloat(10.0f) + self->actor.world.pos.z);
        }
        self->timer = 0;
        self->actor.update = (ActorFunc)FUN_0016761c;
        self->actor.draw = (ActorFunc)FUN_00167204;
        self->actionFunc = FUN_00114930;
        return;
    }
    if (self->unk_1E0 > 0.25f) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
    }
    CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
}

void EnNy_Draw(Actor* thisx, PlayState* play) {
    EnNy* self = (EnNy*)thisx;
    MTX34 mtx;
    Vec3f tempVec;

    MTX34CopyAsm(&mtx, &self->actor.modelMtx);
    Collider_UpdateSpheres(0, &self->collider, &self->actor.modelMtx);
    FUN_003695cc(self->models[1], 0, 4, 0, 0.0f, 0.0f, 0.0f, self->unk_1D8 * (1.0f / 255.0f));
    if (self->unk_1D8 != 0) {
        self->models[1]->SetUnkAC(1);
        self->models[1]->FUN_003721e0(&mtx);
        self->models[1]->FUN_00372170(0);
    }
    FUN_003695cc(self->models[0], 0, 4, 0, 0.0f, 0.0f, 0.0f, self->unk_1D4 * (1.0f / 255.0f));
    if (self->unk_1D4 != 0) {
        self->models[0]->SetUnkAC(1);
        self->models[0]->FUN_003721e0(&mtx);
        self->models[0]->FUN_00372170(0);
    }
    if (self->unk_1E0 > 0.25f) {
        EnNy_ScaleMtx(&mtx, self->unk_1E0, self->unk_1E0, self->unk_1E0);
        self->models[2]->SetUnkAC(1);
        self->models[2]->FUN_003721e0(&mtx);
        self->models[2]->FUN_00372170(0);
    }
    if (self->unk_1CA != 0) {
        s16 temp = self->unk_1CA - 1;
        Vec3f* fireOffset;

        (*(u16*)((u8*)self + 0x11A))++;
        if (temp == 0) {
            fireOffset = &EnNy_sFireOffsets[temp & 3];
            tempVec.x = Rand_CenteredFloat(5.0f) + (thisx->world.pos.x + fireOffset->x);
            tempVec.y = Rand_CenteredFloat(5.0f) + (self->actor.world.pos.y + fireOffset->y);
            tempVec.z = Rand_CenteredFloat(5.0f) + (self->actor.world.pos.z + fireOffset->z);
            EffectSsEnFire_SpawnVec3f(play, &self->actor, &tempVec, 100, 0, 0, -1, 1);
        }
    }
}
#endif

GLOBAL_ASM("asm/EnNy_Update.s")

GLOBAL_ASM("asm/EnNy_Draw.s")
}
