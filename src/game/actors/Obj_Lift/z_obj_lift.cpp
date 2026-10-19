extern "C" {
#include "z_obj_lift.hpp"

#define FLAGS 0x00400010

void ObjLift_Init(Actor* actor, GameState* state);
void ObjLift_Destroy(Actor* actor, GameState* state);
void ObjLift_Update(Actor* actor, GameState* state);
void ObjLift_Draw(Actor* actor, GameState* state);

ActorInit Obj_Lift_InitVars = {
    ACTOR_OBJ_LIFT,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_D_LIFT,
    sizeof(ObjLift),
    (ActorFunc)ObjLift_Init,
    (ActorFunc)ObjLift_Destroy,
    (ActorFunc)ObjLift_Update,
    (ActorFunc)ObjLift_Draw,
};

// Quake / camera helpers (same prototypes as Bg_Haka_Huta)
void Quake_SetSpeed(s32 idx, s16 speed);
void Quake_SetQuakeValues(s32 idx, s16 y, s16 x, s16 zoom, s16 rollOffset);
void Quake_SetCountdown(s32 idx, s16 countdown);
s32 Quake_Add(Camera* cam, u32 type);
s32 FUN_0036adf4(DynaPolyActor* dyna); // DynaPolyActor_IsPlayerOnTop

// Action functions (FUN_xxxxxxxx names are pinned by the linker script)
void FUN_003c05b0(ObjLift* self, PlayState* play); // Wait
void FUN_00110e98(ObjLift* self, PlayState* play); // Shake
void FUN_003bb7bc(ObjLift* self, PlayState* play); // Fall (asm, see #if 0 draft)

__attribute__((used)) static f32 sScales[] = { 0.1f, 0.05f };
__attribute__((used)) static f32 sMaxFallDistances[] = { -18.0f, -9.0f };
__attribute__((used)) static s16 sFallTimerDurations[] = { 0, 15, 30, 45, 60, 75, 90, 0 };


typedef struct ObjLiftFragmentScale {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 z;
} ObjLiftFragmentScale; // size = 0x4

__attribute__((used)) static ObjLiftFragmentScale sFragmentScales[] = {
    { 120, -120 }, { 120, 0 }, { 120, 120 }, { 0, -120 }, { 0, 0 },
    { 0, 120 },    { -120, -120 }, { -120, 0 }, { -120, 120 },
};

__attribute__((used)) static InitChainEntry sInitChain[] = {
    ICHAIN_F32_DIV1000(gravity, -600, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(minVelocityY, -15000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 2000, ICHAIN_STOP),
};

void ObjLift_Init(Actor* actor, GameState* state) {
    ObjLift* self = (ObjLift*)actor;
    PlayState* play = (PlayState*)state;
    void* zsi;

    actor->unk_19A = 1;
    FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
    zsi = play->GetObjectStatus(actor->objBankIndex)->zarInfo.FUN_003532c0(0);
    {
        CollisionHeader* colHeader = NULL;
        DynaPolyActor_Init(&self->dyna, 1);
        ReadCollisionHeaderFromZsi(zsi, &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    }
    if (Flags_GetSwitch(play, (actor->params & 0xFF) >> 2)) {
        Actor_Kill(actor);
        return;
    }
    Actor_SetScale(actor, sScales[(actor->params >> 1) & 1]);
    Actor_ProcessInitChain(actor, sInitChain);
    self->shakeOrientation.x = Rand_ZeroOne() * 65535.5f;
    self->shakeOrientation.y = Rand_ZeroOne() * 65535.5f;
    self->shakeOrientation.z = Rand_ZeroOne() * 65535.5f;
    self->timer = sFallTimerDurations[(actor->params >> 8) & 7];
    self->actionFunc = FUN_003c05b0;
}

void ObjLift_Destroy(Actor* actor, GameState* state) {
    ObjLift* self = (ObjLift*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void ObjLift_Update(Actor* actor, GameState* state) {
    ObjLift* self = (ObjLift*)actor;
    PlayState* play = (PlayState*)state;

    if (self->timer > 0) {
        self->timer--;
    }
    self->actionFunc(self, play);
}

static inline void ObjLift_SetupFall(ObjLift* self) {
    ObjLiftActionFunc fall = FUN_003bb7bc;

    self->actionFunc = fall;
    Math_Vec3f_Copy(&self->dyna.actor.world.pos, &self->dyna.actor.home.pos);
    self->dyna.actor.world.rot = self->dyna.actor.home.rot;
    self->dyna.actor.shape.rot = self->dyna.actor.world.rot;
}

void FUN_003c05b0(ObjLift* self, PlayState* play) {
    s32 quakeIndex;

    if (FUN_0036adf4(&self->dyna)) {
        if (self->timer <= 0) {
            if (((self->dyna.actor.params >> 8) & 7) == 7) {
                ObjLift_SetupFall(self);
            } else {
                quakeIndex = Quake_Add(play->cameraPtrs[play->activeCamera], 1);
                Quake_SetSpeed(quakeIndex, 10000);
                Quake_SetQuakeValues(quakeIndex, 2, 0, 0, 0);
                Quake_SetCountdown(quakeIndex, 20);
                self->timer = 30;
                self->actionFunc = FUN_00110e98;
            }
        }
    } else {
        self->timer = sFallTimerDurations[(self->dyna.actor.params >> 8) & 7];
    }
}

void FUN_00110e98(ObjLift* self, PlayState* play) {
    Actor* actor = &self->dyna.actor;

    if (self->timer <= 0) {
        ObjLift_SetupFall(self);
    } else {
        self->shakeOrientation.x += 10000;
        actor->world.rot.x = (s16)(Math_SinS(self->shakeOrientation.x) * 300.0f) + actor->home.rot.x;
        actor->world.rot.z = (s16)(Math_CosS(self->shakeOrientation.x) * 300.0f) + actor->home.rot.z;
        actor->shape.rot.x = actor->world.rot.x;
        actor->shape.rot.z = actor->world.rot.z;
        self->shakeOrientation.y += 18000;
        actor->world.pos.y = Math_SinS(self->shakeOrientation.y) + actor->home.pos.y;
        self->shakeOrientation.z += 18000;
        actor->world.pos.x = Math_SinS(self->shakeOrientation.z) * 3.0f + actor->home.pos.x;
        actor->world.pos.z = Math_CosS(self->shakeOrientation.z) * 3.0f + actor->home.pos.z;
    }

    if ((self->timer % 5) == 0) {
        Audio_PlaySoundAtPosition(play, &actor->world.pos, 16, 0x0100018F);
    }
}

#if 0 // Draft (Opus closing pass): N64-style inlined SpawnFragments helper. Loop body, stack layout and
// branch shapes all match; only diff: ROM keeps TWO copies of self (r8 outer, r4 helper) -> extra fp push,
// frame 0x5c vs 0x58, all regs shifted. armcc coalesces every variant tried (see notes/Obj_Lift.md).
f32 BgCheck_EntityRaycastFloor4(CollisionContext* colCtx, CollisionPoly** outPoly, s32* bgId, Actor* actor, Vec3f* pos);

static inline void ObjLift_SpawnFragments(ObjLift* self, PlayState* play) {
    Vec3f pos;
    Vec3f velocity;
    Vec3f* temp_s3;
    s32 i;
    s16 arg5;

    temp_s3 = &self->dyna.actor.world.pos;

    for (i = 0; i < 9; i++) {
        pos.x = sFragmentScales[i].x * self->dyna.actor.scale.x + temp_s3->x;
        pos.y = temp_s3->y;
        pos.z = sFragmentScales[i].z * self->dyna.actor.scale.z + temp_s3->z;
        velocity.x = sFragmentScales[i].x * self->dyna.actor.scale.x * 0.8f;
        velocity.y = Rand_ZeroOne() * 10.0f + 6.0f;
        velocity.z = sFragmentScales[i].z * self->dyna.actor.scale.z * 0.8f;
        arg5 = (Rand_ZeroOne() < 0.5f) ? 64 : 32;
        EffectSsKakera_Spawn(play, &pos, &velocity, temp_s3, -256, arg5, 15, 15, 0,
                             (Rand_ZeroOne() * 50.0f + 50.0f) * self->dyna.actor.scale.x, 0, 32, 50, -1,
                             OBJECT_D_LIFT, (void*)1);
    }

    if (((self->dyna.actor.params >> 1) & 1) == 0) {
        FUN_0037378c(play, temp_s3, 120.0f, 12, 120, 100, 1);
    } else if (((self->dyna.actor.params >> 1) & 1) == 1) {
        FUN_0037378c(play, temp_s3, 60.0f, 8, 60, 100, 1);
    }
}

void FUN_003bb7bc(ObjLift* self, PlayState* play) {
    s32 bgId;
    Vec3f pos;

    Actor_MoveForward(&self->dyna.actor);
    Math_Vec3f_Copy(&pos, &self->dyna.actor.prevPos);
    pos.y += sMaxFallDistances[(self->dyna.actor.params >> 1) & 1];
    self->dyna.actor.floorHeight =
        BgCheck_EntityRaycastFloor4(&play->colCtx, &self->dyna.actor.floorPoly, &bgId, &self->dyna.actor, &pos);

    if ((self->dyna.actor.floorHeight - self->dyna.actor.world.pos.y) >=
        (sMaxFallDistances[(self->dyna.actor.params >> 1) & 1] - 0.001f)) {
        ObjLift_SpawnFragments(self, play);
        Audio_PlaySoundAtPosition(play, &self->dyna.actor.world.pos, 20, 0x01000190);
        Flags_SetSwitch(play, (self->dyna.actor.params & 0xFF) >> 2);
        Actor_Kill(&self->dyna.actor);
    }
}
#endif

void ObjLift_Draw(Actor* actor, GameState* state) {
    ObjLift* self = (ObjLift*)actor;

    self->skelAnimModel->FUN_003721e0(&self->dyna.actor.modelMtx);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_00372170(0);
}
}
