extern "C" {
#include "z_item_inbox.hpp"
#include "model_manager.hpp"

#define FLAGS 0x00000009
#define PARAMS_GET_U(params, shift, count) ((((u16)(params)) >> (shift)) & ((1 << (count)) - 1))

void ItemInbox_Init(Actor* actor, GameState* state);
void ItemInbox_Destroy(Actor* actor, GameState* state);
void ItemInbox_Update(Actor* actor, GameState* state);
void ItemInbox_Draw(Actor* actor, GameState* state);
void ItemInbox_Wait(ItemInbox* self, PlayState* play);

SkeletonAnimationModel* FUN_003a0e88(void* owner, GameState* state, s32 entry, s32 arg3);
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
void FUN_0033da8c(ObjectContext* objectCtx, s32 index, GameState* state);
void FUN_00369178(SkeletonAnimationModel* model, s32 entry);
void* ZAR_GetCMBByIndex(void* zar, s32 index);

ActorInit Item_Inbox_InitVars = {
    ACTOR_ITEM_INBOX,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ItemInbox),
    (ActorFunc)ItemInbox_Init,
    (ActorFunc)ItemInbox_Destroy,
    (ActorFunc)ItemInbox_Update,
    (ActorFunc)ItemInbox_Draw,
};

void ItemInbox_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    ItemInbox* self = (ItemInbox*)actor;

    self->actionFunc = ItemInbox_Wait;
    Actor_SetScale(&self->actor, 0.2f);
    self->model = FUN_003a0e88(self->actor.unk_178, state, self->actor.params & 0xFF, 0);
    if (self->model == NULL) {
        Actor_Kill(&self->actor);
    }
}

// The 348 B after Destroy (0x213D80..) are a separate function, split out by scripts/cfg_split.py
// (split/asm/FUN_00213d80.s).
void ItemInbox_Destroy(Actor* actor, GameState* state) {
    ItemInbox* self = (ItemInbox*)actor;

    if (self->model != NULL) {
        ((void (*)(void*))((void**)*(void**)self->model)[1])(self->model);
    }
    self->model = NULL;
}

GLOBAL_ASM("../split/asm/FUN_00213d80.s")

void ItemInbox_Update(Actor* actor, GameState* state) {
    ItemInbox* self = (ItemInbox*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
}

void ItemInbox_Draw(Actor* actor, GameState* state) {
    ItemInbox* self = (ItemInbox*)actor;

    self->model->FUN_003721e0(&self->actor.modelMtx);
    self->model->SetUnkAC(1);
    self->model->FUN_00372170(0);
}

void ItemInbox_Wait(ItemInbox* self, PlayState* play) {
    if (Flags_GetTreasure(play, PARAMS_GET_U(self->actor.params, 8, 5))) {
        Actor_Kill(&self->actor);
    }
}

// FUN_003a0e88: create a model from a table row (0x004D88C8, raw address: debt) via the shared
// GetModelManager() singleton (include/model_manager.hpp; see ws LOADER_RECIPE.md). Matching notes:
// - `GameState* state` param + `PlayState* play` cast local, raw `state` passed to FUN_0033da8c;
// - Object_GetIndex through an inline (PlayState*, s16) wrapper and an `s32 objectId` local from a u16 field
//   (gives ROM's ldrh+sxth and index in r4); `table` local pointer (gives ROM temp-reg order);
// - allocator owner set/call/reset written out (not an inline member).
typedef struct {
    /* 0x00 */ u8 cmbIndex;
    /* 0x01 */ u8 cmabIndex;
} ModelTableAnim;

typedef struct {
    /* 0x00 */ u16 objectId;
    /* 0x02 */ ModelTableAnim anims[2];
} ModelTableEntry; // size = 0x6

static inline s32 FUN_003a0e88_GetObjectIndex(PlayState* play, s16 objectId) {
    return Object_GetIndex(&play->objectCtx, objectId);
}

SkeletonAnimationModel* FUN_003a0e88(void* owner, GameState* state, s32 entry, s32 arg3) {
    PlayState* play = (PlayState*)state;
    ModelTableEntry* table = (ModelTableEntry*)0x004D88C8;
    ModelTableEntry* row = &table[entry];
    SkeletonAnimationModel* result = NULL;
    s32 objectId = row->objectId;

    s32 index = FUN_003a0e88_GetObjectIndex(play, objectId);

    if (index >= 0) {
        ZARInfo* zar;
        SkeletonAnimationModel* model;

        FUN_0033da8c(&play->objectCtx, index, state);
        zar = &play->GetObjectStatus(index)->zarInfo;
        model = NULL;
        if (row->anims[arg3].cmbIndex != 0xFF) {
            void* cmb = ZAR_GetCMBByIndex(zar, row->anims[arg3].cmbIndex);

            ModelAllocator* alloc = GetModelManager().allocator;

            alloc->owner = owner;
            model = alloc->Create(cmb, 1);
            alloc->owner = NULL;
            if (model != NULL && row->anims[arg3].cmabIndex != 0xFF) {
                SkeletonAnimationModel_unk_0C* unk0C = model->unk_0C;

                unk0C->FUN_00372d94(zar->GetCMABByIndex(row->anims[arg3].cmabIndex));
                *((u8*)unk0C + 0x10) = 1;
            }
            FUN_00369178(model, entry);
        }
        result = model;
    }
    return result;
}

}
