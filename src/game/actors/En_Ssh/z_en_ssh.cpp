extern "C" {
#include "z_en_ssh.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x00000035

void EnSsh_Init(Actor* actor, GameState* state);
void EnSsh_Destroy(Actor* actor, GameState* state);
void EnSsh_Update(Actor* actor, GameState* state);
void EnSsh_Draw(Actor* actor, GameState* state);
void EnSsh_Idle(EnSsh* self, PlayState* play);
void EnSsh_Drop(EnSsh* self, PlayState* play);
f32 EnSsh_SetAnimation(EnSsh* self, s32 animIndex);
s32 EnSsh_IsCloseToLink(EnSsh* self, PlayState* play);
s32 FUN_00369a48(Actor* actor, PlayState* play);
void Collider_DestroyJntSph(PlayState* play, void* collider);
void Collider_UpdateSpheres(s32 index, void* collider, MTX34* mtx);
void Effect_Delete(PlayState* play, s32 blureIdx);

static void EnSsh_Bob(EnSsh* self, PlayState* play) {
    f32 bobVel = 0.5f;

    if ((play->state.frames & 8) != 0) {
        bobVel *= -1.0f;
    }
    Math_SmoothStepToF(&self->actor.velocity.y, bobVel, 0.4f, 1000.0f, 0.0f);
}

GLOBAL_ASM("data/z_En_Ssh.data.s")

ActorInit En_Ssh_InitVars = {
    ACTOR_EN_SSH,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_SSH,
    sizeof(EnSsh),
    (ActorFunc)EnSsh_Init,
    (ActorFunc)EnSsh_Destroy,
    (ActorFunc)EnSsh_Update,
    (ActorFunc)EnSsh_Draw,
};

#if 0 // Sol full draft: 996 B, 66 words differ; see STATUS.md.
struct BlureInitVertex {
    BlureInitVertex();
    u8 data[0x24];
};

typedef struct {
    s32 unk_00;
    BlureInitVertex vtx[16];
    Color_RGBA8 p1StartColor;
    Color_RGBA8 p2StartColor;
    Color_RGBA8 p1EndColor;
    Color_RGBA8 p2EndColor;
    s32 elemDuration;
    s32 unkFlag;
    u8 calcMode;
    s32 unk_260;
    u8 unk_264;
} EnSshBlureInit;

typedef struct { ColliderCylinderInit* p[6]; } EnSshCylinderInitPointers;
extern EnSshCylinderInitPointers EnSsh_sCylinderInitPointers;
extern CollisionCheckInfoInit2 EnSsh_sColChkInfoInit;
extern u8 EnSsh_sJntSphInit[];
s32 Effect_Add(PlayState*, s32*, s32, u8, u8, void*);
void Collider_InitJntSph(PlayState*, void*);
void Collider_SetJntSph(PlayState*, void*, Actor*, void*, void*);
DamageTable* DamageTable_Get(s32);
s32 Animation_GetLastFrame(SkelAnime*, s32);
void Animation_Change_00375c08(SkelAnime*, s32, f32, f32, f32, s32, f32);
s32 BgCheck_EntityLineTest1(void*, Vec3f*, Vec3f*, Vec3f*, CollisionPoly**, s32, s32, s32, s32, s32*);
void EnSsh_Start(EnSsh*, PlayState*);
void EnSsh_SetColliderScale(EnSsh*, f32, f32);

static inline s32 EnSsh_CreateBlureEffect(PlayState* play) {
    EnSshBlureInit init;
    s32 i;
    s32 blureIdx;
        u8 p1Start[] = {255,255,255,75};
        u8 p2Start[] = {255,255,255,75};
        u8 p1End[] = {255,255,255,0};
        u8 p2End[] = {255,255,255,0};
        for (i = 0; i < 4; i++) {
            ((u8*)&init.p1StartColor)[i] = p1Start[i];
            ((u8*)&init.p2StartColor)[i] = p2Start[i];
            ((u8*)&init.p1EndColor)[i] = p1End[i];
            ((u8*)&init.p2EndColor)[i] = p2End[i];
        }
        init.elemDuration = 6;
        init.unkFlag = 0;
        init.calcMode = 3;
        init.unk_264 = 22;
        Effect_Add(play, &blureIdx, 1, 0, 0, &init);
    return blureIdx;
}

static inline void EnSsh_InitColliders(EnSsh* self, PlayState* play) {
    EnSshCylinderInitPointers cylinders = EnSsh_sCylinderInitPointers;
    s32 i;
        for (i = 0; i < 6; i++) {
            Collider_InitCylinder(play, &self->colliderCylinders[i]);
            Collider_SetCylinder(play, &self->colliderCylinders[i], &self->actor, cylinders.p[i]);
        }
    self->colliderCylinders[0].info.bumper.dmgFlags = 0x0003F8E9;
    self->colliderCylinders[1].info.bumper.dmgFlags = 0xFFC00716;
    self->colliderCylinders[2].base.colType = 9;
    self->colliderCylinders[2].info.bumperFlags = 0xD;
    self->colliderCylinders[2].info.elemType = 2;
    self->colliderCylinders[2].info.bumper.dmgFlags = 0xFFCC0716;
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, DamageTable_Get(2), &EnSsh_sColChkInfoInit);
    Collider_InitJntSph(play, &self->colliderJntSph);
    Collider_SetJntSph(play, &self->colliderJntSph, &self->actor, EnSsh_sJntSphInit, self->colliderJntSphElements);
}

void EnSsh_Init(Actor* actor, GameState* state) {
    EnSsh* self = (EnSsh*)actor;
    PlayState* play = (PlayState*)state;
    f32 frameCount;
    s32 i;

    if (self->actor.params == 0) {
        if (gSaveContext.inventory.gsTokens >= 100) {
            Actor_Kill(&self->actor);
            return;
        }
    } else if (gSaveContext.inventory.gsTokens >= self->actor.params * 10) {
        Actor_Kill(&self->actor);
        return;
    }
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    FUN_00372f38(&self->actor, play, 0);
    FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 0, &self->unk_228[0], &self->unk_228[0x3A8], 18);
    SkelAnime_SetFaceAnimations((FaceAnimations*)self->faceAnimations, &self->skelAnime, 0, -1, -1);
    frameCount = Animation_GetLastFrame(&self->skelAnime, 0);
    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, frameCount, 1, 0.0f);
    self->blureIdx = EnSsh_CreateBlureEffect(play);
    EnSsh_InitColliders(self, play);
    self->stateFlags = 0;
    self->hitCount = 0;
    {
        CollisionPoly* poly;
        s32 bgId;
        Vec3f posB;
        posB.x = self->actor.world.pos.x;
        posB.y = self->actor.world.pos.y + 1000.0f;
        posB.z = self->actor.world.pos.z;
        BgCheck_EntityLineTest1(&play->colCtx, &self->actor.world.pos, &posB, &self->ceilingPos, &poly, 0, 0, 1, 1, &bgId);
    }
    if (self->actor.params != 0) {
        EnSsh_SetColliderScale(self, 0.5f, 1.0f);
    } else {
        EnSsh_SetColliderScale(self, 0.75f, 1.0f);
    }
    self->actor.gravity = 0.0f;
    self->initialYaw = self->actor.world.rot.y;
    self->actionFunc = EnSsh_Start;
}

#endif
GLOBAL_ASM("asm/EnSsh_Init.s")

void EnSsh_Destroy(Actor* actor, GameState* state) {
    EnSsh* self = (EnSsh*)actor;
    s32 i;

    Effect_Delete((PlayState*)state, self->blureIdx);
    for (i = 0; i < 6; i++) {
        Collider_DestroyCylinder((PlayState*)state, &self->colliderCylinders[i]);
    }
    Collider_DestroyJntSph((PlayState*)state, &self->colliderJntSph);
    FUN_003508b8(actor, 0);
    SkelAnime_Free2(&self->skelAnime);
    ((FaceAnimations*)self->faceAnimations)->Destroy();
}



#if 0 // Sol full draft: 852 B vs 848 B, 147 words differ.
struct EnSshPlayerStickView {
    Actor actor;
    char pad[0x2248 - sizeof(Actor)];
    u16 bodyStickHit;
};
typedef char EnSshPlayerStickOffsetCheck[(offsetof(EnSshPlayerStickView, bodyStickHit) == 0x2248) ? 1 : -1];
f32 __hardfp_sinf(f32);
f32 __hardfp_cosf(f32);
void FUN_0036932c(SkeletonAnimationModel*, s32);
void FUN_0037266c(SkeletonAnimationModel*, s32);
s32 EnSsh_OverrideLimbDraw(PlayState*, s32, MTX34*, void*);
void EnSsh_PostLimbDraw(PlayState*, s32, MTX34*, void*);

static inline void EnSsh_RotateY(MTX34* m, f32 angle) {
    if (angle != 0.0f) {
        f32 sn = __hardfp_sinf(angle);
        f32 cs = __hardfp_cosf(angle);
        f32 a, b;
        a = m->data[0][0]; b = m->data[0][2];
        m->data[0][0] = a * cs - b * sn;
        m->data[0][2] = a * sn + b * cs;
        a = m->data[1][0]; b = m->data[1][2];
        m->data[1][0] = a * cs - b * sn;
        m->data[1][2] = a * sn + b * cs;
        a = m->data[2][0]; b = m->data[2][2];
        m->data[2][0] = a * cs - b * sn;
        m->data[2][2] = a * sn + b * cs;
    }
}

static inline void EnSsh_CheckBodyStickHit(EnSsh* self, PlayState* play) {
    ColliderInfo* elem = &self->colliderCylinders[0].info;
    ColliderInfo* legs1 = &self->colliderCylinders[1].info;
    ColliderInfo* legs2 = &self->colliderCylinders[2].info;
    EnSshPlayerStickView* player = (EnSshPlayerStickView*)GET_PLAYER(play);
    if (player->bodyStickHit != 0) {
        elem->bumper.dmgFlags |= 2;
        legs1->bumper.dmgFlags &= ~2;
        legs2->bumper.dmgFlags &= ~2;
    } else {
        elem->bumper.dmgFlags &= ~2;
        legs1->bumper.dmgFlags |= 2;
        legs2->bumper.dmgFlags |= 2;
    }
}

static inline void EnSsh_Translate(MTX34* m, s32 mode, f32 x, f32 y, f32 z) {
    Vec3f t;
    t.x = x;
    t.y = y;
    t.z = z;
    if (mode == 1) { Matrix_UpdatePosition(m, m, &t); }
    else { MTX34_MakeTrans(m, &t); }
}

static inline void EnSsh_Sway(EnSsh* self) {
    Vec3f swayVecBase;
    Vec3f swayVec;
    f32 temp;
    s16 swayAngle;
    if (self->swayTimer != 0) {
        self->swayAngle += 0x640;
        self->swayTimer--;
        if (self->swayTimer == 0) {
            self->swayAngle = 0;
        }
        temp = self->swayTimer * (1.0f / 6);
        swayAngle = temp * (0x10000 / 360.0f) * Math_SinS(self->swayAngle);
        temp = self->actor.world.pos.y - self->ceilingPos.y;
        swayVecBase.x = Math_SinS(swayAngle) * temp;
        swayVecBase.y = Math_CosS(swayAngle) * temp;
        swayVecBase.z = 0.0f;
        MTX34 mtx;
        MTX34CopyAsm(&mtx, &self->actor.modelMtx);
        EnSsh_Translate(&mtx, 0, self->ceilingPos.x, self->ceilingPos.y, self->ceilingPos.z);
        EnSsh_RotateY(&mtx, self->actor.world.rot.y * 9.58738E-5f);
        VEC3TransformAsm((Vector3f*)&swayVec, &mtx, (Vector3f*)&swayVecBase);
        self->actor.shape.rot.z = -(swayAngle * 2);
        self->actor.world.pos.x = swayVec.x;
        self->actor.world.pos.z = swayVec.z;
    }
}

void EnSsh_Draw(Actor* actor, GameState* state) {
    EnSsh* self = (EnSsh*)actor;
    PlayState* play = (PlayState*)state;
    EnSsh_CheckBodyStickHit(self, play);
    EnSsh_Sway(self);
    FaceAnimations* faceAnimations = (FaceAnimations*)self->faceAnimations;
    faceAnimations->FUN_0035e3a4(0, self->blinkState);
    faceAnimations->FUN_0035e330();
    if (self->actor.params == 0) {
        FUN_0036932c(self->skelAnime.unk_28, 0);
        FUN_0036932c(self->skelAnime.unk_28, 3);
        FUN_0036932c(self->skelAnime.unk_28, 6);
        FUN_0037266c(self->skelAnime.unk_28, 1);
        FUN_0037266c(self->skelAnime.unk_28, 4);
        FUN_0037266c(self->skelAnime.unk_28, 7);
    } else {
        FUN_0037266c(self->skelAnime.unk_28, 0);
        FUN_0037266c(self->skelAnime.unk_28, 3);
        FUN_0037266c(self->skelAnime.unk_28, 6);
        FUN_0036932c(self->skelAnime.unk_28, 1);
        FUN_0036932c(self->skelAnime.unk_28, 4);
        FUN_0036932c(self->skelAnime.unk_28, 7);
    }
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnSsh_OverrideLimbDraw, EnSsh_PostLimbDraw, &self->actor, 0);
}



#endif
GLOBAL_ASM("asm/EnSsh_Draw.s")

void EnSsh_Talk(EnSsh* self, PlayState* play);
void EnSsh_Return(EnSsh* self, PlayState* play);

void EnSsh_Idle(EnSsh* self, PlayState* play) {
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actionFunc = EnSsh_Talk;
        if (self->actor.params == 0) {
            gSaveContext.eventChkInf[9] |= 0x40;
        }
        if (self->actor.textId == 0x26 || self->actor.textId == 0x27) {
            gSaveContext.infTable[25] |= 0x40;
        }
        if (self->actor.textId == 0x24 || self->actor.textId == 0x25) {
            gSaveContext.infTable[25] |= 0x80;
        }
    } else {
        if (self->unkTimer != 0 && --self->unkTimer == 0) {
            EnSsh_SetAnimation(self, 2);
        }
        if (self->animTimer != 0 && --self->animTimer == 0) {
            EnSsh_SetAnimation(self, 2);
        }
        if (!EnSsh_IsCloseToLink(self, play)) {
            Audio_PlayActorSound2(&self->actor, 0x010002D9);
            EnSsh_SetAnimation(self, 1);
            self->actionFunc = EnSsh_Return;
        } else {
            if (DECR(self->sfxTimer) == 0) {
                Audio_PlayActorSound2(&self->actor, 0x010002DA);
                self->sfxTimer = 64;
            }
            EnSsh_Bob(self, play);
            if (self->unkTimer == 0 && self->animTimer == 0) {
                self->actor.textId = Text_GetFaceReaction(play, 0xD);
                if (self->actor.textId == 0) {
                    if (self->actor.params == 0) {
                        if (gSaveContext.inventory.gsTokens >= 50) {
                            self->actor.textId = 0x29;
                        } else if (gSaveContext.inventory.gsTokens >= 10) {
                            if (gSaveContext.infTable[25] & 0x80) {
                                self->actor.textId = 0x24;
                            } else {
                                self->actor.textId = 0x25;
                            }
                        } else {
                            if (gSaveContext.infTable[25] & 0x40) {
                                self->actor.textId = 0x27;
                            } else {
                                self->actor.textId = 0x26;
                            }
                        }
                    } else {
                        self->actor.textId = 0x22;
                    }
                }
                FUN_0036bb28(&self->actor, play, 100.0f);
            }
        }
    }
}

void EnSsh_Land(EnSsh* self, PlayState* play) {
    if ((self->unkTimer != 0) && (--self->unkTimer == 0)) {
        EnSsh_SetAnimation(self, 2);
    }
    if ((self->animTimer != 0) && (--self->animTimer == 0)) {
        EnSsh_SetAnimation(self, 2);
    }
    if ((self->actor.floorHeight + self->floorHeightOffset) <= self->actor.world.pos.y) {
        self->actionFunc = EnSsh_Idle;
        self->sfxTimer = 0;
    } else {
        Math_SmoothStepToF(&self->actor.velocity.y, 2.0f, 0.6f, 1000.0f, 0.0f);
    }
}

s32 EnSsh_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    return false;
}

void EnSsh_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnSsh* self = (EnSsh*)thisx;
    Collider_UpdateSpheres(limbIndex, &self->colliderJntSph, mtx);
}



void EnSsh_SetColliderScale(EnSsh* self, f32 scale, f32 radiusMod) {
    f32 radius;
    f32 height;
    f32 yShift;
    s32 i;

    radius = self->colliderJntSph.elements[0].modelSphereRadius;
    radius *= scale;
    self->colliderJntSph.elements[0].modelSphereRadius = (s16)radius;
    for (i = 0; i < 6; i++) {
        yShift = self->colliderCylinders[i].dim.yShift;
        radius = self->colliderCylinders[i].dim.radius;
        height = self->colliderCylinders[i].dim.height;
        yShift *= scale;
        radius *= scale * radiusMod;
        self->colliderCylinders[i].dim.yShift = (s16)yShift;
        self->colliderCylinders[i].dim.radius = (s16)radius;
        self->colliderCylinders[i].dim.height = (s16)height;
    }
    Actor_SetScale(&self->actor, 0.04f * scale);
    self->floorHeightOffset = 40.0f * scale;
    self->colliderScale = scale * 1.5f;
}

void EnSsh_Talk(EnSsh* self, PlayState* play) {
    EnSsh_Bob(self, play);
    if (FUN_00369a48(&self->actor, play)) {
        self->actionFunc = EnSsh_Idle;
        self->sfxTimer = 0;
    }
}

void EnSsh_Wait(EnSsh* self, PlayState* play) {
    if (EnSsh_IsCloseToLink(self, play)) {
        if (self->unkTimer == 0) {
            self->animTimer = (s16)EnSsh_SetAnimation(self, 4);
        }
        self->actor.velocity.y = -10.0f;
        self->actionFunc = EnSsh_Drop;
        self->sfxTimer = 0;
    } else {
        EnSsh_Bob(self, play);
    }
}

#if 0 // Sol full draft: 2076 B, 334 words differ.
struct EnSshPlayDamageView {
    char pad[0x5BAC];
    void (*damagePlayer)(PlayState*, s32);
};
struct EnSshActorColorView {
    char pad[0x11A];
    u16 colorFilterTimer;
};
typedef char EnSshDamageCallbackOffsetCheck[(offsetof(EnSshPlayDamageView, damagePlayer) == 0x5BAC) ? 1 : -1];
typedef char EnSshColorTimerOffsetCheck[(offsetof(EnSshActorColorView, colorFilterTimer) == 0x11A) ? 1 : -1];
void Actor_SetColorFilter(Actor*, s32, s32, s32, s32);
extern Vec3f EnSsh_sCylinderOffsets[3];
struct EnSshCylinderOffsets { Vec3f pos[3]; };

static inline void EnSsh_SetStunned(EnSsh* self) {
    if (self->stunTimer == 0) {
        self->stateFlags |= 8;
        self->stunTimer = 120;
        ((EnSshActorColorView*)&self->actor)->colorFilterTimer = 0;
    }
}

static inline s32 EnSsh_CheckHitPlayer(EnSsh* self, PlayState* play) {
    s32 i;
    s32 hit;
    if (self->hitCount == 0 && self->spinTimer == 0) {
        return false;
    }
    hit = false;
    for (i = 0; i < 3; i++) {
        if (self->colliderCylinders[i + 3].base.ocFlags2 & 1) {
            self->colliderCylinders[i + 3].base.ocFlags2 &= ~1;
            hit = true;
        }
    }
    if (!hit) { return false; }
    self->hitTimer = 30;
    if (self->swayTimer == 0) {
        self->spinTimer = self->hitTimer;
    }
    Audio_PlayActorSound2(&self->actor, 0x010002F4);
    Audio_PlayActorSound2(&self->actor, 0x01000553);
    ((EnSshPlayDamageView*)play)->damagePlayer(play, -8);
    FUN_00374bb8(play, &self->actor, 2.0f, self->actor.yawTowardsPlayer, 6.0f);
    self->hitCount--;
    return true;
}

static inline s32 EnSsh_CheckHitFront(EnSsh* self) {
    u32 acFlags;
    if (self->colliderCylinders[2].base.acFlags) {}
    acFlags = self->colliderCylinders[2].base.acFlags;
    if (!!(acFlags & 2) == 0) {
        return 0;
    } else {
        self->colliderCylinders[2].base.acFlags &= ~2;
        self->invincibilityTimer = 8;
        if (self->swayTimer == 0 && self->hitTimer == 0 && self->stunTimer == 0) {
            self->swayTimer = 60;
        }
        return 1;
    }
}

static inline s32 EnSsh_CheckHitBack(EnSsh* self, PlayState* play) {
    ColliderCylinder* collider = &self->colliderCylinders[0];
    s32 hit = false;
    if (collider->base.acFlags & 2) {
        collider->base.acFlags &= ~2;
        hit = true;
    }
    collider = &self->colliderCylinders[1];
    if (collider->base.acFlags & 2) {
        collider->base.acFlags &= ~2;
        hit = true;
    }
    if (!hit) { return false; }
    self->invincibilityTimer = 8;
    if (self->hitCount == 0) { self->hitCount++; }
    if (self->stunTimer == 0) {
        Audio_PlayActorSound2(&self->actor, 0x0100030E);
        Audio_PlayActorSound2(&self->actor, 0x01000552);
    }
    EnSsh_SetStunned(self);
    self->stateFlags |= 1;
    return false;
}

static inline s32 EnSsh_CollisionCheck(EnSsh* self, PlayState* play) {
    if (self->stunTimer == 0) { EnSsh_CheckHitPlayer(self, play); }
    if (EnSsh_CheckHitFront(self)) {
        return false;
    } else if (play->actorCtx.unk_02 != 0) {
        self->invincibilityTimer = 8;
        if (self->stunTimer == 0) {
            Audio_PlayActorSound2(&self->actor, 0x0100030E);
            Audio_PlayActorSound2(&self->actor, 0x01000552);
        }
        EnSsh_SetStunned(self);
        self->stateFlags |= 1;
        return false;
    } else {
        return EnSsh_CheckHitBack(self, play);
    }
}

static inline s32 EnSsh_Damaged(EnSsh* self) {
    if (self->stunTimer == 120 && self->stateFlags & 1) {
        Actor_SetColorFilter(&self->actor, 0, 200, 0, self->stunTimer);
    }
    if (DECR(self->stunTimer) != 0) {
        Math_SmoothStepToS(&self->maxTurnRate, 0x2710, 0xA, 0x3E8, 1);
        return false;
    } else {
        self->stunTimer = 0;
        self->stateFlags &= ~1;
        self->spinTimer = 0;
        if (self->swayTimer == 0) { self->spinTimer = 30; }
        Audio_PlayActorSound2(&self->actor, 0x010002F4);
        Audio_PlayActorSound2(&self->actor, 0x01000553);
        return true;
    }
}

static inline void EnSsh_UpdateColliderScale(EnSsh* self) {
    f32 childScale = 0.5f;
    f32 parentScale = 0.75f;
    f32 radiusMod = 1.0f;
    if (self->stateFlags & 0x10) {
        if (self->spinTimer == 0) {
            self->stateFlags &= ~0x10;
            if (self->actor.params != 0) { EnSsh_SetColliderScale(self, childScale, radiusMod); }
            else { EnSsh_SetColliderScale(self, parentScale, radiusMod); }
        }
    } else {
        if (self->spinTimer != 0) {
            self->stateFlags |= 0x10;
            if (self->actor.params != 0) { EnSsh_SetColliderScale(self, childScale, 2.0f); }
            else { EnSsh_SetColliderScale(self, parentScale, 2.0f); }
        }
    }
}

static inline void EnSsh_Turn(EnSsh* self, PlayState* play) {
    if (self->hitTimer != 0) { self->hitTimer--; }
    if (DECR(self->spinTimer) != 0) {
        self->actor.world.rot.y += 10000.0f * (self->spinTimer / 30.0f);
    } else if (self->swayTimer == 0) {
        Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 4, 0x2710, 1);
    }
    self->actor.shape.rot.y = self->actor.world.rot.y;
}
static inline void EnSsh_Stunned(EnSsh* self, PlayState* play) {
    self->actor.shape.rot.y = self->actor.world.rot.y;
    if (self->stunTimer < 30) {
        if (self->stunTimer & 1) { self->actor.shape.rot.y += 0x7D0; }
        else { self->actor.shape.rot.y -= 0x7D0; }
    }
}
static inline void EnSsh_UpdateYaw(EnSsh* self, PlayState* play) {
    if (self->stunTimer != 0) { EnSsh_Stunned(self, play); }
    else { EnSsh_Turn(self, play); }
}

static inline void EnSsh_SetBodyCylinderAC(EnSsh* self, PlayState* play) {
    Collider_UpdateCylinder(&self->actor, &self->colliderCylinders[0]);
    CollisionCheck_SetAC(play, &play->colChkCtx, &self->colliderCylinders[0].base);
}
static inline void EnSsh_SetLegsCylinderAC(EnSsh* self, PlayState* play) {
    s16 angleTowardsLink = ABS((s16)(self->actor.yawTowardsPlayer - self->actor.shape.rot.y));
    if (angleTowardsLink < 0x3FFC) {
        Collider_UpdateCylinder(&self->actor, &self->colliderCylinders[2]);
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->colliderCylinders[2].base);
    } else {
        Collider_UpdateCylinder(&self->actor, &self->colliderCylinders[1]);
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->colliderCylinders[1].base);
    }
}
static inline s32 EnSsh_SetCylinderOC(EnSsh* self, PlayState* play) {
    EnSshCylinderOffsets offsets = *(EnSshCylinderOffsets*)EnSsh_sCylinderOffsets;
    Vec3f cylPos;
    s32 i;
    for (i = 0; i < 3; i++) {
        cylPos = self->actor.world.pos;
        offsets.pos[i].x *= self->colliderScale;
        offsets.pos[i].y *= self->colliderScale;
        offsets.pos[i].z *= self->colliderScale;
        MTX34 mtx;
        MTX34CopyAsm(&mtx, &self->actor.modelMtx);
        EnSsh_Translate(&mtx, 0, cylPos.x, cylPos.y, cylPos.z);
        EnSsh_RotateY(&mtx, (self->initialYaw * (1.0f / 32768.0f)) * 3.1415927f);
        VEC3TransformAsm((Vector3f*)&cylPos, &mtx, (Vector3f*)&offsets.pos[i]);
        self->colliderCylinders[i + 3].dim.pos = cylPos;
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->colliderCylinders[i + 3].base);
    }
    return 1;
}
static inline void EnSsh_SetColliders(EnSsh* self, PlayState* play) {
    if (self->actor.colChkInfo.health == 0) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->colliderJntSph.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->colliderJntSph.base);
    } else {
        if (self->hitTimer == 0) { EnSsh_SetCylinderOC(self, play); }
        if (DECR(self->invincibilityTimer) == 0) {
            EnSsh_SetBodyCylinderAC(self, play);
            EnSsh_SetLegsCylinderAC(self, play);
        }
    }
}

void EnSsh_Update(Actor* actor, GameState* state) {
    EnSsh* self = (EnSsh*)actor;
    PlayState* play = (PlayState*)state;
    EnSsh_UpdateColliderScale(self);
    if (EnSsh_CollisionCheck(self, play)) { return; }
    if (self->stunTimer != 0) { EnSsh_Damaged(self); }
    else {
        FUN_00370734(&self->skelAnime);
        FUN_0036b96c(&self->actor);
        Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);
        self->actionFunc(self, play);
    }
    EnSsh_UpdateYaw(self, play);
    if (DECR(self->blinkTimer) == 0) { self->blinkTimer = Rand_S16Offset_003702c8(60, 60); }
    self->blinkState = self->blinkTimer;
    if (self->blinkState >= 3) { self->blinkState = 0; }
    EnSsh_SetColliders(self, play);
    Actor_SetFocus(actor, self->actor.params == 0 ? 12.0f : 2.0f);
}
#endif
GLOBAL_ASM("asm/EnSsh_Update.s")

}
