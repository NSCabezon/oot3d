extern "C" {
#include "z_item_shield.hpp"
#include "model_manager.hpp"

#define R_UPDATE_RATE SREG(30)

#define FLAGS 0x00000010

void ItemShield_Init(Actor* actor, GameState* state);
void ItemShield_Destroy(ItemShield* self, PlayState* play);
void ItemShield_Update(ItemShield* self, PlayState* play);
void ItemShield_Draw(ItemShield* self, PlayState* play);

// ROM data order: sCylinderInit precedes InitVars (0x00531E5C).
static ColliderCylinderInit sCylinderInit = {
    { 0x0A, 0x00, 0x09, 0x39, 0x10, 0x01 },
    { 0, { 0x00000000, 0, 0 }, { 0x00000004, 0, 0 }, 0, 1, 1 },
    { 15.0f, 15.0f, 0.0f, { 0.0f, 0.0f, 0.0f } },
};

ActorInit Item_Shield_InitVars = {
    ACTOR_ITEM_SHIELD,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_LINK_CHILD,
    sizeof(ItemShield),
    (ActorFunc)ItemShield_Init,
    (ActorFunc)ItemShield_Destroy,
    (ActorFunc)ItemShield_Update,
    (ActorFunc)ItemShield_Draw,
};

void FUN_003f4414(ItemShield* self, PlayState* play);
void FUN_0015c554(ItemShield* self, PlayState* play);
void* ZAR_GetCMBByIndex(void* zar, s32 index);

// Each GetModelManager() use is bound to its own `ModelManager&` local in its own scope: that keeps armcc
// from caching &sInstance in a register (ROM reloads the literal at each of the 3 uses, zar stays in r7).
static inline void ItemShield_SetupAction(ItemShield* self, ItemShieldActionFunc func) {
    self->actionFunc = func;
}

void ItemShield_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    ItemShield* self = (ItemShield*)actor;
    s32 i;
    ZARInfo* zar;

    self->timer = 0;
    self->unk_19C = 0;

    switch (self->actor.params) {
        case 0:
            ActorShape_Init(&self->actor.shape, 1400.0f, NULL, 0.0f);
            actor->shape.rot.x = 0x4000;
            ItemShield_SetupAction(self, FUN_003f4414);
            break;

        case 1:
            ActorShape_Init(&self->actor.shape, 0.0f, NULL, 0.0f);
            ItemShield_SetupAction(self, FUN_0015c554);
            self->unk_19C |= 2;
            for (i = 0; i < 8; i++) {
                self->unk_19E[i] = (s32)((1 + 2 * i) * 3.0f / R_UPDATE_RATE + 0.5f);
                self->unk_1A8[i].x = Rand_CenteredFloat(10.0f);
                self->unk_1A8[i].y = Rand_CenteredFloat(10.0f);
                self->unk_1A8[i].z = Rand_CenteredFloat(10.0f);
            }
            break;
    }

    Actor_SetScale(&self->actor, 0.01f);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &sCylinderInit);

    zar = &play->GetObjectStatus(self->actor.objBankIndex)->zarInfo;
    {
        ModelManager& mm = GetModelManager();
        mm.allocator->owner = self->actor.unk_178;
    }
    {
        ModelManager& mm = GetModelManager();
        self->model = mm.allocator->Create(ZAR_GetCMBByIndex(zar, 1), 1);
    }
    {
        ModelManager& mm = GetModelManager();
        mm.allocator->owner = NULL;
    }
}

void ItemShield_Destroy(ItemShield* self, PlayState* play) {
    if (self->model != NULL) {
        ((void (*)(void*))((void**)*(void**)self->model)[1])(self->model);
        self->model = NULL;
    }
    Collider_DestroyCylinder(play, &self->collider);
}

void ItemShield_Update(ItemShield* self, PlayState* play) {
    self->actionFunc(self, play);
}

void ItemShield_Draw(ItemShield* self, PlayState* play) {
    if (!(self->unk_19C & 2)) {
        self->model->SetUnkAC(1);
        self->model->FUN_003721e0(&self->actor.modelMtx);
        SkeletonAnimationModel* model = self->model;
        f32 x = self->actor.world.pos.x;
        f32 y = self->actor.world.pos.y;
        f32 z = self->actor.world.pos.z;
        model->unk_24 = x;
        model->unk_28 = y;
        model->unk_2C = z;
        self->model->FUN_00372170(0);
    }
}

void FUN_003eddd0(ItemShield* self, PlayState* play);
void FUN_00182500(ItemShield* self, PlayState* play);
void FUN_00359450(void* player, MTX34* out);

// N64 func_80B86BC8
void FUN_003f4414(ItemShield* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        Actor_Kill(&self->actor);
        return;
    }
    GiveItem(&self->actor, play, 0x29, 30.0f, 50.0f);
    if (self->collider.base.acFlags & 2) {
        self->actionFunc = FUN_003eddd0;
        self->actor.velocity.y = 4.0f;
        self->actor.minVelocityY = -4.0f;
        self->actor.gravity = -0.8f;
        self->actor.speedXZ = 0.0f;
        self->timer = (s32)(160 * 3.0f / R_UPDATE_RATE + 0.5f);
    } else {
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
    }
}

// N64 func_80B86F68
void FUN_0015c554(ItemShield* self, PlayState* play) {
    MTX34 mtx;

    FUN_00359450(*(void**)((u8*)play + 0x20AC), &mtx);
    self->actor.world.pos.x = mtx.data[0][3];
    self->actor.world.pos.y = mtx.data[1][3];
    self->actor.world.pos.z = mtx.data[2][3];
    self->unk_19C &= ~2;

    self->actor.shape.rot.y = Math_Atan2S(-mtx.data[2][2], -mtx.data[0][2]);
    self->actor.shape.rot.x = Math_Atan2S(-mtx.data[1][2], sqrtf(mtx.data[0][2] * mtx.data[0][2]) + mtx.data[2][2] * mtx.data[2][2]);
    if (ABS(self->actor.shape.rot.x) > 0x4000) {
        self->unk_19C |= 1;
    }
    self->actionFunc = FUN_00182500;
    self->actor.velocity.y = 4.0f;
    self->actor.minVelocityY = -4.0f;
    self->actor.gravity = -0.8f;
    self->actor.speedXZ = 0.0f;
    self->unk_198 = 0;
    self->timer = (s32)(70 * 3.0f / R_UPDATE_RATE + 0.5f);
}
}
