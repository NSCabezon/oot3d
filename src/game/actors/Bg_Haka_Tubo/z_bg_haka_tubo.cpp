extern "C" {
#include "z_bg_haka_tubo.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x80400010

void BgHakaTubo_Init(Actor* actor, GameState* state);
void BgHakaTubo_Destroy(Actor* actor, GameState* state);
void BgHakaTubo_Update(Actor* actor, GameState* state);
void BgHakaTubo_Draw(Actor* actor, GameState* state);
void BgHakaTubo_Idle(BgHakaTubo* self, PlayState* play);
void BgHakaTubo_DropCollectible(BgHakaTubo* self, PlayState* play);

s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
SkeletonAnimationModel* FUN_0036a924(Actor* actor, PlayState* play, s32 arg2, s32 arg3);
f32 Actor_WorldDistXZToPoint(Actor* actor, Vec3f* point);
int __cxa_guard_acquire(unsigned int* guard_object);
void __cxa_guard_release(unsigned int* guard_object);

void EffectSsBomb2_SpawnLayered(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep);
void EffectSsHahen_SpawnBurst_003a9460(PlayState* play, Vec3f* pos, s16 unused, s16 scale, s16 randScaleRange,
                                       s32 count, s16 objId, s16 life, s32 dList, f32 yJitter, f32 radius,
                                       f32 burstScale);

// Data dump at 0x005114C8: sPotsDestroyed, sInitChain, zero-vector guard, burst floats.
typedef struct {
    /* 0x00 */ s32 potsDestroyed;
    /* 0x04 */ s32 initChain;
    /* 0x08 */ unsigned int zeroVecGuard;
    /* 0x0C */ f32 burstScale;
    /* 0x10 */ f32 yJitter;
    /* 0x14 */ f32 radius;
} BgHakaTuboData;
extern BgHakaTuboData BgHakaTubo_sData;
extern InitChainEntry BgHakaTubo_sInitChain[];
extern Vec3f BgHakaTubo_sZeroVec;
inline void* operator new(unsigned int, void* p) {
    return p;
}
#define BgHakaTubo_sPotsDestroyed BgHakaTubo_sData.potsDestroyed
#define BgHakaTubo_sZeroVecGuard BgHakaTubo_sData.zeroVecGuard
extern ColliderCylinderInit BgHakaTubo_sPotColliderInit;
extern ColliderCylinderInit BgHakaTubo_sFlamesColliderInit;

GLOBAL_ASM("data/z_Bg_Haka_Tubo.data.s")
GLOBAL_ASM("../src/game/actors/Bg_Haka_Tubo/data_syms.s")

ActorInit Bg_Haka_Tubo_InitVars = {
    ACTOR_BG_HAKA_TUBO,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HAKA_OBJECTS,
    sizeof(BgHakaTubo),
    (ActorFunc)BgHakaTubo_Init,
    (ActorFunc)BgHakaTubo_Destroy,
    (ActorFunc)BgHakaTubo_Update,
    (ActorFunc)BgHakaTubo_Draw,
};

static inline u8* BgHakaTubo_GetObjStatus(PlayState* play, s32 idx) {
    if ((u32)idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

void BgHakaTubo_Init(Actor* actor, GameState* state) {
    BgHakaTubo* self = (BgHakaTubo*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;
    ZARInfo* zar;

    Actor_ProcessInitChain(actor, BgHakaTubo_sInitChain);
    DynaPolyActor_Init(&self->dyna, 3);
    FUN_00372f38(actor, play, &self->skelAnimModel, 0x19, 0);
    self->unk_274 = FUN_0036a924(actor, play, 1, 0x37);
    zar = (ZARInfo*)(BgHakaTubo_GetObjStatus(play, (u8)Object_GetIndex(&play->objectCtx, 1)) + 0x10);
    ((SkeletonAnimationModel*)self->unk_274)->unk_0C->FUN_00372d94(zar->GetCMABByIndex(0x22));
    ((SkeletonAnimationModel*)self->unk_274)->unk_0C->SetAnimType(1);
    ((SkeletonAnimationModel*)self->unk_274)->unk_0C->SetAnimSpeed(2.0f);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0x12), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    Collider_InitCylinder(play, &self->potCollider);
    Collider_SetCylinder(play, &self->potCollider, &self->dyna.actor, &BgHakaTubo_sPotColliderInit);
    Collider_InitCylinder(play, &self->flamesCollider);
    Collider_SetCylinder(play, &self->flamesCollider, &self->dyna.actor, &BgHakaTubo_sFlamesColliderInit);
    actor->unk_19B = 2;
    {
        f32 n = Rand_ZeroOne() * 15.0f;
        self->fireScroll = (n > 0.0f) ? (s32)(0.5f + ((Rand_ZeroOne() * 15.0f) * 3.0f) * 0.5f)
                                      : (s32)(((Rand_ZeroOne() * 15.0f) * 3.0f) * 0.5f - 0.5f);
    }
    BgHakaTubo_sPotsDestroyed = 0;
    self->actionFunc = BgHakaTubo_Idle;
}

void FUN_00357248(void* ptr);

void BgHakaTubo_Destroy(Actor* actor, GameState* state) {
    BgHakaTubo* self = (BgHakaTubo*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyCylinder(play, &self->potCollider);
    Collider_DestroyCylinder(play, &self->flamesCollider);
    FUN_00350f34(&self->dyna.actor, &self->skelAnimModel, 0);
    FUN_00357248(&self->unk_274);
}

void BgHakaTubo_Update(Actor* actor, GameState* state) {
    BgHakaTubo* self = (BgHakaTubo*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
    self->fireScroll++;
}

// EffectSsHahen_SpawnBurst_003a9460 (ROM, functions_3A0000s.cpp): near-miss, left as asm.
// Draft kept disabled. Diff: velocity.y is built with an extra vmov copy of 0.5f (+1 insn,
// shifts every later literal-pool load); ROM has `vmov s1,s20; vmov s2,s20; vmla s2,s0,s1` (multiplier AND
// accumulator are separate copies of the hoisted 0.5f). ~35 variants tried: operand order, f32 half locals
// (one/two), double literals, inline Rand_ZeroFloat(f) helper (copy hoisted out of the loop, d11 saved),
// += / *= statement splits, temp chains.
#if 0
typedef struct {
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;
    void* dList;
    s16 unused;
    s16 scale;
    s16 objId;
    s16 life;
} EffectSsHahenInitParams;

void EffectSs_Spawn(PlayState* play, s32 type, s32 priority, void* initParams);
s32 Rand_S16Offset(s16 base, s16 range);

void EffectSsHahen_SpawnBurst_003a9460(PlayState* play, Vec3f* pos, s16 unused, s16 scale, s16 randScaleRange,
                                       s32 count, s16 objId, s16 life, void* dList, f32 yJitter, f32 radius,
                                       f32 burstScale) {
    Vec3f spawnPos;
    Vec3f velocity;
    Vec3f accel;
    s32 randScale;
    EffectSsHahenInitParams initParams;
    s32 i;
    s16 angle;
    f32 cosVal;

    accel.x = accel.z = 0.0f;
    accel.y = -0.07f * burstScale;

    for (i = 0; i < count; i++) {
        spawnPos.x = pos->x;
        spawnPos.y = pos->y;
        spawnPos.z = pos->z;
        spawnPos.y += Rand_ZeroOne() * yJitter;
        angle = (s16)(Rand_ZeroOne() * 65536.0f);
        velocity.x = Math_SinS(angle);
        cosVal = Math_CosS(angle);
        spawnPos.x += velocity.x * radius;
        spawnPos.z += cosVal * radius;
        velocity.x = velocity.x * burstScale;
        velocity.z = cosVal * burstScale;
        velocity.y = ((Rand_ZeroOne() * 0.5f) + 0.5f) * burstScale;

        randScale = Rand_S16Offset(scale, randScaleRange);
        Math_Vec3f_Copy(&initParams.pos, &spawnPos);
        Math_Vec3f_Copy(&initParams.velocity, &velocity);
        Math_Vec3f_Copy(&initParams.accel, &accel);
        initParams.unused = unused;
        initParams.objId = objId;
        initParams.scale = randScale;
        initParams.dList = dList;
        initParams.life = life;

        EffectSs_Spawn(play, 0xF, 0x80, &initParams);
    }
}
#endif

typedef struct {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
} BgHakaTuboDrawColor;
extern BgHakaTuboDrawColor BgHakaTubo_sDrawColor;
void FUN_00358778(SkeletonAnimationModel* model, s32 arg1, s32 arg2, BgHakaTuboDrawColor* color, s32 arg4);
f32 __hardfp_sinf(f32 x);
f32 __hardfp_cosf(f32 x);

static inline void BgHakaTubo_MakeTransXYZ(MTX34* m, f32 x, f32 y, f32 z) {
    Vec3f v;

    v.x = x;
    v.y = y;
    v.z = z;
    MTX34_MakeTrans(m, &v);
}

void BgHakaTubo_Draw(Actor* actor, GameState* state) {
    BgHakaTubo* self = (BgHakaTubo*)actor;
    MTX34 mtx;
    MTX34 flameMtx;
    BgHakaTuboDrawColor color;
    f32 animSpeed;
    f32 angle;

    MTX34CopyAsm(&mtx, &actor->modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&mtx);
        self->skelAnimModel->FUN_00372170(0);
    }
    MTX34CopyAsm(&flameMtx, &actor->modelMtx);
    animSpeed = 2.0f;
    if (PauseContext_GetState() != 0) {
        animSpeed = 0.0f;
    }
    BgHakaTubo_MakeTransXYZ(&flameMtx, actor->world.pos.x, actor->world.pos.y + 235.0f - 20.0f, actor->world.pos.z);
    angle = actor->shape.rot.y * (2.0f * 3.14159274f / 65536.0f);
    if (angle != 0.0f) {
        f32 s = __hardfp_sinf(angle);
        f32 c = __hardfp_cosf(angle);
        f32 a;
        f32 b;

        a = flameMtx.data[0][0];
        b = flameMtx.data[0][2];
        flameMtx.data[0][0] = a * c - b * s;
        flameMtx.data[0][2] = a * s + b * c;
        a = flameMtx.data[1][0];
        b = flameMtx.data[1][2];
        flameMtx.data[1][0] = a * c - b * s;
        flameMtx.data[1][2] = a * s + b * c;
        a = flameMtx.data[2][0];
        b = flameMtx.data[2][2];
        flameMtx.data[2][0] = a * c - b * s;
        flameMtx.data[2][2] = a * s + b * c;
    }
    {
        f32 sx = 0.05f;
        f32 sy = 0.04f;
        f32 sz = 0.05f;

        flameMtx.data[0][0] *= sx;
        flameMtx.data[1][0] *= sx;
        flameMtx.data[2][0] *= sx;
        flameMtx.data[0][1] *= sy;
        flameMtx.data[1][1] *= sy;
        flameMtx.data[2][1] *= sy;
        flameMtx.data[0][2] *= sz;
        flameMtx.data[1][2] *= sz;
        flameMtx.data[2][2] *= sz;
    }
    color = BgHakaTubo_sDrawColor;
    FUN_00358778((SkeletonAnimationModel*)self->unk_274, 0, 0, &color, 0);
    if (self->unk_274 != NULL) {
        ((SkeletonAnimationModel*)self->unk_274)->unk_0C->SetAnimSpeed(animSpeed);
        ((SkeletonAnimationModel*)self->unk_274)->SetUnkAC(1);
        ((SkeletonAnimationModel*)self->unk_274)->FUN_003721e0(&flameMtx);
        ((SkeletonAnimationModel*)self->unk_274)->FUN_00372170(0);
    }
}

void BgHakaTubo_DropCollectible(BgHakaTubo* self, PlayState* play) {
    Actor* collectible;
    f32 rnd;
    Vec3f spawnPos;
    s32 i;
    s32 collectibleParams;

    self->dropTimer--;
    if (self->dropTimer == 0) {
        spawnPos.x = self->dyna.actor.world.pos.x;
        spawnPos.y = self->dyna.actor.world.pos.y + 200.0f;
        spawnPos.z = self->dyna.actor.world.pos.z;
        if (self->dyna.actor.room == 12) {
            rnd = Rand_ZeroOne();
            BgHakaTubo_sPotsDestroyed++;
            if (BgHakaTubo_sPotsDestroyed == 3) {
                collectibleParams = -1;
                FUN_00372244(&play->unk_5FCC, 0x1E, 0x01000487);
                for (i = 0; i < 9; i++) {
                    collectible = (Actor*)Item_DropCollectible(play, &spawnPos, (i % 3) + 0);
                    if (collectible != NULL) {
                        collectible->velocity.y = 15.0f;
                        collectible->world.rot.y = self->dyna.actor.shape.rot.y + (i * 0x1C71);
                    }
                }
            } else if (rnd < 0.2f) {
                collectibleParams = -1;
                Actor_Spawn(&play->actorCtx, play, 0x13, self->dyna.actor.world.pos.x,
                            self->dyna.actor.world.pos.y + 80.0f, self->dyna.actor.world.pos.z, 0,
                            self->dyna.actor.shape.rot.y, 0, 2, 1);
                Audio_PlaySoundGeneral(0x0100048B, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            } else {
                if (rnd < 0.4f) {
                    collectibleParams = 4;
                } else if (rnd < 0.6f) {
                    collectibleParams = 0xE;
                } else if (rnd < 0.8f) {
                    collectibleParams = 0xF;
                } else {
                    collectibleParams = 8;
                }
                FUN_00372244(&play->unk_5FCC, 0x1E, 0x0100048C);
            }
        } else if (Flags_GetCollectible(play, self->dyna.actor.params) != 0) {
            collectibleParams = 3;
            FUN_00372244(&play->unk_5FCC, 0x1E, 0x0100048C);
        } else {
            collectibleParams = ((self->dyna.actor.params & 0x3F) << 8) | 0x11;
            FUN_00372244(&play->unk_5FCC, 0x1E, 0x01000487);
        }
        if (collectibleParams != -1) {
            collectible = (Actor*)Item_DropCollectible(play, &spawnPos, collectibleParams);
            if (collectible != NULL) {
                collectible->velocity.y = 15.0f;
                collectible->world.rot.y = self->dyna.actor.shape.rot.y;
            }
        }
        Actor_Kill(&self->dyna.actor);
    }
}

// BgHakaTubo_Idle: near-miss (4 of 162 words), left as asm. Draft kept disabled. Only diff: in the
// EffectSsHahen_SpawnBurst_003a9460 call the ROM emits `mov r3,#0xaf` BEFORE the three vldr of the burst floats
// ([sData+0xc/0x14/0x10]); armcc schedules the vldrs first. Tried ~30 variants (float locals, play2, s16 casts,
// wrapper helpers with every float/int param order, float block at every position of the prototype).
#if 0
void BgHakaTubo_Idle(BgHakaTubo* self, PlayState* play) {
    Vec3f pos;

    if (!(BgHakaTubo_sZeroVecGuard & 1) && __cxa_guard_acquire(&BgHakaTubo_sData.zeroVecGuard)) {
        new ((void*)&BgHakaTubo_sZeroVec) Vector3f(0.0f, 0.0f, 0.0f);
        __cxa_guard_release(&BgHakaTubo_sData.zeroVecGuard);
    }
    if (self->dyna.actor.room == 12) {
        self->dyna.actor.shape.rot.y += 0x100;
        self->dyna.actor.world.pos.x = Math_SinS(self->dyna.actor.shape.rot.y - 0x4000) * 145.0f + -5559.0f;
        self->dyna.actor.world.pos.z = Math_CosS(self->dyna.actor.shape.rot.y - 0x4000) * 145.0f + -1587.0f;
    }
    if (self->flamesCollider.base.atFlags & 2) {
        self->flamesCollider.base.atFlags &= ~2;
        FUN_00374bb8(play, &self->dyna.actor, 5.0f, self->dyna.actor.yawTowardsPlayer, 5.0f);
    }
    if (self->potCollider.base.acFlags & 2) {
        self->potCollider.base.acFlags &= ~2;
        if (Actor_WorldDistXZToPoint(&self->dyna.actor, &self->potCollider.base.ac->world.pos) < 50.0f &&
            (self->potCollider.base.ac->world.pos.y - self->dyna.actor.world.pos.y) < 50.0f) {
            pos.x = self->dyna.actor.world.pos.x;
            pos.z = self->dyna.actor.world.pos.z;
            pos.y = self->dyna.actor.world.pos.y + 80.0f;
            EffectSsBomb2_SpawnLayered(play, &pos, &BgHakaTubo_sZeroVec, &BgHakaTubo_sZeroVec, 100, 45);
            Audio_PlaySoundAtPosition(play, &self->dyna.actor.world.pos, 50, 0x01000190);
            pos.y = self->dyna.actor.world.pos.y + 20.0f;
            EffectSsHahen_SpawnBurst_003a9460(play, &pos, 0, 175, 350, 50, 0x69, 40, 0x1D, BgHakaTubo_sData.yJitter, BgHakaTubo_sData.radius, BgHakaTubo_sData.burstScale);
            self->dropTimer = 8;
            self->dyna.actor.draw = NULL;
            Actor_SetScale(&self->dyna.actor, 0.0f);
            self->actionFunc = BgHakaTubo_DropCollectible;
        }
    } else {
        Collider_UpdateCylinder(&self->dyna.actor, &self->flamesCollider);
        Collider_UpdateCylinder(&self->dyna.actor, &self->potCollider);
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->potCollider.base);
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->flamesCollider.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->flamesCollider.base);
    }
}
#endif

GLOBAL_ASM("asm/BgHakaTubo_Idle.s")

}
