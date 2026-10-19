extern "C" {
#include "z_magic_wind.hpp"

#define FLAGS 0x02000010
#define NA_SE_PL_MAGIC_WIND_NORMAL 0x0100007B
#define NA_SE_PL_MAGIC_WIND_WARP 0x0100007C
#define R_UPDATE_RATE SREG(30)
#define FRAMES(n) ((n) * 3.0f / R_UPDATE_RATE + 0.5f)

void MagicWind_Init(Actor* actor, GameState* state);
void MagicWind_Destroy(Actor* actor, GameState* state);
void MagicWind_Update(Actor* actor, GameState* state);
void MagicWind_Draw(Actor* actor, GameState* state);

s32 GetMessageMode(PlayState* play);
void FUN_0033579c(void* skelCurve, PlayState* play);
void FUN_0034708c(PlayState* play); // Magic_Reset
s32 FUN_0036c950(void* skelCurve, PlayState* play);
void FUN_0036f59c(void* player, s32 sfxId);
void FUN_001eaf58(MagicWind* self, PlayState* play);
void FUN_001eafc0(MagicWind* self, PlayState* play);
void FUN_001a1080(MagicWind* self, PlayState* play);
void FUN_0021e310(MagicWind* self, PlayState* play);
void FUN_00207b68(MagicWind* self, PlayState* play);
void* ZAR_GetCMBByIndex(void* zar, s32 idx);
void FUN_003357dc(void* skelCurve);
void FUN_00337200(void* skel, PlayState* play, void* cmb, void* unk178, s32 a, void* b, s32 c);
void FUN_0030f9f4(void* skel, s32 anim, f32 a, f32 b, f32 c, f32 d);
void FUN_00331094(void* a, s32 b, s32 c, void* d);
void FUN_003688a8(void* a, s32 b, s32 c, void* d);
void FUN_003334b4(void* a, void* b, s32 c, s32 d, void* e, s32 f);
u32 PauseContext_GetState(void);

ActorInit Magic_Wind_InitVars = {
    ACTOR_MAGIC_WIND,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(MagicWind),
    (ActorFunc)MagicWind_Init,
    (ActorFunc)MagicWind_Destroy,
    (ActorFunc)MagicWind_Update,
    (ActorFunc)MagicWind_Draw,
};

// Opus draft: 2 words off (116 words, same size). Only case 0 differs: ROM emits `vmov s0, s2` before
// `vmov s3, s16`, this emits them swapped. Everything else (zar slot, SkelCurve init arg order,
// hoisted 0.0/47.0/1.0 in s2/s1/s16, case-1 copy chain) matches. Key findings: case 0 must go through a
// 3-float wrapper (cur = start) and case 1 through the 4-float one; the SkelCurve init must be an
// inline wrapper taking all 7 args. ~60 variants tried (literal forms, locals, param orders, nesting).
#if 0
// SkelCurve_SetAnim-style inline wrappers (N64 SkelCurve_SetAnim(skelCurve, anim, start, end, cur, speed)).
static inline void SkelCurve_SetAnim(void* skelCurve, s32 anim, f32 startFrame, f32 endFrame, f32 curFrame, f32 playSpeed) {
    FUN_0030f9f4(skelCurve, anim, startFrame, endFrame, curFrame, playSpeed);
}
static inline void SkelCurve_SetAnim3(void* skelCurve, s32 anim, f32 startFrame, f32 endFrame, f32 playSpeed) {
    SkelCurve_SetAnim(skelCurve, anim, startFrame, endFrame, startFrame, playSpeed);
}

static inline void MagicWind_SkelCurveInit(void* skelCurve, PlayState* play, void* cmb, void* unk178, s32 a, void* b, s32 c) {
    FUN_00337200(skelCurve, play, cmb, unk178, a, b, c);
}

static inline u8* MagicWind_GetObjectSlot(PlayState* play, u8 bank) {
    if (bank < 0x13) {
        u8* slot = (u8*)play + (bank << 7);
        if (*(s32*)(slot + 0x3A64) != 0) {
            return slot + 0x3A5C;
        }
    }
    return NULL;
}

void MagicWind_Init(Actor* actor, GameState* state) {
    MagicWind* self = (MagicWind*)actor;
    PlayState* play = (PlayState*)state;
    void* player = (void*)*(u32*)((u8*)play + 0x20AC);
    ZARInfo* zar;
    void* cmb;

    actor->room = -1;
    zar = (ZARInfo*)(MagicWind_GetObjectSlot(play, actor->objBankIndex) + 0x10);
    cmb = ZAR_GetCMBByIndex(zar, 0x25);
    FUN_003357dc(&self->skelCurve);
    self->zarInfo = zar;
    MagicWind_SkelCurveInit(&self->skelCurve, play, cmb, actor->unk_178, 3, (u8*)self + 0x1EC, 5);

    switch (actor->params) {
        case 0:
            SkelCurve_SetAnim3(&self->skelCurve, 3, 0.0f, 47.0f, 1.0f);
            self->timer = (s16)FRAMES(29);
            self->actionFunc = FUN_001eaf58;
            break;
        case 1:
            SkelCurve_SetAnim(&self->skelCurve, 3, 47.0f, 0.0f, 47.0f, -1.0f);
            self->actionFunc = FUN_001a1080;
            FUN_0036f59c(player, NA_SE_PL_MAGIC_WIND_WARP);
            break;
    }

    self->skelAnimModel->unk_0C->FUN_00372d94(zar->GetCMABByIndex(0x12));
    self->skelAnimModel->unk_0C->SetAnimSpeed(2.0f);
    self->skelAnimModel->unk_0C->SetAnimType(1);
    self->alpha = 1.0f;
}
#endif

GLOBAL_ASM("asm/MagicWind_Init.s")

// Magic_Reset is FUN_0034708c (placed right after this function). The tail call `b FUN_0034708c`
// becomes `mov r0, r0` because armlink --tailreorder puts the callee right after the caller.
void MagicWind_Destroy(Actor* actor, GameState* state) {
    MagicWind* self = (MagicWind*)actor;
    PlayState* play = (PlayState*)state;

    FUN_0033579c(&self->skelCurve, play);
    FUN_0034708c(play); // Magic_Reset
}

void MagicWind_Update(Actor* actor, GameState* state) {
    MagicWind* self = (MagicWind*)actor;
    PlayState* play = (PlayState*)state;
    s32 msgMode = GetMessageMode(play);

    if ((msgMode == 0xD) || (msgMode == 0x11)) {
        Actor_Kill(actor);
        return;
    }

    self->actionFunc(self, play);
}

void FUN_001eaf58(MagicWind* self, PlayState* play) {
    void* player = (void*)*(u32*)((u8*)play + 0x20AC);

    if (self->timer > 0) {
        self->timer--;
        return;
    }

    FUN_0036f59c(player, NA_SE_PL_MAGIC_WIND_NORMAL);
    self->alpha = 1.0f;
    self->actionFunc = FUN_001eafc0;
    FUN_0036c950(self->skelCurve, play);
}

void FUN_001eafc0(MagicWind* self, PlayState* play) {
    if (FUN_0036c950(self->skelCurve, play)) {
        self->actionFunc = FUN_00207b68;
        self->timer = (s16)FRAMES(50);
    }
}

void FUN_001a1080(MagicWind* self, PlayState* play) {
    if (FUN_0036c950(self->skelCurve, play)) {
        Actor_Kill(&self->actor);
    }
}

void FUN_00207b68(MagicWind* self, PlayState* play) {
    if (self->timer > 0) {
        self->timer--;
    } else {
        self->actionFunc = FUN_0021e310;
        self->timer = (s16)FRAMES(30);
    }
}

static inline void MagicWind_UpdateAlpha(MagicWind* self, f32 alpha) {
    self->alpha = alpha;
}

void FUN_0021e310(MagicWind* self, PlayState* play) {
    if (self->timer > 0) {
        MagicWind_UpdateAlpha(self, (s32)((f32)self->timer * R_UPDATE_RATE / 3.0f + 0.5f) * (1.0f / 30.0f));
        self->timer--;
    } else {
        Actor_Kill(&self->actor);
    }
}

void MagicWind_Draw(Actor* actor, GameState* state) {
    MagicWind* self = (MagicWind*)actor;
    f32 speed;
    void* skelCurveSkel;

    if (self->actionFunc != FUN_001eaf58) {
        if (PauseContext_GetState() == 0) {
            speed = 2.0f;
        } else {
            speed = 0.0f;
        }
        self->skelAnimModel->unk_0C->SetAnimSpeed(speed);
        skelCurveSkel = *(void**)((u8*)self->skelAnimModel + 0x10);
        {
            f32 color[4];

            FUN_00331094(skelCurveSkel, 0, 0, color);
            color[3] = self->alpha;
            FUN_003688a8(skelCurveSkel, 0, 0, color);
        }
        {
            void* ptr = *(void**)((u8*)skelCurveSkel + 4);
            *(u8*)((u8*)ptr + 0xA) = 1;
        }
        FUN_003334b4(&self->skelCurve, (u8*)actor + 0x148, 0, 0, self, 0);
    }
}
}
