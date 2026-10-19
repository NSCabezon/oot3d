extern "C" {
#include "z_bg_ddan_kd.hpp"

#define FLAGS 0x80400010

void BgDdanKd_Init(Actor* actor, GameState* state);
void BgDdanKd_Destroy(Actor* actor, GameState* state);
void BgDdanKd_Update(Actor* actor, GameState* state);
void BgDdanKd_Draw(Actor* actor, GameState* state);

void FUN_003e4d28(BgDdanKd* self, PlayState* play);
void FUN_003e16f0(BgDdanKd* self, PlayState* play);
void FUN_003d7f64(BgDdanKd* self, PlayState* play);

s32 Collider_DestroyCylinder(PlayState* play, ColliderCylinder* collider);

s32 OnePointCutscene_Init(PlayState* play, s32 csId, s32 arg2, Actor* actor, s32 camId);
Actor* Actor_GetCollidedExplosive(PlayState* play, Collider* collider);

GLOBAL_ASM("data/z_Bg_Ddan_Kd.data.s")

ActorInit Bg_Ddan_Kd_InitVars = {
    ACTOR_BG_DDAN_KD,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_DDAN_OBJECTS,
    sizeof(BgDdanKd),
    (ActorFunc)BgDdanKd_Init,
    (ActorFunc)BgDdanKd_Destroy,
    (ActorFunc)BgDdanKd_Update,
    (ActorFunc)BgDdanKd_Draw,
};

static __inline void BgDdanKd_SetupAction(BgDdanKd* self, BgDdanKdActionFunc f) { self->actionFunc = f; }

void BgDdanKd_Init(Actor* actor, GameState* state) {
    BgDdanKd* self = (BgDdanKd*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    self->prevExplosive = NULL;
    Actor_ProcessInitChain(actor, (InitChainEntry*)0x00510DB0);
    DynaPolyActor_Init(&self->dyna, 1);
    FUN_00372f38(actor, play, &self->skelAnimModel, 4, 0);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, actor, (ColliderCylinderInit*)0x00510D78);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 2), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    if (!Flags_GetSwitch(play, self->dyna.actor.params)) {
        BgDdanKd_SetupAction(self, FUN_003e4d28);
    } else {
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y - 200.0f - 20.0f;
        BgDdanKd_SetupAction(self, FUN_003e16f0);
    }
}

void BgDdanKd_Destroy(Actor* actor, GameState* state) {
    BgDdanKd* self = (BgDdanKd*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyCylinder(play, &self->collider);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgDdanKd_Update(Actor* actor, GameState* state) {
    BgDdanKd* self = (BgDdanKd*)actor;

    self->actionFunc(self, (PlayState*)state);
}

void BgDdanKd_Draw(Actor* actor, GameState* state) {
    BgDdanKd* self = (BgDdanKd*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

void FUN_003e16f0(BgDdanKd* self, PlayState* play) {
}

/* Draft of FUN_003e4d28 (CheckForExplosions). Not moved: the original 380-byte slot also
 * contains an unreferenced 124-byte helper (FUN_003e4e28) merged after the literal pool, so a
 * C version shifts all following functions. Pending a cfg split. */
void FUN_003e4d28(BgDdanKd* self, PlayState* play) {
    Actor* explosive = Actor_GetCollidedExplosive(play, &self->collider.base);

    if (explosive != NULL) {
        explosive->params = 2;
    }
    if (explosive != NULL && self->prevExplosive != NULL) {
        f32 dx = explosive->world.pos.x - self->prevExplosivePos.x;
        f32 dz = explosive->world.pos.z - self->prevExplosivePos.z;
        f32 d = sqrtf(dx * dx + dz * dz);
        if (d > 80.0f) {
            self->actionFunc = FUN_003d7f64;
            OnePointCutscene_Init(play, 3050, 999, &self->dyna.actor, 0);
            return;
        }
    }
    if (self->timer != 0) {
        self->timer--;
    } else {
        self->prevExplosive = explosive;
        if (explosive != NULL) {
            self->timer = 20;
            self->prevExplosivePos = explosive->world.pos;
        }
    }
    Collider_UpdateCylinder(&self->dyna.actor, &self->collider);
    CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
}

#if 0
/* Draft (unmatched, Haiku pass): LowerStairs = FUN_003d7f64, still GLOBAL_ASM in functions_3C0000s.cpp.
 * Opus pass: ROM uses two function-local statics with dynamic init (Vector3f ctors?): guard words
 * live INSIDE this actor's data dump at 0x510D70 (accel guard) / 0x510D74 (velocity guard), vectors in
 * .bss at 0x569914 (velocity 0,5,0) and 0x569920 (accel 0,-0.45,0). A C version needs those placed as
 * real data symbols -> skipped. 3DS has no Rumble call; quake = FUN_0036627c, effects = FUN_0037378c /
 * FUN_003155bc, SFX = Audio_PlaySoundGeneral. SmoothStep consts: (speed, 4.0, 0.5, 0.025, 0.0),
 * (world.y, home.y-200-20, 0.075, speed, 0.0075).
 * Static velocity/accel vectors (cxa guard) and the random effect spawning are approximations. */
void FUN_003d7f64_draft(BgDdanKd* self, PlayState* play) {
    static Vec3f velocity = { 0.0f, 5.0f, 0.0f };
    static Vec3f accel = { 0.0f, -0.45f, 0.0f };
    Vec3f pos1;
    Vec3f pos2;
    f32 effectStrength;

    Math_SmoothStepToF(&self->dyna.actor.speed, 4.0f, 0.5f, 0.025f, 0.0f);
    Rumble_Request(500.0f, 120, 20, 10);

    if (Math_SmoothStepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y - 200.0f - 20.0f, 0.075f,
                           self->dyna.actor.speed, 0.0075f) == 0.0f) {
        Flags_SetSwitch(play, self->dyna.actor.params);
        self->actionFunc = FUN_003e16f0;
    } else {
        effectStrength = (self->dyna.actor.prevPos.y - self->dyna.actor.world.pos.y) + (self->dyna.actor.speed * 0.25f);
        if (play->state.frames & 1) {
            pos1 = pos2 = self->dyna.actor.world.pos;
            if (play->state.frames & 2) {
                pos1.z += 210.0f + Rand_ZeroOne() * 230.0f;
                pos2.z += 210.0f + Rand_ZeroOne() * 230.0f;
            } else {
                pos1.z += 330.0f + Rand_ZeroOne() * 240.0f;
                pos2.z += 330.0f + Rand_ZeroOne() * 240.0f;
            }
            pos1.x += 80.0f + Rand_ZeroOne() * 10.0f;
            pos2.x -= 80.0f + Rand_ZeroOne() * 10.0f;
            pos1.y = self->dyna.actor.floorHeight + 20.0f + Rand_ZeroOne();
            pos2.y = self->dyna.actor.floorHeight + 20.0f + Rand_ZeroOne();
            func_80033480(play, &pos1, 20.0f, 1, effectStrength * 135.0f, 60, 1);
            func_80033480(play, &pos2, 20.0f, 1, effectStrength * 135.0f, 60, 1);
            velocity.x = Rand_CenteredFloat(3.0f);
            velocity.z = Rand_CenteredFloat(3.0f);
            func_8003555C(play, &pos1, &velocity, &accel);
            func_8003555C(play, &pos2, &velocity, &accel);
            pos1 = self->dyna.actor.world.pos;
            pos1.z += 560.0f + Rand_ZeroOne() * 5.0f;
            pos1.x += (Rand_ZeroOne() - 0.5f) * 160.0f;
            pos1.y = Rand_ZeroOne() * 3.0f + (self->dyna.actor.floorHeight + 20.0f);
            func_80033480(play, &pos1, 20.0f, 1, effectStrength * 135.0f, 60, 1);
            func_8003555C(play, &pos1, &velocity, &accel);
        }
        Camera_RequestQuake(&play->mainCamera, 0, effectStrength * 0.6f, 3);
        SFX_PLAY_AT_POS(&self->dyna.actor.projectedPos, NA_SE_EV_PILLAR_SINK - SFX_FLAG);
    }
}
#endif
}
