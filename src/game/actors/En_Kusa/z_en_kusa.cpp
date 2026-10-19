extern "C" {
#include "z_en_kusa.hpp"
#include "graphics.hpp"

#define FLAGS 0x80800010

// Data lives in data/z_En_Kusa.data.s (no symbols). Addresses are the 3DS data offsets:
// sObjectIds @0x52BDB8, sColChkInfoInit @0x52BDBE, sCylinderInit @0x52BDC8, sInitChain @0x52BE40.
#define sObjectIds ((s16*)0x0052BDB8)
#define sColChkInfoInit ((CollisionCheckInfoInit*)0x0052BDBE)
#define sCylinderInit ((ColliderCylinderInit*)0x0052BDC8)
#define sInitChain ((InitChainEntry*)0x0052BE40)

void EnKusa_Init(Actor* actor, GameState* state);
void EnKusa_Destroy(Actor* actor, GameState* state);
void EnKusa_Update(Actor* actor, GameState* state);

s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
f32 BgCheck_EntityRaycastFloor4(CollisionContext* colCtx, CollisionPoly** outPoly, s32* outBgId, Actor* actor,
                                Vec3f* pos);
void FUN_0035021c(void* model);

void FUN_00110450(EnKusa* self, PlayState* play); // N64: EnKusa_WaitForObject

GLOBAL_ASM("data/z_En_Kusa.data.s")

ActorInit En_Kusa_InitVars = {
    ACTOR_EN_KUSA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnKusa),
    (ActorFunc)EnKusa_Init,
    (ActorFunc)EnKusa_Destroy,
    (ActorFunc)EnKusa_Update,
    NULL,
};

// Draft (not matching yet: compiles to 0x178 B, ROM span 0x1A4 B). Kept as asm until matched.
#if 0
void EnKusa_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnKusa* self = (EnKusa*)actor;
    Vec3f pos;
    CollisionPoly* groundPoly;
    s32 bgId;
    f32 floorY;

    Actor_ProcessInitChain(actor, sInitChain);

    if (play->GetCutsceneState()) {
        self->actor.uncullZoneForward += 1000.0f;
    }

    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, actor, sCylinderInit);
    Collider_UpdateCylinder(actor, &self->collider);
    CollisionCheck_SetInfo(&actor->colChkInfo, NULL, sColChkInfoInit);

    if (self->actor.shape.rot.y == 0) {
        s16 rand = Rand_ZeroFloat(0x10000);

        self->actor.world.rot.y = rand;
        self->actor.home.rot.y = rand;
        self->actor.shape.rot.y = rand;
    }

    pos.x = self->actor.world.pos.x;
    pos.y = self->actor.world.pos.y + 30.0f;
    pos.z = self->actor.world.pos.z;

    floorY = BgCheck_EntityRaycastFloor4(&play->colCtx, &groundPoly, &bgId, actor, &pos);
    if (floorY > -32000.0f) {
        self->actor.world.pos.y = floorY + 0.0f;
        Math_Vec3f_Copy(&self->actor.home.pos, &self->actor.world.pos);
        self->models[0] = NULL;
        self->models[1] = NULL;
        self->models[2] = NULL;

        self->requiredObjectSlot = Object_GetIndex(&play->objectCtx, sObjectIds[self->actor.params & 3]);
        if (self->requiredObjectSlot >= 0) {
            self->timer = 0;
            self->actionFunc = FUN_00110450;
        } else {
            Actor_Kill(&self->actor);
        }
    } else {
        Actor_Kill(&self->actor);
    }
}
#endif

GLOBAL_ASM("asm/EnKusa_Init.s")

// ROM ends `pop; mov r0,r0`: the last call is a tail call to FUN_0035021c, which directly
// follows this function, so armlink --tailreorder turns the `b` into a fall-through nop.
void EnKusa_Destroy(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnKusa* self = (EnKusa*)actor;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_0035021c(self->models[0]);
    FUN_0035021c(self->models[1]);
    FUN_0035021c(self->models[2]);
}


void EnKusa_Update(Actor* actor, GameState* state) {
    EnKusa* self = (EnKusa*)actor;

    self->timer++;
    self->actionFunc((EnKusa*)actor, (PlayState*)state);
    self->actor.shape.yOffset = (self->actor.flags & 0x800) ? -6.25f : 0.0f;
}



// ---- Action / draw functions (ld-pinned FUN_ names; moved here from src/functions/) ----
void FUN_003cbcf0(EnKusa* self, PlayState* play); // N64: EnKusa_LiftedUp
void FUN_003f7748(EnKusa* self, PlayState* play); // N64: EnKusa_CutWaitRegrow
void FUN_003bde20(EnKusa* self, PlayState* play); // N64: EnKusa_Main
void FUN_00111140(EnKusa* self, PlayState* play); // N64: EnKusa_DoNothing, 3DS: lift-only main for cut type 2
void FUN_001f7510(EnKusa* self, PlayState* play); // N64: EnKusa_Draw
void* FUN_0036aaa4(Actor* actor, PlayState* play, s32 slot, s32 idx); // load model from object slot
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 slot);

static void EnKusa_SetupAction(EnKusa* self, EnKusaActionFunc func) {
    self->timer = 0;
    self->actionFunc = func;
}

static void EnKusa_SetupLiftedUp(EnKusa* self) {
    EnKusa_SetupAction(self, FUN_003cbcf0);
    self->actor.room = -1;
    self->actor.flags |= 0x10;
}

static void EnKusa_SetupMain(EnKusa* self) {
    EnKusa_SetupAction(self, FUN_003bde20);
    self->actor.flags &= ~0x10;
    self->collider.base.acFlags &= ~2;
}

static void EnKusa_SetupCut(EnKusa* self) {
    switch (self->actor.params & 3) {
        case 2:
            EnKusa_SetupAction(self, FUN_00111140);
            break;
        case 1:
            EnKusa_SetupAction(self, FUN_003f7748);
            break;
    }
}

void FUN_00110450(EnKusa* self, PlayState* play) {
    if (Object_IsLoaded(&play->objectCtx, self->requiredObjectSlot)) {
        if (self->actor.params & 3) {
            self->models[0] = FUN_0036aaa4(&self->actor, play, self->requiredObjectSlot, 0);
            self->models[1] = FUN_0036aaa4(&self->actor, play, self->requiredObjectSlot, 1);
        } else {
            self->models[2] = FUN_0036aaa4(&self->actor, play, self->requiredObjectSlot, 10);
        }
        if (self->actor.flags & 0x800) {
            EnKusa_SetupCut(self);
        } else {
            EnKusa_SetupMain(self);
        }
        self->actor.draw = (ActorFunc)FUN_001f7510;
        self->actor.objBankIndex = self->requiredObjectSlot;
        self->actor.flags &= ~0x10;
    }
}

void FUN_00111140(EnKusa* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        EnKusa_SetupLiftedUp(self);
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 20, 0x0100006a);
    }
}

void FUN_001f7510(EnKusa* self, PlayState* play) {
    SkeletonAnimationModel* models[] = { (SkeletonAnimationModel*)self->models[2], (SkeletonAnimationModel*)self->models[0], (SkeletonAnimationModel*)self->models[0] };
    if (self->actor.flags & 0x800) {
        if (self->models[1] != NULL) {
            ((SkeletonAnimationModel*)self->models[1])->SetUnkAC(1);
            ((SkeletonAnimationModel*)self->models[1])->FUN_003721e0(&self->actor.modelMtx);
            ((SkeletonAnimationModel*)self->models[1])->FUN_00372170(0);
        }
    } else if (models[self->actor.params & 3] != NULL) {
        models[self->actor.params & 3]->SetUnkAC(1);
        models[self->actor.params & 3]->FUN_003721e0(&self->actor.modelMtx);
        models[self->actor.params & 3]->FUN_00372170(0);
    }
}

}
