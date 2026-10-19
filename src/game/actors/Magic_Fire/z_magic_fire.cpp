extern "C" {
#include "z_magic_fire.hpp"
#include "model_manager.hpp"

#define FLAGS 0x02000010

void MagicFire_Init(Actor* actor, GameState* state);
void MagicFire_Destroy(Actor* actor, PlayState* play);
void MagicFire_Update(Actor* actor, GameState* state);
void MagicFire_UpdateBeforeCast(Actor* actor, GameState* state);
void MagicFire_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Magic_Fire.data.s")
GLOBAL_ASM("../src/game/actors/Magic_Fire/data_syms.s")

extern InitChainEntry MagicFire_sInitChain[];
extern u8 MagicFire_sCylinderInit[];
#define R_UPDATE_RATE SREG(30)

ActorInit Magic_Fire_InitVars = {
    ACTOR_MAGIC_FIRE,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(MagicFire),
    (ActorFunc)MagicFire_Init,
    (ActorFunc)MagicFire_Destroy,
    (ActorFunc)MagicFire_Update,
    (ActorFunc)MagicFire_Draw,
};

void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_0047d548(SkeletonAnimationModel* model, s32 arg);

void MagicFire_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    MagicFire* self = (MagicFire*)actor;
    ZARInfo* zar;
    void* cmb;
    ModelAllocator* alloc;

    Actor_ProcessInitChain(&self->actor, MagicFire_sInitChain);
    self->action = 0;
    self->screenTintBehaviour = 0;
    self->actionTimer = 0;
    self->alphaMultiplier = -3.0f;
    Actor_SetScale(&self->actor, 0.0f);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, (ColliderCylinderInit*)MagicFire_sCylinderInit);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    self->actor.update = MagicFire_UpdateBeforeCast;
    self->actionTimer = (s32)(20 * 3.0f / R_UPDATE_RATE + 0.5f);
    self->actor.room = -1;
    zar = &play->GetObjectStatus(actor->objBankIndex)->zarInfo;
    {
        ModelManager& mm = GetModelManager();
        mm.allocator->owner = actor->unk_178;
    }
    { ModelManager& mm = GetModelManager(); alloc = mm.allocator; }
    cmb = ZAR_GetCMBByIndex(zar, 0x23);
    self->model = (MagicFireModel*)alloc->Create(cmb, 1);
    {
        ModelManager& mm = GetModelManager();
        mm.allocator->owner = NULL;
    }
    self->anim = ((SkeletonAnimationModel*)self->model)->unk_0C;
    self->anim->FUN_00372d94(zar->GetCMABByIndex(0x10));
    self->anim->SetAnimSpeed(2.0f);
    self->anim->SetAnimType(1);
    FUN_0047d548((SkeletonAnimationModel*)self->model, 2);
    self->actor.unk_19A = 1;
}

void FUN_0034708c(PlayState* play); // Magic_Reset
s32 GetMessageMode(PlayState* play);
void FUN_0036f59c(Player* player, s32 sfxId);

void MagicFire_Destroy(Actor* thisx, PlayState* play) {
    MagicFire* self = (MagicFire*)thisx;

    FUN_0034708c(play);
    if (self->model != NULL) {
        self->model->Destroy();
        self->model = NULL;
    }
}

void MagicFire_Update(Actor* thisx, GameState* state) {
    MagicFire* self = (MagicFire*)thisx;
    PlayState* play = (PlayState*)state;
    Player* player = GET_PLAYER(play);

    thisx->world.pos = player->actor.world.pos;

    s32 msgMode = GetMessageMode(play);
    if ((msgMode == 0xD) || (msgMode == 0x11)) {
        Actor_Kill(thisx);
        return;
    }

    if (self->action == 1) {
        self->collider.info.toucher.damage = (s32)((self->actionTimer > 0) ? ((f32)self->actionTimer * (f32)R_UPDATE_RATE * (1.0f / 3.0f) + 0.5f) : ((f32)self->actionTimer * (f32)R_UPDATE_RATE * (1.0f / 3.0f) - 0.5f)) + 25;
    } else if (self->action == 2) {
        ((MagicFire*)thisx)->collider.info.toucher.damage = (s32)((self->actionTimer > 0) ? ((f32)self->actionTimer * (f32)R_UPDATE_RATE * (1.0f / 3.0f) + 0.5f) : ((f32)self->actionTimer * (f32)R_UPDATE_RATE * (1.0f / 3.0f) - 0.5f));
    }
    Collider_UpdateCylinder(&self->actor, &self->collider);
    self->collider.dim.radius = self->actor.scale.x * 325.0f;
    self->collider.dim.height = self->actor.scale.y * 450.0f;
    self->collider.dim.yShift = self->actor.scale.y * -225.0f;
    CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);

    switch (self->action) {
        case 0:
            self->actor.scale.x = self->actor.scale.y = self->actor.scale.z = 0.0f;
            self->actor.world.rot.x = self->actor.world.rot.y = self->actor.world.rot.z = 0;
            self->actor.shape.rot.x = self->actor.shape.rot.y = self->actor.shape.rot.z = 0;
            self->alphaMultiplier = 0.0f;
            self->actionTimer = (s32)(30 * 3.0f / R_UPDATE_RATE + 0.5f);
            self->scalingSpeed = 0.08f;
            self->action++;
            break;
        case 1:
            Math_StepToF(&self->alphaMultiplier, 1.0f, 1.0f / 30.0f);
            if (self->actionTimer > 0) {
                Math_SmoothStepToF(&self->actor.scale.x, 0.4f, self->scalingSpeed, 0.1f, 0.001f);
                self->actor.scale.y = self->actor.scale.z = self->actor.scale.x;
            } else {
                self->actionTimer = (s32)(25 * 3.0f / R_UPDATE_RATE + 0.5f);
                self->action++;
            }
            break;
        case 2:
            if (self->actionTimer <= 0) {
                self->actionTimer = (s32)(15 * 3.0f / R_UPDATE_RATE + 0.5f);
                self->action++;
                self->scalingSpeed = 0.05f;
            }
            break;
        case 3:
            self->alphaMultiplier -= 8.0f / 119.00001f * R_UPDATE_RATE / 3.0f;
            self->actor.scale.x += self->scalingSpeed * (R_UPDATE_RATE / 3.0f);
            self->actor.scale.y += self->scalingSpeed * (R_UPDATE_RATE / 3.0f);
            self->actor.scale.z += self->scalingSpeed * (R_UPDATE_RATE / 3.0f);
            if (self->alphaMultiplier <= 0.0f) {
                self->action = 0;
                Actor_Kill(&self->actor);
            }
            break;
    }
    switch (self->screenTintBehaviour) {
        case 0:
            if (self->screenTintBehaviourTimer <= 0) {
                self->screenTintBehaviourTimer = (s32)(20 * 3.0f / R_UPDATE_RATE + 0.5f);
                self->screenTintBehaviour = 1;
            }
            break;
        case 1:
            self->screenTintIntensity = 1.0f - (self->screenTintBehaviourTimer / (f32)(s32)(20 * 3.0f / R_UPDATE_RATE + 0.5f));
            if (self->screenTintBehaviourTimer <= 0) {
                self->screenTintBehaviourTimer = (s32)(45 * 3.0f / R_UPDATE_RATE + 0.5f);
                self->screenTintBehaviour = 2;
            }
            break;
        case 2:
            if (self->screenTintBehaviourTimer <= 0) {
                self->screenTintBehaviourTimer = (s32)(5 * 3.0f / R_UPDATE_RATE + 0.5f);
                self->screenTintBehaviour = 3;
            }
            break;
        case 3:
            self->screenTintIntensity = (self->screenTintBehaviourTimer / (f32)(s32)(5 * 3.0f / R_UPDATE_RATE + 0.5f));
            if (self->screenTintBehaviourTimer <= 0) {
                self->screenTintBehaviour = 4;
            }
            break;
    }
    if (self->actionTimer > 0) {
        self->actionTimer--;
    }
    if (self->screenTintBehaviourTimer > 0) {
        self->screenTintBehaviourTimer--;
    }
}

// MagicFire_Draw draft: ~34 words off (early-return store r0/r1 swap, tint 1/255 multiplies scheduled interleaved vs ROM batch). Tried ~25 variants.
#if 0
void FUN_00333abc(void* mat, s32 idx, f32* color);
void FUN_00333a38(void* mat, s32 idx, f32* color);
void FUN_003339e8(void* mgr, s32 a, f32* color, s32 b);

static inline void MagicFire_ScaleMtx(MTX34* m, f32 x, f32 y, f32 z) {
    m->data[0][0] *= x;
    m->data[1][0] *= x;
    m->data[2][0] *= x;
    m->data[0][1] *= y;
    m->data[1][1] *= y;
    m->data[2][1] *= y;
    m->data[0][2] *= z;
    m->data[1][2] *= z;
    m->data[2][2] *= z;
}

static inline void MagicFire_SetColor(f32* c, f32 r, f32 g, f32 b, f32 a) {
    c[0] = r * (1.0f / 255.0f);
    c[1] = g * (1.0f / 255.0f);
    c[2] = b * (1.0f / 255.0f);
    c[3] = a * (1.0f / 255.0f);
}

void MagicFire_Draw(Actor* thisx, GameState* state) {
    MagicFire* self = (MagicFire*)thisx;
    MTX34 mtx;
    f32 tint[4];
    f32 color[4];
    s32 i;

    if (self->action < 1) {
        ((SkeletonAnimationModel*)self->model)->unk_AD[0] = 0;
        return;
    }
    mtx = *(MTX34*)((u8*)self + 0x148);
    MagicFire_ScaleMtx(&mtx, 0.15f, 0.15f, 0.15f);
    ((SkeletonAnimationModel*)self->model)->SetUnkAC(1);
    ((SkeletonAnimationModel*)self->model)->FUN_003721e0(&mtx);
    {
        SkeletonAnimationModel* m = (SkeletonAnimationModel*)self->model;
        f32 y;
        f32 x;
        f32 z;
        x = mtx.data[0][3];
        y = mtx.data[1][3];
        z = mtx.data[2][3];
        m->unk_24 = x;
        m->unk_28 = y;
        m->unk_2C = z;
    }
    ((SkeletonAnimationModel*)self->model)->unk_AD[0] = 1;
    {
        f32 speed = (PauseContext_GetState() != 0) ? 0.0f : 2.0f;
        self->anim->SetAnimSpeed(speed);
    }
    for (i = 0; i < 2; i++) {
        void* mat = ((SkeletonAnimationModel*)self->model)->unk_10;

        FUN_00333abc(mat, i, color);
        color[3] = self->alphaMultiplier;
        FUN_00333a38(mat, i, color);
        ((u8*)*(void**)((u8*)mat + 4))[i * 0x124] = 1;
    }
    ((SkeletonAnimationModel*)self->model)->FUN_00372170(0);
    MagicFire_SetColor(tint, (f32)(u8)(s32)(self->screenTintIntensity * 60.0f), (f32)(u8)(s32)(self->screenTintIntensity * 20.0f), 0.0f, (f32)(u8)(s32)(self->screenTintIntensity * 120.0f));
    FUN_003339e8((u8*)&GetModelManager() + 0x25F0, 2, tint, 0);
}
#endif

GLOBAL_ASM("asm/MagicFire_Draw.s")

void MagicFire_UpdateBeforeCast(Actor* thisx, GameState* state) {
    MagicFire* self = (MagicFire*)thisx;
    PlayState* play = (PlayState*)state;
    Player* player = GET_PLAYER(play);
    s32 msgMode = GetMessageMode(play);

    if ((msgMode == 0xD) || (msgMode == 0x11)) {
        Actor_Kill(thisx);
        return;
    }

    if (self->actionTimer > 0) {
        self->actionTimer--;
    } else {
        thisx->update = MagicFire_Update;
        FUN_0036f59c(player, 0x0100007A);
    }
    thisx->world.pos = player->actor.world.pos;
}

}
