extern "C" {
#include "z_bg_ice_turara.hpp"

#define FLAGS 0x80400000

#define NA_SE_EV_ICE_BROKEN 0x28CB
#define NA_SE_EV_ICE_SWING 0x28D4
#define UPDBGCHECKINFO_FLAG_2 (1 << 2)
#define BGCHECKFLAG_GROUND (1 << 0)
#define TURARA_STALAGMITE 0
#define TURARA_STALACTITE 1
#define TURARA_STALACTITE_REGROW 2

void Actor_PlaySfx(Actor* actor, u16 sfxId);
void Actor_MoveForward(Actor* actor);
void FUN_0036b940(PlayState* play, DynaCollisionContext* dyna, s32 bgId);
void FUN_0036d15c(PlayState* play, DynaCollisionContext* dyna, s32 bgId);
void SfxSource_PlaySfxAtFixedWorldPos(PlayState* play, Vec3f* worldPos, s32 duration, u16 sfxId);
void EffectSsEnIce_Spawn(PlayState* play, Vec3f* pos, f32 scale, Vec3f* velocity, Vec3f* accel,
                         Color_RGBA8* primColor, Color_RGBA8* envColor, s32 life);

void BgIceTurara_Init(Actor* actor, PlayState* play);
void BgIceTurara_Destroy(Actor* actor, PlayState* play);
void BgIceTurara_Update(Actor* actor, PlayState* play);
void BgIceTurara_Draw(Actor* actor, PlayState* play);

void BgIceTurara_Break(BgIceTurara* self, PlayState* play, f32 arg2);
void BgIceTurara_Stalagmite(BgIceTurara* self, PlayState* play);
void BgIceTurara_Wait(BgIceTurara* self, PlayState* play);
void BgIceTurara_Shiver(BgIceTurara* self, PlayState* play);
void BgIceTurara_Fall(BgIceTurara* self, PlayState* play);
void BgIceTurara_Regrow(BgIceTurara* self, PlayState* play);

GLOBAL_ASM("data/z_Bg_Ice_Turara.data.s")

GLOBAL_ASM("../src/game/actors/Bg_Ice_Turara/data_syms.s")

extern InitChainEntry BgIceTurara_sInitChain[];
extern ColliderCylinderInit BgIceTurara_sCylinderInit;
#define sInitChain BgIceTurara_sInitChain
#define sCylinderInit BgIceTurara_sCylinderInit

ActorInit Bg_Ice_Turara_InitVars = {
    ACTOR_BG_ICE_TURARA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_ICE_OBJECTS,
    sizeof(BgIceTurara),
    (ActorFunc)BgIceTurara_Init,
    (ActorFunc)BgIceTurara_Destroy,
    (ActorFunc)BgIceTurara_Update,
    (ActorFunc)BgIceTurara_Draw,
};

void BgIceTurara_Init(Actor* actor, PlayState* play) {
    BgIceTurara* self = (BgIceTurara*)actor;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, 0);
    FUN_00372f38(&self->dyna.actor, play, &self->model, 5, 0);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 2), &colHeader);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->dyna.actor, &sCylinderInit);
    Collider_UpdateCylinder(&self->dyna.actor, &self->collider);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
    if (self->dyna.actor.params == TURARA_STALAGMITE) {
        self->actionFunc = BgIceTurara_Stalagmite;
    } else {
        self->dyna.actor.shape.rot.x = -0x8000;
        self->dyna.actor.shape.yOffset = 1200.0f;
        self->actionFunc = BgIceTurara_Wait;
    }
    actor->unk_19B = 2;
}


void BgIceTurara_Destroy(Actor* actor, PlayState* play) {
    BgIceTurara* self = (BgIceTurara*)actor;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyCylinder(play, &self->collider);
    FUN_00350f34(&self->dyna.actor, &self->model, 0);
}

// BgIceTurara_Break: own TU z_bg_ice_turara_break.cpp (its guarded statics need their own .data/.bss sections).

void BgIceTurara_Stalagmite(BgIceTurara* self, PlayState* play) {
    if (self->collider.base.acFlags & AC_HIT) {
        BgIceTurara_Break(self, play, 50.0f);
        Actor_Kill(&self->dyna.actor);
        return;
    }
    CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
}

void BgIceTurara_Wait(BgIceTurara* self, PlayState* play) {
    if (self->dyna.actor.xzDistToPlayer < 60.0f) {
        self->shiverTimer = 15;
        self->actionFunc = BgIceTurara_Shiver;
    }
}

#if 0
// near-miss draft (18 words off: VFP temp register for the shiver offset, ROM t_x=s18 / z-copy=s17). R5 sweep: sign type,
// t local/inline, mul/add operand orders, f32 const locals decl/assign perms (300), static inline/static helpers, N64 shape.
void BgIceTurara_Shiver(BgIceTurara* self, PlayState* play) {
    s16 phi_v0_3;
    s16 phi_v0_2;
    f32 sp28;

    if (self->shiverTimer != 0) {
        self->shiverTimer--;
    }
    if (!(self->shiverTimer % 6)) {
        Actor_PlaySfx(&self->dyna.actor, NA_SE_EV_ICE_SWING);
    }
    if (self->shiverTimer == 0) {
        self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x;
        self->dyna.actor.world.pos.z = self->dyna.actor.home.pos.z;
        Collider_UpdateCylinder(&self->dyna.actor, &self->collider);
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
        FUN_0036b940(play, &play->colCtx.dyna, self->dyna.bgId);
        self->actionFunc = BgIceTurara_Fall;
    } else {
        f32 t;

        sp28 = Rand_ZeroOne();
        t = (0.5f * sp28) + 0.5f;
        phi_v0_2 = (Rand_ZeroOne() < 0.5f ? -1 : 1);
        self->dyna.actor.world.pos.x = (t * phi_v0_2) + self->dyna.actor.home.pos.x;
        sp28 = Rand_ZeroOne();
        t = (0.5f * sp28) + 0.5f;
        phi_v0_3 = (Rand_ZeroOne() < 0.5f ? -1 : 1);
        self->dyna.actor.world.pos.z = (t * phi_v0_3) + self->dyna.actor.home.pos.z;
    }
}
#endif

GLOBAL_ASM("asm/BgIceTurara_Shiver.s")

void BgIceTurara_Fall(BgIceTurara* self, PlayState* play) {
    if ((self->collider.base.atFlags & AT_HIT) || (self->dyna.actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        self->collider.base.atFlags &= ~AT_HIT;
        self->dyna.actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
        if (self->dyna.actor.world.pos.y < self->dyna.actor.floorHeight) {
            self->dyna.actor.world.pos.y = self->dyna.actor.floorHeight;
        }
        BgIceTurara_Break(self, play, 40.0f);
        if (self->dyna.actor.params == TURARA_STALACTITE_REGROW) {
            self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y + 120.0f;
            FUN_0036d15c(play, &play->colCtx.dyna, self->dyna.bgId);
            self->actionFunc = BgIceTurara_Regrow;
        } else {
            Actor_Kill(&self->dyna.actor);
            return;
        }
    } else {
        Actor_MoveForward(&self->dyna.actor);
        self->dyna.actor.world.pos.y += 40.0f;
        Actor_UpdateBgCheckInfo(play, &self->dyna.actor, 0.0f, 0.0f, 0.0f, UPDBGCHECKINFO_FLAG_2);
        self->dyna.actor.world.pos.y -= 40.0f;
        Collider_UpdateCylinder(&self->dyna.actor, &self->collider);
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
    }
}


void BgIceTurara_Regrow(BgIceTurara* self, PlayState* play) {
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 1.0f)) {
        self->dyna.actor.velocity.y = 0.0f;
        self->actionFunc = BgIceTurara_Wait;
    }
}

void BgIceTurara_Update(Actor* actor, PlayState* play) {
    BgIceTurara* self = (BgIceTurara*)actor;

    self->actionFunc(self, play);
}

void BgIceTurara_Draw(Actor* actor, PlayState* play) {
    BgIceTurara* self = (BgIceTurara*)actor;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &self->dyna.actor.modelMtx);
    if (self->model != NULL) {
        self->model->SetUnkAC(1);
        self->model->FUN_003721e0(&mtx);
        self->model->FUN_00372170(1);
    }
}

}
