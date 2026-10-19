extern "C" {
#include "z_en_nutsball.hpp"

#define FLAGS 0x00000010

void EnNutsball_Init(EnNutsball* self, PlayState* play);
void EnNutsball_Destroy(Actor* actor, GameState* state);
void EnNutsball_Update(Actor* actor, GameState* state);

void FUN_00154ba8(EnNutsball* self, PlayState* play); // EnNutsball_WaitForObject (still asm)
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
void Actor_Kill(Actor* actor);
void Actor_MoveForward(Actor* actor);
void Collider_UpdateCylinder(Actor* actor, ColliderCylinder* collider);
s32 CollisionCheck_SetAT(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);

ActorInit En_Nutsball_InitVars = {
    ACTOR_EN_NUTSBALL,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnNutsball),
    (ActorFunc)EnNutsball_Init,
    (ActorFunc)EnNutsball_Destroy,
    (ActorFunc)EnNutsball_Update,
    NULL,
};

static ColliderCylinderInit sCylinderInit = {
    { 0x0A, 0x11, 0x09, 0x39, 0x20, 0x01 },
    { 0, { 0xFFCFFFFF, 0, 8 }, { 0xFFCFFFFF, 0, 0 }, 17, 1, 1 },
    { 13.0f, 13.0f, 0.0f, { 0.0f, 0.0f, 0.0f } },
};

#include "model_manager.hpp"

void* ZAR_GetCMBByIndex(void* zar, s32 idx);

// sObjectIds (in the shared rodata dump) has no symbol in the scatter file -> absolute symbol (EQU) in data_syms.s.
GLOBAL_ASM("../src/game/actors/En_Nutsball/data_syms.s")
extern s16 EnNutsball_sObjectIds[];

// Taking `Actor* thisx` + cast inside is what puts the actionFunc literal in r1 (ldrge r1/strge r1).
static inline void EnNutsball_SetupWaitForObject(Actor* thisx, PlayState* play) {
    EnNutsball* self = (EnNutsball*)thisx;

    if (self->requiredObjectSlot < 0) {
        Actor_Kill(thisx);
    } else {
        self->actionFunc = FUN_00154ba8;
    }
}

void EnNutsball_Init(EnNutsball* self, PlayState* play) {
    ModelAllocator* alloc;
    ZARInfo* zar;

    ActorShape_Init(&self->actor.shape, 400.0f, ActorShadow_DrawCircle, 13.0f);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &sCylinderInit);
    self->requiredObjectSlot = Object_GetIndex(&play->objectCtx, EnNutsball_sObjectIds[self->actor.params]);
    zar = &play->GetObjectStatus(self->requiredObjectSlot)->zarInfo;
    alloc = GetModelManager().allocator;
    alloc->owner = self->actor.unk_178;
    if ((u32)self->actor.params <= 4) {
        self->model = alloc->Create(ZAR_GetCMBByIndex(zar, 1), 1);
    }
    alloc->owner = NULL;
    EnNutsball_SetupWaitForObject(&self->actor, play);
    self->actor.unk_19A = 1;
}

// (Actor*, GameState*) with the state cast inline (no play local) gives ROM's model-in-r1, non-predicated call.
void EnNutsball_Destroy(Actor* actor, GameState* state) {
    EnNutsball* self = (EnNutsball*)actor;

    if (self->model != NULL) {
        ((void (*)(void*))((void**)*(void**)self->model)[1])(self->model);
    }
    Collider_DestroyCylinder((PlayState*)state, &self->collider);
}

// Draft Update (Haiku, NOT matched: 3 words differ). Everything matches except the radius/height loads:
// ROM takes base 0x0052D168 from the literal pool and reads +0x20/+0x24, the compiler folds the base to
// 0x0052D000 (+0x188/+0x18C). Also pool words 0xCC/0xD0 (FUN_00154ba8, 0x52D168) are relocs.
void EnNutsball_Update(Actor* actor, GameState* state) {
    EnNutsball* self = (EnNutsball*)actor;
    PlayState* play = (PlayState*)state;
    Player* player = GET_PLAYER(play);

    if (!(player->stateFlags1 & 0x300000C0) || (self->actionFunc == FUN_00154ba8)) {
        self->actionFunc(self, play);
        Actor_MoveForward(actor);
        Actor_UpdateBgCheckInfo(play, actor, 10.0f, sCylinderInit.dim.radius,
                                sCylinderInit.dim.height, 5);
        Collider_UpdateCylinder(actor, &self->collider);
        actor->flags |= 0x01000000; // ACTOR_FLAG_SFX_FOR_PLAYER_BODY_HIT
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    }
}

GLOBAL_ASM("../split/asm/FUN_0026be60.s")
void FUN_0026beb8(void) {
}

GLOBAL_ASM("../split/asm/FUN_0026bebc.s")
}
