extern "C" {
#include "z_en_kakasi.hpp"

#define FLAGS 0x02000009

void EnKakasi_Init(Actor* actor, GameState* state);
void EnKakasi_Destroy(Actor* actor, GameState* state);
void EnKakasi_Update(EnKakasi* self, PlayState* play);
void EnKakasi_Draw(Actor* actor, GameState* state);
s32 EnKakasi_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg);
void EnKakasi_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg);

void FUN_001808cc(EnKakasi* self, PlayState* play);
void FUN_003e48f8(EnKakasi* self, PlayState* play); // N64: func_80A8F75C (idle/talk), functions_3E0000s.cpp
// 3DS Animation_Change: same argument order as N64 (mode after endFrame).
void FUN_00353020(SkelAnime* skelAnime, void* animation, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, void* animation);
void __ARM_common_memcpy2_6(void* dst, void* src);
void FUN_0034fe20(Actor* actor, PlayState* play, SkelAnime* skelAnime, s32 arg3, void* anim, s32 arg5, s32 arg6,
                  s32 arg7);

// object_ka animation reference at 0x004D97A8 (no data symbol in the scatter file yet; absolute
// EQU in ka_symbols.s, same address as En_Kakasi3's kk3AnimRef). A real symbol makes armcc reload
// the address from the literal pool after a call, which a constant-address cast does not.
extern void* kaAnimRef;

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        0x00,
        0x04,
        0x01,
    },
    { 20.0f, 70.0f, 0.0f, { 0.0f, 0.0f, 0.0f } },
};

ActorInit En_Kakasi_InitVars = {
    ACTOR_EN_KAKASI,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_KA,
    sizeof(EnKakasi),
    (ActorFunc)EnKakasi_Init,
    (ActorFunc)EnKakasi_Destroy,
    (ActorFunc)EnKakasi_Update,
    (ActorFunc)EnKakasi_Draw,
};

void EnKakasi_Init(Actor* actor, GameState* state) {
    EnKakasi* self = (EnKakasi*)actor;
    PlayState* play = (PlayState*)state;

    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, actor, &sCylinderInit);
    actor->targetMode = 6;
    FUN_00372f38(actor, play, NULL);
    {
        FUN_0034fe20(actor, play, &self->skelanime, 0, kaAnimRef, 0, 0, 0);
    }
    __ARM_common_memcpy2_6(&self->rot, &actor->world.rot);
    actor->flags |= 0x400;
    actor->colChkInfo.mass = 0xFF;
    Actor_SetScale(actor, 0.01f);
    self->actionFunc = FUN_001808cc;
}

void EnKakasi_Destroy(Actor* actor, GameState* state) {
    EnKakasi* self = (EnKakasi*)actor;
    PlayState* play = (PlayState*)state;

    FUN_003508b8(actor, NULL);
    SkelAnime_Free2(&self->skelanime);
    Collider_DestroyCylinder(play, &self->collider);
}

// N64: func_80A8F660 (idle setup). Moved from functions_180000s.cpp.
void FUN_001808cc(EnKakasi* self, PlayState* play) {
    s32 frameCount = Animation_GetLastFrame(&self->skelanime, kaAnimRef);

    FUN_00353020(&self->skelanime, &kaAnimRef, 1.0f, 0.0f, (f32)frameCount, 0, -10.0f);
    self->actor.textId = 0x4076;
    self->unk_22E = TEXT_STATE_DONE;
    if (!LINK_IS_ADULT) {
        self->unk_22C = 0;
        if (*((u8*)&gSaveContext + 0xF58) != 0) { // scarecrowLongSongSet (raw offset, debt)
            self->actor.textId = 0x407A;
            self->unk_22E = TEXT_STATE_EVENT;
        }
    } else {
        self->unk_22C = 1;
        if (*((u8*)&gSaveContext + 0xF58) != 0) {
            self->actor.textId = 0x4079;
            self->unk_22E = TEXT_STATE_EVENT;
        }
    }
    self->actionFunc = FUN_003e48f8;
}

// DECR (ternary with a side effect) keeps armcc from fully unrolling the 4-timer loop; a plain
// `if (x != 0) x--;` gets unrolled. (EnKakasi*, PlayState*) signature fixes the loop registers.
void EnKakasi_Update(EnKakasi* self, PlayState* play) {
    s32 i;

    self->unk_230++;
    self->actor.world.rot = self->actor.shape.rot;
    for (i = 0; i < ARRAY_COUNT(self->unk_234); i++) {
        DECR(self->unk_234[i]);
    }

    self->height = 60.0f;
    Actor_SetFocus(&self->actor, self->height);
    self->actionFunc(self, play);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 50.0f, 50.0f, 100.0f, 0x1C);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
}

void EnKakasi_Draw(Actor* actor, GameState* state) {
    EnKakasi* self = (EnKakasi*)actor;

    FUN_00357fd0(GET_PLAYER((PlayState*)state), actor->unk_178, &actor->world.pos);
    SkelAnime_DrawOpa(&self->skelanime, &actor->modelMtx, EnKakasi_OverrideLimbDraw, EnKakasi_PostLimbDraw, actor, 0);
}

s32 EnKakasi_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg) {
}

void EnKakasi_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg) {
}

GLOBAL_ASM("../src/game/actors/En_Kakasi/ka_symbols.s")

}
