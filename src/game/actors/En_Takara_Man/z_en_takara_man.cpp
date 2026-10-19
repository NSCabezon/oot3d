extern "C" {
#include "z_en_takara_man.hpp"

#define FLAGS 0x08000039

void EnTakaraMan_Init(Actor* actor, GameState* state);
void EnTakaraMan_Destroy(Actor* actor, GameState* state);
void EnTakaraMan_Update(Actor* actor, GameState* state);
void EnTakaraMan_Draw(Actor* actor, GameState* state);

// Callees outside this actor's directory.
void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx);
void FUN_001810fc(EnTakaraMan* self, PlayState* play); // N64: func_80B176E0 (first action, still asm)
void FUN_0034df48(MTX34* mtx, Vec3s* rot);
void FUN_0036bcc8(PlayState* play, Actor* actor, Vec3s* headRot, Vec3s* torsoRot, Vec3f focusPos, s32 yawLimit);

ActorInit En_Takara_Man_InitVars = {
    ACTOR_EN_TAKARA_MAN,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_TS,
    sizeof(EnTakaraMan),
    (ActorFunc)EnTakaraMan_Init,
    (ActorFunc)EnTakaraMan_Destroy,
    (ActorFunc)EnTakaraMan_Update,
    (ActorFunc)EnTakaraMan_Draw,
};

GLOBAL_ASM("asm/EnTakaraMan_Init.s")

#if 0 // draft, not byte-identical yet (see STATUS.md)
void EnTakaraMan_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnTakaraMan* self = (EnTakaraMan*)actor;

    if (*((u8*)play + 0x7F94)) { // "already initialized" static, lives in PlayState on 3DS
        Actor_Kill(actor);
        return;
    }
    *((u8*)play + 0x7F94) = true;
    play->actorCtx.flags.chest = 0;
    gSaveContext.save.info.inventory.dungeonKeys[gSaveContext.mapIndex] = -1;
    FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
    FUN_00353c9c(actor, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 9);
    SkelAnime_SetFaceAnimations(&self->faceAnims, &self->skelAnime, 0, -1, -1);
    actor->focus.pos = actor->world.pos;
    self->pos = actor->world.pos;
    actor->world.pos.x = 133.0f;
    actor->world.pos.y = -12.0f;
    actor->world.pos.z = 102.0f;
    Actor_SetScale(actor, 0.013f);
    self->height = 90.0f;
    self->originalRoomNum = actor->room;
    actor->room = -1;
    actor->world.rot.y = actor->shape.rot.y = -0x4E20;
    actor->attentionRangeType = ATTENTION_RANGE_1;
    self->actionFunc = FUN_001810fc;
}
#endif

void EnTakaraMan_Destroy(Actor* actor, GameState* state) {
    EnTakaraMan* self = (EnTakaraMan*)actor;

    self->faceAnims.Destroy();
    FUN_003508b8(actor, self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
}

GLOBAL_ASM("asm/EnTakaraMan_Update.s")

#if 0 // draft, not byte-identical yet (see STATUS.md)
void EnTakaraMan_Update(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnTakaraMan* self = (EnTakaraMan*)actor;
    s16 eyeTimer = self->eyeTimer;

    if (eyeTimer != 0) {
        self->eyeTimer = eyeTimer - 1;
    }

    Actor_SetFocus(actor, self->height);
    FUN_0036bcc8(play, actor, &self->unk_22C, &self->unk_232, actor->focus.pos, 0x4300);
    if (self->eyeTimer == 0) {
        self->eyeTextureIdx++;
        if (self->eyeTextureIdx >= 2) {
            self->eyeTextureIdx = 0;
            self->eyeTimer = (s16)Rand_ZeroFloat(60.0f) + 20;
        }
    }
    self->unk_212++;
    self->actionFunc(self, play);
}
#endif

s32 EnTakaraMan_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnTakaraMan* self = (EnTakaraMan*)thisx;

    if (limbIndex == 7) {
        FUN_0034df48(mtx, &self->unk_22C);
    } else if (limbIndex == 0) {
        FUN_0034df48(mtx, &self->unk_232);
    }
    return 0;
}

GLOBAL_ASM("asm/EnTakaraMan_Draw.s")

#if 0 // draft, 4 words differ (register order around eyeTextureIdx load), see STATUS.md
void EnTakaraMan_Draw(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnTakaraMan* self = (EnTakaraMan*)actor;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    FaceAnimations* faceAnims = &self->faceAnims;
    faceAnims->FUN_0035e3a4(0, self->eyeTextureIdx);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, (OverrideLimbDrawOpa)EnTakaraMan_OverrideLimbDraw,
                      NULL, actor, 0);
    faceAnims->FUN_0035e330();
}
#endif

}
