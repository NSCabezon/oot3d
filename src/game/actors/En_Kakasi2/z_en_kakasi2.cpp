extern "C" {
#include "z_en_kakasi2.hpp"

#define FLAGS 0x0A000031

void EnKakasi2_Init(Actor* actor, GameState* state);
void EnKakasi2_Destroy(Actor* actor, GameState* state);
void EnKakasi2_Update(Actor* actor, GameState* state);
void FUN_0024c6b8(Actor* actor, GameState* state); // Draw (N64 func_80A90948)
void FUN_003e552c(EnKakasi2* self, PlayState* play); // Hidden wait, switch/ocarina trigger (N64 func_80A90264)
void FUN_001785ec(EnKakasi2* self, PlayState* play); // Rise start (N64 func_80A904D8)
void FUN_003deda0(EnKakasi2* self, PlayState* play); // Rise (N64 func_80A90578)
void FUN_00180488(EnKakasi2* self, PlayState* play); // Idle anim start (N64 func_80A9062C)
void FUN_003e1884(EnKakasi2* self, PlayState* play); // Idle anim approach (N64 func_80A906C4)

s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 index);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 index, s32 mode, f32 speed, f32 start, f32 end, f32 morph);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skelAnime, void* cmb, void* p, s32 a, s32 b, s32 c, s32 d);
void OnePointCutscene_Attention(PlayState* play, Actor* actor, s32 a, s32 b);

// ROM data order: sCylinderInit precedes InitVars (0x0052B3E0).
static ColliderCylinderInit sCylinderInit = {
    { 0x0A, 0x00, 0x09, 0x39, 0x20, 0x01 },
    { 0, { 0xFFCFFFFF, 0, 0 }, { 0xFFCFFFFF, 0, 0 }, 0, 5, 1 },
    { 20.0f, 70.0f, 0.0f, { 0.0f, 0.0f, 0.0f } },
};

ActorInit En_Kakasi2_InitVars = {
    ACTOR_EN_KAKASI2,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_KA,
    sizeof(EnKakasi2),
    (ActorFunc)EnKakasi2_Init,
    (ActorFunc)EnKakasi2_Destroy,
    (ActorFunc)EnKakasi2_Update,
    NULL,
};

extern __pure float _fabsf(float);
static __inline __pure float kk2_fabsf(float f) {
    return _fabsf(f);
}

// Object bank lookup: NULL if the slot is out of range or not loaded (N64 has no equivalent; 3DS ZAR loading).
static inline u8* Kk2_GetObjStatus(PlayState* play, s32 idx) {
    if ((u32)idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

// Shared by Init and FUN_003e552c. The `p` local is needed: it moves the unk_178 load into ip and adds r8 + an
// extra stack word to the frame, exactly as in the ROM.
static inline void Kk2_InitSkel(EnKakasi2* self, PlayState* play) {
    void* zar = Kk2_GetObjStatus(play, (u8)self->actor.objBankIndex) + 0x10;
    SkelAnime* skel = &self->skelAnime;
    void* cmb = ZAR_GetCMBByIndex(zar, 0);
    void* p = self->actor.unk_178;

    FUN_00353e78(zar, play, skel, cmb, p, 0, 0, 0, 0);
}

// Init: not matched (5 words: zar/skel callee-saved regs vs spawnRange float regs see-saw; every variant fixes one
// side and breaks the other). Left as asm, draft below. Alt: wrapping `p` in its own block inside Kk2_InitSkel
// fixes the float regs but swaps r5/r7 for zar/skel.
GLOBAL_ASM("asm/EnKakasi2_Init.s")

#if 0
void EnKakasi2_Init(Actor* actor, GameState* state) {
    EnKakasi2* self = (EnKakasi2*)actor;
    PlayState* play = (PlayState*)state;
    f32 spawnRangeY;
    f32 spawnRangeXZ;

    self->switchFlag = actor->params & 0x3F;
    spawnRangeY = (u32)((u16)actor->params << 0x12) >> 0x18;
    spawnRangeXZ = actor->world.rot.z;
    if (self->switchFlag == 0x3F) {
        self->switchFlag = -1;
    }
    self->maxSpawnDistance.x = (spawnRangeY * 40.0f) + 40.0f;
    self->maxSpawnDistance.y = (spawnRangeXZ * 40.0f) + 40.0f;
    actor->targetMode = 4;
    self->height = 60.0f;
    actor->colChkInfo.mass = 0xFF;
    Actor_SetScale(actor, 0.01f);
    actor->flags |= 0x400;
    self->unk_230 = actor->shape.rot.y;

    if (self->switchFlag >= 0 && Flags_GetSwitch(play, self->switchFlag)) {
        self->actor.draw = FUN_0024c6b8;
        Collider_InitCylinder(play, &self->collider);
        Collider_SetCylinder(play, &self->collider, &self->actor, &sCylinderInit);
        Kk2_InitSkel(self, play);
        self->unk_29C = 0;
        self->actionFunc = FUN_00180488;
    } else {
        self->actor.shape.yOffset = -8000.0f;
        self->actionFunc = FUN_003e552c;
    }
}
#endif

void EnKakasi2_Destroy(Actor* actor, GameState* state) {
    EnKakasi2* self = (EnKakasi2*)actor;

    Collider_DestroyCylinder((PlayState*)state, &self->collider);
    SkelAnime_Free2(&self->skelAnime);
}

// N64 func_80A90264 (hidden wait).
void FUN_003e552c(EnKakasi2* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    self->unk_22C++;
    if ((self->actor.xzDistToPlayer < self->maxSpawnDistance.x) &&
        (kk2_fabsf(player->actor.world.pos.y - self->actor.world.pos.y) < self->maxSpawnDistance.y) &&
        (*(u16*)((u8*)&gSaveContext + 0xE00 + 0xFE) & 0x1000)) {
        self->unk_22C = 0;
        if (*(u16*)((u8*)play + 0x2B7E) == 0x0B) {
            if (self->switchFlag >= 0) {
                Flags_SetSwitch(play, self->switchFlag);
            }
            *(u16*)((u8*)play + 0x2B7E) = 4;
            self->actor.draw = FUN_0024c6b8;
            Collider_InitCylinder(play, &self->collider);
            Collider_SetCylinder(play, &self->collider, &self->actor, &sCylinderInit);
            Kk2_InitSkel(self, play);
            OnePointCutscene_Attention(play, &self->actor, 0, 0);
            FUN_00372244((u8*)play + 0x5FCC, 0x1E, 0x01000487);
            self->actor.flags |= 0x8000001;
            self->actionFunc = FUN_001785ec;
        }
    }
}

void FUN_001785ec(EnKakasi2* self, PlayState* play) {
    Animation_Change_00375c08(&self->skelAnime, 0, 0, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 0), -10.0f);
    Audio_PlayActorSound2(&self->actor, 0x01000216);
    self->actionFunc = FUN_003deda0;
}

void FUN_003deda0(EnKakasi2* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if ((s16)self->skelAnime.curFrame == 11 || (s16)self->skelAnime.curFrame == 17) {
        Audio_PlayActorSound2(&self->actor, 0x010001C1);
    }
    self->actor.shape.rot.y += 0x800;
    Math_ApproachZeroF(&self->actor.shape.yOffset, 0.5f, 500.0f);
    if (self->actor.shape.yOffset > -100.0f) {
        self->actor.shape.yOffset = 0.0f;
        self->actionFunc = FUN_00180488;
    }
}

void FUN_00180488(EnKakasi2* self, PlayState* play) {
    f32 frameCount = Animation_GetLastFrame(&self->skelAnime, 0);

    Animation_Change_00375c08(&self->skelAnime, 0, 2, 0.0f, 0.0f, frameCount, -10.0f);
    self->actionFunc = FUN_003e1884;
}

// N64 func_80A906C4 (idle approach). The ROM takes &curFrame through a sub-object at skelAnime+0x30
// (presumably the real 3DS SkelAnime has an embedded animation-state struct there).
typedef struct {
    /* 0x00 */ s32 animationType;
    /* 0x04 */ char unk_04[0x8];
    /* 0x0C */ f32 curFrame;
} EnKakasi2AnimState;

void FUN_003e1884(EnKakasi2* self, PlayState* play) {
    SkelAnime* skel = &self->skelAnime;
    EnKakasi2AnimState* state = (EnKakasi2AnimState*)&skel->animationType;

    if (skel->curFrame != 0.0f) {
        Math_ApproachZeroF(&state->curFrame, 0.5f, 1.0f);
    }
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->unk_230, 5, 0xBB8, 0);
    FUN_003731e0(&self->skelAnime);
}

void EnKakasi2_Update(Actor* actor, GameState* state) {
    EnKakasi2* self = (EnKakasi2*)actor;
    PlayState* play = (PlayState*)state;

    actor->world.rot = actor->shape.rot;
    Actor_SetFocus(actor, self->height);
    self->actionFunc(self, play);
    Actor_MoveForward(actor);

    if (actor->shape.yOffset == 0.0f) {
        Collider_UpdateCylinder(actor, &self->collider);
        if (self->collider.base.acFlags & 2) {
            self->collider.base.acFlags &= ~2;
            self->unk_29C = 6;
        }
        if (self->unk_29C <= 0) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
        } else {
            self->unk_29C--;
        }
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    }
}

void FUN_0024c6b8(Actor* actor, GameState* state) {
    EnKakasi2* self = (EnKakasi2*)actor;
    PlayState* play = (PlayState*)state;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &actor->modelMtx);
    FUN_00357fd0(GET_PLAYER(play), actor->unk_178, &actor->world.pos);
    SkelAnime_DrawOpa(&self->skelAnime, &actor->modelMtx, NULL, NULL, actor, 0);
}

}
