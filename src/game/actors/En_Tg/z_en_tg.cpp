extern "C" {
#include "z_en_tg.hpp"

#define FLAGS 0x00000009

#define NPC_TALK_STATE_IDLE 0
#define NPC_TALK_STATE_TALKING 1

void EnTg_Init(Actor* actor, GameState* state);
void EnTg_Destroy(Actor* actor, GameState* state);
void EnTg_Update(Actor* actor, GameState* state);
void EnTg_Draw(Actor* actor, GameState* state);

s32 EnTg_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg);
void EnTg_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);

void FUN_0034f558(SkelAnime* skelAnime, PlayState* play);
void FUN_00342714(PlayState* play, Actor* actor, void* talkState, f32 range, void* getTextId,
                  void* updateTalkState); // Npc_UpdateTalking
s16 FUN_0018645c(PlayState* play, Actor* thisx); // EnTg_UpdateTalkState

GLOBAL_ASM("data/z_En_Tg.data.s")

ActorInit En_Tg_InitVars = {
    ACTOR_EN_TG,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_MU,
    sizeof(EnTg),
    (ActorFunc)EnTg_Init,
    (ActorFunc)EnTg_Destroy,
    (ActorFunc)EnTg_Update,
    (ActorFunc)EnTg_Draw,
};

// Moved from functions_3A0000s.cpp (keeps ld-pinned name). EnTg_SpinIfNotTalking; only referenced by EnTg_Init.
void FUN_003ade94(EnTg* self, PlayState* play) {
    if (!self->interactInfo.talkState) {
        self->actor.shape.rot.y += 0x800;
    }
}

// Moved from functions_180000s.cpp (keeps ld-pinned name). EnTg_GetTextId; only referenced by EnTg_Update.
u32 FUN_001864dc(PlayState* play, Actor* thisx) {
    EnTg* self = (EnTg*)thisx;
    u16 maskReactionTextId = Text_GetFaceReaction(play, 0x24);
    u32 textId;

    if (maskReactionTextId != 0) {
        return maskReactionTextId;
    }
    if (play->sceneNum == 0x52) {
        if (self->nextDialogue % 2 != 0) {
            textId = 0x5089;
        } else {
            textId = 0x508A;
        }
        return textId;
    } else {
        if (self->nextDialogue % 2 != 0) {
            textId = 0x7025;
        } else {
            textId = 0x7026;
        }
        return textId;
    }
}

// Moved from functions_180000s.cpp (keeps ld-pinned name). EnTg_UpdateTalkState; only referenced by EnTg_Update.
// The full N64 case list (0,1,3..9 -> TALKING) is what makes armcc emit the 3-entry jump table: trailing
// cases equal to default are trimmed from the table.
s16 FUN_0018645c(PlayState* play, Actor* thisx) {
    EnTg* self = (EnTg*)thisx;

    switch (Message_GetState(&play->msgCtx)) {
        case 0: // TEXT_STATE_NONE
        case 1: // TEXT_STATE_DONE_HAS_NEXT
        case 3: // TEXT_STATE_DONE_FADING
        case 4: // TEXT_STATE_CHOICE
        case 5: // TEXT_STATE_EVENT
        case 6: // TEXT_STATE_DONE
        case 7: // TEXT_STATE_SONG_DEMO_DONE
        case 8:
        case 9:
            return NPC_TALK_STATE_TALKING;
        case 2: // TEXT_STATE_CLOSING
            switch (self->actor.textId) {
                case 0x5089:
                case 0x508A:
                    self->nextDialogue++;
                    break;
                case 0x7025:
                case 0x7026:
                    self->actor.params ^= 1;
                    self->nextDialogue++;
                    break;
            }
            return NPC_TALK_STATE_IDLE;
        default:
            return NPC_TALK_STATE_TALKING;
    }
}

// Init: matches (0 words differ). Data: sCylinderInit @0x0052F770, sColChkInfoInit @0x0052F7A8 (raw, debt).
void EnTg_Init(Actor* actor, GameState* state) {
    EnTg* self = (EnTg*)actor;
    PlayState* play = (PlayState*)state;

    self->unk_864 = 1;
    ActorShape_Init(&actor->shape, 0.0f, ActorShadow_DrawCircle, 28.0f);
    FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
    FUN_00353c9c(actor, play, &self->skelAnime, 0, 1, self->jointTable, self->morphTable, 0xE);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, actor, (ColliderCylinderInit*)0x0052F770);
    CollisionCheck_SetInfo2(&actor->colChkInfo, NULL, (CollisionCheckInfoInit2*)0x0052F7A8);
    if (play->sceneNum == 0x20) {
        actor->flags &= ~0x10;
        actor->uncullZoneScale = 50.0f;
    }
    actor->targetMode = 6;
    Actor_SetScale(actor, 0.01f);
    self->nextDialogue = play->state.frames & 1;
    self->actionFunc = (EnTgActionFunc)FUN_003ade94;
}

// Destroy: matches (the armlink-inlined Collider_DestroyCylinder shows as mov r0,#1 in ROM).
void EnTg_Destroy(Actor* actor, GameState* state) {
    EnTg* self = (EnTg*)actor;
    PlayState* play = (PlayState*)state;

    FUN_0034f558(&self->skelAnime, play);
    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(actor, self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
}

// Draft (not matched, best 16/58 words differ). A ColliderCylinder* local gives the ROM frame
// (push {r3-r7,lr}, play in r5) but armcc keeps col (this+0x7DC) in r6 from the start, while ROM materializes
// this+0x800 in r6 only after FUN_003731e0 (base for the radius vldr at +0x1C); ROM also puts pos.z in s0,
// x in s2 and uses r1 (not r0) for the unk_864 = 0 store. ~30 variants tried (Opus pass): field order, Vec3f
// copies, Vector3f ctor, GameState* param, thisx routing, inline helpers, scoped pointer locals.
#if 0
void EnTg_Update(Actor* thisx, PlayState* play) {
    EnTg* self = (EnTg*)thisx;
    Vec3f sp2C;

    sp2C.x = self->actor.world.pos.x;
    sp2C.y = self->actor.world.pos.y;
    sp2C.z = self->actor.world.pos.z + 3.0f;
    self->collider.dim.pos = sp2C;
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    FUN_003731e0(&self->skelAnime);
    {
        ColliderCylinder* col = &self->collider;

        if (self->unk_864) {
            Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);
            if (play->sceneNum == 0x20) {
                self->unk_864 = 0;
            }
        }
        self->actionFunc(self, play);
        f32 temp = col->dim.radius + 30.0f;
        FUN_00342714(play, &self->actor, &self->interactInfo, temp, (void*)FUN_001864dc, (void*)FUN_0018645c);
    }
}
#endif
GLOBAL_ASM("asm/EnTg_Update.s")

s32 EnTg_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg) {
    return 0;
}

// N64 Matrix_MultVec3f(src, dest) argument order; the wrapper gives the predicated call shape.
static inline void EnTg_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void EnTg_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnTg* self = (EnTg*)thisx;
    Vector3f targetOffset(0.0f, 800.0f, 0.0f);

    if (limbIndex == 11) {
        EnTg_MultVec3f(&targetOffset, (Vector3f*)&self->actor.focus.pos, mtx);
    }
}

void EnTg_Draw(Actor* actor, GameState* state) {
    EnTg* self = (EnTg*)actor;

    SkelAnime_DrawOpa(&self->skelAnime, &actor->modelMtx, EnTg_OverrideLimbDraw, EnTg_PostLimbDraw, actor, 0);
}

}
