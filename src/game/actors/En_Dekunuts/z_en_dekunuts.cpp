extern "C" {
#include "z_en_dekunuts.hpp"

#define FLAGS 0x00000005
#define DEKUNUTS_FLOWER 10

void EnDekunuts_Init(Actor* actor, GameState* state);
void EnDekunuts_Destroy(Actor* thisx, GameState* state);
void EnDekunuts_Update(Actor* actor, GameState* state);
void EnDekunuts_Draw(Actor* thisx, GameState* state);

void FUN_00378aa8(void);
s32 FUN_0037896c(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg);
void FUN_0036c174(MTX34* dst, MTX34* a, MTX34* b);

void Actor_PlaySfx(Actor* actor, u16 sfxId);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
void Animation_PlayOnceSetSpeed(SkelAnime* skelAnime, s32 idx, f32 speed);
void Animation_PlayOnce(SkelAnime* skelAnime, s32 idx);
void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);
void Animation_MorphToPlayOnce(SkelAnime* skelAnime, s32 idx, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void FUN_0013e408(EnDekunuts* self, PlayState* play); // N64: EnDekunuts_Wait (asm, not yet matched)
void FUN_00123ef4(EnDekunuts* self, PlayState* play); // N64: EnDekunuts_Run (asm, not yet matched)
void FUN_0011d56c(EnDekunuts* self, PlayState* play); // N64: EnDekunuts_ThrowNut (asm, not yet matched)
void FUN_001c6b54(EnDekunuts* self, PlayState* play); // N64: EnDekunuts_Stand (asm, not yet matched)
void FUN_001c82dc(EnDekunuts* self); // N64: EnDekunuts_SetupRun (own TU: En_Dekunuts/z_en_dekunuts_setup.cpp)
// Math_ApproachS in the header is a different symbol; the ROM calls FUN_00370084 (N64 Math_ApproachS).
void FUN_00370084(s16* pValue, s16 target, s16 scale, s16 step);
void FUN_001e8ac0(EnDekunuts* self); // N64: EnDekunuts_SetupWait (own TU: En_Dekunuts/z_en_dekunuts_setup.cpp)
void FUN_00258b58(EnDekunuts* self); // N64: EnDekunuts_SetupStand (own TU: En_Dekunuts/z_en_dekunuts_setup.cpp)
f32* EnDekunuts_CurFramePtr(SkelAnime* skelAnime); // own TU: En_Dekunuts/z_en_dekunuts_anim.cpp
void FUN_003d89c8(EnDekunuts* self, PlayState* play); // N64: EnDekunuts_Die (asm, not yet matched)
void FUN_00176d30(EnDekunuts* self, PlayState* play); // N64: EnDekunuts_Gasp? (asm, not yet matched)

GLOBAL_ASM("data/z_En_Dekunuts.data.s")

GLOBAL_ASM("../src/game/actors/En_Dekunuts/syms.s")

ActorInit En_Dekunuts_InitVars = {
    ACTOR_EN_DEKUNUTS,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_DEKUNUTS,
    sizeof(EnDekunuts),
    (ActorFunc)EnDekunuts_Init,
    (ActorFunc)EnDekunuts_Destroy,
    (ActorFunc)EnDekunuts_Update,
    (ActorFunc)EnDekunuts_Draw,
};

GLOBAL_ASM("asm/EnDekunuts_Init.s")

GLOBAL_ASM("asm/EnDekunuts_Update.s")

void EnDekunuts_Destroy(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnDekunuts* self = (EnDekunuts*)thisx;

    if (thisx->params == DEKUNUTS_FLOWER) {
        if (self->model != NULL) {
            ((EnDekunutsModel*)self->model)->Destroy();
        }
    } else {
        SkelAnime_Free2(&self->skelAnime);
        Collider_DestroyCylinder(play, &self->collider);
    }
}

void EnDekunuts_Draw(Actor* thisx, GameState* state) {
    EnDekunuts* self = (EnDekunuts*)thisx;

    if (thisx->params == DEKUNUTS_FLOWER) {
        self->model->SetUnkAC(1);
        self->model->FUN_003721e0((MTX34*)&thisx->modelMtx);
        self->model->FUN_00372170(0);
    } else {
        SkelAnime_DrawOpa(&self->skelAnime, (MTX34*)&self->actor.modelMtx, (OverrideLimbDrawOpa)FUN_0037896c,
                          (PostLimbDrawOpa)FUN_00378aa8, &self->actor, 0);
    }
}

void FUN_00378aa8(void) {
}

// Shared throw-nut setup (ROM has it as one tail block reached from both timer tests of Stand).
static inline void EnDekunuts_SetupThrowNut(EnDekunuts* self) {
    Animation_PlayOnce(&self->skelAnime, sEnDekunutsAnimData.unk_00[0]);
    self->animFlagAndTimer = self->shotsPerRound;
    self->actionFunc = FUN_0011d56c;
}

void FUN_001c6b54(EnDekunuts* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) && (self->animFlagAndTimer != 0)) {
        self->animFlagAndTimer--;
    }
    if (!(self->animFlagAndTimer & 0x1000)) {
        FUN_00370084(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0xE38);
    }
    if (self->animFlagAndTimer == 0x1000) {
        if ((self->actor.xzDistToPlayer > 480.0f) || (self->actor.xzDistToPlayer < 120.0f)) {
            Animation_MorphToPlayOnce(&self->skelAnime, sEnDekunutsAnimData.unk_00[2], -5.0f);
            Audio_PlayActorSound2(&self->actor, 0x010002ED);
            self->actionFunc = FUN_00176d30;
        } else {
            EnDekunuts_SetupThrowNut(self);
        }
    } else if (self->animFlagAndTimer == 0) {
        EnDekunuts_SetupThrowNut(self);
    }
}

void FUN_001218c4(EnDekunuts* self) {
    f32 step = 1.0f;
    f32 zero = 0.0f;

    Math_StepToF(&self->actor.speedXZ, zero, step);
    if (FUN_003731e0(&self->skelAnime)) {
        Animation_PlayOnce(&self->skelAnime, sEnDekunutsAnimData.unk_00[3]);
        self->actor.speedXZ = zero;
        self->actionFunc = FUN_003d89c8;
        Audio_PlayActorSound2(&self->actor, 0x010002F1);
    }
}

void FUN_003e1bac(EnDekunuts* self) {
    FUN_003731e0(&self->skelAnime);
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f)) {
        if (self->animFlagAndTimer != 0) {
            self->animFlagAndTimer--;
            if (self->animFlagAndTimer != 0) {
                Audio_PlayActorSound2(&self->actor, 0x010002F2);
                return;
            }
        }
        Animation_PlayLoop(&self->skelAnime, sEnDekunutsAnimData.animIdx24);
        self->animFlagAndTimer = 3;
        self->playWalkSfx = false;
        self->collider.base.acFlags |= AC_ON;
        self->actionFunc = FUN_00123ef4;
    }
}

void FUN_00176d30(EnDekunuts* self, PlayState* play) {
    f32 clamped;

    if (FUN_003731e0(&self->skelAnime)) {
        FUN_001e8ac0(self);
    } else {
        if (*EnDekunuts_CurFramePtr(&self->skelAnime) < 1.0f) {
            clamped = 1.0f;
        } else if (*EnDekunuts_CurFramePtr(&self->skelAnime) > 3.0f) {
            clamped = 3.0f;
        } else {
            clamped = *EnDekunuts_CurFramePtr(&self->skelAnime);
        }
        self->collider.dim.height = (3.0f - clamped) * 12.0f;
    }
    if (Animation_OnFrameImpl(&self->skelAnime, 4.0f, 1.0f)) {
        self->collider.base.acFlags &= ~AC_ON;
    }
    Math_ApproachF(&self->actor.world.pos.x, self->actor.home.pos.x, 0.5f, 3.0f);
    Math_ApproachF(&self->actor.world.pos.z, self->actor.home.pos.z, 0.5f, 3.0f);
}

void FUN_0011d56c(EnDekunuts* self, PlayState* play) {
    f32 spawnX;
    f32 spawnY;
    f32 spawnZ;

    FUN_00370084(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0xE38);
    if (FUN_003731e0(&self->skelAnime)) {
        FUN_00258b58(self);
        return;
    }
    if (Animation_OnFrameImpl(&self->skelAnime, 6.0f, 1.0f)) {
        spawnX = self->actor.world.pos.x + (Math_SinS(self->actor.shape.rot.y) * 35.0f);
        spawnY = self->actor.world.pos.y + 12.0f;
        spawnZ = self->actor.world.pos.z + (Math_CosS(self->actor.shape.rot.y) * 35.0f);
        if (Actor_Spawn(&play->actorCtx, play, ACTOR_EN_NUTSBALL, spawnX, spawnY, spawnZ, self->actor.shape.rot.x,
                        self->actor.shape.rot.y, self->actor.shape.rot.z, 0, 1) != NULL) {
            Audio_PlayActorSound2(&self->actor, 0x010002EE);
        }
    } else if ((self->animFlagAndTimer > 1) && Animation_OnFrameImpl(&self->skelAnime, 12.0f, 1.0f)) {
        f32 morph = -3.0f;

        Animation_MorphToPlayOnce(&self->skelAnime, sEnDekunutsAnimData.unk_00[0], morph);
        if (self->animFlagAndTimer != 0) {
            self->animFlagAndTimer--;
        }
    }
}

void FUN_001cb604(EnDekunuts* self) {
    FUN_003731e0(&self->skelAnime);
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) && (self->animFlagAndTimer != 0)) {
        self->animFlagAndTimer--;
    }
    if ((self->actor.xzDistToPlayer < 120.0f) || (self->animFlagAndTimer == 0)) {
        f32 morph = -5.0f;

        Animation_MorphToPlayOnce(&self->skelAnime, sEnDekunutsAnimData.unk_00[2], morph);
        Audio_PlayActorSound2(&self->actor, 0x010002ED);
        self->actionFunc = FUN_00176d30;
    }
}

void FUN_003dce00(EnDekunuts* self) {
    if (FUN_003731e0(&self->skelAnime)) {
        self->runDirection = self->actor.yawTowardsPlayer + 0x8000;
        self->runAwayCount = 3;
        Animation_PlayLoop(&self->skelAnime, sEnDekunutsAnimData.animIdx24);
        self->animFlagAndTimer = 3;
        self->playWalkSfx = false;
        self->collider.base.acFlags |= AC_ON;
        self->actionFunc = FUN_00123ef4;
    }
    FUN_00370084(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0xE38);
}

void FUN_003e0be8(EnDekunuts* self) {
    FUN_003731e0(&self->skelAnime);
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f)) {
        if (self->animFlagAndTimer == 0) {
            FUN_001c82dc(self);
            return;
        }
        self->animFlagAndTimer--;
    }
    if (self->animFlagAndTimer == 0) {
        FUN_001c82dc(self);
    }
}

// FUN_0037896c (limb override, ThrowNut head/neck scale). The action-func literal goes through a local declared inside
// the limb test (gives ROM's predicated ldreq r0 / cmpeq r1,r0).
s32 FUN_0037896c(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg) {
    EnDekunuts* self = (EnDekunuts*)arg;
    f32 z;
    f32 x;
    f32 y;
    f32 curFrame;
    MTX34 scale;

    if (limbIndex == 5) {
        EnDekunutsActionFunc fn = FUN_0011d56c;
        if (self->actionFunc != fn) {
            return 0;
        }
        curFrame = *EnDekunuts_CurFramePtr(&self->skelAnime);
        if (curFrame <= 6.0f) {
            x = 1.0f - (curFrame * 0.0833f);
            z = 1.0f + (curFrame * 0.1167f);
            y = z;
        } else if (curFrame <= 7.0f) {
            curFrame -= 6.0f;
            x = 0.5f + curFrame;
            z = 1.7f - (curFrame * 0.7f);
            y = 1.7f - (curFrame * 0.7f);
        } else if (curFrame <= 10.0f) {
            x = 1.5f - ((curFrame - 7.0f) * 0.1667f);
            z = 1.0f;
            y = z;
        } else {
            return 0;
        }
        f32 d0 = y;
        scale.data[0][1] = 0.0f;
        scale.data[0][2] = 0.0f;
        scale.data[0][3] = 0.0f;
        scale.data[1][0] = 0.0f;
        scale.data[1][2] = 0.0f;
        scale.data[1][3] = 0.0f;
        scale.data[2][0] = 0.0f;
        scale.data[2][1] = 0.0f;
        scale.data[2][3] = 0.0f;
        scale.data[0][0] = d0;
        scale.data[1][1] = x;
        scale.data[2][2] = z;
        FUN_0036c174(mtx, mtx, &scale);
    }
    return 0;
}

// FUN_003d89c8 (EnDekunuts_Die): draft only, not compiled. Blocker: the N64 `static Vec3f effectVelAndAccel` is a
// guarded function-local static here (guard 0x00522028 in the actor data block, object 0x0059908C outside it); the object
// needs a linker pin (LOADER_RECIPE, orchestrator-only). Kept as GLOBAL_ASM in functions_3C0000s.cpp.
#if 0
void FUN_003d89c8(EnDekunuts* self, PlayState* play) {
    static Vec3f effectVelAndAccel = { 0.0f, 0.0f, 0.0f };
    Vec3f effectPos;

    if (FUN_003731e0(&self->skelAnime)) {
        effectPos.x = self->actor.world.pos.x;
        effectPos.y = self->actor.world.pos.y + 18.0f;
        effectPos.z = self->actor.world.pos.z;
        EffectSsDeadDb_Spawn(play, &effectPos, &effectVelAndAccel, &effectVelAndAccel, 200, 0, 255, 255, 255, 255, 150,
                             150, 150, 1, 13, 1);
        effectPos.y = self->actor.world.pos.y + 10.0f;
        EffectSsHahen_SpawnBurst(play, &effectPos, 3.0f, 0, 12, 3, 15, HAHEN_OBJECT_DEFAULT, 10, NULL);
        Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos,
                                   COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_3, false));
        if (self->actor.child != NULL) {
            Actor_ChangeCategory(play, &play->actorCtx, self->actor.child, ACTORCAT_PROP);
        }
        Actor_Kill(&self->actor);
    }
}
#endif
}
