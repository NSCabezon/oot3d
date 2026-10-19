extern "C" {
#include "z_bg_jya_haheniron.hpp"

#define FLAGS 0x80400010

#define UPDBGCHECKINFO_FLAG_0 (1 << 0)
#define UPDBGCHECKINFO_FLAG_2 (1 << 2)
#define UPDBGCHECKINFO_FLAG_7 (1 << 7)
#define BGCHECKFLAG_GROUND (1 << 0)
#define BGCHECKFLAG_WALL (1 << 3)

// Helpers not declared in include/ yet (3DS layout, local for now, as in Bg_Jya_Goroiwa).
void Collider_InitJntSph(PlayState* play, BgJyaHahenironColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, BgJyaHahenironColliderJntSph* collider, Actor* actor,
                        BgJyaHahenironJntSphInit* init, BgJyaHahenironColliderElement* elements);
void Collider_DestroyJntSph(PlayState* play, BgJyaHahenironColliderJntSph* collider);
void Collider_UpdateSpheres(s32 index, BgJyaHahenironColliderJntSph* collider, MTX34* mtx);
void FUN_0036a154(PlayState* play, Vec3f* pos, Vec3f* vel);

void BgJyaHaheniron_Init(Actor* actor, GameState* state);
void BgJyaHaheniron_Destroy(Actor* actor, GameState* state);
void BgJyaHaheniron_Update(Actor* actor, GameState* state);
void BgJyaHaheniron_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Jya_Haheniron.data_005128D8.s")
GLOBAL_ASM("data/z_Bg_Jya_Haheniron.data_00512948.s")
GLOBAL_ASM("../src/game/actors/Bg_Jya_Haheniron/data_syms.s")

extern s32 BgJyaHaheniron_sObjIndex[];
extern BgJyaHahenironJntSphInit BgJyaHaheniron_sJntSphInit;
extern s16 BgJyaHaheniron_sKakeraScales[];
extern InitChainEntry BgJyaHaheniron_sInitChain[];
extern f32 BgJyaHaheniron_sScales[];

ActorInit Bg_Jya_Haheniron_InitVars = {
    ACTOR_BG_JYA_HAHENIRON,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_JYA_IRON,
    sizeof(BgJyaHaheniron),
    (ActorFunc)BgJyaHaheniron_Init,
    (ActorFunc)BgJyaHaheniron_Destroy,
    (ActorFunc)BgJyaHaheniron_Update,
    (ActorFunc)BgJyaHaheniron_Draw,
};

void BgJyaHaheniron_ChairCrumble(BgJyaHaheniron* self, PlayState* play);
void BgJyaHaheniron_PillarCrumble(BgJyaHaheniron* self, PlayState* play);
void BgJyaHaheniron_RubbleCollide(BgJyaHaheniron* self, PlayState* play);

static inline void BgJyaHaheniron_SetupAction(BgJyaHaheniron* self, BgJyaHahenironActionFunc func) {
    self->actionFunc = func;
}

void BgJyaHaheniron_Init(Actor* actor, GameState* state) {
    BgJyaHaheniron* self = (BgJyaHaheniron*)actor;
    PlayState* play = (PlayState*)state;

    Actor_ProcessInitChain(actor, BgJyaHaheniron_sInitChain);
    Actor_SetScale(actor, BgJyaHaheniron_sScales[self->actor.params]);
    FUN_00372f38(actor, play, &self->skelAnimModel, BgJyaHaheniron_sObjIndex[self->actor.params], 0);
    if (self->actor.params == 0) {
        Collider_InitJntSph(play, &self->collider);
        Collider_SetJntSph(play, &self->collider, actor, &BgJyaHaheniron_sJntSphInit, self->colliderElements);
        self->actor.shape.rot.z = Rand_ZeroOne() * 65535.0f;
        BgJyaHaheniron_SetupAction(self, BgJyaHaheniron_ChairCrumble);
    } else if (self->actor.params == 1) {
        BgJyaHaheniron_SetupAction(self, BgJyaHaheniron_PillarCrumble);
    } else if (self->actor.params == 2) {
        BgJyaHaheniron_SetupAction(self, BgJyaHaheniron_RubbleCollide);
    }
}

void BgJyaHaheniron_Destroy(Actor* actor, GameState* state) {
    BgJyaHaheniron* self = (BgJyaHaheniron*)actor;
    PlayState* play = (PlayState*)state;

    if (self->actor.params == 0) {
        Collider_DestroyJntSph(play, &self->collider);
    }
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgJyaHaheniron_ChairCrumble(BgJyaHaheniron* self, PlayState* play) {
    Vec3f vec;

    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 5.0f, 8.0f, 0.0f,
                            UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_7);
    if ((self->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_WALL)) ||
        ((self->collider.base.atFlags & AT_HIT) && (self->collider.base.at != NULL) &&
         (self->collider.base.at->type == ACTORCAT_PLAYER))) {
        vec.x = -Rand_ZeroOne() * self->actor.velocity.x;
        vec.y = -Rand_ZeroOne() * self->actor.velocity.y;
        vec.z = -Rand_ZeroOne() * self->actor.velocity.z;
        FUN_0036a154(play, &self->actor.world.pos, &vec);
        Actor_Kill(&self->actor);
    } else if (self->timer > 90) {
        Actor_Kill(&self->actor);
    } else {
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
    }
    self->actor.shape.rot.y += 0x4B0;
    self->actor.shape.rot.x += 0xFA0;
}

// PillarCrumble / RubbleCollide: own TU z_bg_jya_haheniron_crumble.cpp (their guarded statics need their own
// .data/.bss sections, pinned in data/oot3d.ld).

void BgJyaHaheniron_Update(Actor* actor, GameState* state) {
    BgJyaHaheniron* self = (BgJyaHaheniron*)actor;
    PlayState* play = (PlayState*)state;

    self->timer++;
    self->actionFunc(self, play);
}

void BgJyaHaheniron_Draw(Actor* actor, GameState* state) {
    BgJyaHaheniron* self = (BgJyaHaheniron*)actor;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &actor->modelMtx);
    if (self->actor.params == 0) {
        Collider_UpdateSpheres(0, &self->collider, &mtx);
    }
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&mtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

void FUN_0036a154(PlayState* play, Vec3f* vec1, Vec3f* vec2) {
    Vec3f vel;
    Vec3f pos;
    s16 arg5;
    s32 i;
    f32 rand1;

    for (i = 0; i < 5; i++) {
        s16 angle;

        rand1 = Rand_ZeroOne() * 10.0f;
        angle = i * 0x3333;
        vel.x = (Math_SinS(angle) * rand1) + vec2->x;
        vel.y = (Rand_ZeroOne() * 10.0f) + vec2->y;
        vel.z = (Math_CosS(angle) * rand1) + vec2->z;

        rand1 = Rand_ZeroOne();
        if (rand1 < 0.2f) {
            arg5 = 96;
        } else if (rand1 < 0.8f) {
            arg5 = 64;
        } else {
            arg5 = 32;
        }

        EffectSsKakera_Spawn(play, vec1, &vel, vec1, -350, arg5, 40, 4, 0, BgJyaHaheniron_sKakeraScales[i], 0, 20, 40,
                             -1, OBJECT_JYA_IRON, (void*)1);
    }
    pos.x = vec1->x + (vec2->x * 5.0f);
    pos.y = vec1->y + (vec2->y * 5.0f);
    pos.z = vec1->z + (vec2->z * 5.0f);
    FUN_0037378c(play, &pos, 100.0f, 4, 100, 160, 1);
}

}
