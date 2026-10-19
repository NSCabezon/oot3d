extern "C" {
#include "z_en_brob.hpp"

#define FLAGS 0x00000005

void EnBrob_Init(Actor* actor, GameState* state);
void EnBrob_Destroy(Actor* thisx, PlayState* play);
void EnBrob_Update(Actor* actor, GameState* state);
void EnBrob_Draw(Actor* thisx, PlayState* play);
void FUN_00272e34(void);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);

// Actor data minus the 8 leading bytes (EnBrob_Shock's colour statics, now C after InitVars).
GLOBAL_ASM("../src/game/actors/En_Brob/z_En_Brob_data_tail.s")

ActorInit En_Brob_InitVars = {
    ACTOR_EN_BROB,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_BROB,
    sizeof(EnBrob),
    (ActorFunc)EnBrob_Init,
    (ActorFunc)EnBrob_Destroy,
    (ActorFunc)EnBrob_Update,
    (ActorFunc)EnBrob_Draw,
};

GLOBAL_ASM("../src/game/actors/En_Brob/data_syms.s")


GLOBAL_ASM("asm/EnBrob_Init.s")

void EnBrob_Destroy(Actor* thisx, PlayState* play) {
    EnBrob* self = (EnBrob*)thisx;

    SkelAnime_Free2(&self->skelAnime);
    FUN_003508b8(thisx, NULL);
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyCylinder(play, &self->colliders[0]);
    Collider_DestroyCylinder(play, &self->colliders[1]);
}

GLOBAL_ASM("asm/EnBrob_Update.s")

void Animation_PlayOnce(SkelAnime* skelAnime, s32 idx);
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
s32 FUN_0036adf4(DynaPolyActor* dyna);
void FUN_0036d15c(PlayState* play, DynaCollisionContext* dyna, s32 bgId);
void FUN_001208cc(EnBrob* self, PlayState* play);
void FUN_003e440c(EnBrob* self, PlayState* play);
void EffectSsLightning_Spawn(PlayState* play, Vec3f* pos, Color_RGBA8* prim, Color_RGBA8* env, s16 scale, s16 rot, s32 p7, s32 p8);
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);
void FUN_003e2b80(EnBrob* self, PlayState* play);

// EnBrob_SetupIdle
void FUN_0036461c(EnBrob* self, PlayState* play) {
    FUN_0036d15c(play, &play->colCtx.dyna, self->dyna.bgId);
    if (self->actionFunc == FUN_001208cc) {
        self->timer = 300;
    } else {
        self->timer = 0;
    }
    self->modelOffsetY = 0;
    self->actionFunc = FUN_003e440c;
}

void FUN_003e440c(EnBrob* self, PlayState* play);
void FUN_003b96d8(EnBrob* self, PlayState* play);
void FUN_003e2b80(EnBrob* self, PlayState* play);
void FUN_003d6d70(EnBrob* self, PlayState* play);
void FUN_003da5f8(EnBrob* self, PlayState* play);

// Actor::colorFilterTimer is not in the repo Actor struct yet (raw offset 0x11A on 3DS).
static inline void EnBrob_SetColorFilterTimer(EnBrob* self, s16 value) {
    *(s16*)((u8*)&self->dyna.actor + 0x11A) = value;
}

// EnBrob_SetupMoveUp
static inline void EnBrob_SetupMoveUp(EnBrob* self, PlayState* play) {
    Animation_PlayOnce(&self->skelAnime, 0);
    FUN_0036b940(play, &play->colCtx.dyna, self->dyna.bgId);
    self->modelOffsetY = 1000;
    self->actionFunc = FUN_003b96d8;
}

f32* EnBrob_CurFramePtr(SkelAnime* skelAnime);

// EnBrob_Stunned
void FUN_001208cc(EnBrob* self, PlayState* play) {
    if (((s32 (*)(SkelAnime*))FUN_003731e0)(&self->skelAnime)) {
        FUN_0036461c(self, play);
    } else if (*EnBrob_CurFramePtr(&self->skelAnime) < 8.0f) {
        self->modelOffsetY -= 1000.0f * 2.0f / 3.0f;
    }
    EnBrob_SetColorFilterTimer(self, 120);
}

// EnBrob_Idle
void FUN_003e440c(EnBrob* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        if (FUN_0036adf4(&self->dyna)) {
            FUN_00374bb8(play, &self->dyna.actor, 5.0f, self->dyna.actor.yawTowardsPlayer, 1.0f);
            EnBrob_SetupMoveUp(self, play);
        } else if (self->dyna.actor.xzDistToPlayer < 300.0f) {
            EnBrob_SetupMoveUp(self, play);
        }
    } else if (self->timer > 120) {
        EnBrob_SetColorFilterTimer(self, 120);
    }
}

// EnBrob_Wobble
void FUN_003e2b80(EnBrob* self, PlayState* play) {
    ((s32 (*)(SkelAnime*))FUN_003731e0)(&self->skelAnime);
    if (Animation_OnFrameImpl(&self->skelAnime, 6.0f, 1.0f) || Animation_OnFrameImpl(&self->skelAnime, 15.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->dyna.actor, 0x010003BA);
    }
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0 && self->dyna.actor.xzDistToPlayer > 500.0f) {
        Animation_Change_00375c08(&self->skelAnime, 0, -1.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 0), 0.0f,
                                  2, -5.0f);
        self->modelOffsetY = 8250;
        self->actionFunc = FUN_003d6d70;
    }
}

// EnBrob_MoveDown
void FUN_003d6d70(EnBrob* self, PlayState* play) {
    if (((s32 (*)(SkelAnime*))FUN_003731e0)(&self->skelAnime)) {
        FUN_0036461c(self, play);
    } else {
        f32 curFrame = *EnBrob_CurFramePtr(&self->skelAnime);

        if (curFrame < 8.0f) {
            self->modelOffsetY -= 1000.0f * 2.0f / 3.0f;
        } else if (curFrame < 12.0f) {
            self->modelOffsetY -= 250.0f * 2.0f / 3.0f;
        } else {
            self->modelOffsetY += 250.0f * 2.0f / 3.0f;
        }
    }
}

// EnBrob_MoveUp
void FUN_003b96d8(EnBrob* self, PlayState* play) {
    if (((s32 (*)(SkelAnime*))FUN_003731e0)(&self->skelAnime)) {
        Animation_MorphToLoop(&self->skelAnime, 1, -5.0f);
        self->modelOffsetY = 8000;
        self->timer = 1800;
        self->actionFunc = FUN_003e2b80;
    } else {
        f32 curFrame = *EnBrob_CurFramePtr(&self->skelAnime);

        if (curFrame < 8.0f) {
            self->modelOffsetY += 1000.0f * 2.0f / 3.0f;
        } else if (curFrame < 12.0f) {
            self->modelOffsetY += 250.0f * 2.0f / 3.0f;
        } else {
            self->modelOffsetY -= 250.0f * 2.0f / 3.0f;
        }
    }
}

// EnBrob_Shock. The N64 function-local colour statics are real statics here (ROM derives prim as env-4); they
// sit at 0x521408 right after InitVars, so z_En_Brob_data_tail.s omits those 8 bytes.
void FUN_003da5f8(EnBrob* self, PlayState* play) {
    static Color_RGBA8 primColor = { 255, 255, 255, 255 };
    static Color_RGBA8 envColor = { 200, 255, 255, 255 };
    Vec3f pos;
    f32 dist1;
    f32 dist2;
    s32 i;

    FUN_003731e0(&self->skelAnime);
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) && self->timer != 0) {
        self->timer--;
    }

    dist1 = (*(u32*)((u8*)play + 0x5BF4)) % 2 ? 0.0f : self->dyna.actor.scale.x * 5500.0f;
    dist2 = self->dyna.actor.scale.x * 5500.0f;

    for (i = 0; i < 4; i++) {
        if (i % 2) {
            pos.x = self->dyna.actor.world.pos.x + dist1;
            pos.z = self->dyna.actor.world.pos.z + dist2;
        } else {
            pos.x = self->dyna.actor.world.pos.x + dist2;
            pos.z = self->dyna.actor.world.pos.z + dist1;
            dist1 = -dist1;
            dist2 = -dist2;
        }
        pos.y = (((Rand_ZeroOne() * 15000.0f) + 1000.0f) * self->dyna.actor.scale.y) + self->dyna.actor.world.pos.y;
        EffectSsLightning_Spawn(play, &pos, &primColor, &envColor,
                                self->dyna.actor.scale.y * 6400.0f, Rand_ZeroOne() * 65536.0f, 4, 1);
    }

    Audio_PlayActorSound2(&self->dyna.actor, 0x0100030A);
    if (self->timer == 0) {
        Animation_MorphToLoop(&self->skelAnime, 1, -5.0f);
        self->modelOffsetY = 8000;
        self->timer = 1800;
        self->actionFunc = FUN_003e2b80;
    }
}

// Draft (not matched yet: 23-24/36 words off; the ROM holds 1.0 in s0 before the PauseContext compare and
// copies it to pos.x). The model call goes to FUN_00272e34 (cfg split of FUN_00272930, asm).
#if 0
void EnBrob_Draw(Actor* thisx, PlayState* play) {
    EnBrob* self = (EnBrob*)thisx;
    f32 one = 1.0f;
    f32 alpha;

    alpha = PauseContext_GetState() != 0 ? one : 0.0f;
    self->unk_240[3] = alpha;
    {
        Vec3f pos;

        pos.x = one;
        pos.y = self->modelOffsetY;
        pos.z = one;
        Matrix_UpdatePosition(&thisx->modelMtx, &thisx->modelMtx, &pos);
    }
    SkelAnime_DrawOpa(&self->skelAnime, &thisx->modelMtx, NULL, (PostLimbDrawOpa)FUN_00272e34, thisx, 0);
}
#endif
GLOBAL_ASM("asm/EnBrob_Draw.s")
}
