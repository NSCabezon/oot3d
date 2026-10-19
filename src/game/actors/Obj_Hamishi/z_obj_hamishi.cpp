extern "C" {
#include "z_obj_hamishi.hpp"

#define FLAGS 0x80000000

void ObjHamishi_Init(Actor* actor, PlayState* play);
void ObjHamishi_Destroy(Actor* actor, PlayState* play2);
void ObjHamishi_Update(Actor* thisx, PlayState* play);
void ObjHamishi_Draw(Actor* actor, PlayState* play);

GLOBAL_ASM("data/z_Obj_Hamishi.data.s")

ActorInit Obj_Hamishi_InitVars = {
    ACTOR_OBJ_HAMISHI,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_FIELD_KEEP,
    sizeof(ObjHamishi),
    (ActorFunc)ObjHamishi_Init,
    (ActorFunc)ObjHamishi_Destroy,
    (ActorFunc)ObjHamishi_Update,
    (ActorFunc)ObjHamishi_Draw,
};

GLOBAL_ASM("../src/game/actors/Obj_Hamishi/data_syms.s")

extern ColliderCylinderInit ObjHamishi_sCylinderInit;
extern CollisionCheckInfoInit ObjHamishi_sColChkInfoInit;
extern InitChainEntry ObjHamishi_sInitChain[];
extern s16 ObjHamishi_sEffectScales[];

s32 Math_ScaledStepToS(s16* value, s16 target, s16 step);

static void ObjHamishi_InitCollision(Actor* thisx, PlayState* play) {
    ObjHamishi* self = (ObjHamishi*)thisx;

    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &ObjHamishi_sCylinderInit);
    Collider_UpdateCylinder(&self->actor, &self->collider);
}

void ObjHamishi_Init(Actor* thisx, PlayState* play) {
    ObjHamishi* self = (ObjHamishi*)thisx;

    Actor_ProcessInitChain(&self->actor, ObjHamishi_sInitChain);
    if (play->GetCutsceneState()) {
        self->actor.uncullZoneForward += 1000.0f;
    }
    if (self->actor.shape.rot.y == 0) {
        self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.home.rot.y = Rand_ZeroFloat(65536.0f);
    }
    ObjHamishi_InitCollision(&self->actor, play);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, &ObjHamishi_sColChkInfoInit);
    if (Flags_GetSwitch(play, self->actor.params & 0x3f)) {
        Actor_Kill(&self->actor);
        return;
    }
    self->actor.shape.yOffset = 80.0f;
    FUN_00372f38(&self->actor, play, &self->model, 4, 0);
}

void ObjHamishi_Destroy(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    ObjHamishi* self = (ObjHamishi*)thisx;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_00350f34(thisx, &self->model, 0);
}

GLOBAL_ASM("asm/ObjHamishi_Update.s")

// Draft (Sonnet G3, ~70 variants, still ~50 insns off): ROM keeps `thisx` in a separate callee-saved reg (fp) next to
// `self` (r5) and base (self+0x200) in r4; armcc here coalesces them. Break loop constants (0x421/-450/...) come out
// derived instead of pool loads. Kept off.
#if 0
static inline void ObjHamishi_Shake(ObjHamishi* self) {
    if (self->shakeFrames > 0) {
        self->shakeFrames--;
        self->shakePosPhase += 5000;
        self->shakeRotPhase += 0xE10;

        Math_StepToF(&self->shakePosSize, 0.0f, 0.15f);
        Math_StepToF(&self->shakeRotSize, 0.0f, 40.0f);

        self->actor.world.pos.x = self->actor.home.pos.x + (Math_SinS(self->shakePosPhase * 4) * self->shakePosSize);
        self->actor.world.pos.z = self->actor.home.pos.z + (Math_CosS(self->shakePosPhase * 7) * self->shakePosSize);
        self->actor.shape.rot.x =
            self->actor.home.rot.x + (s16)(Math_SinS(self->shakeRotPhase * 4) * self->shakeRotSize);
        self->actor.shape.rot.z =
            self->actor.home.rot.z + (s16)(Math_CosS(self->shakeRotPhase * 7) * self->shakeRotSize);
    } else {
        Math_StepToF(&self->actor.world.pos.x, self->actor.home.pos.x, 1.0f);
        Math_StepToF(&self->actor.world.pos.z, self->actor.home.pos.z, 1.0f);
        Math_ScaledStepToS(&self->actor.shape.rot.x, self->actor.home.rot.x, 0xBB8);
        Math_ScaledStepToS(&self->actor.shape.rot.z, self->actor.home.rot.z, 0xBB8);
    }
}

static inline void ObjHamishi_Break(ObjHamishi* self, PlayState* play) {
    Vec3f velocity;
    Vec3f pos;
    s16 phi_s0 = 1000;
    s16 gravity;
    s16 phi_v0;
    f32 temp_f20;
    f32 temp_f22;
    s32 i;

    for (i = 0; i < 9; i++) {
        phi_s0 += 20000;

        temp_f20 = Rand_ZeroOne() * 10.0f;
        pos.x = (Math_SinS(phi_s0) * temp_f20) + self->actor.world.pos.x;
        pos.y = (Rand_ZeroOne() * 40.0f) + self->actor.world.pos.y + 5.0f;
        pos.z = (Math_CosS(phi_s0) * temp_f20) + self->actor.world.pos.z;

        temp_f20 = (Rand_ZeroOne() * 10.0f) + 2.0f;
        velocity.x = Math_SinS(phi_s0) * temp_f20;
        temp_f22 = Rand_ZeroOne();
        velocity.y = (Rand_ZeroOne() * i * 2.5f) + (temp_f22 * 15.0f);
        velocity.z = Math_CosS(phi_s0) * temp_f20;

        if (i == 0) {
            phi_v0 = 0x421;
            gravity = -450;
        } else if (i < 4) {
            phi_v0 = 0x221;
            gravity = -380;
        } else {
            phi_v0 = 0x241;
            gravity = -320;
        }

        EffectSsKakera_Spawn(play, &pos, &velocity, &self->actor.world.pos, gravity, phi_v0, 30, 5, 0,
                             ObjHamishi_sEffectScales[i], 3, 0, 70, 1, 2, (void*)3);
    }

    FUN_0037378c(play, &self->actor.world.pos, 140.0f, 6, 180, 90, 1);
    FUN_0037378c(play, &self->actor.world.pos, 140.0f, 12, 80, 90, 1);
}

void ObjHamishi_Update(Actor* thisx, PlayState* play) {
    ObjHamishi* self = (ObjHamishi*)thisx;
    Actor* actor = thisx;

    ObjHamishi_Shake((ObjHamishi*)thisx);

    if ((self->collider.base.acFlags & 0x2) && (self->collider.info.acHitInfo->toucher.dmgFlags & 0x40000040)) {
        self->collider.base.acFlags &= ~0x2;
        self->hitCount++;
        if (self->hitCount < 2) {
            self->shakeFrames = 15;
            self->shakePosSize = 2.0f;
            self->shakeRotSize = 400.0f;
        } else {
            { PlayState* play2 = play; ObjHamishi_Break(self, play2); };
            Audio_PlaySoundAtPosition(play, &actor->world.pos, 40, 0x1000167);
            Flags_SetSwitch(play, actor->params & 0x3F);
            if (actor->params & 0x100) {
                if (play->sceneNum == 0x61 && actor->room == 1 &&
                    ((actor->params == 0x115 && Flags_GetSwitch(play, 0x16)) ||
                     (actor->params == 0x116 && Flags_GetSwitch(play, 0x15)))) {
                } else {
                    FUN_00372244(&play->unk_5FCC, 0x1E, 0x1000487);
                }
            }
            Actor_Kill(actor);
        }
    } else {
        self->collider.base.acFlags &= ~0x2;
        if (actor->xzDistToPlayer < 600.0f) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
            CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
        }
    }
}
#endif

void ObjHamishi_Draw(Actor* thisx, PlayState* play) {
    ObjHamishi* self = (ObjHamishi*)thisx;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->actor.modelMtx);
    FUN_003695cc(self->model, 0, 4, 0, 1.0f, 2.0f / 3.0f, 26.0f / 51.0f, 1.0f);
    if (self->model != NULL) {
        self->model->SetUnkAC(1);
        self->model->FUN_003721e0(&modelMtx);
        self->model->FUN_00372170(0);
    }
}
}
