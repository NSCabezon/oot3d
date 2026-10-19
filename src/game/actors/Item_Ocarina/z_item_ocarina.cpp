extern "C" {
#include "z_item_ocarina.hpp"
#include "model_manager.hpp"

#define FLAGS 0x00000010

void ItemOcarina_Init(Actor* thisx, GameState* state);
void ItemOcarina_Destroy(Actor* actor, GameState* state);
void ItemOcarina_Update(Actor* actor, GameState* state);
void ItemOcarina_Draw(Actor* actor, GameState* state);
void ItemOcarina_Fly(ItemOcarina* self, PlayState* play);
void FUN_0010e2ec(Actor* actor, GameState* state);


// Model at +0x1AC: real virtual call, slot 1. _ZTI17ItemOcarinaModel is an EQU in syms.s so armlink VFE
// (--vfemode=force, --no_rtti_data) knows the call class (see Item_Etcetera).
extern "C++" {
struct ItemOcarinaModel {
    virtual void unk_vf0();
    virtual void Destroy();
};
}

ActorInit Item_Ocarina_InitVars = {
    ACTOR_ITEM_OCARINA,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GI_OCARINA,
    sizeof(ItemOcarina),
    (ActorFunc)ItemOcarina_Init,
    (ActorFunc)ItemOcarina_Destroy,
    (ActorFunc)ItemOcarina_Update,
    (ActorFunc)ItemOcarina_Draw,
};

void ItemOcarina_DoNothing(Actor* actor, GameState* state);
void ItemOcarina_GetThrown(Actor* actor, GameState* state);
void FUN_0010fb2c(Actor* actor, GameState* state);
void ItemOcarina_WaitInWater(Actor* actor, GameState* state);
void* ZAR_GetCMBByIndex(void* zar, s32 index);

// Init: switch with case 0/1 grouped + ternary SetupAction gives the ROM's shared actionFunc store.
static inline void ItemOcarina_SetupAction(ItemOcarina* self, ActorFunc actionFunc) {
    self->actionFunc = actionFunc;
}

// 3DS addition after the N64 Init body: unk_19A + model load (as an inline helper; gives ROM's `mov r0,#1` for unk_19A).
static inline void ItemOcarina_LoadModel(ItemOcarina* self, PlayState* play) {
    self->actor.unk_19A = 1;
    void* zar = (u8*)play->GetObjectStatus(self->actor.objBankIndex) + 0x10;
    {
        ModelAllocator* alloc = GetModelManager().allocator;
        alloc->owner = self->actor.unk_178;
        self->model = alloc->Create(ZAR_GetCMBByIndex(zar, 1), 1);
        alloc->owner = NULL;
    }
}

void ItemOcarina_Init(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    ItemOcarina* self = (ItemOcarina*)thisx;
    s32 params = thisx->params;

    ActorShape_Init(&thisx->shape, 0.0f, NULL, 0.0f);
    Actor_SetScale(thisx, 0.1f);

    switch (params) {
        case 0:
        case 1:
            ItemOcarina_SetupAction(self, params == 0 ? ItemOcarina_GetThrown : FUN_0010fb2c);
            break;
        case 2:
            ItemOcarina_SetupAction(self, ItemOcarina_DoNothing);
            break;
        case 3:
            ItemOcarina_SetupAction(self, ItemOcarina_WaitInWater);
            if (!(gSaveContext.eventChkInf[8] & 1) || (gSaveContext.eventChkInf[4] & 8)) {
                Actor_Kill(thisx);
                return;
            }
            Actor_Spawn(&play->actorCtx, play, 0x173, 299.0f, -140.0f, 884.0f, 0, 4, 1, 0x3800, 1);
            Actor_SetScale(thisx, 0.2f);
            break;
        default:
            Actor_Kill(thisx);
            return;
    }

    self->spinRotOffset = 0x400;
    ItemOcarina_LoadModel(self, play);
}

// Destroy draft: 0-word match via fastchk, but the split tail (FUN_00253a4c, cfg split) breaks the link
// (L6221E overlap at 0x253ab4); kept as asm until that is resolved.
#if 0
void ItemOcarina_Destroy(Actor* actor, GameState* state) {
    ItemOcarina* self = (ItemOcarina*)actor;

    if (self->model != NULL) {
        ((ItemOcarinaModel*)self->model)->Destroy();
        self->model = NULL;
    }
}
#endif

GLOBAL_ASM("asm/ItemOcarina_Destroy.s")

void ItemOcarina_Update(Actor* actor, GameState* state) {
    ItemOcarina* self = (ItemOcarina*)actor;

    self->actionFunc(actor, state);
}

void ItemOcarina_Draw(Actor* actor, GameState* state) {
    ItemOcarina* self = (ItemOcarina*)actor;

    self->model->SetUnkAC(1);
    self->model->FUN_003721e0(&actor->modelMtx);
    self->model->FUN_00372170(0);
}

void ItemOcarina_DoNothing(Actor* actor, GameState* state) {
}

#define R_UPDATE_RATE SREG(30)
#define FRAMES(n) ((n) * 3.0f / R_UPDATE_RATE + 0.5f)

void EffectSsGRipple_Spawn(PlayState* play, Vec3f* pos, s16 radius, s16 maxRadius, s16 life);
void EffectSsGSplash_Spawn(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, s16 type, s16 scale);

#define SCALE_ROT(n) ((n) > 0 ? (s32)((f32)(n) * R_UPDATE_RATE / 3.0f + 0.5f) : (s32)((f32)(n) * R_UPDATE_RATE / 3.0f - 0.5f))

void ItemOcarina_Fly(ItemOcarina* self, PlayState* play) {
    Vec3f ripplePos;

    FUN_0036b96c(&self->actor);
    self->actor.shape.rot.x += SCALE_ROT(self->spinRotOffset * 2);
    self->actor.shape.rot.y += SCALE_ROT(self->spinRotOffset * 3);

    if (self->actor.velocity.y > self->actor.minVelocityY) {
        self->actor.velocity.y += self->actor.gravity * R_UPDATE_RATE / 3.0f;
        if (self->actor.velocity.y < self->actor.minVelocityY) {
            self->actor.velocity.y = self->actor.minVelocityY;
        }
    }

    if (play->csCtx.frames == (s32)FRAMES(881)) {
        self->actor.world.pos.x = 250.0f;
        self->actor.world.pos.y = 60.0f;
        self->actor.world.pos.z = 1075.0f;
        self->actor.velocity.x = 1.0f;
        self->actor.velocity.y = -5.0f;
        self->actor.velocity.z = -7.0f;
    }

    if (play->csCtx.frames == (s32)FRAMES(897)) {
        EffectSsGRipple_Spawn(play, &self->actor.world.pos, 100, 500, 0);
        EffectSsGSplash_Spawn(play, &self->actor.world.pos, 0, 0, 1, 0);
        self->actor.velocity.x = 0.0f;
        self->actor.velocity.y = 0.0f;
        self->actor.velocity.z = 0.0f;
        self->actor.gravity = -0.1f;
        self->actor.minVelocityY = -0.5f;
        self->spinRotOffset = 0;
        Audio_PlayActorSound2(&self->actor, 0x0100016E);
    }

    if (play->csCtx.frames == (s32)FRAMES(906)) {
        ripplePos.x = 274.0f;
        ripplePos.y = -60.0f;
        ripplePos.z = 907.0f;
        EffectSsGRipple_Spawn(play, &ripplePos, 100, 500, 0);
    }
}

void FUN_0010e2ec(Actor* actor, GameState* state) {
    ItemOcarina* self = (ItemOcarina*)actor;
    PlayState* play = (PlayState*)state;

    FUN_0036b96c(actor);
    actor->shape.rot.x += SCALE_ROT(self->spinRotOffset * 2);
    actor->shape.rot.y += SCALE_ROT(self->spinRotOffset * 3);

    if (actor->velocity.y > actor->minVelocityY) {
        actor->velocity.y += actor->gravity * R_UPDATE_RATE / 3.0f;
        if (actor->velocity.y < actor->minVelocityY) {
            actor->velocity.y = actor->minVelocityY;
        }
    }

    if (play->csCtx.frames == (s32)FRAMES(220)) {
        actor->world.pos.x = 144.0f;
        actor->world.pos.y = 80.0f;
        actor->world.pos.z = 1686.0f;
        actor->velocity.x = 1.0f;
        actor->velocity.y = 2.0f;
        actor->velocity.z = -7.0f;
        actor->gravity = -0.15f;
        actor->minVelocityY = -5.0f;
    }
}

void ItemOcarina_GetThrown(Actor* actor, GameState* state) {
    ItemOcarina* self = (ItemOcarina*)actor;

    actor->gravity = -0.3f;
    actor->minVelocityY = -5.0f;
    actor->velocity.x = 0.0f;
    actor->velocity.y = 6.0f;
    actor->velocity.z = 0.0f;
    self->actionFunc = (ActorFunc)ItemOcarina_Fly;
}

void FUN_0010fb2c(Actor* actor, GameState* state) {
    ItemOcarina* self = (ItemOcarina*)actor;

    actor->gravity = -0.3f;
    actor->minVelocityY = -5.0f;
    actor->velocity.x = 0.0f;
    actor->velocity.y = 4.0f;
    actor->velocity.z = 6.0f;
    self->actionFunc = FUN_0010e2ec;
}

s32 FUN_00369a48(Actor* actor);

void ItemOcarina_StartSoTCutscene(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;

    if (FUN_00369a48(actor) != 0) {
        play->FUN_0037573c(((ZARInfo*)((u8*)play->GetObjectStatus(actor->objBankIndex) + 0x10))->GetQDBByIndex(0));
        gSaveContext.cutsceneTrigger = 1;
    }
}

void EffectSsBubble_Spawn(PlayState* play, Vec3f* pos, f32 arg2, f32 arg3, f32 arg4, f32 scale);

void ItemOcarina_WaitInWater(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;

    if (Actor_HasParent(actor, play) != 0) {
        gSaveContext.eventChkInf[4] |= 8;
        Flags_SetSwitch(play, 3);
        ((ItemOcarina*)actor)->actionFunc = ItemOcarina_StartSoTCutscene;
        actor->draw = NULL;
        return;
    }

    GiveItem(actor, play, 0xC, 30.0f, 50.0f);
    if ((*(u32*)((u8*)play + 0x5BF4) & 0xD) == 0) {
        EffectSsBubble_Spawn(play, &actor->world.pos, 0.0f, 0.0f, 10.0f, 0.13f);
    }
}

}
