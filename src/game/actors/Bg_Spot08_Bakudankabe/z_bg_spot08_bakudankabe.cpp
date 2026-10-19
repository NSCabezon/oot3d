extern "C" {
#include "z_bg_spot08_bakudankabe.hpp"

#define FLAGS 0x80400000

#define NA_SE_EV_WALL_BROKEN 0x01000167
#define NA_SE_SY_CORRECT_CHIME 0x01000487
#define PARAMS_GET_U(params, start, count) (((params) >> (start)) & ((1 << (count)) - 1))

void BgSpot08Bakudankabe_Init(Actor* actor, GameState* state);
void BgSpot08Bakudankabe_Destroy(Actor* actor, GameState* state);
void BgSpot08Bakudankabe_Update(Actor* actor, GameState* state);
void BgSpot08Bakudankabe_Draw(Actor* actor, GameState* state);

void Collider_InitJntSph(PlayState* play, BgSpot08BakudankabeColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, BgSpot08BakudankabeColliderJntSph* collider, Actor* actor, void* init,
                        BgSpot08BakudankabeColliderElement* elements);
void Collider_DestroyJntSph(PlayState* play, BgSpot08BakudankabeColliderJntSph* collider);
void Collider_UpdateSpheres(s32 index, BgSpot08BakudankabeColliderJntSph* collider, MTX34* mtx);

GLOBAL_ASM("data/z_Bg_Spot08_Bakudankabe.data.s")

ActorInit Bg_Spot08_Bakudankabe_InitVars = {
    ACTOR_BG_SPOT08_BAKUDANKABE,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT08_OBJ,
    sizeof(BgSpot08Bakudankabe),
    (ActorFunc)BgSpot08Bakudankabe_Init,
    (ActorFunc)BgSpot08Bakudankabe_Destroy,
    (ActorFunc)BgSpot08Bakudankabe_Update,
    (ActorFunc)BgSpot08Bakudankabe_Draw,
};

// Data lives in the data dump above (no labels). Absolute symbols in data_syms.s (EQU).
GLOBAL_ASM("../src/game/actors/Bg_Spot08_Bakudankabe/data_syms.s")
extern void* BgSpot08Bakudankabe_sJntSphInit[];
extern Vec3f BgSpot08Bakudankabe_sBurstPos[];
extern InitChainEntry BgSpot08Bakudankabe_sInitChain[];

// Init: matched. The ROM keeps the incoming actor in r5 and a late copy in r4: the JntSph helper takes `Actor* thisx`
// and casts inside, and DynaPolyInfo_Alloc gets `(DynaPolyActor*)actor`.
static __inline void BgSpot08Bakudankabe_InitJntSph(Actor* thisx, PlayState* play) {
    BgSpot08Bakudankabe* self = (BgSpot08Bakudankabe*)thisx;
    Collider_InitJntSph(play, &self->collider);
    Collider_SetJntSph(play, &self->collider, &self->dyna.actor, BgSpot08Bakudankabe_sJntSphInit,
                       self->colliderElements);
}

void BgSpot08Bakudankabe_Init(Actor* actor, GameState* state) {
    BgSpot08Bakudankabe* self = (BgSpot08Bakudankabe*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, 0);
    FUN_00372f38(actor, play, &self->skelAnimModel, 2, 0);
    if (Flags_GetSwitch(play, PARAMS_GET_U(actor->params, 0, 6))) {
        Actor_Kill(actor);
        return;
    }
    BgSpot08Bakudankabe_InitJntSph(actor, play);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc((DynaPolyActor*)actor, state, 2), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    Actor_ProcessInitChain(actor, BgSpot08Bakudankabe_sInitChain);
}

// Update draft: 173/183 words match; only the s-register numbering of the 65/12 constant differs (ROM s26,
// ours s21; 8 other float constants shift by one). ~25 variants tried (see STATUS.md).
// Stage 3 (Opus): s-reg numbering follows the creation order of the hoisted constants (statement order): ROM has
// 65/12 created after 0.2/12/75/10, i.e. as if the y statement came after `scale = ...`, yet it is scheduled in
// the first block. Moving the y statement later gives s26 but breaks scheduling (calls are barriers). ~35 more
// variants (expression shapes, double/divide forms, locals, inline helpers, += splits, decl orders) no effect.
#if 0
static __inline void BgSpot08Bakudankabe_SpawnBurst(BgSpot08Bakudankabe* self, PlayState* play) {
    s32 i;
    Vec3f burstDepthY;
    Vec3f burstDepthX;
    f32 sinY;
    f32 cosY;

    sinY = Math_SinS(self->dyna.actor.shape.rot.y);
    cosY = Math_CosS(self->dyna.actor.shape.rot.y);

    burstDepthX.z = 0.0f;
    burstDepthX.x = 0.0f;

    for (i = 0; i < 24; i++) {
        s16 scale;
        s16 gravityInfluence;
        f32 temp1;
        f32 temp2;
        s16 rotationSpeed;

        temp1 = (Rand_ZeroOne() - 0.5f) * 440.0f;
        temp2 = (Rand_ZeroOne() - 0.5f) * 20.0f;

        burstDepthY.x = self->dyna.actor.world.pos.x + temp2 * sinY + (temp1 * cosY);
        burstDepthY.y = (self->dyna.actor.world.pos.y + 20.0f) + (i * (65.0f / 12.0f));
        burstDepthY.z = self->dyna.actor.world.pos.z + temp2 * cosY - (temp1 * sinY);

        burstDepthX.y = (Rand_ZeroOne() - 0.2f) * 12.0f;
        scale = Rand_ZeroOne() * 75.0f + 10.0f;

        if (scale < 25) {
            gravityInfluence = -300;
        } else if (scale < 50) {
            gravityInfluence = -360;
        } else {
            gravityInfluence = -420;
        }

        rotationSpeed = (Rand_ZeroOne() < 0.4f) ? 65 : 33;

        EffectSsKakera_Spawn(play, &burstDepthY, &burstDepthX, &burstDepthY, gravityInfluence, rotationSpeed, 0x1E, 4,
                             0, scale, 1, 3, 80, -1, OBJECT_GAMEPLAY_FIELD_KEEP, (void*)5);
    }

    for (i = 0; i < 3; i++) {
        burstDepthY.x = (BgSpot08Bakudankabe_sBurstPos[i].z * sinY + BgSpot08Bakudankabe_sBurstPos[i].x * cosY) +
                        self->dyna.actor.world.pos.x;
        burstDepthY.y = self->dyna.actor.world.pos.y + BgSpot08Bakudankabe_sBurstPos[i].y;
        burstDepthY.z = (self->dyna.actor.world.pos.z + BgSpot08Bakudankabe_sBurstPos[i].z * cosY) -
                        BgSpot08Bakudankabe_sBurstPos[i].x * sinY;
        FUN_0037378c(play, &burstDepthY, 120.0f, 4, 0x78, 0xA0, 1);
    }
}

void BgSpot08Bakudankabe_Update(Actor* thisx, GameState* state) {
    BgSpot08Bakudankabe* self = (BgSpot08Bakudankabe*)thisx;
    PlayState* play = (PlayState*)state;


    if (((BgSpot08Bakudankabe*)thisx)->collider.base.acFlags & AC_HIT) {
        BgSpot08Bakudankabe* s2 = self;
        BgSpot08Bakudankabe_SpawnBurst(s2, play);
        Flags_SetSwitch(play, PARAMS_GET_U(thisx->params, 0, 6));
        Audio_PlaySoundAtPosition(play, &thisx->world.pos, 40, NA_SE_EV_WALL_BROKEN);
        FUN_00372244((u8*)play + 0x5FCC, 30, NA_SE_SY_CORRECT_CHIME);
        Actor_Kill(thisx);
    } else if (thisx->xzDistToPlayer < 800.0f) {
        CollisionCheck_SetAC(((PlayState*)state), &((PlayState*)state)->colChkCtx, &self->collider.base);
    }
}


#endif

GLOBAL_ASM("asm/BgSpot08Bakudankabe_Update.s")

void BgSpot08Bakudankabe_Destroy(Actor* actor, GameState* state) {
    BgSpot08Bakudankabe* self = (BgSpot08Bakudankabe*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyJntSph(play, &self->collider);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgSpot08Bakudankabe_Draw(Actor* actor, GameState* state) {
    BgSpot08Bakudankabe* self = (BgSpot08Bakudankabe*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    Collider_UpdateSpheres(0, &self->collider, &modelMtx);
    Collider_UpdateSpheres(1, &self->collider, &modelMtx);
    Collider_UpdateSpheres(2, &self->collider, &modelMtx);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}
}
