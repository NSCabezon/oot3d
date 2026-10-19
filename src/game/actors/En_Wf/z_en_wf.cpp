extern "C" {
#include "z_en_wf.hpp"
#include "../En_Encount1/z_en_encount1.hpp"

#define FLAGS 0x00000015

void EnWf_Init(Actor* thisx, PlayState* play);
void EnWf_Destroy(Actor* thisx, PlayState* play);
void EnWf_Update(Actor* actor, GameState* state);
void EnWf_Draw(Actor* thisx, PlayState* play);

void Collider_DestroyJntSph(PlayState* play, void* collider);
void FUN_00373d0c(PlayState* play);
void FUN_0034c3e4(SkelAnime* skelAnime, void* anim);
void FUN_0035a49c(SkelAnime* skelAnime, void* anim, f32 morph);
void FUN_0037043c(SkelAnime* skelAnime, f32 endFrame);
void FUN_00353020(SkelAnime* skelAnime, void* anim, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
s32 FUN_00369608(PlayState* play, Actor* actor);
void FUN_00362f48(EnWf* self, PlayState* play);
void FUN_00364938(EnWf* self);
void FUN_00364aa4(EnWf* self);
void FUN_0034c440(EnWf* self, PlayState* play);
void FUN_0013d5d0(EnWf* self, PlayState* play);
void FUN_00138158(EnWf* self, PlayState* play);
void FUN_003dd5c8(EnWf* self, PlayState* play);
void FUN_00173e68(EnWf* self, PlayState* play);

extern u8 EnWf_sAnimRun[];
extern u8 EnWf_sAnimBackflip[];
extern u8 EnWf_sAnimAttack[];
extern u8 EnWf_sAnimBlock[];

GLOBAL_ASM("data/z_En_Wf.data.s")
GLOBAL_ASM("data/z_En_Wf.data_00530280.s")

ActorInit En_Wf_InitVars = {
    ACTOR_EN_WF,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_WF,
    sizeof(EnWf),
    (ActorFunc)EnWf_Init,
    (ActorFunc)EnWf_Destroy,
    (ActorFunc)EnWf_Update,
    (ActorFunc)EnWf_Draw,
};

GLOBAL_ASM("../src/game/actors/En_Wf/data_syms.s")

void FUN_0035b578(EnWf* self) {
    f32 animEndFrame = (f32)Animation_GetLastFrame(&self->skelAnime, 4);

    if (self->attackState != 0) {
        self->attackState = -1;
    }
    self->actor.speedXZ =0.0f;
    self->action = 7;
    self->timer = 15;
    FUN_00353020(&self->skelAnime, EnWf_sAnimBlock, 0.0f, 0.0f, animEndFrame, 3, -4.0f);
    self->actionFunc = FUN_00138158;
}

s32 FUN_00364c18(PlayState* play, EnWf* self, s32 mustPick);
void FUN_003d6230(EnWf* self, PlayState* play);
f32* EnWf_AnimLengthPtr(SkelAnime* skelAnime);
extern u8 EnWf_sAnimHowl[];

void FUN_00157734(EnWf* self, PlayState* play) {
    if (self->actor.bgCheckFlags & 2) {
        self->actor.speedXZ = 0.0f;
    }
    if (self->actor.bgCheckFlags & 1) {
        if (self->actor.speedXZ < 0.0f) {
            self->actor.speedXZ += 0.05f * 2.0f / 3.0f;
        }
        self->unk_300 = 0;
    }
    if ((*(u16*)((u8*)self + 0x11A) == 0) && (self->actor.bgCheckFlags & 1)) {
        if (self->actor.colChkInfo.health != 0) {
            FUN_00364c18(play, self, 1);
            return;
        }
        FUN_0035a49c(&self->skelAnime, EnWf_sAnimHowl, -4.0f);
        self->actor.world.rot.y = self->actor.yawTowardsPlayer;
        if (self->actor.bgCheckFlags & 1) {
            self->actor.speedXZ = -6.0f;
            self->unk_300 = 0;
        } else {
            self->unk_300 = 1;
        }
        self->action = 2;
        self->actor.flags &= ~1;
        self->timer = (s32)*EnWf_AnimLengthPtr(&self->skelAnime);
        Audio_PlayActorSound2(&self->actor, 0x010002BB);
        self->actionFunc = FUN_003d6230;
    }
}

#define R_UPDATE_RATE SREG(30)
void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* pos, s32 params);
void EffectSsDeadDb_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep, s16 primR,
                          s16 primG, s16 primB, s16 primA, s16 envR, s16 envG, s16 envB, s32 unk, s32 life, s32 playSfx);

void FUN_003d6230(EnWf* self, PlayState* play) {
    if (self->actor.bgCheckFlags & 2) {
        self->actor.speedXZ = 0.0f;
    }
    if (self->actor.bgCheckFlags & 1) {
        Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);
        self->unk_300 = 0;
    }
    if (FUN_00370734(&self->skelAnime)) {
        Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos, 0xD0);
        if (self->switchFlag != 0xFF) {
            Flags_SetSwitch(play, self->switchFlag);
        }
        Actor_Kill(&self->actor);
    } else {
        Vec3f pos;
        Vector3f vel(0.0f, 0.5f, 0.0f);
        s32 i;

        self->timer--;
        if ((((s32)*EnWf_AnimLengthPtr(&self->skelAnime) - self->timer) >> 1) > 0) {
            i = (s32)(0.5f + ((f32)(((s32)*EnWf_AnimLengthPtr(&self->skelAnime) - self->timer) >> 1) * (f32)R_UPDATE_RATE) * 0.33333334f);
        } else {
            i = (s32)(((f32)(((s32)*EnWf_AnimLengthPtr(&self->skelAnime) - self->timer) >> 1) * (f32)R_UPDATE_RATE) * 0.33333334f - 0.5f);
        }
        for (; i >= 0; i--) {
            pos.x = Rand_CenteredFloat(60.0f) + self->actor.world.pos.x;
            pos.z = Rand_CenteredFloat(60.0f) + self->actor.world.pos.z;
            pos.y = Rand_CenteredFloat(50.0f) + 20.0f + self->actor.world.pos.y;
            EffectSsDeadDb_Spawn(play, (Vec3f*)&pos, (Vec3f*)&vel, (Vec3f*)&vel, 100, 0, 255, 255, 255, 255, 0, 0, 255, 1, 11, 1);
        }
    }
}

s32 Actor_IsFacingPlayer(Actor* actor, s16 angle);
void FUN_00362de8(EnWf* self);

#define RT(a, b) ((s32)(a + (Rand_ZeroOne() * b)))
#define WAIT_TIMER() self->timer = (RT(5.0f, 5.0f) > 0) \
                      ? (s32)(0.5f + ((f32)(RT(5.0f, 5.0f)) * 3.0f) * 0.5f) \
                      : (s32)(((f32)(RT(5.0f, 5.0f)) * 3.0f) * 0.5f - 0.5f)
void FUN_00126060(EnWf* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 angle1 = player->actor.shape.rot.y - self->actor.shape.rot.y;
    s16 angle2 = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    angle1 = ABS(angle1);
    angle2 = ABS(angle2);
    if (FUN_00370734(&self->skelAnime)) {
        if (!Actor_IsFacingPlayer(&self->actor, 0x1554)) {
            FUN_00364938(self);
            WAIT_TIMER();
            if (angle2 > 0x32C8) {
                self->unk_CC6 = 0x2D;
            }
        } else if ((Rand_ZeroOne() > 0.7f) || (self->actor.xzDistToPlayer >= 120.0f)) {
            FUN_00364938(self);
            WAIT_TIMER();
        } else {
            self->actor.world.rot.y = self->actor.yawTowardsPlayer;
            if (Rand_ZeroOne() > 0.7f) {
                FUN_00362f48(self, play);
            } else if (angle1 > 0x2710) {
                FUN_00362de8(self);
            } else if (angle2 > 0x3E80) {
                self->actor.world.rot.y = self->actor.yawTowardsPlayer;
                FUN_00362de8(self);
            } else {
                FUN_00364c18(play, self, 1);
            }
        }
    }
}

s32 FUN_00364b1c(PlayState* play, EnWf* self);
void FUN_00364a2c(EnWf* self);
void FUN_0017770c(EnWf* self, PlayState* play);

void FUN_00120078(EnWf* self, PlayState* play) {
    s16 angle;

    if (self->actor.bgCheckFlags & 2) {
        self->actor.speedXZ = 0.0f;
    }
    if (self->actor.bgCheckFlags & 1) {
        if (self->actor.speedXZ < 0.0f) {
            self->actor.speedXZ += 0.05f * 2.0f / 3.0f;
        }
        self->unk_300 = 0;
    }
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0x1194, 0);
    if (!FUN_00364c18(play, self, 0)) {
        if (FUN_00370734(&self->skelAnime) && (self->actor.bgCheckFlags & 1)) {
            angle = self->actor.wallYaw - self->actor.shape.rot.y;
            angle = ABS(angle);
            if ((self->actor.bgCheckFlags & 8) && (ABS(angle) < 0x2EE0) && (self->actor.xzDistToPlayer < 120.0f)) {
                FUN_00353020(&self->skelAnime, EnWf_sAnimBackflip, -1.0f,
                             (f32)Animation_GetLastFrame(&self->skelAnime, 3), 0.0f, 2, -3.0f);
                self->timer = 0;
                self->actor.speedXZ = 6.5f;
                self->actor.velocity.y = 15.0f;
                self->unk_300 = 0;
                self->action = 4;
                Audio_PlayActorSound2(&self->actor, 0x010002DC);
                self->actor.world.rot.y = self->actor.shape.rot.y;
                self->actionFunc = FUN_0017770c;
            } else {
                if (FUN_00364b1c(play, self)) {
                    return;
                }
                if (!(self->actor.xzDistToPlayer > ((self->actor.params == 0) ? 77.0f : 80.0f)) &&
                    !FUN_00369608(play, &self->actor) && ((*(u32*)((u8*)play + 0x5BF4) % 12) != 0)) {
                    FUN_00364aa4(self);
                    return;
                }
                if (Rand_ZeroOne() <= 0.5f) {
                    FUN_00364a2c(self);
                    return;
                }
                FUN_00364938(self);
                WAIT_TIMER();
                self->unk_CC6 = 0x2D;
            }
        }
    }
}

void Collider_InitJntSph(PlayState* play, void* collider);
void Collider_SetJntSph(PlayState* play, void* collider, Actor* actor, void* init, void* elements);
extern InitChainEntry EnWf_sInitChain[];
extern u8 EnWf_sJntSphInit[];
extern ColliderCylinderInit EnWf_sBodyInit;
extern ColliderCylinderInit EnWf_sTailInit;
extern DamageTable EnWf_sDamageTable;
void FUN_001c70f0(EnWf* self, PlayState* play);

void EnWf_Init(Actor* thisx, PlayState* play) {
    EnWf* self = (EnWf*)thisx;

    Actor_ProcessInitChain(&self->actor, EnWf_sInitChain);
    self->actor.colChkInfo.damageTable = &EnWf_sDamageTable;
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
    self->actor.focus.pos = self->actor.world.pos;
    self->actor.colChkInfo.mass = MASS_HEAVY;
    self->actor.colChkInfo.health = 8;
    self->actor.colChkInfo.cylRadius = 50;
    self->actor.colChkInfo.cylHeight = 100;
    self->unk_2F4 = 10.0f;
    self->switchFlag = (self->actor.params >> 8) & 0xFF;
    self->actor.params &= 0xFF;
    self->eyeIndex = 0;
    Collider_InitJntSph(play, self->attackAndBlockCollider);
    Collider_SetJntSph(play, self->attackAndBlockCollider, &self->actor, EnWf_sJntSphInit,
                       self->attackAndBlockCollider + 0x20);
    Collider_InitCylinder(play, &self->bodyCollider);
    Collider_SetCylinder(play, &self->bodyCollider, &self->actor, &EnWf_sBodyInit);
    Collider_InitCylinder(play, &self->tailCollider);
    Collider_SetCylinder(play, &self->tailCollider, &self->actor, &EnWf_sTailInit);
    if (self->actor.params == 0) {
        FUN_00372f38(&self->actor, play, 0);
        FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 0x15);
        SkelAnime_SetFaceAnimations(&self->faceAnimations, &self->skelAnime, 0, 2, -1);
        Actor_SetScale(&self->actor, 0.0075f);
        thisx->naviEnemyId = 0x4C;
    } else {
        FUN_00372f38(&self->actor, play, 0);
        FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 0x15);
        SkelAnime_SetFaceAnimations(&self->faceAnimations, &self->skelAnime, 1, 2, -1);
        self->actor.targetArrowOffset += 500.0f;
        Actor_SetScale(&self->actor, 0.01f);
        *(u8*)(*(u32*)(self->attackAndBlockCollider + 0x1C) + 0x55) = 8;
        *(u8*)(*(u32*)(self->attackAndBlockCollider + 0x1C) + 5) = 8;
        thisx->naviEnemyId = 0x57;
    }
    FUN_00353020(&self->skelAnime, EnWf_sAnimHowl, 0.5f, 0.0f, 7.0f, 3, 0.0f);
    self->actor.scale.y = 0.0f;
    self->actor.world.pos.y = self->actor.home.pos.y - 5.0f;
    self->actor.gravity = 0.0f;
    self->timer = 30;
    self->unk_300 = 0;
    self->action = 0;
    thisx->flags &= ~1;
    self->actionFunc = FUN_001c70f0;
    if ((self->switchFlag != 0xFF) && Flags_GetSwitch(play, self->switchFlag)) {
        Actor_Kill(&self->actor);
    }
}

void EnWf_Destroy(Actor* thisx, PlayState* play) {
    EnWf* self = (EnWf*)thisx;
    EnEncount1* parent;

    Collider_DestroyJntSph(play, self->attackAndBlockCollider);
    Collider_DestroyCylinder(play, &self->bodyCollider);
    Collider_DestroyCylinder(play, &self->tailCollider);
    FUN_003508b8(thisx, 0);
    SkelAnime_Free2(&self->skelAnime);
    self->faceAnimations.Destroy();
    if ((self->actor.params != 0) && (self->switchFlag != 0xFF)) {
        FUN_00373d0c(play);
    }
    if (self->actor.parent != NULL) {
        parent = (EnEncount1*)self->actor.parent;
        if (parent->actor.update != NULL) {
            if (parent->curNumSpawn > 0) {
                parent->curNumSpawn--;
            }
        }
    }
}

GLOBAL_ASM("asm/EnWf_Update.s")

s32 EnWf_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnWf_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void FUN_00357fd0(Player* player, void* arg1, Vec3f* pos);
void FUN_0020d318(PlayState* play, Actor* actor, Vec3s* pos, s16 scale, s16 arg4, s16 flags, s16 bodyPart,
                  s32 sfxType);

void EnWf_Draw(Actor* thisx, PlayState* play) {
    EnWf* self = (EnWf*)thisx;

    if ((self->action != 0) || (self->unk_300 == 0)) {
        if (self->actor.scale.y <= 0.0f) {
            return;
        }
        {
            FUN_00357fd0(GET_PLAYER(play), self->actor.unk_178, &self->actor.world.pos);
            u8 eye = self->eyeIndex;
            {
                FaceAnimations* fa = &self->faceAnimations;
                fa->FUN_0035e3a4(0, eye);
            }
            {
                FaceAnimations* fa = &self->faceAnimations;
                fa->FUN_0035e3a4(1, self->actor.params != 0);
            }
            self->faceAnimations.FUN_0035e330();
            SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnWf_OverrideLimbDraw, EnWf_PostLimbDraw,
                              thisx, 0);
            s16 fireTimer = self->onFireTimer;
            if (fireTimer != 0) {
                s32 bodyPartIndex;

                *(u16*)((u8*)self + 0x11A) += 1;
                fireTimer = (s16)(fireTimer - 1);
                self->onFireTimer = fireTimer;
                if ((fireTimer % 6) == 0) {
                    s32 wide = (s32)self->onFireTimer / 6;

                    bodyPartIndex = CLAMP(wide, 0, 10);
                    FUN_0020d318(play, thisx, &self->bodyPartsPos[bodyPartIndex], 75, 0, 0, bodyPartIndex, 1);
                }
            }
        }
    }
}

f32 __hardfp_sinf(f32 x);
f32 __hardfp_cosf(f32 x);

static inline u32 EnWf_GameplayFrames(PlayState* play) {
    return *(u32*)((u8*)play + 0x5BF4);
}

void FUN_0034c128(PlayState* play, Vec3f* pos);

void FUN_0017770c(EnWf* self, PlayState* play) {
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0xFA0, 1);
    if (self->actor.velocity.y >= 5.0f) {
        FUN_0034c128(play, &self->unk_4C8);
        FUN_0034c128(play, &self->unk_4BC);
    }
    if (FUN_00370734(&self->skelAnime) && ((self->actor.bgCheckFlags & 3) != 0)) {
        self->actor.world.rot.y = self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
        self->actor.world.pos.y = self->actor.floorHeight;
        self->actor.velocity.y = 0.0f;
        self->actor.speedXZ = 0.0f;
        self->actor.shape.rot.x = 0;
        if (!FUN_00369608(play, &self->actor)) {
            FUN_00364aa4(self);
        } else {
            FUN_00364938(self);
        }
    }
}

void FUN_00173e68(EnWf* self, PlayState* play) {
    if (FUN_00370734(&self->skelAnime)) {
        if (!FUN_00369608(play, &self->actor) && (self->actor.xzDistToPlayer < 170.0f) &&
            (self->actor.xzDistToPlayer > 140.0f) && (Rand_ZeroOne() < 0.2f)) {
            FUN_0034c440(self, play);
        } else if ((EnWf_GameplayFrames(play) % 2) != 0) {
            FUN_00362f48(self, play);
        } else {
            FUN_00364938(self);
        }
    }
    if ((play->state.frames & 0x5F) == 0) {
        Audio_PlayActorSound2(&self->actor, 0x010002AE);
    }
}

s32 EnWf_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnWf* self = (EnWf*)thisx;

    if ((limbIndex == 0xC) || (limbIndex == 0xB)) {
        f32 t = (f32)self->headRot * (1.0f / 32768.0f);
        f32 p = t * M_PI;
        f32 angle = -p;

        if (angle != 0.0f) {
            f32 s = __hardfp_sinf(angle);
            f32 c = __hardfp_cosf(angle);
            {
                f32 x = mtx->data[0][0];
                f32 z = mtx->data[0][2];
                mtx->data[0][0] = x * c - z * s;
                mtx->data[0][2] = x * s + z * c;
            }
            {
                f32 x = mtx->data[1][0];
                f32 z = mtx->data[1][2];
                mtx->data[1][0] = x * c - z * s;
                mtx->data[1][2] = x * s + z * c;
            }
            {
                f32 x = mtx->data[2][0];
                f32 z = mtx->data[2][2];
                mtx->data[2][0] = x * c - z * s;
                mtx->data[2][2] = x * s + z * c;
            }
        }
    }
    return 0;
}


typedef struct EnWfFireInitParams {
    /* 0x00 */ Actor* actor;
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ s16 scale;
    /* 0x12 */ s16 unk_12;
    /* 0x14 */ s16 flags;
    /* 0x16 */ s16 bodyPart;
} EnWfFireInitParams;

void EffectSs_Spawn(PlayState* play, s32 type, s32 priority, void* initParams);

// Near-miss drafts (2 and 4 words off, see STATUS.md): kept under #if 0, the ROM asm stays linked.
#if 0
void FUN_0020d318(PlayState* play, Actor* actor, Vec3s* pos, s16 scale, s16 arg4, s16 flags, s16 bodyPart,
                  s32 sfxType) {
    EnWfFireInitParams initParams;

    initParams.pos.x = pos->x;
    initParams.pos.y = pos->y;
    initParams.pos.z = pos->z;
    initParams.actor = actor;
    initParams.scale = scale;
    initParams.bodyPart = bodyPart;
    initParams.unk_12 = arg4;
    initParams.flags = flags | 0x8000;

    if ((actor != NULL) && (sfxType != 0)) {
        if (sfxType == 1) {
            Audio_PlayActorSound2(actor, 0x01000179);
        } else if (sfxType == 2) {
            Audio_PlayActorSound2(actor, 0x01000265);
        }
    }
    EffectSs_Spawn(play, 0x1D, 0x80, &initParams);
}
#endif

GLOBAL_ASM("asm/FUN_0020d318.s")

#if 0
// Unmatched near-miss drafts (ROM asm stays linked via GLOBAL_ASM in src/functions/functions_*.cpp). See STATUS.md.

// FUN_00362f48: 9w, VFP numbering of the 5.0/3.0 timer constants (ROM 5.0->s20, 3.0->s19).
void FUN_00362f48(EnWf* self, PlayState* play) {
    s16 angle;

    FUN_00353020(&self->skelAnime, EnWf_sAnimRun, 1.0f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 1), 1,
                 -4.0f);
    angle = GET_PLAYER(play)->actor.shape.rot.y + self->circlePlayerDirection;
    if (Math_SinS(angle - self->actor.yawTowardsPlayer) > 0.0f) {
        self->circlePlayerDirection = 0x3E80;
    } else if (Math_SinS(angle - self->actor.yawTowardsPlayer) < 0.0f) {
        self->circlePlayerDirection = -0x3E80;
    } else {
        if (Rand_ZeroOne() > 0.5f) {
            self->circlePlayerDirection = 0x3E80;
        } else {
            self->circlePlayerDirection = -0x3E80;
        }
    }
    self->actor.speedXZ = 6.0f;
    *EnWf_PlaySpeedPtr(&self->skelAnime) = self->actor.speedXZ * 0.175f;
    s16 yaw = self->actor.shape.rot.y;
    self->actor.world.rot.y = yaw;
    self->circlePlayerExtraSpeed = 0.0f;
    self->timer = (RT(5.0f, 10.0f) > 0)
                      ? (s32)(0.5f + ((f32)(RT(5.0f, 10.0f)) * 3.0f) * 0.5f)
                      : (s32)(((f32)(RT(5.0f, 10.0f)) * 3.0f) * 0.5f - 0.5f);
    self->action = 14;
    self->actionFunc = FUN_00179fe0;
}

// FUN_00362de8: 15w, zero const s21 / 30.0 s19 / 3.0 s18 numbering.
void FUN_00362de8(EnWf* self) {
    FUN_00353020(&self->skelAnime, EnWf_sAnimRun, 1.0f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 1), 1,
                 -4.0f);
    if (Rand_ZeroOne() > 0.5f) {
        self->circlePlayerDirection = 0x3E80;
    } else {
        self->circlePlayerDirection = -0x3E80;
    }
    self->actor.speedXZ = 6.0f;
    *EnWf_PlaySpeedPtr(&self->skelAnime) = self->actor.speedXZ * 0.175f;
    s16 yaw = self->actor.shape.rot.y;
    self->actor.world.rot.y = yaw;
    self->timer = (RT(30.0f, 30.0f) > 0)
                      ? (s32)(0.5f + ((f32)(RT(30.0f, 30.0f)) * 3.0f) * 0.5f)
                      : (s32)(((f32)(RT(30.0f, 30.0f)) * 3.0f) * 0.5f - 0.5f);
    self->circlePlayerExtraSpeed = 0.0f;
    self->action = 11;
    self->actionFunc = FUN_002aa290;
}

// FUN_001c70f0: 3w, ROM keeps timer in r2 (ours r1).
void FUN_001c70f0(EnWf* self, PlayState* play) {
    s32 t = self->timer;
    if (t > 8) {
        self->actor.world.pos.y = self->actor.home.pos.y - 5.0f;
        if (self->actor.xzDistToPlayer < 240.0f) {
            self->timer = 8;
            self->actor.flags |= 1;
            if ((self->actor.params != 0) && (self->switchFlag != 0xFF)) {
                FUN_0034f724(play, self->switchFlag);
                return;
            }
        }
    } else if (t != 0) {
        f32 scaleY = self->actor.scale.y;
        f32 grow = self->actor.scale.x * 0.2f;
        f32 third;

        third = 0.33333334f;
        scaleY += grow * 0.6666667f;
        self->actor.scale.y = scaleY;
        self->actor.world.pos.y += third;
        FUN_0036e168(&self->actor.shape.shadowScale, 70.0f, 1.0f, 14.0f, 0.0f);
        self->timer--;
        if (self->timer == 0) {
            Audio_PlayActorSound2(&self->actor, 0x010002AC);
        }
    } else {
        if (FUN_00370734(&self->skelAnime)) {
            self->actor.scale.y = self->actor.scale.x;
            self->actor.gravity = -2.0f;
            FUN_00364938(self);
        }
    }
}

// FUN_00138158: 10w, r5/r6 swap (player vs play+0x5000 base).
void FUN_00138158(EnWf* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 yawDiff;

    if (self->timer != 0) {
        self->timer--;
    } else {
        *EnWf_PlaySpeedPtr(&self->skelAnime) = 1.0f;
    }
    if (FUN_00370734(&self->skelAnime)) {
        s16 relYaw = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

        if ((ABS(relYaw) <= 0x4000) && (self->actor.xzDistToPlayer < 60.0f) && (ABS(self->actor.yDistToPlayer) < 50.0f)) {
            if (FUN_0035b950(play, &self->actor, 100.0f, 0x2710, 0x4000, self->actor.shape.rot.y)) {
                if (*(u8*)((u8*)player + 0x2226) == 0x11) {
                    FUN_0035b578(self);
                } else if ((*(u32*)((u8*)play + 0x5BF4) & 1) != 0) {
                    FUN_0035b578(self);
                } else {
                    FUN_00364a2c(self);
                }
            } else {
                yawDiff = player->actor.shape.rot.y - self->actor.shape.rot.y;
                if (!FUN_00369608(play, &self->actor) &&
                    (((*(u32*)((u8*)play + 0x5BF4) & 1) != 0) || (ABS(yawDiff) < 0x38E0))) {
                    FUN_00364aa4(self);
                } else {
                    FUN_00362de8(self);
                }
            }
        } else {
            FUN_00362de8(self);
        }
    } else if ((self->timer == 0) && FUN_0035b950(play, &self->actor, 100.0f, 0x2710, 0x4000, self->actor.shape.rot.y)) {
        if (*(u8*)((u8*)player + 0x2226) == 0x11) {
            FUN_0035b578(self);
        } else if ((*(u32*)((u8*)play + 0x5BF4) & 1) != 0) {
            FUN_0035b578(self);
        } else {
            FUN_00364a2c(self);
        }
    }
}

// EnWf_Update: far (size +-1 word, register order play r6/r7, non-predicated 0x11A/0x11C test).
void EnWf_Update(Actor* thisx, PlayState* play) {
    EnWf* self = (EnWf*)thisx;
    s32 pad;

    if (self->attackAndBlockCollider[0x11] & 0x80) {
        self->attackAndBlockCollider[0x11] &= ~0x82;
        self->bodyCollider.base.acFlags &= ~2;
        self->tailCollider.base.acFlags &= ~2;
    } else if ((self->bodyCollider.base.acFlags & 2) || (self->tailCollider.base.acFlags & 2)) {
        if (self->action >= 6) {
            s16 yaw = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

            if ((!(self->bodyCollider.base.acFlags & 2) && (self->tailCollider.base.acFlags & 2)) ||
                ((u32)(yaw + 0x4A38) > 0x9470)) {
                self->actor.colChkInfo.damage <<= 2;
            }
            self->bodyCollider.base.acFlags &= ~2;
            self->tailCollider.base.acFlags &= ~2;
            if (self->actor.colChkInfo.damageEffect != 6) {
                *(u8*)((u8*)self + 0xCCA) = self->actor.colChkInfo.damageEffect;
                FUN_00375fd0(&self->actor, &self->bodyCollider.info, 1);
                self->attackState = 0;
                if ((self->actor.colChkInfo.damageEffect == 1) || (self->actor.colChkInfo.damageEffect == 0xF)) {
                    if (self->action != 0xF) {
                        Actor_SetColorFilter(&self->actor, 0, 0x78, 0, 0x50);
                        Actor_ApplyDamage(&self->actor);
                        if (self->actor.bgCheckFlags & 1) {
                            self->actor.speedXZ = 0.0f;
                        }
                        Audio_PlayActorSound2(&self->actor, 0x0100030E);
                        FUN_003ab92c(&self->skelAnime, (void**)EnWf_sAnimHit, 0.0f);
                        self->actionFunc = FUN_00157734;
                        self->action = 0xF;
                    }
                } else {
                    Actor_SetColorFilter(&self->actor, 0x400000, 0xFF, 0, 8);
                    if (*(u8*)((u8*)self + 0xCCA) == 0xE) {
                        self->onFireTimer = 0x3C;
                    }
                    if (Actor_ApplyDamage(&self->actor) == 0) {
                        FUN_0035a49c(&self->skelAnime, EnWf_sAnimHowl, -4.0f);
                        self->actor.world.rot.y = self->actor.yawTowardsPlayer;
                        if (self->actor.bgCheckFlags & 1) {
                            self->actor.speedXZ = -6.0f;
                            self->unk_300 = 0;
                        } else {
                            self->unk_300 = 1;
                        }
                        self->action = 2;
                        self->actor.flags &= ~1;
                        self->timer = (s32)*EnWf_AnimLengthPtr(&self->skelAnime);
                        Audio_PlayActorSound2(&self->actor, 0x010002BB);
                        self->actionFunc = FUN_003d6230;
                        FUN_00375b70(play, &self->actor);
                    } else {
                        FUN_0035a49c(&self->skelAnime, EnWf_sAnimHit, -4.0f);
                        if (self->actor.bgCheckFlags & 1) {
                            self->actor.speedXZ = -4.0f;
                            self->unk_300 = 0;
                        } else {
                            self->unk_300 = 1;
                        }
                        self->unk_CC6 = 0;
                        self->actor.world.rot.y = self->actor.yawTowardsPlayer;
                        Audio_PlayActorSound2(&self->actor, 0x010002B3);
                        self->actionFunc = FUN_00120078;
                        self->action = 3;
                    }
                }
            }
        }
    }
    if (self->actor.colChkInfo.damageEffect != 6) {
        Actor_MoveForward(&self->actor);
        Actor_UpdateBgCheckInfo(play, &self->actor, 32.0f, 30.0f, 60.0f, 0x1D);
        self->actionFunc(self, play);
        if (self->action == 6) {
            if (self->unk_CC6 != 0) {
                self->headRot = (s16)(Math_SinS(self->unk_CC6 * 0x1068) * 66.0f);
            } else {
                goto smooth;
            }
        } else if (self->action != 0xF) {
            if (self->action == 8) {
                self->headRot = 0;
            } else {
            smooth:
                Math_SmoothStepToS(&self->headRot, self->actor.yawTowardsPlayer - self->actor.shape.rot.y, 1, 0x5DC, 0);
                self->headRot = CLAMP(self->headRot, -0x3127, 0x3127);
            }
        }
    }
    if (self->actor.bgCheckFlags & 3) {
        FUN_0035e7b4(&self->actor, self->actor.shape.rot.y, &self->actor.shape);
    } else {
        Math_SmoothStepToS(&self->actor.shape.rot.x, 0, 1, 0x3E8, 0);
        Math_SmoothStepToS(&self->actor.shape.rot.z, 0, 1, 0x3E8, 0);
    }
    CollisionCheck_SetOC(play, COLCTX, (Collider*)self->attackAndBlockCollider);
    if (self->action >= 6) {
        if ((*(u16*)((u8*)self + 0x11A) == 0) || !(*(u32*)((u8*)self + 0x11C) & 0x400000)) {
            Collider_UpdateCylinder(&self->actor, &self->bodyCollider);
            CollisionCheck_SetAC(play, COLCTX, (Collider*)&self->tailCollider);
            CollisionCheck_SetAC(play, COLCTX, (Collider*)&self->bodyCollider);
        }
    }
    if (self->action == 7) {
        CollisionCheck_SetAC(play, COLCTX, (Collider*)self->attackAndBlockCollider);
    }
    if (self->attackState > 0) {
        if (!(self->attackAndBlockCollider[0x10] & 4)) {
            CollisionCheck_SetAT(play, COLCTX, (Collider*)self->attackAndBlockCollider);
        } else {
            f32 one = 1.0f;
            f32 endFrame = one;

            if ((s32)*EnWf_CurFramePtr(&self->skelAnime) > 0xF) {
                endFrame = 15.0f;
            }
            FUN_00353020(&self->skelAnime, EnWf_sAnimAttack, -0.5f, *EnWf_CurFramePtr(&self->skelAnime) - one, endFrame, 3, 0.0f);
            self->action = 0xC;
            self->attackState = 0;
            self->actionFunc = FUN_00126060;
        }
    }
    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += 25.0f;
    if (self->eyeIndex != 0) {
        self->eyeIndex = (self->eyeIndex + 1) & 3;
    } else if ((Rand_ZeroOne() < 0.2f) && ((*(u32*)((u8*)play + 0x5BF4) % 6) == 0) && (*(u16*)((u8*)self + 0x11A) == 0)) {
        self->eyeIndex++;
    }
}

// FUN_0019ce2c (EnWf_Idle): far, 3DS rounding of yaw step.
void FUN_0019ce2c(EnWf* self, PlayState* play) {
    s16 yawDiff;
    s16 step;
    f32 playSpeed;

    if (FUN_00364b1c(play, self)) {
        return;
    }
    if (FUN_00364c18(play, self, 0)) {
        return;
    }
    yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    if (yawDiff > 0) {
        step = (s16)(s32)(2000.0f + (f32)yawDiff * 0.25f);
    } else {
        step = (s16)(s32)((f32)yawDiff * 0.25f - 2000.0f);
    }
    if (step > 0) {
        self->actor.shape.rot.y += (s32)(0.5f + ((f32)step * 2.0f) * 0.33333334f);
    } else {
        self->actor.shape.rot.y += (s32)(((f32)step * 2.0f) * 0.33333334f - 0.5f);
    }
    self->actor.world.rot.y = self->actor.shape.rot.y;
    playSpeed = (f32)step * 0.5f;
    if (yawDiff > 0) {
        if (playSpeed > 1.0f) {
            playSpeed = 1.0f;
        }
    } else if (playSpeed < -1.0f) {
        playSpeed = -1.0f;
    }
    *EnWf_PlaySpeedPtr(&self->skelAnime) = -playSpeed;
    FUN_00370734(&self->skelAnime);
    if (Actor_IsFacingPlayer(&self->actor, 0x1555)) {
        if (Rand_ZeroOne() > 0.8f) {
            FUN_00362de8(self);
        } else {
            FUN_00353020(&self->skelAnime, EnWf_sAnimRun, 1.0f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 1), 1, -4.0f);
            self->action = 9;
            self->actionFunc = FUN_0013d5d0;
        }
    }
    if ((*(u32*)((u8*)play + 0x5BF4) & 0x5F) == 0) {
        Audio_PlayActorSound2(&self->actor, 0x010002AE);
    }
}
#endif

}
