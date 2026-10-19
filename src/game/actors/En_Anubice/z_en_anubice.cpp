extern "C" {
#include "z_en_anubice.hpp"
#include "../En_Anubice_Tag/z_en_anubice_tag.hpp"
#include "../Bg_Hidan_Curtain/z_bg_hidan_curtain.hpp"
#include <math.h>

#define FLAGS 0x00000015

void EnAnubice_Init(Actor* actor, GameState* state);
void EnAnubice_Destroy(Actor* actor, GameState* state);
void EnAnubice_Update(Actor* actor, GameState* state);
void EnAnubice_Draw(Actor* actor, GameState* state);
s32 FUN_002a94f4(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void FUN_00378924(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);

GLOBAL_ASM("data/z_En_Anubice.data.s")

GLOBAL_ASM("../src/game/actors/En_Anubice/data_syms.s")

ActorInit En_Anubice_InitVars = {
    ACTOR_EN_ANUBICE,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_ANUBICE,
    sizeof(EnAnubice),
    (ActorFunc)EnAnubice_Init,
    (ActorFunc)EnAnubice_Destroy,
    (ActorFunc)EnAnubice_Update,
    (ActorFunc)EnAnubice_Draw,
};

void FUN_003e4e28(EnAnubice* self, PlayState* play);
extern ColliderCylinderInit EnAnubice_sCylinderInit;
extern DamageTable EnAnubice_sDamageTable;

void EnAnubice_Init(Actor* actor, GameState* state) {
    EnAnubice* self = (EnAnubice*)actor;
    PlayState* play = (PlayState*)state;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 20.0f);
    FUN_00372f38(actor, play, 0);
    FUN_00353c9c(actor, play, &self->skelAnime, 0, 6, self->jointTable, self->morphTable, 10);
    self->actor.naviEnemyId = 0x3A;
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, actor, &EnAnubice_sCylinderInit);
    Actor_SetScale(actor, 0.015f);
    self->actor.colChkInfo.damageTable = &EnAnubice_sDamageTable;
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    self->actor.shape.yOffset = -4230.0f;
    self->focusHeightOffset = 0.0f;
    self->actor.flags &= ~1;
    self->home = self->actor.world.pos;
    self->actor.targetMode = 3;
    self->actionFunc = FUN_003e4e28;
    self->actor.unk_19B = 4;
}



void EnAnubice_Destroy(Actor* actor, GameState* state) {
    EnAnubice* self = (EnAnubice*)actor;
    PlayState* play = (PlayState*)state;
    EnAnubiceTag* tag;

    FUN_003508b8(actor, NULL);
    SkelAnime_Free2(&self->skelAnime);
    Collider_DestroyCylinder(play, &self->collider);

    if (self->actor.params != 0) {
        tag = (EnAnubiceTag*)self->actor.parent;
        if (tag != NULL && tag->actor.update != NULL) {
            tag->anubis = NULL;
        }
    }
}

void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
s32 FUN_0034c3b8(void* p, Vec3f* pos, f32 radius);
void FUN_003d5a38(EnAnubice* self, PlayState* play);

// N64 EnAnubice_SetupDie
void FUN_001746d4(EnAnubice* self, PlayState* play) {
    f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 1);

    self->animLastFrame = lastFrame;
    Animation_Change_00375c08(&self->skelAnime, 1, 1.0f, 0.0f, lastFrame, 2, -20.0f);
    self->actor.velocity.x = self->actor.velocity.z = 0.0f;
    self->actor.gravity = -1.0f;
    self->isNearWall = 0;
    self->fallTargetYaw = 0;
    self->deathTimer = 30;

    if (FUN_0034c3b8((u8*)play + 0xA98, &self->headPos, 70.0f) != 0) {
        self->isNearWall = 1;
        self->fallTargetYaw = self->actor.shape.rot.x - 0x7F00;
    }

    self->actionFunc = FUN_003d5a38;
}

void FUN_00375b70(PlayState* play, Actor* actor);
void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);
void FUN_003741e4(PlayState* play, u32 dmgFlags, s32 idx, Vec3f* pos, s32 zero);
void FUN_003d5a38(EnAnubice* self, PlayState* play);

// Draft, 35 instr-lines off vs ROM (fastchk cascades): 10.0f/0.1f hoisted to entry (ROM reloads), sl/sb swap (acHitInfo vs sound const),
// flameCircle ptr in r2 not r1, Vec3s->Vec3f hitPos conversion load order. Stack layout/knockback consts match.
#if 0
// N64 EnAnubice_Hover
static inline void EnAnubice_Hover(EnAnubice* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    self->hoverVelocityTimer += 1500.0f;
    self->targetHeight = player->actor.world.pos.y + self->playerHeightOffset;
    Math_ApproachF(&self->actor.world.pos.y, self->targetHeight, 0.1f, 10.0f);
    Math_ApproachF(&self->playerHeightOffset, 10.0f, 0.1f, 0.5f);
    self->actor.velocity.y = Math_SinS((s16)self->hoverVelocityTimer);
}

void EnAnubice_Update(Actor* thisx, GameState* state) {
    f32 zero;
    BgHidanCurtain* flameCircle;
    s32 i;
    Vec3f baseKnockbackVelocity;
    Vec3f rotatedKnockbackVelocity;
    Vec3f hitPos;
    MTX34 mtx;
    EnAnubice* self = (EnAnubice*)thisx;
    PlayState* play = (PlayState*)state;

    if ((self->actionFunc != FUN_001746d4) && (self->actionFunc != FUN_003d5a38) && (self->actor.shape.yOffset == 0.0f)) {
        EnAnubice_Hover(self, play);
        for (i = 0; i < 5; i++) {
            flameCircle = self->flameCircles[i];

            if ((flameCircle != NULL) && (fabsf(flameCircle->actor.world.pos.x - self->actor.world.pos.x) < 60.0f) &&
                (fabsf(self->flameCircles[i]->actor.world.pos.z - self->actor.world.pos.z) < 60.0f) &&
                (self->flameCircles[i]->timer != 0)) {
                Actor_ChangeType(play, &play->actorCtx, &self->actor, 6);
                self->actor.flags &= ~1;
                FUN_00375b70(play, &self->actor);
                Audio_PlayActorSound2(&self->actor, 0x0100040F);
                self->actionFunc = FUN_001746d4;
                return;
            }
        }

        if (self->collider.base.acFlags & 2) {
            ColliderInfo* acHitInfo;

            self->collider.base.acFlags &= ~2;
            Vec3s* hp = &((EnAnubice*)thisx)->collider.info.bumper.hitPos;
            hitPos.x = hp->x;
            acHitInfo = self->collider.info.acHitInfo;
            hitPos.y = hp->y;
            hitPos.z = hp->z;
            if (self->actor.colChkInfo.damageEffect == 2) {
                Actor_ChangeType(play, &play->actorCtx, &self->actor, 6);
                self->actor.flags &= ~1;
                FUN_00375b70(play, &self->actor);
                Audio_PlayActorSound2(&self->actor, 0x0100040F);
                self->actionFunc = FUN_001746d4;
                FUN_003741e4(play, acHitInfo->toucher.dmgFlags, 0, &hitPos, 0);
                return;
            }

            if (!self->isKnockedback) {
                self->knockbackTimer = 15;
                self->isKnockedback = 1;

                baseKnockbackVelocity.x = baseKnockbackVelocity.y = rotatedKnockbackVelocity.x = rotatedKnockbackVelocity.y = rotatedKnockbackVelocity.z = 0.0f;
                baseKnockbackVelocity.z = -10.0f;

                Matrix_RotateY(&mtx, 0, self->actor.shape.rot.y * 3.0517578E-5f * 3.14159274f);
                VEC3TransformAsm((Vector3f*)&rotatedKnockbackVelocity, &mtx, (Vector3f*)&baseKnockbackVelocity);

                self->actor.velocity.x = rotatedKnockbackVelocity.x;
                self->actor.velocity.z = rotatedKnockbackVelocity.z;
                self->knockbackRecoveryVelocity.x = -rotatedKnockbackVelocity.x;
                self->knockbackRecoveryVelocity.z = -rotatedKnockbackVelocity.z;

                Audio_PlayActorSound2(&self->actor, 0x010003AB);
                FUN_003741e4(play, acHitInfo->toucher.dmgFlags, 1, &hitPos, 0);
            }
        }

        if (self->isKnockedback) {
            self->actor.shape.rot.y += 6500;
            Math_ApproachF(&self->actor.velocity.x, self->knockbackRecoveryVelocity.x, 0.3f, 1.0f);
            Math_ApproachF(&self->actor.velocity.z, self->knockbackRecoveryVelocity.z, 0.3f, 1.0f);

            zero = 0.0f;
            if (zero) {}

            if (self->knockbackTimer == 0) {
                self->actor.velocity.x = self->actor.velocity.z = 0.0f;
                self->knockbackRecoveryVelocity.x = self->knockbackRecoveryVelocity.z = 0.0f;
                self->isKnockedback = 0;
            }
        }
    }

    self->timeAlive++;

    if (self->knockbackTimer != 0) {
        self->knockbackTimer--;
    }

    if (self->deathTimer != 0) {
        self->deathTimer--;
    }

    self->actionFunc(self, play);

    self->actor.velocity.y += self->actor.gravity;
    FUN_0036b96c(&self->actor);

    if (!self->isPlayerOutOfRange) {
        Actor_UpdateBgCheckInfo(play, &self->actor, 5.0f, 5.0f, 10.0f, 0x1D);
    } else {
        Actor_UpdateBgCheckInfo(play, &self->actor, 5.0f, 5.0f, 10.0f, 0x1C);
    }

    if ((self->actionFunc != FUN_001746d4) && (self->actionFunc != FUN_003d5a38)) {
        Actor_SetFocus(&self->actor, self->focusHeightOffset);
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);

        if (!self->isKnockedback && (self->actor.shape.yOffset == 0.0f)) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
        }
    }
}

#endif
GLOBAL_ASM("asm/EnAnubice_Update.s")

// N64 EnAnubice_OverrideLimbDraw: the head pitch is applied as a Z rotation of the first two columns.
s32 FUN_002a94f4(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnAnubice* self = (EnAnubice*)thisx;

    if (limbIndex == 1) {
        f32 pitch = self->headPitch;
        f32 angle = pitch * 9.58738E-5f;

        if (angle != 0.0f) {
            f32 sinAngle = sinf(angle);
            f32 cosAngle = cosf(angle);
            s32 i;

            for (i = 0; i < 3; i++) {
                f32 x = mtx->data[i][0];
                f32 y = mtx->data[i][1];

                mtx->data[i][0] = x * cosAngle + y * sinAngle;
                mtx->data[i][1] = y * cosAngle - x * sinAngle;
            }
        }
    }
    return 0;
}

// N64 Matrix_MultVec3f(src, dest) wrapper: the N64 argument order keeps the limb check predicated like the ROM.
static inline void EnAnubice_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

// N64 EnAnubice_PostLimbDraw (eyes): transform the head position.
void FUN_00378924(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnAnubice* self = (EnAnubice*)thisx;
    Vector3f pos(0.0f, 0.0f, 0.0f);

    if (limbIndex == 1) {
        EnAnubice_MultVec3f(&pos, (Vector3f*)&self->headPos, mtx);
    }
}

void EnAnubice_Draw(Actor* actor, GameState* state) {
    EnAnubice* self = (EnAnubice*)actor;

    FUN_00357fd0(GET_PLAYER((PlayState*)state), self->actor.unk_178, (Vec3f*)&self->actor.world.pos);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, FUN_002a94f4, FUN_00378924, actor, 0);
}
}
