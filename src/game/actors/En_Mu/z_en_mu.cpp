extern "C" {
#include "z_en_mu.hpp"

#define FLAGS 0x00000009

#define NPC_TALK_STATE_IDLE 0
#define NPC_TALK_STATE_TALKING 1

void EnMu_Init(Actor* actor, GameState* state);
void EnMu_Destroy(Actor* actor, GameState* state);
void EnMu_Update(Actor* actor, GameState* state);
void EnMu_Draw(Actor* actor, GameState* state);

void EnMu_Pose(EnMu* self, PlayState* play);
void EnMu_Interact(EnMu* self, PlayState* play);
s16 EnMu_CheckDialogState(PlayState* play, Actor* thisx);
u16 EnMu_GetFaceReaction(PlayState* play, Actor* thisx);

void FUN_0034f558(SkelAnime* skelAnime, PlayState* play);
void FUN_00342714(PlayState* play, Actor* actor, void* talkState, f32 range, void* getTextId,
                  void* updateTalkState); // Npc_UpdateTalking
void FUN_00370f5c(PlayState* play, s16* arg1, s16* arg2, s32 arg3);            // Actor_UpdateFidgetTables
void FUN_00357a50(SkelAnime* skelAnime, s32 idx, s32 count, void* colors, s32 arg4);

GLOBAL_ASM("data/z_En_Mu.data.s")

ActorInit En_Mu_InitVars = {
    ACTOR_EN_MU,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_MU,
    sizeof(EnMu),
    (ActorFunc)EnMu_Init,
    (ActorFunc)EnMu_Destroy,
    (ActorFunc)EnMu_Update,
    (ActorFunc)EnMu_Draw,
};

void EnMu_Destroy(Actor* actor, GameState* state) {
    EnMu* self = (EnMu*)actor;
    PlayState* play = (PlayState*)state;

    FUN_0034f558(&self->skelAnime, play);
    FUN_003508b8(actor, self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
}

void EnMu_Pose(EnMu* self, PlayState* play) {
    FUN_00370f5c(play, self->fidgetTableY, self->fidgetTableZ, 16);
}

void EnMu_Init(Actor* actor, GameState* state) {
    EnMu* self = (EnMu*)actor;
    PlayState* play = (PlayState*)state;

    ActorShape_Init(&actor->shape, 0.0f, ActorShadow_DrawCircle, 160.0f);
    FUN_00372f38(actor, play, &self->skelAnimModel, 1, 0);
    FUN_00353c9c(actor, play, &self->skelAnime, 1, 0, self->jointTable, self->morphTable, 8);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, actor, (ColliderCylinderInit*)0x0052CDF8);
    CollisionCheck_SetInfo2(&actor->colChkInfo, NULL, (CollisionCheckInfoInit2*)0x0052CE30);
    actor->targetMode = 6;
    Actor_SetScale(actor, 0.01f);
    EnMu_Interact(self, play);
    self->actionFunc = EnMu_Pose;
}

// Draft (not matched, 13/57 words differ: struct-copy of world.pos gets r0 instead of r1 as the ldm temp,
// ROM keeps play in r6 before the copy). ~10 variants tried (signature, local order, Vec3f copy forms).
#if 0
void EnMu_Update(Actor* actor, GameState* state) {
    EnMu* self = (EnMu*)actor;
    PlayState* play = (PlayState*)state;
    f32 talkDist;
    Vec3f pos;

    pos = self->actor.world.pos;
    self->collider.dim.pos = pos;
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    FUN_00370734(&self->skelAnime);
    Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    self->actionFunc(self, play);
    talkDist = self->collider.dim.radius + 30.0f;
    FUN_00342714(play, &self->actor, &self->interactInfo, talkDist, (void*)EnMu_GetFaceReaction,
                 (void*)EnMu_CheckDialogState);
    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += 60.0f;
}
#endif
GLOBAL_ASM("asm/EnMu_Update.s")

s32 EnMu_OverrideDrawLimb(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg) {
    return 0;
}

void EnMu_PostDrawLimb(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
}

void EnMu_Draw(Actor* actor, GameState* state) {
    EnMu* self = (EnMu*)actor;
    // Raw ROM data (debt): colors table @0x004D99E4 (copied to the stack by armcc), face table @0x004D99DC.
    EnMuColors colors = *(EnMuColors*)0x004D99E4;
    f32 (*row)[4] = colors.c[actor->params];
    s8(*faces)[2] = (s8(*)[2])0x004D99DC;
    s32 i;

    for (i = 0; i < 4; i++) {
        f32* color = row[i];

        FUN_00357a50(&self->skelAnime, faces[i][0], faces[i][1], color, 1);
    }
    SkelAnime_DrawOpa(&self->skelAnime, &actor->modelMtx, EnMu_OverrideDrawLimb, EnMu_PostDrawLimb, actor, 0);
}

void EnMu_Interact(EnMu* self, PlayState* play) {
    u8 textIdOffset[] = { 0x42, 0x43, 0x3F, 0x41, 0x3E };
    u8 bitmask[] = { 0x01, 0x02, 0x04, 0x08, 0x10 };
    u8 talkFlags;
    s32 randomIndex;
    s32 i;

    talkFlags = gSaveContext.eventInf[2] & 0x1F;
    gSaveContext.eventInf[2] &= ~0x1F;
    randomIndex = (play->state.frames + (s32)(Rand_ZeroOne() * 5.0f)) % 5;

    for (i = 0; i < 5; i++) {
        if (!(talkFlags & bitmask[randomIndex])) {
            break;
        }

        randomIndex++;
        if (randomIndex >= 5) {
            randomIndex = 0;
        }
    }

    if (i == 5) {
        if (self->defaultTextId == (textIdOffset[randomIndex] | 0x7000)) {
            randomIndex++;
            if (randomIndex >= 5) {
                randomIndex = 0;
            }
        }
        talkFlags = 0;
    }

    talkFlags |= bitmask[randomIndex];
    self->defaultTextId = textIdOffset[randomIndex] | 0x7000;
    gSaveContext.eventInf[2] |= talkFlags;
}

s16 EnMu_CheckDialogState(PlayState* play, Actor* thisx) {
    switch (Message_GetState(&play->msgCtx)) {
        case 0:
        case 1:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            return NPC_TALK_STATE_TALKING;
        case 2:
            EnMu_Interact((EnMu*)thisx, play);
            return NPC_TALK_STATE_IDLE;
        default:
            return NPC_TALK_STATE_TALKING;
    }
}

u16 EnMu_GetFaceReaction(PlayState* play, Actor* thisx) {
    EnMu* self = (EnMu*)thisx;
    u16 textId = Text_GetFaceReaction(play, thisx->params + 0x3A);

    if (textId != 0) {
        return textId;
    }
    return self->defaultTextId;
}

}
