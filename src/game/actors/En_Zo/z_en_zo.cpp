extern "C" {
#include "z_en_zo.hpp"

#define FLAGS 0x00000009

void EnZo_Init(Actor* actor, GameState* state);
void EnZo_Destroy(Actor* actor, GameState* state);
void EnZo_Update(Actor* actor, GameState* state);
void EnZo_Draw(Actor* actor, GameState* state);
void EnZo_Surface(EnZo* self, PlayState* play);
void EnZo_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void FUN_0034fc7c(void* ptr);
void SystemArena_Free(void* ptr);
void FUN_0034e01c(MTX34* mtx, Vec3s* rot);
void EnZo_Standing(EnZo* self, PlayState* play);
void EnZo_Submerged(EnZo* self, PlayState* play);
void EnZo_TreadWater(EnZo* self, PlayState* play);
void FUN_00315ba8(EnZo* self);
f32 __hardfp_sinf(f32);
f32 __hardfp_cosf(f32);
void EnZo_Dive(EnZo* self, PlayState* play);
void* ZAR_GetCMBByIndex(ZARInfo* zar, s32 index);
s32 FUN_00353e78(ZARInfo* zar, PlayState* play, SkelAnime* skel, void* cmb, void* owner,
                s32 anim, void* joints, void* morphs, s32 count);
void* SystemArena_Malloc_0035010c(s32 size);
void* FUN_003500c4(void* p);
void FUN_0034ff2c(void* obj, s32 a, s32 b, s32 c, s32 d, s32 e);
void FUN_0034fea8(void* obj, s32 bank, GameState* state, s32 index, s32 a, s32 b, s32 c, s32 d);
struct EnZoParticleSub { char unk_000[0x178]; u32 flags; char unk_17C[0x80]; u32 particleCount; };
struct EnZoParticleObject { char unk_00[8]; EnZoParticleSub* sub; };
struct EnZoQuad { Vec3f v[4]; };
struct EnZoUV { Vec2f v[4]; };
void FUN_0034ea6c(EnZoParticleSub*, Vec3f*);
void FUN_0034ea48(EnZoParticleSub*, EnZoUV*);
extern ColliderCylinderInit EnZo_sCylinderInit;
extern CollisionCheckInfoInit2 EnZo_sColChkInfoInit;
extern f32 EnZo_sParticleScale;
extern const EnZoUV EnZo_sParticleUV;
s16 FUN_001889a0(PlayState*, Actor*);
u16 FUN_00188ad4(PlayState*, Actor*);
void FUN_0034c664(Actor* actor, void* info, s32 preset, s32 mode);
s32 FUN_00342714(PlayState* play, Actor* actor, void* talk, f32 radius,
                  u16 (*getText)(PlayState*, Actor*), s16 (*updateTalk)(PlayState*, Actor*));
void FUN_0035619c(PlayState*, f32*, f32*, s32, s32, s32, s32, s32, s32, s32);
void FUN_00342988(void*, f32*, s32);
void FUN_003693b4(void*, Vec3f*, MTX34*, Vec3f*, f32*, s32);
void FUN_00371eac(EnZoParticleSub*, s32);
void* FUN_003687a8(SkeletonAnimationModel*);
void FUN_003589cc(void*, s32);
void FUN_00358964(void*, s32, f32*);
struct EnZoFloatColor { f32 data[4]; };
struct EnZoSinCosEntry { f32 sine, cosine, sineDelta, cosineDelta; };
extern const EnZoFloatColor EnZo_sWhiteColor;
extern const EnZoSinCosEntry EnZo_sSinCosTable[];
extern struct_80034EC0_Entry EnZo_sAnimationInfo[];

extern __pure f32 _fabsf(f32);
static __inline __pure f32 EnZo_Fabsf(f32 x) { return _fabsf(x); }

static inline Vector3f EnZo_VecSub(Vector3f* a, Vector3f* b) {
    return Vector3f(a->x - b->x, a->y - b->y, a->z - b->z);
}
static s32 EnZo_PlayerInProximity(EnZo* self, PlayState* play) {
    Actor* player = (Actor*)GET_PLAYER(play);
    Vector3f surfacePos(0.0f, 0.0f, 0.0f);
    surfacePos.z = self->actor.world.pos.z;
    surfacePos.y = self->actor.world.pos.y + self->actor.yDistToWater;
    surfacePos.x = self->actor.world.pos.x;

    f32 yDist;
    f32 hDist;

    Vector3f diff = EnZo_VecSub((Vector3f*)&player->world.pos, &surfacePos);
    hDist = __sqrtf(SQ(diff.x) + SQ(diff.z));
    yDist = EnZo_Fabsf(diff.y);
    if (hDist < 240.0f && yDist < 80.0f) {
        return 1;
    }
    return 0;
}

void EnZo_Destroy(Actor* actor, GameState* state) {
    EnZo* self = (EnZo*)actor;
    s32 zero = 0;

    SkelAnime_Free2(&self->skelAnime);
    self->faceAnimations.Destroy();
    if (self->unk_C80 != NULL) {
        FUN_0034fc7c(self->unk_C80);
        SystemArena_Free(self->unk_C80);
        self->unk_C80 = (void*)zero;
    }
    if (self->unk_C84 != NULL) {
        FUN_0034fc7c(self->unk_C84);
        SystemArena_Free(self->unk_C84);
        self->unk_C84 = (void*)zero;
    }
    if (self->unk_C88 != NULL) {
        FUN_0034fc7c(self->unk_C88);
        SystemArena_Free(self->unk_C88);
        self->unk_C88 = (void*)zero;
    }
}

#if 0
void EnZo_Submerged(EnZo* self, PlayState* play) {
    if (EnZo_PlayerInProximity(self, play)) {
        self->actor.velocity.y = 4.0f;
        self->actionFunc = EnZo_Surface;
    }
}


#endif
GLOBAL_ASM("asm/EnZo_Submerged.s")

static inline void EnZo_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}
void EnZo_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnZo* self = (EnZo*)thisx;
    if (limbIndex == 9) {
        Vector3f vec(0.0f, 600.0f, 0.0f);
        EnZo_MultVec3f(&vec, (Vector3f*)&self->actor.focus.pos, mtx);
    }
}

s32 EnZo_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnZo* self = (EnZo*)thisx;

    if (limbIndex == 9) {
        FUN_0034e01c(mtx, &self->headRot);
    } else if (limbIndex == 8) {
        FUN_0034e01c(mtx, &self->torsoRot);
    }
    return 0;
}

#if 0
void EnZo_Standing(EnZo* self, PlayState* play) {
    s16 angle;

    FUN_00370f5c(play, self->fidgetTableY, self->fidgetTableZ, 21);
    s32 animId = 10;
    if (self->skelAnime.animationType == 3 || self->skelAnime.animationType == 4) {
        if (self->talkState == 0) {
            if (EnZo_Standing == self->actionFunc) {
                animId = 0;
            } else {
                animId = 3;
            }
        }
    }
    if (self->talkState != 0 && self->actor.textId == 0x4006 && self->skelAnime.animationType != 3) {
        animId = 6;
    }
    if (self->talkState != 0 && self->actor.textId == 0x4007 && self->skelAnime.animationType != 4) {
        animId = 7;
    }
    if (animId != 10) {
        FUN_003717ac(&self->skelAnime, EnZo_sAnimationInfo, animId);
        if (animId == 3) {
            self->skelAnime.curFrame = self->skelAnime.endFrame;
            self->skelAnime.playSpeed = 0.0f;
        }
    }
    if (self->talkState != 0) {
        self->trackingMode = 4;
    } else {
        angle = ABS((s16)((f32)self->actor.shape.rot.y - (f32)self->actor.yawTowardsPlayer));
        if (angle < 0x4718) {
            if (EnZo_PlayerInProximity(self, play)) {
                self->trackingMode = 2;
            } else {
                self->trackingMode = 1;
            }
        } else {
            self->trackingMode = 1;
        }
    }
}
#endif

GLOBAL_ASM("asm/EnZo_Standing.s")

GLOBAL_ASM("data/z_En_Zo.data.s")
GLOBAL_ASM("../src/game/actors/En_Zo/data_syms.s")

ActorInit En_Zo_InitVars = {
    ACTOR_EN_ZO,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_ZO,
    sizeof(EnZo),
    (ActorFunc)EnZo_Init,
    (ActorFunc)EnZo_Destroy,
    (ActorFunc)EnZo_Update,
    (ActorFunc)EnZo_Draw,
};

#if 0
static inline EnZoUV EnZo_CopyParticleUV() { return EnZo_sParticleUV; }
void EnZo_Init(Actor* actor, GameState* state) {
    EnZo* self = (EnZo*)actor;
    PlayState* play = (PlayState*)state;
    void* obj;
    Actor* thisx = actor;
    actor->unk_19A = 1;
    self->zar = &play->GetObjectStatus(actor->objBankIndex)->zarInfo;
    ActorShape_Init(&actor->shape, 0.0f, NULL, 0.0f);
    FUN_00353e78(self->zar, play, &self->skelAnime, ZAR_GetCMBByIndex(self->zar, 0), actor->unk_178,
                -1, self->jointTable, self->morphTable, 21);
    SkelAnime_SetFaceAnimations(&self->faceAnimations, &self->skelAnime, 0, -1, -1);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, actor, &EnZo_sCylinderInit);
    CollisionCheck_SetInfo2(&actor->colChkInfo, NULL, &EnZo_sColChkInfoInit);
    obj = SystemArena_Malloc_0035010c(0x28);
    if (obj != NULL) obj = FUN_003500c4(obj);
    self->unk_C80 = obj;
    FUN_0034ff2c(self->unk_C80, 2, 1, 15, 2, 0);
    FUN_0034fea8(self->unk_C80, actor->objBankIndex, state, 0, 0x2601, 0x2601, 0x812F, 0x812F);
    {
    EnZoQuad quad = {};
    EnZoUV uv;
    EnZoUV* uvPtr = &uv;
    f32 neg, extent, pos;
    extent = EnZo_sParticleScale * 0.5f;
    neg = extent * -64.0f;
    pos = extent * 64.0f;
    quad.v[0].x = neg;
    quad.v[0].z = neg;
    quad.v[1].x = pos;
    quad.v[1].z = neg;
    quad.v[2].x = neg;
    quad.v[3].x = pos;
    *uvPtr = EnZo_sParticleUV;
    obj = SystemArena_Malloc_0035010c(0x28);
    if (obj != NULL) obj = FUN_003500c4(obj);
    self->unk_C84 = obj;
    FUN_0034ff2c(self->unk_C84, 1, 1, 30, 2, 0);
    FUN_0034fea8(self->unk_C84, actor->objBankIndex, state, 1, 0x2601, 0x2601, 0x812F, 0x812F);
    FUN_0034ea6c(((EnZoParticleObject*)self->unk_C84)->sub, quad.v);
    FUN_0034ea48(((EnZoParticleObject*)self->unk_C84)->sub, &uv);
    ((EnZoParticleObject*)self->unk_C84)->sub->flags |= 1;
    obj = SystemArena_Malloc_0035010c(0x28);
    if (obj != NULL) obj = FUN_003500c4(obj);
    self->unk_C88 = obj;
    FUN_0034ff2c(self->unk_C88, 1, 1, 15, 2, 0);
    FUN_0034fea8(self->unk_C88, actor->objBankIndex, state, 2, 0x2601, 0x2601, 0x812F, 0x812F);
    }
    if (LINK_IS_ADULT && (actor->params & 0x3F) == 8) {
        Actor_Kill(actor);
        return;
    }
    FUN_003717ac(&self->skelAnime, EnZo_sAnimationInfo, 2);
    Actor_SetScale(thisx, 0.01f);
    actor->targetMode = 6;
    self->dialogRadius = self->collider.dim.radius + 30.0f;
    self->trackingMode = 1;
    self->canSpeak = false;
    self->talkState = 0;
    Actor_UpdateBgCheckInfo(play, actor, self->collider.dim.height * 0.5f, self->collider.dim.radius, 0.0f, 5);
    if (actor->yDistToWater < 54.0f || (actor->params & 0x3F) == 8) {
        actor->shape.shadowDraw = ActorShadow_DrawCircle;
        actor->shape.shadowScale = 24.0f;
        FUN_003717ac(&self->skelAnime, EnZo_sAnimationInfo, 1);
        self->alpha = 255.0f;
        self->canSpeak = true;
        self->actionFunc = EnZo_Standing;
    } else {
        actor->flags &= ~1;
        self->actionFunc = EnZo_Submerged;
    }
}

#endif
GLOBAL_ASM("asm/EnZo_Init.s")

#if 0
static inline void EnZo_UpdateSpawnRipple(EnZo* self, Vec3f* pos, f32 scale, f32 target, s32 alpha) {
    EnZoEffect* effect = self->effects;
    s32 i;
    for (i = 0; i < 15; i++, effect++) {
        if (effect->type == 0) {
            effect->type = 1;
            effect->pos = *pos;
            effect->scale = scale;
            effect->targetScale = target;
            effect->color.a = alpha;
            break;
        }
    }
}
static inline void EnZo_UpdateSpawnBubble(EnZo* self, Vec3f* pos) {
    EnZoEffect* effect;
    Vector3f vel(0.0f, 1.0f, 0.0f);
    s32 i;
    f32 surface;
    effect = self->effects;
    for (i = 0; i < 15; i++, effect++) {
        if (effect->type != 0) continue;
        surface = self->actor.world.pos.y + self->actor.yDistToWater;
        if (pos->y >= surface) continue;
        {
            effect->type = 3;
            effect->pos = *pos;
            effect->accel = *pos;
            effect->vel = *(Vec3f*)&vel;
            effect->scale = (Rand_ZeroOne() - 0.5f) * 0.02f + 0.12f;
            break;
        }
    }
}
static inline void EnZo_UpdateEffectsRipples(EnZo* self) {
    EnZoEffect* effect = self->effects;
    s16 i;
    for (i = 0; i < 15; i++, effect++) {
        if (effect->type == 1) {
            Math_ApproachF(&effect->scale, effect->targetScale, 0.2f, 0.8f);
            s32 alpha;
            if (effect->color.a > (s32)(20.0f * SREG(30) * (1.0f / 3.0f) + 0.5f)) {
                alpha = effect->color.a - (s32)(20.0f * SREG(30) * (1.0f / 3.0f) + 0.5f);
            } else {
                alpha = 0;
            }
            effect->color.a = alpha;
            if (effect->color.a == 0) effect->type = 0;
        }
    }
}
static inline void EnZo_UpdateEffectsBubbles(EnZo* self) {
    EnZoEffect* effect = self->effects;
    s16 i;
    f32 surface;
    for (i = 0; i < 15; i++, effect++) {
        if (effect->type == 3) {
            effect->pos.x = ((Rand_ZeroOne() * 0.5f - 0.25f) * SREG(30)) * (1.0f / 3.0f) + effect->accel.x;
            effect->pos.z = ((Rand_ZeroOne() * 0.5f - 0.25f) * SREG(30)) * (1.0f / 3.0f) + effect->accel.z;
            effect->pos.y += effect->vel.y * (SREG(30) * (1.0f / 3.0f));
            surface = self->actor.world.pos.y + self->actor.yDistToWater;
            if (surface <= effect->pos.y) {
                effect->type = 0;
                effect->pos.y = surface;
                EnZo_UpdateSpawnRipple(self, &effect->pos, 0.06f, 0.12f, 200);
            }
        }
    }
}
static inline void EnZo_UpdateEffectsSplashes(EnZo* self) {
    EnZoEffect* effect = self->effects;
    s16 i;
    f32 surface;
    for (i = 0; i < 15; i++, effect++) {
        if (effect->type == 2) {
            effect->pos.x += effect->vel.x * (SREG(30) * (1.0f / 3.0f));
            effect->pos.y += effect->vel.y * (SREG(30) * (1.0f / 3.0f));
            effect->pos.z += effect->vel.z * (SREG(30) * (1.0f / 3.0f));
            if (effect->vel.y >= -20.0f) {
                effect->vel.y += effect->accel.y * (SREG(30) * (1.0f / 3.0f));
            } else {
                effect->vel.y = -20.0f;
                effect->accel.y = 0.0f;
            }
            surface = self->actor.world.pos.y + self->actor.yDistToWater;
            if (effect->pos.y < surface) {
                effect->type = 0;
                effect->pos.y = surface;
                EnZo_UpdateSpawnRipple(self, &effect->pos, 0.06f, 0.12f, 200);
            }
        }
    }
}
static inline void EnZo_UpdateBlink(EnZo* self, PlayState* play) {
    if (DECR(self->blinkTimer) == 0 && (play->state.frames & 1)) {
        self->eyeTexture++;
        if (self->eyeTexture > 2) {
            self->blinkTimer = Rand_S16Offset_003702c8(30, 30);
            self->eyeTexture = 0;
        }
    }
}
static inline void EnZo_UpdateDialog(EnZo* self, PlayState* play) {
    Actor* player = (Actor*)GET_PLAYER(play);
    self->trackPos = player->world.pos;
    if (self->actionFunc == EnZo_Standing) {
        self->trackingHeight = gSaveContext.linkAge != 0 ? 10.0f : -10.0f;
    } else {
        self->trackPos.y = self->actor.world.pos.y;
    }
    FUN_0034c664(&self->actor, &self->talkState, 11, self->trackingMode);
    if (self->canSpeak == true) {
        FUN_00342714(play, &self->actor, &self->talkState, self->dialogRadius, FUN_00188ad4, FUN_001889a0);
    }
}
void EnZo_Update(Actor* actor, GameState* state) {
    EnZo* self = (EnZo*)actor;
    PlayState* play = (PlayState*)state;
    Vec3f pos;
    if ((s32)self->alpha != 0) {
        FUN_00370734(&self->skelAnime);
        EnZo_UpdateBlink(self, play);
    }
    Actor_MoveForward(actor);
    Actor_UpdateBgCheckInfo(play, actor, self->collider.dim.radius, self->collider.dim.height * 0.25f, 0.0f, 5);
    self->actionFunc(self, play);
    EnZo_UpdateDialog(self, play);
    if (play->state.frames & 8) {
        pos = self->actor.world.pos;
        pos.y += (Rand_ZeroOne() - 0.5f) * 10.0f + 18.0f;
        pos.x += (Rand_ZeroOne() - 0.5f) * 28.0f;
        pos.z += (Rand_ZeroOne() - 0.5f) * 28.0f;
        EnZo_UpdateSpawnBubble(self, &pos);
    }
    if ((s32)self->alpha != 0) {
        Collider_UpdateCylinder(actor, &self->collider);
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    }
    EnZo_UpdateEffectsRipples(self);
    EnZo_UpdateEffectsBubbles(self);
    EnZo_UpdateEffectsSplashes(self);
}

#endif
GLOBAL_ASM("asm/EnZo_Update.s")

#if 0
inline void* operator new(unsigned int, void* p) { return p; }
static void EnZo_SinCos(f32* sine, f32* cosine, f32 angle) {
    f32 a = EnZo_Fabsf(angle);
    while (a >= 65536.0f) a -= 65536.0f;
    u32 whole = (u16)(u32)a;
    f32 part = a - (f32)whole;
    const EnZoSinCosEntry* e = &EnZo_sSinCosTable[whole & 255];
    f32 sv = e->sine + part * e->sineDelta;
    *cosine = e->cosine + part * e->cosineDelta;
    *sine = angle < 0.0f ? -sv : sv;
}
static f32 EnZo_RotationProduct(f32 a, f32 b) { return a * b; }
static void EnZo_RotationXYZ(MTX34* m, const Vector3f& angles) {
    f32 cx, sx, cy, sy, cz, sz;
    EnZo_SinCos(&sx, &cx, angles.x);
    EnZo_SinCos(&sy, &cy, angles.y);
    EnZo_SinCos(&sz, &cz, angles.z);
    m->data[0][0] = EnZo_RotationProduct(cz, cy);
    m->data[1][0] = EnZo_RotationProduct(sz, cy);
    m->data[2][2] = EnZo_RotationProduct(cx, cy);
    m->data[2][1] = EnZo_RotationProduct(sx, cy);
    f32 a = EnZo_RotationProduct(cx, sz);
    f32 b = EnZo_RotationProduct(sx, cz);
    m->data[0][1] = b * sy - a;
    m->data[1][2] = a * sy - b;
    a = EnZo_RotationProduct(sx, sz);
    b = EnZo_RotationProduct(cx, cz);
    m->data[0][2] = a + b * sy;
    m->data[1][1] = b + a * sy;
    m->data[2][0] = -sy;
    m->data[0][3] = m->data[1][3] = m->data[2][3] = 0.0f;
}
static inline void EnZo_FlushBatch(void* resource) {
    EnZoParticleSub* sub = ((EnZoParticleObject*)resource)->sub;
    if (sub->particleCount != 0) FUN_00371eac(sub, 0);
}

void EnZo_Draw(Actor* actor, GameState* state) {
    EnZo* self = (EnZo*)actor;
    PlayState* play = (PlayState*)state;
    f32 prim[4], env[4];
    Vec3f scale;
    MTX34 rotation;
    EnZoEffect* effect;
    s32 i;

    effect = self->effects;
    Vector3f angles(0.0f, 128.0f, 0.0f);
    EnZo_RotationXYZ(&rotation, angles);
    FUN_0035619c(play, prim, env, 255, 255, 255, 255, 155, 155, 155);
    FUN_00342988(self->unk_C84, env, -1);
    for (i = 0; i < 15; i++, effect++) {
        if (effect->type == 1) {
            prim[3] = effect->color.a * (1.0f / 255.0f);
            new (&scale) Vector3f(effect->scale, 1.0f, effect->scale);
            FUN_003693b4(self->unk_C84, &effect->pos, NULL, &scale, prim, 0);
            FUN_003693b4(self->unk_C84, &effect->pos, &rotation, &scale, prim, 0);
        }
    }
    EnZo_FlushBatch(self->unk_C84);
    effect = self->effects;
    FUN_0035619c(play, prim, env, 255, 255, 255, 255, 150, 150, 150);
    FUN_00342988(self->unk_C80, env, -1);
    for (i = 0; i < 15; i++, effect++) {
        if (effect->type == 3) {
            new (&scale) Vector3f(effect->scale * 16.0f, 1.0f, effect->scale * 16.0f);
            FUN_003693b4(self->unk_C80, &effect->pos, NULL, &scale, prim, 0);
        }
    }
    EnZo_FlushBatch(self->unk_C80);
    effect = self->effects;
    FUN_0035619c(play, prim, env, 180, 180, 180, 255, 200, 200, 200);
    FUN_00342988(self->unk_C88, env, -1);
    for (i = 0; i < 15; i++, effect++) {
        if (effect->type == 2) {
            prim[3] = effect->color.a * (1.0f / 255.0f);
            new (&scale) Vector3f(effect->scale * 50.0f, effect->scale * 50.0f, 1.0f);
            FUN_003693b4(self->unk_C88, &effect->pos, NULL, &scale, prim, 0);
        }
    }
    EnZo_FlushBatch(self->unk_C88);    s16 eye = self->eyeTexture;
    FaceAnimations* face = &self->faceAnimations;
    face->FUN_0035e3a4(0, eye);
    self->faceAnimations.FUN_0035e330();
    if (self->alpha != 0.0f) {
        *(EnZoFloatColor*)prim = EnZo_sWhiteColor;
        prim[3] = self->alpha * (1.0f / 255.0f);
        void* material = FUN_003687a8(self->skelAnime.unk_28);
        FUN_003589cc(material, 4);
        FUN_00358964(material, 4, prim);
        SkelAnime_DrawOpa(&self->skelAnime, &actor->modelMtx, EnZo_OverrideLimbDraw, EnZo_PostLimbDraw, actor, 0);
    }
}

#endif
GLOBAL_ASM("asm/EnZo_Draw.s")

void EnZo_Dive(EnZo* self, PlayState* play) {
    if (FUN_0036e5e0(&self->skelAnime, self->skelAnime.endFrame, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, 0x01000160);
        FUN_00315ba8(self);
        self->actor.flags &= ~1;
        self->actor.velocity.y = -4.0f;
        self->skelAnime.playSpeed = 0.0f;
    }
    if (self->skelAnime.playSpeed > 0.0f) {
        return;
    }
    if (self->actor.yDistToWater > 80.0f || self->actor.bgCheckFlags & 1) {
        Math_ApproachF(&self->actor.velocity.y, -1.0f, 0.4f, 0.6f);
        Math_ApproachF(&self->alpha, 0.0f, 0.3f, 10.0f);
    }
    if ((s16)self->alpha == 0) {
        FUN_003717ac(&self->skelAnime, EnZo_sAnimationInfo, 2);
        self->actor.world.pos = self->actor.home.pos;
        self->alpha = 0.0f;
        self->actionFunc = EnZo_Submerged;
    }
}

void EnZo_Surface(EnZo* self, PlayState* play) {
    if (self->actor.yDistToWater < 54.0f) {
        Audio_PlayActorSound2(&self->actor, 0x0100015f);
        FUN_00315ba8(self);
        FUN_003717ac(&self->skelAnime, EnZo_sAnimationInfo, 8);
        self->actor.flags |= 1;
        self->actor.velocity.y = 0.0f;
        self->alpha = 255.0f;
        self->actionFunc = EnZo_TreadWater;
    } else if (self->actor.yDistToWater < 80.0f) {
        Math_ApproachF(&self->actor.velocity.y, 2.0f, 0.4f, 0.6f);
        Math_ApproachF(&self->alpha, 255.0f, 0.3f, 10.0f);
    }
}

#if 0
static inline void EnZo_SpawnRipple(EnZo* self, Vec3f* pos, f32 scale, f32 targetScale, u8 alpha) {
    EnZoEffect* effect = self->effects;
    s32 i;
    for (i = 0; i < 15; i++) {
        if (effect->type == 0) {
            effect->type = 1;
            effect->pos = *pos;
            effect->scale = scale;
            effect->targetScale = targetScale;
            effect->color.a = alpha;
            break;
        }
        effect++;
    }
}
static inline void EnZo_TreadWaterRipples(EnZo* self, f32 scale, f32 targetScale, u8 alpha) {
    Vector3f pos(0.0f, 0.0f, 0.0f);
    pos.x = self->actor.world.pos.x;
    pos.y = self->actor.world.pos.y + self->actor.yDistToWater;
    pos.z = self->actor.world.pos.z;
    EnZo_SpawnRipple(self, (Vec3f*)&pos, scale, targetScale, alpha);
}
static inline s32 EnZo_TreadProximity(EnZo* self, PlayState* play) {
    Actor* player = (Actor*)GET_PLAYER(play);
    Vector3f surfacePos(self->actor.world.pos.x, self->actor.world.pos.y + self->actor.yDistToWater, self->actor.world.pos.z);
    Vector3f diff = EnZo_VecSub((Vector3f*)&player->world.pos, &surfacePos);
    f32 hDist = __sqrtf(SQ(diff.x) + SQ(diff.z));
    f32 yDist = EnZo_Fabsf(diff.y);
    return hDist < 240.0f && yDist < 80.0f;
}
static inline void EnZo_ReverseAnimation(EnZo* self) {
    f32 startFrame;
    SkelAnime* sa = &self->skelAnime;
    startFrame = sa->startFrame;
    sa->startFrame = sa->endFrame;
    sa->curFrame = sa->endFrame;
    sa->endFrame = startFrame;
    sa->playSpeed = -1.0f;
}
static inline void EnZo_BeginDive(EnZo* self, u8* speak) {
    FUN_003717ac(&self->skelAnime, EnZo_sAnimationInfo, 4);
    EnZo_ReverseAnimation(self);
    self->canSpeak = *speak;
    self->trackingMode = 1;
    self->actionFunc = EnZo_Dive;
}
void EnZo_TreadWater(EnZo* self, PlayState* play) {
    FUN_00370f5c(play, self->fidgetTableY, self->fidgetTableZ, 21);
    if (self->skelAnime.animationType != 6 && FUN_0036e5e0(&self->skelAnime, self->skelAnime.endFrame, 1.0f)) {
        self->skelAnime.playSpeed = 0.0f;
        self->canSpeak = true;
        self->trackingMode = 4;
        FUN_003717ac(&self->skelAnime, EnZo_sAnimationInfo, 9);
    }
    Math_ApproachF(&self->actor.velocity.y, self->actor.yDistToWater < 54.0f ? -0.3f : 0.3f, 0.15f, 0.1f);
    if (self->rippleTimer != 0) {
        self->rippleTimer--;
        if (self->rippleTimer == 5 || self->rippleTimer == 9) {
            EnZo_TreadWaterRipples(self, 0.2f, 1.0f, 200);
        }
    } else {
        EnZo_TreadWaterRipples(self, 0.2f, 1.0f, 200);
        self->rippleTimer = 18;
    }
    if (EnZo_TreadProximity(self, play)) {
        self->timeToDive = Rand_S16Offset_003702c8(40, 40);
    } else if (DECR(self->timeToDive) == 0) {
        u8 zero = 0;
        EnZo_BeginDive(self, &zero);
    }
}

#endif
GLOBAL_ASM("asm/EnZo_TreadWater.s")

#if 0
static inline void EnZo_SpawnSplash(EnZo* self, Vec3f* pos, Vec3f* vel, f32 scale) {
    EnZoEffect* effect;
    Vec3f accel;
    Vector3f nativeAccel(0.0f, -1.0f, 0.0f);
    Vec3f* accelPtr = &accel;
    *accelPtr = *(Vec3f*)&nativeAccel;
    s16 i;
    effect = self->effects;
    for (i = 0; i < 15; i++, effect++) {
        if (effect->type == 2) {
            continue;
        }
        effect->type = 2;
        effect->pos = *pos;
        effect->accel = accel;
        effect->vel = *vel;
        effect->color.a = (Rand_ZeroOne() * 100.0f) + 100.0f;
        effect->scale = scale;
        break;
    }
}
void FUN_00315ba8(EnZo* self) {
    Vec3f pos;
    Vec3f vel;
    s32 i;
    Vec3f* posPtr = &pos;
    Vec3f* velPtr = &vel;
    for (i = 0; i < 13; i++) {
        f32 speed = Rand_ZeroOne() * 1.5f + 0.5f;
        f32 angle = Rand_ZeroOne() * 6.28f;
        vel.y = Rand_ZeroOne() * 3.0f + 3.0f;
        vel.x = __hardfp_sinf(angle) * speed;
        vel.z = __hardfp_cosf(angle) * speed;
        pos = self->actor.world.pos;
        pos.x += vel.x * 6.0f;
        pos.z += vel.z * 6.0f;
        pos.y += self->actor.yDistToWater;
        EnZo_SpawnSplash(self, posPtr, velPtr, 0.08f);
    }
}



#endif
GLOBAL_ASM("asm/FUN_00315ba8.s")

static inline void EnZo_SetEventFlag(s32 flag) {
    gSaveContext.eventChkInf[flag >> 4] |= 1 << (flag & 15);
}
static inline s32 EnZo_TextId(Actor* actor) { return actor->textId; }
static inline s16 EnZo_CloseTalk(Actor* actor) {
    switch (actor->textId) {
        case 0x4020:
        case 0x4021:
            return 0;
        case 0x4008:
            gSaveContext.infTable[18] |= 0x10;
            break;
        case 0x402F:
            gSaveContext.infTable[18] |= 0x200;
            break;
    }
    EnZo_SetEventFlag(0x30);
    return 0;
}

s16 FUN_001889a0(PlayState* play, Actor* actor) {
    switch (Message_GetState(&play->msgCtx)) {
        case 0:
        case 1:
        case 3:
        case 6:
        case 7:
        case 8:
        case 9:
            return 1;
        case 2:
            return EnZo_CloseTalk(actor);
        case 4:
            switch (Message_ShouldAdvance(play)) {
                case 0:
                    return 1;
                default:
                    EnZo* self = (EnZo*)actor;
                    switch (EnZo_TextId(&self->actor)) {
                    case 0x400C:
                        self->actor.textId = FUN_00369f3c(play) == 0 ? 0x400D : 0x400E;
                        Message_ContinueTextbox(play, self->actor.textId);
                        break;
                    }
                    break;
            }
            return 1;
        case 5:
            switch (Message_ShouldAdvance(play)) {
                case 0:
                    return 1;
                default:
                    return 2;
            }
    }
    return 1;
}

}
