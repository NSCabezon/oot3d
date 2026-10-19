extern "C" {
#include "z_en_guest.hpp"

#define FLAGS 0x00000019

s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 bankIndex);

void EnGuest_Init(Actor* actor, GameState* state);
void EnGuest_Destroy(Actor* actor, GameState* state);
void EnGuest_Update(Actor* actor, GameState* state);

void FUN_002108ac(Actor* thisx, PlayState* play); // N64: func_80A505CC
void FUN_00210678(Actor* thisx, PlayState* play); // N64: EnGuest_Draw
void FUN_0016c9f0(EnGuest* self, PlayState* play); // N64: func_80A50518 (shared with other actors)
s32 FUN_0016d77c(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg); // shared limb draw
void FUN_0034c664(Actor* actor, EnGuestInteractInfo* info, s32 arg2, s32 arg3); // Npc_TrackPoint
void FUN_00370f5c(PlayState* play, s16* arg1, s16* arg2, s32 arg3);            // Actor_UpdateFidgetTables
void FUN_00353c9c(Actor* actor, PlayState* play, SkelAnime* skelAnime, s32 cmbIndex, s32 csabIndex,
                  void* jointTable, void* morphTable, s32 limbCount);
void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);
void FUN_0037266c(SkeletonAnimationModel* model, s32 idx);
void FUN_00357a50(SkelAnime* skelAnime, s32 idx, s32 count, void* colors, s32 arg4);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
f32 Rand_ZeroFloat(f32 f);
s32 Collider_DestroyCylinder(PlayState* play, ColliderCylinder* collider);
s32 Collider_SetCylinderType1(PlayState* play, ColliderCylinder* collider, Actor* actor, ColliderCylinderInit* src);

// Data that lives in the asm blob (GLOBAL_ASM("data/z_En_Guest.data.s")) and fixed addresses in the main binary.
extern InitChainEntry EnGuest_sInitChain[];
extern ColliderCylinderInit EnGuest_sCylinderInit;
extern s32 EnGuest_sColorA[];
extern s32 EnGuest_sColorB[];

GLOBAL_ASM("data/z_En_Guest.data.s")
GLOBAL_ASM("../src/game/actors/En_Guest/syms.s")

ActorInit En_Guest_InitVars = {
    ACTOR_EN_GUEST,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_BOJ,
    sizeof(EnGuest),
    (ActorFunc)EnGuest_Init,
    (ActorFunc)EnGuest_Destroy,
    (ActorFunc)EnGuest_Update,
    NULL,
};

void EnGuest_Init(Actor* thisx, GameState* state) {
    EnGuest* self = (EnGuest*)thisx;
    PlayState* play = (PlayState*)state;

    if (gSaveContext.infTable[7] & 0x40) {
        Actor_Kill(thisx);
    } else {
        self->osAnimeObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_OS_ANIME);
        // N64 prints "There is no bank!!" and asserts here; the early return keeps the sxtb.
        if (self->osAnimeObjectSlot < 0) {
            return;
        }
    }
}

void EnGuest_Destroy(Actor* thisx, GameState* state) {
    EnGuest* self = (EnGuest*)thisx;
    PlayState* play = (PlayState*)state;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(&self->actor, self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
}

void EnGuest_Update(Actor* thisx, GameState* state) {
    EnGuest* self = (EnGuest*)thisx;
    PlayState* play = (PlayState*)state;

    if (Object_IsLoaded(&play->objectCtx, self->osAnimeObjectSlot)) {
        thisx->flags &= ~0x10;
        Actor_ProcessInitChain(thisx, EnGuest_sInitChain);

        FUN_00372f38(&self->actor, play, &self->skelAnimModel, 0, 0);
        FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 16);
        Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 0), 0,
                                  0.0f);

        self->actor.draw = (ActorFunc)FUN_00210678;
        self->actor.update = (ActorFunc)FUN_002108ac;

        Collider_InitCylinder(play, &self->collider);
        Collider_SetCylinderType1(play, &self->collider, &self->actor, &EnGuest_sCylinderInit);

        Actor_SetFocus(&self->actor, 60.0f);

        self->unk_30E = 0;
        self->unk_30D = 0;
        self->unk_2CA = 0;
        self->actor.textId = 0x700D;
        self->actionFunc = FUN_0016c9f0;
    }
}

void FUN_002108ac(Actor* thisx, PlayState* play) {
    EnGuest* self = (EnGuest*)thisx;
    Player* player;

    player = GET_PLAYER(play);
    self->unk_2C8++;

    if (self->unk_30D == 0) {
        if (self->unk_2CA != 0) {
            self->unk_2CA--;
        } else {
            self->unk_30D = 1;
        }
    } else {
        if (self->unk_2CA != 0) {
            self->unk_2CA--;
        } else {
            self->unk_30E++;
            if (self->unk_30E >= 3) {
                self->unk_30E = 0;
                self->unk_30D = 0;
                self->unk_2CA = (s32)Rand_ZeroFloat(60.0f) + 20;
            } else {
                self->unk_2CA = 1;
            }
        }
    }

    self->actionFunc(self, play);

    self->interactInfo.trackPos = player->actor.world.pos;
    self->interactInfo.yOffset = LINK_IS_ADULT ? 10.0f : 20.0f;
    FUN_0034c664(&self->actor, &self->interactInfo, 6, 2);

    FUN_00370f5c(play, self->fidgetTableY, self->fidgetTableZ, 16);

    FUN_00370734(&self->skelAnime);
    Actor_SetFocus(&self->actor, 60.0f);

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
}

void FUN_00210678(Actor* thisx, PlayState* play) {
    EnGuest* self = (EnGuest*)thisx;

    FUN_0036932c(self->skelAnime.unk_28, 8);
    FUN_0036932c(self->skelAnime.unk_28, 2);
    FUN_0036932c(self->skelAnime.unk_28, 3);
    FUN_0036932c(self->skelAnime.unk_28, 4);
    FUN_0036932c(self->skelAnime.unk_28, 5);
    FUN_0036932c(self->skelAnime.unk_28, 6);
    FUN_0036932c(self->skelAnime.unk_28, 7);
    FUN_0036932c(self->skelAnime.unk_28, 1);
    FUN_0037266c(self->skelAnime.unk_28, 6);
    FUN_00357a50(&self->skelAnime, 0, 4, EnGuest_sColorA, 1);
    FUN_00357a50(&self->skelAnime, 1, 3, EnGuest_sColorB, 1);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, (OverrideLimbDrawOpa)FUN_0016d77c, NULL, &self->actor, 0);
}

}
