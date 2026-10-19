extern "C" {
#include "z_bg_jya_ironobj.hpp"

#define FLAGS 0x80400000

#define PARAMS_GET_U(params, shift, count) (((params) >> (shift)) & ((1 << (count)) - 1))

typedef void (*BgJyaIronobjParticleFunc)(BgJyaIronobj* self, PlayState* play, Actor* enIk);

void BgJyaIronobj_Init(Actor* actor, GameState* state);
void BgJyaIronobj_Destroy(Actor* actor, GameState* state);
void BgJyaIronobj_Update(Actor* actor, GameState* state);
void BgJyaIronobj_Draw(Actor* actor, GameState* state);

ActorInit Bg_Jya_Ironobj_InitVars = {
    ACTOR_BG_JYA_IRONOBJ,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_JYA_IRON,
    sizeof(BgJyaIronobj),
    (ActorFunc)BgJyaIronobj_Init,
    (ActorFunc)BgJyaIronobj_Destroy,
    (ActorFunc)BgJyaIronobj_Update,
    (ActorFunc)BgJyaIronobj_Draw,
};

void FUN_0025ae88(BgJyaIronobj* self, PlayState* play, Actor* enIk); // SpawnPillarParticles
void FUN_001eb8d4(BgJyaIronobj* self, PlayState* play, Actor* enIk); // SpawnThroneParticles
void FUN_003bf344(BgJyaIronobj* self, PlayState* play);             // action

// Data order in the ROM: D_808994D8, sModelIndices, sCollisionHeaderIndices, sParticleFuncs, sCylinderInit, tables, sInitChain.
// (armcc emits statics in declaration order; the unreferenced D_808994D8 must be declared first.)
s16 D_808994D8[] = { 0x8700, 0x4000, 0xC000, 0x0000 };
static s32 sModelIndices[] = { 0, 2 };
static s32 sCollisionHeaderIndices[] = { 0, 1 };
__attribute__((used)) static BgJyaIronobjParticleFunc sParticleFuncs[] = { FUN_0025ae88, FUN_001eb8d4 };

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_ENEMY,
        OC1_NONE,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        0x00,
    },
    { 30, 150, 0, { 0, 0, 0 } },
};

// Pillar
__attribute__((used)) static s16 D_808994E0[] = { 5, 8, 11, 14, 17, 20, 23, 26 };
__attribute__((used)) static s16 D_808994F0[] = { 18, 26, 34, 42, 50, 60, 70, 80 };
__attribute__((used)) static s16 D_80899500[] = { 48, 42, 36, 32, 28, 24, 20, 16 };

// Throne
__attribute__((used)) static s16 D_80899510[] = { 5, 8, 11, 14, 17, 20, 23, 26 };
__attribute__((used)) static s16 D_80899520[] = { 18, 26, 34, 42, 50, 60, 70, 80 };
__attribute__((used)) static s16 D_80899530[] = { 48, 42, 36, 32, 28, 24, 20, 16 };

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

void BgJyaIronobj_Init(Actor* actor, GameState* state) {
    BgJyaIronobj* self = (BgJyaIronobj*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, 0);
    Actor_ProcessInitChain(actor, sInitChain);
    Collider_InitCylinder(play, &self->colliderCylinder);
    Collider_SetCylinder(play, &self->colliderCylinder, actor, &sCylinderInit);
    if (PARAMS_GET_U(actor->params, 0, 1) == 1) {
        self->colliderCylinder.dim.radius = 40;
        self->colliderCylinder.dim.height = 100;
    }
    Collider_UpdateCylinder(actor, &self->colliderCylinder);
    FUN_00372f38(actor, play, &self->skelAnimModel, sModelIndices[PARAMS_GET_U(actor->params, 0, 1)], 0);
    ReadCollisionHeaderFromZsi(
        DynaPolyInfo_Alloc(&self->dyna, state, sCollisionHeaderIndices[PARAMS_GET_U(actor->params, 0, 1)]),
        &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    self->actionFunc = FUN_003bf344;
}

void BgJyaIronobj_Destroy(Actor* actor, GameState* state) {
    BgJyaIronobj* self = (BgJyaIronobj*)actor;
    PlayState* play = (PlayState*)state;

    Collider_DestroyCylinder(play, &self->colliderCylinder);
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgJyaIronobj_Update(Actor* actor, GameState* state) {
    BgJyaIronobj* self = (BgJyaIronobj*)actor;

    self->actionFunc(self, (PlayState*)state);
}

void BgJyaIronobj_Draw(Actor* actor, GameState* state) {
    BgJyaIronobj* self = (BgJyaIronobj*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

// Action. dropPos lives in an inner scope so its address-taken slot doesn't block the Actor_Kill tail call.
void FUN_003bf344(BgJyaIronobj* self, PlayState* play) {
    Actor* actor;
    s32 i;

    if (self->colliderCylinder.base.acFlags & AC_HIT) {
        self->colliderCylinder.base.acFlags &= ~AC_HIT;
        actor = self->colliderCylinder.base.ac;
        if (actor != NULL && actor->id == ACTOR_EN_IK) {
            sParticleFuncs[PARAMS_GET_U(self->dyna.actor.params, 0, 1)](self, play, actor);
            Audio_PlaySoundAtPosition(play, &self->dyna.actor.world.pos, 80, 0x0100041D);
            {
                Vec3f dropPos;
                dropPos.x = self->dyna.actor.world.pos.x;
                dropPos.y = self->dyna.actor.world.pos.y + 20.0f;
                dropPos.z = self->dyna.actor.world.pos.z;
                for (i = 0; i < 3; i++) {
                    Item_DropCollectible(play, &dropPos, 3);
                    dropPos.y += 18.0f;
                }
            }
            Actor_Kill(&self->dyna.actor);
        }
    } else {
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->colliderCylinder.base);
    }
}

/*
 * Particle spawners (asm: FUN_0025ae88 = pillar, FUN_001eb8d4 = throne); still GLOBAL_ASM in
 * functions_240000s.cpp / functions_1C0000s.cpp.
 * FUN_0025ae88 draft (Opus closing pass): right size (952 B), same instruction stream except register
 * allocation. ROM keeps D_808994F0 in sb, D_808994E0 = sb-0x10 in sl and reloads 0.8f / D_80899500 from
 * literals each iteration; this draft instead keeps 0.8f bits in fp. The VFP constants also land in other
 * registers (ROM: 16384 s17, 80 s20, 20 s21, 9 s16, 10 s29, 6 s19). enIk attack type is s8 at +0xE13; the
 * last EffectSsKakera_Spawn arg is 1; Actor_Spawn's extra arg is 1.
 * FUN_001eb8d4 draft below is still the unverified N64 port.
 */
#if 0
s16 Actor_WorldYawTowardActor(Actor* actorA, Actor* actorB);
extern __pure float _fabsf(float);
static __inline __pure float BgJyaIronobj_Fabsf(float f) {
    return _fabsf(f);
}

void FUN_0025ae88(BgJyaIronobj* self, PlayState* play, Actor* enIk) {
    s32 i;
    s32 j;
    s16 unkArg5;
    f32 temp_f22;
    Vec3f pos;
    Vec3f vel;
    f32 sins;
    f32 coss;
    s16 rotY;

    rotY = Actor_WorldYawTowardActor(&self->dyna.actor, enIk) + D_808994D8[*((s8*)enIk + 0xE13) - 1];

    for (i = 0; i < 8; i++) {
        Actor* actor = Actor_Spawn(&play->actorCtx, play, ACTOR_BG_JYA_HAHENIRON, self->dyna.actor.world.pos.x,
                                   Rand_ZeroOne() * 80.0f + 20.0f + self->dyna.actor.world.pos.y,
                                   self->dyna.actor.world.pos.z, 0, (s16)(Rand_ZeroOne() * 0x4000) + rotY - 0x2000, 0, 0, 1);
        if (actor != NULL) {
            actor->speedXZ = Rand_ZeroOne() * 8.0f + 9.0f;
            actor->velocity.y = Rand_ZeroOne() * 10.0f + 6.0f;
        }
    }
    Actor_Spawn(&play->actorCtx, play, ACTOR_BG_JYA_HAHENIRON, self->dyna.actor.world.pos.x,
                self->dyna.actor.world.pos.y + 150.0f, self->dyna.actor.world.pos.z, 0, 0, 0, 1, 1);
    Actor_Spawn(&play->actorCtx, play, ACTOR_BG_JYA_HAHENIRON, self->dyna.actor.world.pos.x,
                self->dyna.actor.world.pos.y, self->dyna.actor.world.pos.z, 0, 0, 0, 2, 1);
    sins = Math_SinS(rotY);
    coss = Math_CosS(rotY);
    for (j = 0; j < 32; j++) {
        s32 idx = j & 7;
        f32 rand = Rand_ZeroOne();

        if (rand < 0.1f) {
            unkArg5 = 0x60;
        } else if (rand < 0.8f) {
            unkArg5 = 0x40;
        } else {
            unkArg5 = 0x20;
        }
        pos.x = self->dyna.actor.world.pos.x;
        pos.y = self->dyna.actor.world.pos.y + ((4.375f * j) + 10.0f);
        pos.z = self->dyna.actor.world.pos.z;
        temp_f22 = BgJyaIronobj_Fabsf(j - 15.5f) * (1.0f / 31) + 0.5f;
        vel.x = 2.0f * ((Rand_ZeroOne() * 6.0f) - 3.0f) + (Rand_ZeroOne() * sins * (temp_f22 * 8.0f));
        vel.y = (Rand_ZeroOne() * 8.0f) - 3.0f;
        vel.z = 2.0f * ((Rand_ZeroOne() * 6.0f) - 3.0f) + (Rand_ZeroOne() * coss * (temp_f22 * 8.0f));
        EffectSsKakera_Spawn(play, &pos, &vel, &pos, -350, unkArg5, D_80899500[idx], 4, 0, D_808994E0[idx], 0, 5,
                             D_808994F0[idx], -1, OBJECT_JYA_IRON, (void*)1);
        if (Rand_ZeroOne() < 0.26f) {
            FUN_0037378c(play, &pos, 200.0f, 1, D_808994E0[idx] * 4 + 60, D_808994E0[idx] * 4 + 80, 1);
        }
    }
}

void FUN_001eb8d4(BgJyaIronobj* self, PlayState* play, Actor* enIk) {
    s32 i;
    s32 j;
    s16 unkArg5;
    f32 temp_f22;
    Vec3f pos;
    Vec3f vel;
    f32 coss;
    s16 rotY;
    f32 sins;

    PRINTF("attack_type(%d)\n", ((EnIk*)enIk)->unk_2FF);
    rotY = Actor_WorldYawTowardActor(&self->dyna.actor, enIk) + D_808994D8[((EnIk*)enIk)->unk_2FF - 1];
    for (i = 0; i < 8; i++) {
        Actor* actor = Actor_Spawn(&play->actorCtx, play, ACTOR_BG_JYA_HAHENIRON, self->dyna.actor.world.pos.x,
                                   (Rand_ZeroOne() * 80.0f) + self->dyna.actor.world.pos.y + 10.0f,
                                   self->dyna.actor.world.pos.z, 0, ((s16)(s32)(Rand_ZeroOne() * 0x4000) + rotY) - 0x2000,
                                   0, 0);
        if (actor != NULL) {
            actor->speed = Rand_ZeroOne() * 8.0f + 9.0f;
            actor->velocity.y = Rand_ZeroOne() * 10.0f + 6.0f;
        }
    }
    sins = Math_SinS(rotY);
    coss = Math_CosS(rotY);
    for (j = 0; j < 32; j++) {
        f32 rand = Rand_ZeroOne();

        if (rand < 0.1f) {
            unkArg5 = 0x60;
        } else if (rand < 0.8f) {
            unkArg5 = 0x40;
        } else {
            unkArg5 = 0x20;
        }
        pos.x = self->dyna.actor.world.pos.x + (Rand_ZeroOne() * 40 - 20);
        pos.y = self->dyna.actor.world.pos.y + (3.75f * j);
        pos.z = self->dyna.actor.world.pos.z + (Rand_ZeroOne() * 40 - 20);
        temp_f22 = fabsf(j - 15.5f) * (1.0f / 31) + 0.5f;
        vel.x = 2.0f * (Rand_ZeroOne() * 6.0f - 3.0f) + (Rand_ZeroOne() * sins * 8.0f * temp_f22);
        vel.y = Rand_ZeroOne() * 8.0f - 3.0f;
        vel.z = 2.0f * (Rand_ZeroOne() * 6.0f - 3.0f) + (Rand_ZeroOne() * coss * 8.0f * temp_f22);
        EffectSsKakera_Spawn(play, &pos, &vel, &pos, -350, unkArg5, D_80899530[j & 7], 4, 0, D_80899510[j & 7], 0, 5,
                             D_80899520[j & 7], -1, OBJECT_JYA_IRON, 0);
        if (Rand_ZeroOne() < 0.26f) {
            func_80033480(play, &pos, 200.0f, 1, D_80899510[j & 7] * 4 + 60, D_80899510[j & 7] * 4 + 80, 1);
        }
    }
}
#endif
}
