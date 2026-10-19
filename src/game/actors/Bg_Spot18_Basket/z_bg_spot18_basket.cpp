extern "C" {
#include "z_bg_spot18_basket.hpp"

#define FLAGS 0x80400010

#define PARAMS_GET_U(params, start, count) (((params) >> (start)) & ((1 << (count)) - 1))

// Data lives in GLOBAL_ASM("data/z_Bg_Spot18_Basket.data.s"); no C symbols for it yet.
#define sInitChain ((InitChainEntry*)0x00513F58)
#define sJntSphInit ((void*)0x00513F48)

void Collider_InitJntSph(PlayState* play, BgSpot18BasketColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, BgSpot18BasketColliderJntSph* collider, Actor* actor, void* init,
                        BgSpot18BasketColliderElement* elements);
void Collider_DestroyJntSph(PlayState* play, BgSpot18BasketColliderJntSph* collider);
void Collider_UpdateSpheres(s32 index, BgSpot18BasketColliderJntSph* collider, MTX34* mtx);
f32 BgCheck_EntityRaycastFloor4(CollisionContext* colCtx, CollisionPoly** outPoly, s32* outBgId, Actor* actor,
                                Vec3f* pos);
void FUN_003bf948(BgSpot18Basket* self, PlayState* play); // Inactive (GLOBAL_ASM, cfg-merged tail)
void FUN_003c1398(BgSpot18Basket* self, PlayState* play); // Spinning
void FUN_003cc628(BgSpot18Basket* self, PlayState* play); // Activation
void FUN_001843e0(BgSpot18Basket* self, PlayState* play); // ExplosionCs (not in this actor's dir)
s32 Math_StepToS(s16* pValue, s16 target, s16 step);
void FUN_0036b940(PlayState* play, DynaCollisionContext* dyna, s32 bgId);

s32 OnePointCutscene_Init(PlayState* play, s32 csId, s32 arg2, Actor* actor, s32 camId);

void BgSpot18Basket_Init(Actor* actor, GameState* state);
void BgSpot18Basket_Destroy(Actor* actor, GameState* state);
void BgSpot18Basket_Update(Actor* actor, GameState* state);
void BgSpot18Basket_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Spot18_Basket.data.s")

ActorInit Bg_Spot18_Basket_InitVars = {
    ACTOR_BG_SPOT18_BASKET,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_SPOT18_OBJ,
    sizeof(BgSpot18Basket),
    (ActorFunc)BgSpot18Basket_Init,
    (ActorFunc)BgSpot18Basket_Destroy,
    (ActorFunc)BgSpot18Basket_Update,
    (ActorFunc)BgSpot18Basket_Draw,
};

void BgSpot18Basket_Init(Actor* actor, GameState* state) {
    BgSpot18Basket* self = (BgSpot18Basket*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, 3);
    FUN_00372f38(actor, play, &self->skelAnimModel, 2, 0);
    Collider_InitJntSph(play, &self->colliderJntSph);
    Collider_SetJntSph(play, &self->colliderJntSph, actor, sJntSphInit, self->colliderJntSphElements);
    actor->colChkInfo.mass = MASS_IMMOVABLE;
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 2), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    Actor_ProcessInitChain(actor, sInitChain);
    ActorShape_Init(&actor->shape, 0.0f, ActorShadow_DrawCircle, 15.0f);
    actor->home.pos.y += 0.01f;
    actor->world.pos.y = actor->home.pos.y;
    actor->unk_19B = 2;

    if (Flags_GetSwitch(play, PARAMS_GET_U(actor->params, 8, 6))) {
        self->actionFunc = FUN_003c1398;
        self->circleRate = 0;
        self->spinRate = 0;
        return;
    }

    self->actionFunc = FUN_003bf948;
    PlayState* p2 = play; // separate alias: keeps arg setup order (play before actor) like the 3DS build
    Actor_SpawnAsChild(&p2->actorCtx, actor, p2, ACTOR_BG_SPOT18_FUTA, actor->world.pos.x, actor->world.pos.y,
                       actor->world.pos.z, actor->shape.rot.x, actor->shape.rot.y + 0x1555, actor->shape.rot.z, -1);
    if (actor->child == NULL) {
        Actor_Kill(actor);
    }
}

void BgSpot18Basket_Destroy(Actor* actor, GameState* state) {
    BgSpot18Basket* self = (BgSpot18Basket*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyJntSph(play, &self->colliderJntSph);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgSpot18Basket_Update(Actor* actor, GameState* state) {
    BgSpot18Basket* self = (BgSpot18Basket*)actor;
    PlayState* play = (PlayState*)state;
    s32 bgId;

    self->timer++;
    self->actionFunc(self, play);
    actor->floorHeight = BgCheck_EntityRaycastFloor4(&play->colCtx, &actor->floorPoly, &bgId, actor, &actor->world.pos);
    if (self->actionFunc != FUN_003bf948) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->colliderJntSph.base);
        if (self->actionFunc != FUN_003cc628) {
            self->colliderJntSph.base.acFlags &= ~AC_HIT;
            CollisionCheck_SetAC(play, &play->colChkCtx, &self->colliderJntSph.base);
        }
    }
}

void BgSpot18Basket_Draw(Actor* actor, GameState* state) {
    BgSpot18Basket* self = (BgSpot18Basket*)actor;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &actor->modelMtx);
    Collider_UpdateSpheres(0, &self->colliderJntSph, &mtx);
    Collider_UpdateSpheres(1, &self->colliderJntSph, &mtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&mtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

void FUN_003cc628(BgSpot18Basket* self, PlayState* play) {
    if (self->timer > 30) {
        self->actionFunc = FUN_003c1398;
        self->circleRate = 0;
        self->spinRate = 0;
        self->dyna.actor.child->parent = NULL;
        self->dyna.actor.child = NULL;
    }
}

// Spinning

static inline f32 Math3D_Dist2DSq(f32 x0, f32 y0, f32 x1, f32 y1) {
    f32 dx = x0 - x1;
    f32 dy = y0 - y1;

    return dx * dx + dy * dy;
}

void FUN_003c1398(BgSpot18Basket* self, PlayState* play) {
    Actor* actor = &self->dyna.actor;
    Actor* colliderBaseAc;
    f32 positionDiff;

    Math_StepToS(&self->spinRate, 0x1F4, 0x1E);
    actor->shape.rot.y += self->spinRate;
    Math_StepToF(&self->circleRadius, 50.0f, 1.5f);
    Math_StepToS(&self->circleRate, 400, 15);
    self->circleMoveAngle += self->circleRate;
    actor->world.pos.x = (Math_SinS(self->circleMoveAngle) * self->circleRadius) + actor->home.pos.x;
    actor->world.pos.z = (Math_CosS(self->circleMoveAngle) * self->circleRadius) + actor->home.pos.z;

    if (self->colliderJntSph.base.acFlags & AC_HIT) {
        colliderBaseAc = self->colliderJntSph.base.ac;
        if (colliderBaseAc != NULL) {
            positionDiff = colliderBaseAc->world.pos.y - actor->world.pos.y;
            if (positionDiff > 120.0f && positionDiff < 200.0f) {
                // N64 arg form (re-read of base.ac, self->dyna.actor) is what fixes the register choice.
                if (Math3D_Dist2DSq(colliderBaseAc->world.pos.z, self->colliderJntSph.base.ac->world.pos.x,
                                    self->dyna.actor.world.pos.z, self->dyna.actor.world.pos.x) < SQ(32.0f)) {
                    OnePointCutscene_Init(play, 4210, 240, actor, 0);
                    self->actionFunc = FUN_001843e0;
                    self->timer = 0;
                    self->pivotAltitude = 0;
                    FUN_0036b940(play, &play->colCtx.dyna, self->dyna.bgId);
                }
            }
        }
    }
    FUN_00373264(actor, 0x0100017B);
}

// Inactive (FUN_003bf948): GLOBAL_ASM kept; its functions.tsv span (260 B) also covers an
// unrelated cfg-merged tail, so a C body cannot match the span yet.
void FUN_003bf948(BgSpot18Basket* self, PlayState* play) {
    if (Flags_GetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 8, 6))) {
        OnePointCutscene_Init(play, 4220, 80, &self->dyna.actor, 0);
        self->actionFunc = FUN_003cc628;
        self->timer = 0;
    }
}
}
