extern "C" {
#include "z_arms_hook.hpp"
#include "model_manager.hpp"

void* ZAR_GetCMBByIndex(void* zar, s32 index);

#define FLAGS 0x00000030

void ArmsHook_Init(Actor* actor, GameState* state);
void ArmsHook_Destroy(Actor* actor, GameState* state);
void ArmsHook_Update(Actor* actor, GameState* state);
void ArmsHook_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Arms_Hook.data.s")

ActorInit Arms_Hook_InitVars = {
    ACTOR_ARMS_HOOK,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_LINK_BOY,
    sizeof(ArmsHook),
    (ActorFunc)ArmsHook_Init,
    (ActorFunc)ArmsHook_Destroy,
    (ActorFunc)ArmsHook_Update,
    (ActorFunc)ArmsHook_Draw,
};

GLOBAL_ASM("../src/game/actors/Arms_Hook/data_syms.s")

void FUN_0010cf2c(ArmsHook* self, PlayState* play);
void FUN_003efc9c(ArmsHook* self, PlayState* play);
void FUN_0036c648(Actor* actor, f32 speed);
#define R_UPDATE_RATE SREG(30)
s32 Collider_InitQuad(PlayState* play, ArmsHookQuad* quad);
s32 Collider_SetQuad(PlayState* play, ArmsHookQuad* dest, Actor* actor, void* src);
extern u8 ArmsHook_sQuadInit[];

static inline void ArmsHook_SetupAction(ArmsHook* self, ArmsHookActionFunc func) {
    self->actionFunc = func;
}

void FUN_0010cf2c(ArmsHook* self, PlayState* play) {
    Player* player;
    s32 length;

    if (self->actor.parent == NULL) {
        player = GET_PLAYER(play);
        if (((u8*)player)[0x1A9] == 0x10) {
            length = (s32)(13 * 3.0f / R_UPDATE_RATE + 0.5f);
        } else {
            length = (s32)(13 * 3.0f / R_UPDATE_RATE + 0.5f) * 2;
        }
        ArmsHook_SetupAction(self, FUN_003efc9c);
        FUN_0036c648(&self->actor, 20.0f);
        self->actor.parent = &GET_PLAYER(play)->actor;
        self->timer = length;
        self->unk_288 = 0.0f;
    }
}

void ArmsHook_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    ArmsHook* self = (ArmsHook*)actor;
    ZARInfo* zar;
    ModelAllocator* alloc;
    s32 i;

    Collider_InitQuad(play, &self->collider);
    Collider_SetQuad(play, &self->collider, &self->actor, ArmsHook_sQuadInit);
    ArmsHook_SetupAction(self, FUN_0010cf2c);
    self->unk_1E8 = actor->world.pos;
    actor->unk_19A = 1;

    zar = &play->GetObjectStatus(self->actor.objBankIndex)->zarInfo;
    alloc = GetModelManager().allocator;
    alloc->owner = self->actor.unk_178;
    for (i = 0; i < 2; i++) {
        self->models[i] = alloc->Create(ZAR_GetCMBByIndex(zar, i + 1), 1);
    }
    alloc->owner = NULL;
    self->unk_288 = 0.0f;
}


void FUN_0034eaf8(PlayState* play, ArmsHookQuad* quad); // Collider_DestroyQuad

void ArmsHook_Destroy(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    ArmsHook* self = (ArmsHook*)actor;
    s32 i;

    if (self->attachedActor != NULL) {
        self->attachedActor->flags &= ~0x2000;
    }
    FUN_0034eaf8(play, &self->collider);
    for (i = 0; i < 2; i++) {
        if (self->models[i] != NULL) {
            ((void (*)(void*))((void**)*(void**)self->models[i])[1])(self->models[i]);
            self->models[i] = NULL;
        }
    }
}

// Draft (not matched): 2 guarded function-local static Vector3f {0,0.5,0}. Guards/objects need the
// pin_static recipe (guard word in actor data chunk, objects at 0x5698A8/0x5698B4); VFP numbering still 13 words off.
#if 0
void ArmsHook_Update(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    ArmsHook* self = (ArmsHook*)actor;
    static Vector3f sVecA(0.0f, 0.5f, 0.0f);
    static Vector3f sVecB(0.0f, 0.5f, 0.0f);

    self->actionFunc(self, play);
    self->unk_1F4 = self->unk_1E8;
}
#endif
GLOBAL_ASM("asm/ArmsHook_Update.s")


GLOBAL_ASM("asm/ArmsHook_Draw.s")
}
