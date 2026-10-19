extern "C" {
#include "z_en_zf.hpp"

#define FLAGS 0x00000015

void EnZf_Init(Actor* actor, PlayState* state);
void EnZf_Destroy(Actor* thisx, PlayState* play);
void EnZf_Update(Actor* actor, GameState* state);
void EnZf_Draw(Actor* actor, PlayState* state);
s32 EnZf_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void FUN_002b5000(EnZf* self, PlayState* play);
void FUN_002b4b40(EnZf* self, PlayState* play);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
extern s32 EnZf_sPairedAnims[];
void FUN_00295fec(EnZf* self, PlayState* play);
void FUN_0024f78c(EnZf* self, PlayState* play);
s16 FUN_00373fa4(Vec3f* pos, s16 preferred);
s16 FUN_003262b8(Vec3f* pos, s16 cur, s16 home, PlayState* play);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void FUN_0032ffc0(EnZf* self, PlayState* play);
s32 FUN_00328cac(PlayState* play, EnZf* self);
void FUN_0034eb00(EnZf* self);
void FUN_003301b8(EnZf* self);
s32 FUN_003731e0(SkelAnime* skelAnime);
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);
void Animation_MorphToPlayOnce(SkelAnime* skelAnime, s32 idx, f32 morph);

GLOBAL_ASM("data/z_En_Zf.data.s")
GLOBAL_ASM("../src/game/actors/En_Zf/data_syms.s")

ActorInit En_Zf_InitVars = {
    ACTOR_EN_ZF,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_ZF,
    sizeof(EnZf),
    (ActorFunc)EnZf_Init,
    (ActorFunc)EnZf_Destroy,
    (ActorFunc)EnZf_Update,
    (ActorFunc)EnZf_Draw,
};

void FUN_0038894c(EnZf* self, PlayState* play);
void FUN_0034eb00(EnZf* self);
extern s16 EnZf_sPairedFlags[];
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);

Actor* Actor_FindNearby(PlayState* play, Actor* actor, s16 actorId, u8 actorCategory, f32 maxDist);
s32 Effect_Add(PlayState* play, s32* pIndex, s32 type, u8 arg3, u8 arg4, void* initParams);
void ActorShadow_DrawFeet(Actor* actor, Lights* lights, PlayState* play);
void Collider_InitQuad(PlayState* play, void* collider);
void Collider_SetQuad(PlayState* play, void* collider, Actor* actor, void* init);
void Actor_Kill(Actor* actor);

extern InitChainEntry EnZf_sInitChain[];
extern DamageTable EnZf_sDamageTable;
extern ColliderCylinderInit EnZf_sBodyCylinderInit;
extern u8 EnZf_sSwordColliderQuadInit[];

// 3DS EffectBlure init params (N64 EffectBlureInit1). The vertex array element has an
// out-of-line empty constructor in ROM (FUN_003fd780, _ZN15BlureInitVertexC1Ev in shared_syms.s).
struct BlureInitVertex {
    BlureInitVertex();
    u8 data[0x24];
};

typedef struct {
    /* 0x000 */ s32 unk_00;
    /* 0x004 */ BlureInitVertex vtx[16];
    /* 0x244 */ u8 p1StartColor[4];
    /* 0x248 */ u8 p2StartColor[4];
    /* 0x24C */ u8 p1EndColor[4];
    /* 0x250 */ u8 p2EndColor[4];
    /* 0x254 */ s32 elemDuration;
    /* 0x258 */ s32 unkFlag;
    /* 0x25C */ u8 calcMode;
    /* 0x260 */ s32 unk_260;
    /* 0x264 */ u8 unk_264;
} EnZfBlureInit; // size = 0x268

static inline void EnZf_SetupWaitAppear(EnZf* self) {
    Animation_Change_00375c08(&self->skelAnime, 2, 0.0f, 9.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 2), 0, 0.0f);
    self->actor.world.pos.y = self->actor.floorHeight + 300.0f;
    self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    self->actor.shape.shadowAlpha = 0;
    self->alpha = 0;
    self->action = 0;
    self->timer = 15;
    self->actionState = 1;
    self->actor.bgCheckFlags &= ~2;
    self->actor.flags &= ~1;
    self->actionFunc = FUN_0038894c;
}

void EnZf_Init(Actor* thisx, PlayState* play) {
    EnZf* self = (EnZf*)thisx;
    Player* player = GET_PLAYER(play);
    EnZfBlureInit swordBlure;
    f32 yDiff;

    Actor_ProcessInitChain(&self->actor, EnZf_sInitChain);
    self->actor.targetMode = 3;
    self->switchFlag = (self->actor.params & 0xFF00) >> 8;
    self->actor.params = self->actor.params & 0xFF;
    if (self->actor.params & 0x80) {
        self->actor.params |= 0xFF00;
    }
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawFeet, 90.0f);
    self->unk_A4C = 0;
    self->actor.colChkInfo.mass = 0xFE;
    self->actor.colChkInfo.damageTable = &EnZf_sDamageTable;
    swordBlure.p1StartColor[0] = swordBlure.p1StartColor[1] = swordBlure.p1StartColor[2] = swordBlure.p1StartColor[3] =
        swordBlure.p2StartColor[0] = swordBlure.p2StartColor[1] = swordBlure.p2StartColor[2] =
            swordBlure.p1EndColor[0] = swordBlure.p1EndColor[1] = swordBlure.p1EndColor[2] = swordBlure.p2EndColor[0] =
                swordBlure.p2EndColor[1] = swordBlure.p2EndColor[2] = 255;
    swordBlure.p2StartColor[3] = 64;
    swordBlure.p1EndColor[3] = 0;
    swordBlure.p2EndColor[3] = 0;
    swordBlure.elemDuration = 8;
    swordBlure.unkFlag = 0;
    swordBlure.calcMode = 2;
    swordBlure.unk_264 = 0x17;
    Effect_Add(play, &self->swordBlureEffectIndex, 1, 0, 0, &swordBlure);
    Actor_UpdateBgCheckInfo(play, &self->actor, 75.0f, 45.0f, 45.0f, 0x1D);
    self->alpha = 255;
    self->actor.colChkInfo.cylRadius = 40;
    self->actor.colChkInfo.cylHeight = 100;
    { ColliderCylinder* c = &self->bodyCollider; PlayState* play2 = play; Collider_InitCylinder(play2, c); }
    Collider_SetCylinder(play, &self->bodyCollider, &self->actor, &EnZf_sBodyCylinderInit);
    Collider_InitQuad(play, &self->swordCollider);
    Collider_SetQuad(play, &self->swordCollider, &self->actor, EnZf_sSwordColliderQuadInit);
    if (self->actor.params == -2) {
        FUN_00372f38(&self->actor, play, &self->unk_BE0, 1, 0);
        self->actor.colChkInfo.health = 12;
        self->actor.naviEnemyId = 0x10;
        FUN_00353c9c(&self->actor, play, &self->skelAnime, 1, 4, (u8*)self + 0x228, (u8*)self + 0x638, 20);
    } else {
        FUN_00372f38(&self->actor, play, &self->unk_BE0, 0, 0);
        self->actor.colChkInfo.health = 6;
        self->actor.naviEnemyId = 0xF;
        FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 4, (u8*)self + 0x228, (u8*)self + 0x638, 20);
        self->isSwordSheathed = 0;
    }
    self->actor.unk_19B = 4;
    if (self->actor.params < 0) {
        self->homePlatform = self->curPlatform = -1;
        EnZf_sPairedFlags[1] = -1;
        self->actionState = 1;
        if (self->actor.params == -1) {
            EnZf_SetupWaitAppear(self);
        } else {
            FUN_0034eb00(self);
        }
    } else {
        yDiff = player->actor.world.pos.y - self->actor.world.pos.y;
        if ((ABS(yDiff) <= 100.0f) && !Flags_GetSwitch(play, self->switchFlag)) {
            self->homePlatform = self->curPlatform = FUN_00373fa4(&self->actor.world.pos, 0);
            EnZf_SetupWaitAppear(self);
            EnZf_sPairedFlags[1] = 1;
        } else {
            Actor_Kill(thisx);
        }
    }
}

Actor* Actor_FindNearby(PlayState* play, Actor* actor, s16 actorId, u8 actorCategory, f32 maxDist);
void Effect_Delete(PlayState* play, s32 index);
void FUN_00373d0c(PlayState* play);
void FUN_0034eaf8(PlayState* play, void* collider);

void EnZf_Destroy(Actor* thisx, PlayState* play) {
    EnZf* self = (EnZf*)thisx;

    FUN_003508b8(thisx, self->unk_BE0, 0);
    FUN_003508b8(thisx, self->unk_BE4, 0);
    SkelAnime_Free2(&self->skelAnime);
    if ((self->actor.params >= 0) &&
        (Actor_FindNearby(play, &self->actor, ACTOR_EN_ZF, ACTORCAT_ENEMY, 10000.0f) == NULL)) {
        FUN_00373d0c(play);
    }
    Effect_Delete(play, self->swordBlureEffectIndex);
    Collider_DestroyCylinder(play, &self->bodyCollider);
    FUN_0034eaf8(play, &self->swordCollider);
}

GLOBAL_ASM("asm/EnZf_Update.s")

void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);
void FUN_0037266c(SkeletonAnimationModel* model, s32 idx);
void* FUN_003687a8(SkeletonAnimationModel* model);
void FUN_00357a28(void* p, s32 idx, f32* color);
void FUN_00358964(void* p, s32 idx, f32* color);
void FUN_003589cc(void* p, s32 idx);
void EffectSsEnIce_SpawnFlyingVec3f(PlayState* play, Actor* actor, Vec3f* pos, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, f32 scale);
void EnZf_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void GraphicsContext_UnknownStub(GraphicsContext*);

void EnZf_Draw(Actor* thisx, PlayState* play) {
    EnZf* self = (EnZf*)thisx;
    void* p;

    FUN_00357fd0(GET_PLAYER(play), thisx->unk_178, &thisx->world.pos);
    if (self->actor.params == -2 || self->isSwordSheathed == 0) {
        FUN_0037266c(self->skelAnime.unk_28, 3);
        FUN_0037266c(self->skelAnime.unk_28, 7);
        FUN_0037266c(self->skelAnime.unk_28, 5);
        FUN_0036932c(self->skelAnime.unk_28, 4);
        FUN_0036932c(self->skelAnime.unk_28, 6);
    } else {
        FUN_0036932c(self->skelAnime.unk_28, 3);
        FUN_0036932c(self->skelAnime.unk_28, 7);
        FUN_0036932c(self->skelAnime.unk_28, 5);
        FUN_0037266c(self->skelAnime.unk_28, 4);
        FUN_0037266c(self->skelAnime.unk_28, 6);
    }
    if (self->alpha == 255) {
        f32 color[4];
        GraphicsContext_UnknownStub(play->state.gfxCtx);
        p = FUN_003687a8(self->skelAnime.unk_28);
        FUN_00357a28(p, 5, color);
        color[3] = 1.0f;
        FUN_00358964(p, 5, color);
        SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnZf_OverrideLimbDraw, EnZf_PostLimbDraw, thisx, 0);
        {
            s16 timer = self->frozenTimer;
            if (timer != 0) {
                *(u16*)((u8*)self + 0x11A) += 1;
                timer = timer - 1;
                self->frozenTimer = timer;
                if ((timer & 3) == 0) {
                    s32 idx = timer >> 2;
                    EffectSsEnIce_SpawnFlyingVec3f(play, thisx, &self->bodyPartsPos[idx], 150, 150, 150, 250, 235, 245, 255, 1.4f);
                }
            }
        }
    } else {
        GraphicsContext_UnknownStub(play->state.gfxCtx);
        f32 col[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        col[3] = (f32)self->alpha * (1.0f / 255.0f);
        p = FUN_003687a8(self->skelAnime.unk_28);
        FUN_003589cc(p, 5);
        FUN_00358964(p, 5, col);
        SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnZf_OverrideLimbDraw, EnZf_PostLimbDraw, thisx, 0);
    }
}

void FUN_0034df48(MTX34* mtx, Vec3s* rot);

s32 EnZf_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnZf* self = (EnZf*)thisx;

    if (limbIndex == 0xF) {
        Vec3s rot;

        rot.x = -self->headRot;
        rot.y = 0;
        rot.z = 0;
        FUN_0034df48(mtx, &rot);
    }
    return 0;
}

GLOBAL_ASM("asm/EnZf_PostLimbDraw.s")

void FUN_00320e28(EnZf* self) {
    self->actionState = 0;
    Animation_MorphToPlayOnce(&self->skelAnime, EnZf_sPairedAnims[0], -4.0f);
    self->action = 0x14;
    self->actor.speedXZ = 0.0f;
    self->dodgeVelocityY = 0.0f;
    self->dodgeSpeed = 0.0f;
    self->actionFunc = FUN_002b5000;
}

void FUN_00328d50(EnZf* self) {
    Animation_Change_00375c08(&self->skelAnime, 2, 1.0f, 0.0f, 3.0f, 2, 0.0f);
    self->actor.velocity.y = 22.0f;
    self->timer = 0;
    self->actionState = 1;
    self->actor.speedXZ = 7.5f;
    self->action = 0x16;
    Audio_PlayActorSound2(&self->actor, 0x0100029f);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->actionFunc = FUN_002b4b40;
}

void FUN_0032fb3c(EnZf* self, PlayState* play) {
    Animation_MorphToLoop(&self->skelAnime, 0, -4.0f);
    self->action = 5;
    if (self->actor.params >= 0) {
        self->curPlatform = FUN_00373fa4(&self->actor.world.pos, self->curPlatform);
        self->targetPlatform = FUN_003262b8(&self->actor.world.pos, self->curPlatform, self->homePlatform, play);
        self->actionState = 0;
    }
    self->actor.speedXZ = 0.0f;
    self->actionFunc = FUN_00295fec;
}


void FUN_0034c128(PlayState* play, Vec3f* pos);
void FUN_0024fa94(EnZf* self, PlayState* play);

s32 FUN_00369608(PlayState* play, Actor* actor);

void FUN_002a7124(EnZf* self, PlayState* play) {
    s16 yawTowardsPlayer = self->actor.yawTowardsPlayer;

    if (self->skelAnime.curFrame >= 26.0f) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, yawTowardsPlayer, 1, 0x1770, 0);
    }
    if (FUN_003731e0(&self->skelAnime)) {
        Audio_PlayActorSound2(&self->actor, 0x01000299);
        self->actor.world.rot.y = yawTowardsPlayer;
        self->actionState = -1;
        FUN_0034eb00(self);
    }
    if (Animation_OnFrameImpl(&self->skelAnime, 29.0f, 1.0f)) {
        self->isSwordSheathed = 0;
    }
}

extern s16 EnZf_sPairedFlags[];
void FUN_002b53c4(EnZf* self, PlayState* play);
void Actor_ChangeType(PlayState* play, ActorContext* actorCtx, Actor* actor, u8 actorCategory);

void FUN_0024f670(EnZf* self, PlayState* play) {
    if (self->actor.bgCheckFlags & 2) {
        self->actor.speedXZ = 0.0f;
    }
    if (self->actor.bgCheckFlags & 1) {
        Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.15f, 0.0f);
        self->actionState = 0;
    }
    if (FUN_003731e0(&self->skelAnime)) {
        if (self->actor.type != ACTORCAT_PROP) {
            if ((self->actor.params >= 0) && (EnZf_sPairedFlags[1] == -1)) {
                Flags_SetSwitch(play, self->switchFlag);
                FUN_00373d0c(play);
            } else {
                EnZf_sPairedFlags[1] = -1;
            }
            Actor_ChangeType(play, &play->actorCtx, &self->actor, ACTORCAT_PROP);
        }
        if (self->alpha <= 0) {
            self->alpha = 0;
            Actor_Kill(&self->actor);
            return;
        }
        self->alpha -= 5;
        self->actor.shape.shadowAlpha = self->alpha;
    } else {
        if (((s32)self->skelAnime.curFrame == 15) || ((s32)self->skelAnime.curFrame == 30)) {
            Audio_PlayActorSound2(&self->actor, 0x010002EA);
        }
    }
}

void FUN_002b4b40(EnZf* self, PlayState* play) {
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0xFA0, 1);
    if (self->actor.velocity.y >= 5.0f) {
        FUN_0034c128(play, &self->leftFootPos);
        FUN_0034c128(play, &self->rightFootPos);
    }
    if (FUN_003731e0(&self->skelAnime)) {
        if (self->timer == 0) {
            Animation_Change_00375c08(&self->skelAnime, 3, 3.0f, 0.0f, 13.0f, 2, -4.0f);
            self->timer = 15;
        } else if (self->actor.bgCheckFlags & 3) {
            self->actor.world.rot.y = self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
            self->actor.world.pos.y = self->actor.floorHeight;
            self->actor.velocity.y = 0.0f;
            self->actor.speedXZ = 0.0f;
            FUN_003301b8(self);
            Audio_PlayActorSound2(&self->actor, 0x0100029A);
            self->skelAnime.curFrame = 13.0f;
        }
    }
}

void FUN_00328c18(EnZf* self, PlayState* play) {
    s16 absRelYawTowardsPlayer;

    absRelYawTowardsPlayer = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    absRelYawTowardsPlayer = ABS(absRelYawTowardsPlayer);
    if (absRelYawTowardsPlayer >= 0x1B58) {
        FUN_0032ffc0(self, play);
    } else if ((self->actor.xzDistToPlayer <= 100.0f) && ((*(u32*)((u8*)play + 0x5BF4) % 8) != 0) &&
               FUN_00328cac(play, self)) {
        FUN_003301b8(self);
    } else {
        FUN_0034eb00(self);
    }
}

extern Vec3f EnZf_sDCPlatformPositions[];
s16 Actor_WorldYawTowardPoint(Actor* actor, Vec3f* point);
s16 FUN_00373d98(Vec3f* pos, s16 curPlatform, s16 homePlatform, PlayState* play);
void Animation_PlayOnce(SkelAnime* skelAnime, s32 idx);
void FUN_002b4c60(EnZf* self, PlayState* play);

void FUN_00380604(EnZf* self, PlayState* play) {
    s16 yaw = Actor_WorldYawTowardPoint(&self->actor, &EnZf_sDCPlatformPositions[self->targetPlatform]) + 0x8000;

    Math_SmoothStepToS(&self->actor.world.rot.y, yaw, 1, 0x3E8, 0);
    self->actor.shape.rot.y = self->actor.world.rot.y;
    if (FUN_003731e0(&self->skelAnime)) {
        self->actor.world.rot.y = yaw - 0x8000;
        self->actionState = 0;
        Animation_PlayOnce(&self->skelAnime, EnZf_sPairedAnims[0]);
        self->action = 0x13;
        self->curPlatform = FUN_00373fa4(&self->actor.world.pos, self->curPlatform);
        self->targetPlatform = FUN_00373d98(&self->actor.world.pos, self->curPlatform, self->homePlatform, play);
        self->actionFunc = FUN_002b4c60;
        self->isSwordSheathed = 1;
    }
}

void FUN_003170f4(EnZf* self, f32 speed) {
    s32 t;

    Animation_MorphToLoop(&self->skelAnime, 12, -1.0f);
    if ((s32)(Rand_ZeroOne() * 10.0f + 8.0f) > 0) {
        t = (s32)(0.5f + (f32)(s32)(Rand_ZeroOne() * 10.0f + 8.0f) * 3.0f * 0.5f);
    } else {
        t = (s32)((f32)(s32)(Rand_ZeroOne() * 10.0f + 8.0f) * 3.0f * 0.5f - 0.5f);
    }
    self->timer = t;
    if (self->actor.params == -2) {
        self->actor.speedXZ = 2.0f * speed;
        self->timer /= 2;
    } else {
        self->actor.speedXZ = speed;
    }
    self->actionState = 0;
    self->actor.world.rot.y = self->actor.shape.rot.y + 0x3FFF;
    self->action = 12;
    self->actionFunc = FUN_002b53c4;
}

void FUN_0028525c(EnZf* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime)) {
        if (Rand_ZeroOne() > 0.7f) {
            FUN_0034eb00(self);
        } else if ((Rand_ZeroOne() > 0.2f) && FUN_00328cac(play, self)) {
            FUN_003301b8(self);
        } else {
            FUN_0032ffc0(self, play);
        }
    }
}

}
