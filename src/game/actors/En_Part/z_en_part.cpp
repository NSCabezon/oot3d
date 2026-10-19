extern "C" {
#include "z_en_part.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x00000010

void EnPart_Init(Actor* actor, GameState* state);
void EnPart_Destroy(Actor* actor, GameState* state);
void EnPart_Update(Actor* actor, GameState* state);
void EnPart_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Part.data.s")

GLOBAL_ASM("../src/game/actors/En_Part/syms.s")

#define R_UPDATE_RATE SREG(30)
#define ADJUST_FRAMES(n) ((s32)((f32)(3 * (n)) / R_UPDATE_RATE + 0.5f))

void SystemArena_Free(void* ptr);
void EffectSsEnFire_SpawnVec3f(PlayState* play, Actor* actor, Vec3f* pos, s16 scale, s16 params, s16 flags, s16 bodyPart,
                               s32 unk);
void EffectSsDeadDb_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep, s16 primR,
                          s16 primG, s16 primB, s16 primA, s16 envR, s16 envG, s16 envB, s32 unk, s32 life, s32 playSfx);
void EffectSsDtBubble_SpawnColorProfile(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 size, s16 life,
                                        s32 colorProfile, s32 random);
s16 Rand_S16Offset(s16 base, s16 range);
void EffectSsHahen_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 flags, s16 scale, s16 objectSlot,
                         s16 params, void* dList);
f32 sinf(f32 x);
f32 cosf(f32 x);
void FUN_003589cc(void* arg0, s32 arg1);
void FUN_00358964(void* arg0, s32 arg1, void* arg2);
typedef struct {
    f32 v[4];
} EnPartColor;
extern EnPartColor EnPart_sColor;
void FUN_003679d0(f32 x, f32 y, f32 z, MTX34* mtx, Vec3s* rot);
void FUN_0036c174(MTX34* dst, MTX34* a, MTX34* b);
typedef void (*EnPartActionFunc)(EnPart*, PlayState*);
typedef void (*EnPartParticleFunc)(EnPart*, PlayState*, EnPartParticle*);
extern EnPartActionFunc EnPart_sActionFuncs[];
extern EnPartParticleFunc EnPart_sParticleFuncs[];
void FUN_00374bb8(PlayState* play, Actor* actor, f32 arg2, s16 arg3, f32 arg4);
void FUN_0037378c(PlayState* play, Vec3f* pos, f32 arg2, s32 arg3, s16 arg4, s16 arg5, u8 arg6);

ActorInit En_Part_InitVars = {
    ACTOR_EN_PART,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnPart),
    (ActorFunc)EnPart_Init,
    (ActorFunc)EnPart_Destroy,
    (ActorFunc)EnPart_Update,
    (ActorFunc)EnPart_Draw,
};

void EnPart_Init(Actor* actor, GameState* state) {
    EnPart* self = (EnPart*)actor;

    self->action = 0xFF;
    self->unk_1A8 = -1;
}

void EnPart_Destroy(Actor* actor, GameState* state) {
    EnPart* self = (EnPart*)actor;

    if (self->unk_1C4 != NULL) {
        self->unk_1C4->Destroy();
    }
    if (self->skelAnimModel != NULL) {
        FUN_00350f34(actor, &self->skelAnimModel, 0);
    }
    if (self->particles != NULL) {
        SystemArena_Free(self->particles);
    }
}

void FUN_00282820(EnPart* self, PlayState* play) {
    self->action = 3;
}

void FUN_0037ea38(EnPart* self, PlayState* play2) {
    f32 sign = 1.0f;
    PlayState* play = play2;

    self->action = 1;
    self->actor.world.rot.y = Rand_ZeroOne() * 20000.0f;

    switch (self->actor.params) {
        case 0:
            self->actor.velocity.y = 0.0f;
            self->actor.gravity = -0.3f - Rand_ZeroOne() * 0.5f;
            self->rotZSpeed = 0.3f;
            self->timer = ADJUST_FRAMES(25);
            self->actor.speedXZ = (Rand_ZeroOne() - 0.5f) * 2.0f;
            break;
        case 13:
            self->timer = ADJUST_FRAMES(400);
        case 12:
            self->actor.speedXZ = Rand_CenteredFloat(6.0f);
            self->actor.home.pos = self->actor.world.pos;
            self->timer += ADJUST_FRAMES(60);
            self->actor.velocity.y = Rand_ZeroOne() * 5.0f + 4.0f;
            self->actor.gravity = -0.6f - Rand_ZeroOne() * 0.5f;
            self->rotZSpeed = 0.15f;
            break;
        case 14:
            EffectSsEnFire_SpawnVec3f(play, &self->actor, &self->actor.world.pos, 40, 0x8001, 0, -1, 1);
        case 1:
        case 4:
        case 9:
        case 10:
            self->timer += (s32)(((s16)(Rand_ZeroOne() * 17.0f) + 5) * 3.0f / R_UPDATE_RATE + 0.5f);
        case 2:
            self->actor.velocity.y = Rand_ZeroOne() * 5.0f + 4.0f;
            self->actor.gravity = -0.6f - Rand_ZeroOne() * 0.5f;
            self->rotZSpeed = 0.15f;
            break;
        case 11:
            EffectSsEnFire_SpawnVec3f(play, &self->actor, &self->actor.world.pos, 40, 0x8001, 0, -1, 1);
        case 3:
            self->actor.speedXZ = (Rand_ZeroOne() - 0.5f) * 3.0f;
            self->timer = (s32)(((s16)(Rand_ZeroOne() * 17.0f) + 10) * 3.0f / R_UPDATE_RATE + 0.5f);
            self->actor.velocity.y = Rand_ZeroOne() * 3.0f + 8.0f;
            self->actor.gravity = -0.6f - Rand_ZeroOne() * 0.3f;
            self->rotZSpeed = 0.15f;
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            self->actor.world.rot.y = self->actor.parent->shape.rot.y;
            if (self->unk_1A8 == 1) {
                sign = -1.0f;
            }
            self->actor.speedXZ = 6.0f * sign;
            self->actor.velocity.y = 0.0f;
            self->actor.gravity = -1.2f;
            self->rotZSpeed = 0.15f * sign;
            ActorShape_Init(&self->actor.shape, 0.0f, (ActorShadowFunc)0x002096a0, 30.0f);
            self->timer = ADJUST_FRAMES(18);
            break;
    }
}

// NOT MATCHED (layout only): C below is semantically identical, but the literal pool is flushed 0x4C bytes later than in
// the ROM (ROM: pool before the type==4 block with a `b`), shifting every later pc-relative load. GLOBAL_ASM stays in
// src/functions/functions_280000s.cpp.
#if 0
static inline void EnPart_AdjustPos(EnPart* self, EnPartParticle* p) {
    if (p->type == 4) {
        Vec3f* q = &self->particles[3].pos;
        f32 x = p->pos.x - q->x;
        f32 y = p->pos.y - q->y;
        f32 z = p->pos.z - q->z;
        p->pos.x = x;
        p->pos.y = y;
        p->pos.z = z;
    } else if (p->type == 6) {
        Vec3f* q = &self->particles[5].pos;
        f32 x = p->pos.x - q->x;
        f32 y = p->pos.y - q->y;
        f32 z = p->pos.z - q->z;
        p->pos.x = x;
        p->pos.y = y;
        p->pos.z = z;
    }
}

void FUN_00281edc(EnPart* self, PlayState* play2, EnPartParticle* p) {
    PlayState* play = play2;
    f32 sign = 1.0f;

    p->action = 1;

    switch (self->actor.params) {
        case 0:
            p->velocity.y = 0.0f;
            p->gravity = -0.3f - Rand_ZeroOne() * 0.5f;
            p->rotZSpeed = 0.3f;
            p->timer = ADJUST_FRAMES(25);
            p->speedXZ = (Rand_ZeroOne() - 0.5f) * 2.0f;
            break;
        case 13:
            p->timer = ADJUST_FRAMES(400);
        case 12:
            p->speedXZ = Rand_CenteredFloat(6.0f);
            p->timer += ADJUST_FRAMES(60);
            p->velocity.y = Rand_ZeroOne() * 5.0f + 4.0f;
            p->gravity = -0.6f - Rand_ZeroOne() * 0.5f;
            p->rotZSpeed = 0.15f;
            break;
        case 14:
            EffectSsEnFire_SpawnVec3f(play, &self->actor, &p->pos, 40, 0x8001, 0, -1, 1);
        case 1:
        case 9:
        case 10:
            p->timer += (s32)(((s16)(Rand_ZeroOne() * 17.0f) + 5) * 3.0f / R_UPDATE_RATE + 0.5f);
            p->velocity.y = Rand_ZeroOne() * 5.0f + 4.0f;
            p->gravity = -0.6f - Rand_ZeroOne() * 0.5f;
            p->rotZSpeed = 0.15f;
            break;
        case 4:
            p->timer += ADJUST_FRAMES(13);
        case 2:
            p->velocity.y = Rand_ZeroOne() * 5.0f + 4.0f;
            p->gravity = -0.6f - Rand_ZeroOne() * 0.5f;
            p->rotZSpeed = 0.15f;
            break;
        case 11:
            EffectSsEnFire_SpawnVec3f(play, &self->actor, &p->pos, 40, 0x8001, 0, -1, 1);
        case 3:
            p->speedXZ = (Rand_ZeroOne() - 0.5f) * 3.0f;
            p->timer = (s32)(((s16)(Rand_ZeroOne() * 17.0f) + 10) * 3.0f / R_UPDATE_RATE + 0.5f);
            p->velocity.y = Rand_ZeroOne() * 3.0f + 8.0f;
            p->gravity = -0.6f - Rand_ZeroOne() * 0.3f;
            p->rotZSpeed = 0.15f;
            EnPart_AdjustPos(self, p);
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            p->rot.y = self->actor.parent->shape.rot.y;
            if (p->type == 0x11) {
                sign = -1.0f;
            }
            p->speedXZ = 6.0f * sign;
            p->velocity.y = 0.0f;
            p->gravity = -1.2f;
            p->rotZSpeed = 0.15f * sign;
            p->timer = ADJUST_FRAMES(18);
            break;
    }
}
#endif

void FUN_002823f4(EnPart* self, PlayState* play, EnPartParticle* p) {
    EnPart* s2 = self;
    s32 i;
    Vec3f pos;
    Vector3f velocity(0.0f, 0.0f, 0.0f);
    Vector3f accel(0.0f, 0.0f, 0.0f);
    Vector3f zeroVec(0.0f, 0.0f, 0.0f);

    if ((self->actor.params == 12) || (self->actor.params == 13)) {
        self->actor.world.pos = p->pos;
        s2->actor.prevPos = p->prevPos;
        Actor_UpdateBgCheckInfo(play, &self->actor, 5.0f, 15.0f, 0.0f, 0x1D);
        p->pos = self->actor.world.pos;

        if ((self->actor.bgCheckFlags & 1) || (self->actor.world.pos.y <= self->actor.floorHeight)) {
            p->action = 4;
            p->speedXZ = 0.0f;
            p->gravity = 0.0f;
            p->velocity.y = 0.0f;
        }

        if ((self->actor.params == 13) && (self->actor.parent != NULL) && (self->actor.parent->update == NULL)) {
            self->actor.parent = NULL;
        }
    } else if (p->timer <= 0) {
        switch (self->actor.params) {
            case 1:
            case 9:
            case 10:
            case 14:
                EffectSsDeadDb_Spawn(play, &p->pos, (Vec3f*)&zeroVec, (Vec3f*)&zeroVec, (s32)(self->actor.scale.y * 100.0f) * 40, 7, 255,
                                     255, 255, 255, 0, 255, 0, 1, 11, 1);
                break;
            case 3:
            case 11:
                if ((p->type != 4) && (p->type != 6)) {
                    EffectSsDeadDb_Spawn(play, &p->pos, (Vec3f*)&zeroVec, (Vec3f*)&zeroVec, (s32)(self->actor.scale.y * 100.0f) * 40, 7,
                                         255, 255, 255, 255, 0, 0, 255, 1, 11, 1);
                }
                break;
            case 4:
                for (i = 7; i >= 0; i--) {
                    pos.x = p->pos.x + Rand_CenteredFloat(60.0f);
                    pos.y = p->pos.y + Rand_CenteredFloat(50.0f);
                    pos.z = p->pos.z + Rand_CenteredFloat(60.0f);
                    velocity.y = Rand_ZeroOne() + 1.0f;
                    EffectSsDtBubble_SpawnColorProfile(play, &pos, (Vec3f*)&velocity, (Vec3f*)&accel, Rand_S16Offset(80, 100), 25, 0, 1);
                }
                break;
            case 5:
            case 6:
            case 7:
            case 8:
                for (i = 4; i >= 0; i--) {
                    pos.x = p->pos.x + Rand_CenteredFloat(25.0f);
                    pos.y = p->pos.y + Rand_CenteredFloat(40.0f);
                    pos.z = p->pos.z + Rand_CenteredFloat(25.0f);
                    EffectSsDeadDb_Spawn(play, &pos, (Vec3f*)&zeroVec, (Vec3f*)&zeroVec, 40, 7, 255, 255, 255, 255, 0, 0, 255, 1, 11, 1);
                }
                break;
        }

        p->action = 5;
        return;
    }

    p->rotZ += p->rotZSpeed * (R_UPDATE_RATE / 3.0f);
    p->timer--;
}

void FUN_0037eea0(EnPart* self, PlayState* play) {
    s32 i;
    Vec3f pos;
    Vector3f velocity(0.0f, 0.0f, 0.0f);
    Vector3f accel(0.0f, 0.0f, 0.0f);
    Vector3f zeroVec(0.0f, 0.0f, 0.0f);
    EnPart* s2 = self;

    if ((self->actor.params == 12) || (self->actor.params == 13)) {
        Actor_UpdateBgCheckInfo(play, &self->actor, 5.0f, 15.0f, 0.0f, 0x1D);

        if ((s2->actor.bgCheckFlags & 1) || (self->actor.world.pos.y <= self->actor.floorHeight)) {
            self->action = 4;
            self->actor.speedXZ = 0.0f;
            self->actor.gravity = 0.0f;
            self->actor.velocity.y = 0.0f;
        }

        if ((self->actor.params == 13) && (self->actor.parent != NULL) && (self->actor.parent->update == NULL)) {
            self->actor.parent = NULL;
        }
    } else if (self->timer <= 0) {
        switch (self->actor.params) {
            case 1:
            case 9:
            case 10:
            case 14:
                EffectSsDeadDb_Spawn(play, &self->actor.world.pos, (Vec3f*)&zeroVec, (Vec3f*)&zeroVec,
                                     (s32)(self->actor.scale.y * 100.0f) * 40, 7, 255, 255, 255, 255, 0, 255, 0, 1, 11, 1);
                break;
            case 3:
            case 11:
                EffectSsDeadDb_Spawn(play, &self->actor.world.pos, (Vec3f*)&zeroVec, (Vec3f*)&zeroVec,
                                     (s32)(self->actor.scale.y * 100.0f) * 40, 7, 255, 255, 255, 255, 0, 0, 255, 1, 11, 1);
                break;
            case 4:
                for (i = 7; i >= 0; i--) {
                    pos.x = self->actor.world.pos.x + Rand_CenteredFloat(60.0f);
                    pos.y = self->actor.world.pos.y + self->actor.shape.yOffset * self->actor.scale.y +
                            Rand_CenteredFloat(50.0f);
                    pos.z = self->actor.world.pos.z + Rand_CenteredFloat(60.0f);
                    velocity.y = Rand_ZeroOne() + 1.0f;
                    EffectSsDtBubble_SpawnColorProfile(play, &pos, (Vec3f*)&velocity, (Vec3f*)&accel,
                                                       Rand_S16Offset(80, 100), 25, 0, 1);
                }
                break;
            case 5:
            case 6:
            case 7:
            case 8:
                for (i = 4; i >= 0; i--) {
                    pos.x = self->actor.world.pos.x + Rand_CenteredFloat(25.0f);
                    pos.y = self->actor.world.pos.y + Rand_CenteredFloat(40.0f);
                    pos.z = self->actor.world.pos.z + Rand_CenteredFloat(25.0f);
                    EffectSsDeadDb_Spawn(play, &pos, (Vec3f*)&zeroVec, (Vec3f*)&zeroVec, 40, 7, 255, 255, 255, 255, 0, 0, 255,
                                         1, 11, 1);
                }
                break;
        }

        Actor_Kill(&self->actor);
        return;
    }

    self->rotZ += self->rotZSpeed * (R_UPDATE_RATE / 3.0f);
    self->timer--;
}

void FUN_0026c56c(EnPart* self, PlayState* play, EnPartParticle* p) {
    Vector3f zeroVec(0.0f, 0.0f, 0.0f);
    EnPart* s2 = self;

    if ((s2->actor.parent == NULL) || (self->actor.parent->update == NULL)) {
        EffectSsDeadDb_Spawn(play, &p->pos, (Vec3f*)&zeroVec, (Vec3f*)&zeroVec, (s32)(self->actor.scale.y * 100.0f) * 40, 7,
                             255, 255, 255, 255, 0, 255, 0, 1, 11, 1);
        p->action = 5;
        return;
    }

    if (p->timer == 0) {
        f32 diffsSum = Math_SmoothStepToF(&p->pos.x, p->home.x, 1.0f, 5.0f, 0.0f);

        diffsSum += Math_SmoothStepToF(&p->pos.y, p->home.y, 1.0f, 5.0f, 0.0f);
        diffsSum += Math_SmoothStepToF(&p->pos.z, p->home.z, 1.0f, 5.0f, 0.0f);
        diffsSum += Math_SmoothStepToF(&p->rotZ, 0.0f, 1.0f, 0.25f, 0.0f);
        if (ABS(diffsSum) == 0.0f) {
            self->actor.parent->home.rot.x--;
            p->timer--;
            Audio_PlayActorSound2(&self->actor, 0x0100046D);
        }
    } else if (p->timer > 0) {
        p->timer--;
    }

    if (self->actor.parent->colChkInfo.health != 0) {
        p->action = 5;
    }
}

void FUN_002b2940(EnPart* self, PlayState* play) {
    f32 a, b, c;
    a = 0.0f;
    c = 0.0f;
    b = 0.0f;
    Vector3f zeroVec(c, b, a);

    if ((self->actor.parent == NULL) || (self->actor.parent->update == NULL)) {
        EffectSsDeadDb_Spawn(play, &self->actor.world.pos, (Vec3f*)&zeroVec, (Vec3f*)&zeroVec,
                             (s32)(self->actor.scale.y * 100.0f) * 40, 7, 255, 255, 255, 255, 0, 255, 0, 1, 11, 1);
        Actor_Kill(&self->actor);
        return;
    }

    if (self->timer == 0) {
        f32 diffsSum = Math_SmoothStepToF(&self->actor.world.pos.x, self->actor.home.pos.x, 1.0f, 5.0f, 0.0f);

        diffsSum += Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.home.pos.y, 1.0f, 5.0f, 0.0f);
        diffsSum += Math_SmoothStepToF(&self->actor.world.pos.z, self->actor.home.pos.z, 1.0f, 5.0f, 0.0f);
        diffsSum += Math_SmoothStepToF(&self->rotZ, 0.0f, 1.0f, 0.25f, 0.0f);
        if (diffsSum == 0.0f) {
            self->actor.parent->home.rot.x--;
            self->timer--;
            Audio_PlayActorSound2(&self->actor, 0x0100046D);
        }
    } else if (self->timer > 0) {
        self->timer--;
    }

    if (self->actor.parent->colChkInfo.health != 0) {
        Actor_Kill(&self->actor);
    }
}

void FUN_0028282c(EnPart* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    self->timer--;
    if (self->timer == 0) {
        Actor_Kill(&self->actor);
    } else {
        Vector3f velocity(0.0f, 8.0f, 0.0f);
        Vector3f accel(0.0f, -1.5f, 0.0f);

        f32 dist = __sqrtf(self->actor.xyzDistToPlayerSq);
        if (dist <= 40.0f) {
            u8 prevInvincibilityTimer = *((u8*)player + 0x2488);

            if (*((s8*)player + 0x2488) <= 0) {
                if (*((s8*)player + 0x2488) <= -40) {
                    *((u8*)player + 0x2488) = 0;
                } else {
                    *((u8*)player + 0x2488) = 0;
                    (*(void (**)(PlayState*, s32))((u8*)play + 0x5BAC))(play, -8);
                }
            }
            FUN_00374bb8(play, self->actor.parent, (650.0f - self->actor.parent->xzDistToPlayer) * 0.04f + 4.0f,
                         self->actor.parent->world.rot.y, 8.0f);
            *((u8*)player + 0x2488) = prevInvincibilityTimer;
            self->timer = 1;
        }

        FUN_0037378c(play, &self->actor.world.pos, 0.0f, 1, 300, 150, 1);
        velocity.x = Rand_CenteredFloat(16.0f);
        EffectSsHahen_Spawn(play, &self->actor.world.pos, (Vec3f*)&velocity, (Vec3f*)&accel, 20,
                            (s32)((Rand_ZeroOne() * 5.0f + 12.0f) * 2), -1, 10, NULL);
        Audio_PlayActorSound2(&self->actor, 0x01000350);
    }
}

// EnPart_Update: NOT MATCHED (recipe R9 MakeScale, 191/336 words, tag: zero-CSE + VFP numbering). The per-particle scale
// matrix now uses the shared MTX34_MakeScale helper (row-major u32 copy; its store schedule matches the ROM). Draft found
// the ROM shapes for: play kept in the push arg-home slot (`#define play ((PlayState*)state2)`, no local), skyboxOffset via
// play->mainCamera, no tail call / stack layout (MTX34 in the inner block), Vec3f diff via out-param helper returned by
// value (2 temps per branch), velocity store before the bgCheckFlags clear through a 2nd alias. Still off: ROM keeps the
// MakeScale zero rematerialized (`mov r0,#0`) where armcc hoists it to sl (so the type-6 temp address is not hoisted and
// the branch tails don't cross-jump), entry `ldr r0,[sp,#0x88]` hoist, VFP numbering (ROM s18=0.5 s21=1.0 s17=1/3 s20=sin).
// Needs prototype `void EnPart_Update(Actor*, PlayState*)` when enabled. EQUs EnPart_sActionFuncs/sParticleFuncs in syms.s.
#if 0
static inline void EnPart_Vec3fDiff(Vec3f* a, Vec3f* b, Vec3f* dest) {
    dest->x = a->x - b->x;
    dest->y = a->y - b->y;
    dest->z = a->z - b->z;
}

static inline Vec3f EnPart_VecSub(Vec3f* a, Vec3f* b) {
    Vec3f r;

    EnPart_Vec3fDiff(a, b, &r);
    return r;
}

void EnPart_Update(Actor* thisx, PlayState* state2) {
    EnPart* self = (EnPart*)thisx;
#define play ((PlayState*)state2)
    s32 i;
    s32 allDone;

    self->unk_1D0 = 1;

    if (self->action != 0xFF) {
        Actor_MoveForward(&self->actor);

        if (((u32)(self->actor.params - 5) <= 3) || (self->actor.params < 0)) {
            Actor_UpdateBgCheckInfo(play, &self->actor, 5.0f, 15.0f, 0.0f, 5);
            if (self->actor.params >= 0) {
                Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);
                if (self->actor.bgCheckFlags & 1) {
                    self->actor.velocity.y = 6.0f;
                    thisx->bgCheckFlags &= ~1;
                }
            }
        }

        EnPart_sActionFuncs[self->action](self, play);
        return;
    }

    allDone = 1;
    for (i = 0; i < self->numParticles; i++) {
        EnPartParticle* p = &self->particles[i];

        p->prevPos = p->pos;
        if (p->action == 5) {
            p->type = -1;
            continue;
        }
        allDone = 0;

        if (!(((self->actor.params == 3) || (self->actor.params == 11)) && ((p->type == 4) || (p->type == 6)))) {
            p->velocity.x = Math_SinS(p->rot.y) * p->speedXZ;
            p->velocity.z = Math_CosS(p->rot.y) * p->speedXZ;
            p->velocity.y += p->gravity * (R_UPDATE_RATE * (1.0f / 3.0f));
            if (p->velocity.y < -20.0f) {
                p->velocity.y = -20.0f;
            }
            {
                f32 step = R_UPDATE_RATE * 0.5f;

                p->pos.x += p->velocity.x * step;
                p->pos.y += p->velocity.y * step;
                p->pos.z += p->velocity.z * step;
            }

            if (((u32)(self->actor.params - 5) <= 3) && (self->actor.params >= 0)) {
                Math_SmoothStepToF(&p->speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);
            }

            EnPart_sParticleFuncs[p->action](self, play, p);

            if (self->actor.flags & 0x1000) {
                FUN_003679d0(p->pos.x + play->mainCamera.skyboxOffset.x,
                             p->pos.y + play->mainCamera.skyboxOffset.y + self->actor.shape.yOffset * self->actor.scale.y,
                             p->pos.z + play->mainCamera.skyboxOffset.z, &p->mtx, &p->rot);
            } else {
                FUN_003679d0(p->pos.x, p->pos.y + self->actor.shape.yOffset * self->actor.scale.y, p->pos.z, &p->mtx,
                             &p->rot);
            }

            {
                MTX34 sm;

                MTX34_MakeScale(&sm, &self->actor.scale);
                FUN_0036c174(&p->mtx, &p->mtx, &sm);
            }

            if ((self->actor.params > 0) && (p->rotZ != 0.0f)) {
                f32 angle = p->rotZ;
                f32 sn = sinf(angle);
                f32 cs = cosf(angle);
                f32 a;
                f32 b;

                a = p->mtx.data[0][0];
                b = p->mtx.data[0][1];
                p->mtx.data[0][0] = a * cs + b * sn;
                p->mtx.data[0][1] = b * cs - a * sn;
                a = p->mtx.data[1][0];
                b = p->mtx.data[1][1];
                p->mtx.data[1][0] = a * cs + b * sn;
                p->mtx.data[1][1] = b * cs - a * sn;
                a = p->mtx.data[2][0];
                b = p->mtx.data[2][1];
                p->mtx.data[2][0] = a * cs + b * sn;
                p->mtx.data[2][1] = b * cs - a * sn;
            }
        } else {
                if (p->action == 0) {
                    if (p->type == 4) {
                        p->pos = EnPart_VecSub(&p->pos, &self->particles[i - 1].pos);
                    } else if (p->type == 6) {
                        p->pos = EnPart_VecSub(&p->pos, &self->particles[i - 1].pos);
                    }
                    p->action = 1;
                }
                if (self->particles[i - 1].type != -1) {
                    p->mtx = self->particles[i - 1].mtx;
                    p->mtx.data[0][3] += p->pos.x;
                    p->mtx.data[1][3] += p->pos.y;
                    p->mtx.data[2][3] += p->pos.z;
                } else {
                    p->action = 5;
                    p->type = -1;
                }
        }
    }
    if (allDone != 0) {
        Actor_Kill(&self->actor);
    }
}
#undef play
#endif

GLOBAL_ASM("asm/EnPart_Update.s")

void EnPart_Draw(Actor* actor, GameState* state) {
    EnPart* self = (EnPart*)actor;

    if ((self->skelAnimModel != NULL) && (self->unk_1D0 != 0)) {
        if ((self->action != 0xFF) && (actor->params > 0)) {
            self->drawMtx = actor->modelMtx;
            if (self->rotZ != 0.0f) {
                f32 angle = self->rotZ;
                f32 sn = sinf(angle);
                f32 cs = cosf(angle);
                f32 a;
                f32 b;

                a = self->drawMtx.data[0][0];
                b = self->drawMtx.data[0][1];
                self->drawMtx.data[0][0] = a * cs + b * sn;
                self->drawMtx.data[0][1] = b * cs - a * sn;
                a = self->drawMtx.data[1][0];
                b = self->drawMtx.data[1][1];
                self->drawMtx.data[1][0] = a * cs + b * sn;
                self->drawMtx.data[1][1] = b * cs - a * sn;
                a = self->drawMtx.data[2][0];
                b = self->drawMtx.data[2][1];
                self->drawMtx.data[2][0] = a * cs + b * sn;
                self->drawMtx.data[2][1] = b * cs - a * sn;
            }
        }

        FUN_00357fd0(GET_PLAYER((PlayState*)state), actor->unk_178, &actor->world.pos);

        if (actor->params == 4) {
            EnPartParticle* p0 = self->particles;
            s16 timer = p0->timer;
            EnPartColor color = EnPart_sColor;

            if (timer < 7) {
                color.v[3] = timer * (1.0f / 7.0f);
            }
            FUN_003589cc(actor->unk_178, 4);
            FUN_00358964(actor->unk_178, 4, &color);
        }

        self->skelAnimModel->FUN_00372170(0);
    }
}
}
