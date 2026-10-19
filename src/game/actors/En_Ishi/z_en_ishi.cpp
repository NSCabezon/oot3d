extern "C" {
#include "z_en_ishi.hpp"

#define FLAGS 0x80C00000

void EnIshi_Init(Actor* actor, GameState* state);
void EnIshi_Destroy(Actor* actor, GameState* state);
void EnIshi_Update(Actor* actor, GameState* state);
void EnIshi_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Ishi.data.s")
GLOBAL_ASM("../src/game/actors/En_Ishi/data_syms.s")

extern ActorFunc EnIshi_sDrawFuncs[];

typedef void (*EnIshiEffectFunc)(EnIshi* self, PlayState* play);

extern s16 EnIshi_sRotSpeed[];
extern u32 EnIshi_sBreakSfxIds[];
#define EnIshi_sBreakSfxDurations ((u8*)((u32)EnIshi_sBreakSfxIds - 0x30))
extern CollisionCheckInfoInit EnIshi_sColChkInfoInit;
extern f32 EnIshi_sRockScales[];
extern f32 EnIshi_sYOffsets[];
extern f32 EnIshi_sDragCoefs[];
extern EnIshiEffectFunc EnIshi_sFragmentSpawnFuncs[];
extern EnIshiEffectFunc EnIshi_sDustSpawnFuncs[];
extern ColliderCylinderInit EnIshi_sCylinderInits[];
extern InitChainEntry EnIshi_sInitChains[][5];
extern u32 EnIshi_sLiftSfxIds[];

f32 BgCheck_EntityRaycastFloor4(CollisionContext* colCtx, CollisionPoly** outPoly, s32* bgId, Actor* actor,
                                Vec3f* pos);
void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* pos, s16 params);
s32 Quake_Add(Camera* camera, s32 type);
void Quake_SetSpeed(s32 quakeIndex, s32 speed);
void Quake_SetQuakeValues(s32 quakeIndex, s32 a, s32 b, s32 c, s32 d);
void Quake_SetCountdown(s32 quakeIndex, s32 countdown);
void EffectSsGSplash_Spawn(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, s16 type, s16 scale);
void EffectSsGRipple_Spawn(PlayState* play, Vec3f* pos, s16 radius, s16 maxRadius, s16 life);

void EnIshi_Wait(EnIshi* self, PlayState* play);
void EnIshi_LiftedUp(EnIshi* self, PlayState* play);
void EnIshi_Fly(EnIshi* self, PlayState* play);

ActorInit En_Ishi_InitVars = {
    ACTOR_EN_ISHI,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_FIELD_KEEP,
    sizeof(EnIshi),
    (ActorFunc)EnIshi_Init,
    (ActorFunc)EnIshi_Destroy,
    (ActorFunc)EnIshi_Update,
    (ActorFunc)EnIshi_Draw,
};

// Unmatched (EnIshi_Init, ~40 words): armcc keeps `self` (r6, only model + actionFunc) and `thisx` (r4) in separate
// registers in the ROM; every copy-propagation-proof variant (aliases, helper signatures) folds them into r4.
GLOBAL_ASM("asm/EnIshi_Init.s")

#if 0
static inline s32 EnIshi_SnapToFloor(EnIshi* self, PlayState* play, f32 arg2) {
    CollisionPoly* poly;
    Vec3f checkPos;
    s32 bgId;
    f32 floorY;

    checkPos.x = self->actor.world.pos.x;
    checkPos.y = self->actor.world.pos.y + 30.0f;
    checkPos.z = self->actor.world.pos.z;
    floorY = BgCheck_EntityRaycastFloor4(&play->colCtx, &poly, &bgId, &self->actor, &checkPos);
    if (floorY > -32000.0f) {
        self->actor.world.pos.y = floorY + arg2;
        Math_Vec3f_Copy(&self->actor.home.pos, &self->actor.world.pos);
        return 1;
    }
    return 0;
}

static inline void EnIshi_InitCollider(Actor* thisx, PlayState* play) {
    EnIshi* self = (EnIshi*)thisx;

    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &EnIshi_sCylinderInits[self->actor.params & 3]);
    Collider_UpdateCylinder(&self->actor, &self->collider);
}

void EnIshi_Init(Actor* thisx, GameState* state) {
    EnIshi* self = (EnIshi*)thisx;
    PlayState* play = (PlayState*)state;
    s32 type = thisx->params & 3;
    s32 modelIdx = 5;

    if (type == 1 || type == 2) {
        modelIdx = 4;
    }
    FUN_00372f38(thisx, play, &self->model, modelIdx, 0);
    Actor_ProcessInitChain(thisx, EnIshi_sInitChains[type]);
    if (play->GetCutsceneState() != 0) {
        thisx->uncullZoneForward += 1000.0f;
    }
    if (thisx->shape.rot.y == 0) {
        thisx->shape.rot.y = thisx->world.rot.y = Rand_ZeroFloat(65536.0f);
    }
    Actor_SetScale(thisx, EnIshi_sRockScales[type]);
    EnIshi_InitCollider(thisx, play);
    if (type == 1) {
        if (Flags_GetSwitch(play, ((thisx->params >> 10) & 0x3C) | ((u32)(thisx->params << 24) >> 30))) {
            Actor_Kill(thisx);
            return;
        }
    } else if (type == 2) {
        if (gSaveContext.eventChkInf[6] & 0x80) {
            Actor_Kill(thisx);
            return;
        }
    }
    CollisionCheck_SetInfo(&thisx->colChkInfo, NULL, &EnIshi_sColChkInfoInit);
    thisx->shape.yOffset = EnIshi_sYOffsets[type];
    if (!((thisx->params >> 5) & 1) && !EnIshi_SnapToFloor(self, play, 0.0f)) {
        Actor_Kill(thisx);
        return;
    }
    self->actionFunc = EnIshi_Wait;
}
#endif

void EnIshi_Destroy(Actor* thisx, GameState* state) {
    EnIshi* self = (EnIshi*)thisx;
    PlayState* play = (PlayState*)state;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_00350f34(thisx, &self->model, 0);
}

void EnIshi_Update(Actor* thisx, GameState* state) {
    EnIshi* self = (EnIshi*)thisx;

    self->actionFunc(self, (PlayState*)state);
}

void EnIshi_Draw(Actor* thisx, GameState* state) {
    ActorFunc drawFunc = EnIshi_sDrawFuncs[thisx->params & 3];

    drawFunc(thisx, state);
}

static inline void EnIshi_Fall(EnIshi* self) {
    self->actor.velocity.y += self->actor.gravity;
    if (self->actor.velocity.y < self->actor.minVelocityY) {
        self->actor.velocity.y = self->actor.minVelocityY;
    }
}

static inline void EnIshi_ApplyDrag(Vec3f* vel, f32 k) {
    k += ((Rand_ZeroOne() * 0.2f) - 0.1f) * k;
    vel->x -= vel->x * k;
    vel->y -= vel->y * k;
    vel->z -= vel->z * k;
}

static inline f32 EnIshi_RoundSpeed(f32 s) {
    s32 r;
    if (s > 0.0f) {
        r = (s32)(s * 1.5f + 0.5f);
    } else {
        r = (s32)(s * 1.5f - 0.5f);
    }
    return (f32)r;
}

void EnIshi_LiftedUp(EnIshi* self, PlayState* play) {
    if (Actor_HasNoParent(&self->actor, play)) {
        self->actor.room = play->roomCtx.unk_00[0];
        self->actor.velocity.x = Math_SinS(self->actor.world.rot.y) * EnIshi_RoundSpeed(self->actor.speedXZ);
        self->actor.velocity.z = Math_CosS(self->actor.world.rot.y) * EnIshi_RoundSpeed(self->actor.speedXZ);
        if ((self->actor.params & 3) == 0) {
            EnIshi_sRotSpeed[0] = (Rand_ZeroOne() - 0.5f) * 16000.0f;
            EnIshi_sRotSpeed[1] = (Rand_ZeroOne() - 0.5f) * 2400.0f;
        } else {
            EnIshi_sRotSpeed[0] = (Rand_ZeroOne() - 0.5f) * 8000.0f;
            EnIshi_sRotSpeed[1] = (Rand_ZeroOne() - 0.5f) * 1600.0f;
        }
        self->actor.colChkInfo.mass = 240;
        self->actionFunc = EnIshi_Fly;
        EnIshi_Fall(self);
        EnIshi_ApplyDrag(&self->actor.velocity, EnIshi_sDragCoefs[self->actor.params & 3]);
        FUN_0036b96c(&self->actor);
        Actor_UpdateBgCheckInfo(play, &self->actor, 7.5f, 35.0f, 0.0f, 0xC5);
    }
}

static inline void EnIshi_DropCollectible(EnIshi* self, PlayState* play) {
    s16 dropParams;

    if ((self->actor.params & 3) == 0) {
        dropParams = (self->actor.params >> 8) & 0xF;
        if (dropParams >= 0xD) {
            dropParams = 0;
        }
        Item_DropCollectibleRandom(play, NULL, &self->actor.world.pos, dropParams << 4);
    }
}

void EnIshi_Fly(EnIshi* self, PlayState* play) {
    Actor* thisx = &self->actor;
    Actor* t2 = &self->actor;
    s16 type = thisx->params & 3;
    s32 quakeIndex;

    if (thisx->bgCheckFlags & 9) {
        EnIshi_DropCollectible(self, play);
        EnIshi_sFragmentSpawnFuncs[type](self, play);
        if (!(self->actor.bgCheckFlags & 0x20)) {
            {
                u32* ids = EnIshi_sBreakSfxIds;
                u32 sfx = ids[type];
                u8* dur = (u8*)ids - 0x30;
                Audio_PlaySoundAtPosition(play, &self->actor.world.pos, dur[type], sfx);
            }
            EnIshi_sDustSpawnFuncs[type](self, play);
        }
        if (type == 1) {
            quakeIndex = Quake_Add(play->cameraPtrs[play->activeCamera], 3);
            Quake_SetSpeed(quakeIndex, -0x3CB0);
            Quake_SetQuakeValues(quakeIndex, 3, 0, 0, 0);
            Quake_SetCountdown(quakeIndex, 7);
        }
        Actor_Kill(&self->actor);
        return;
    }
    if (self->actor.bgCheckFlags & 0x40) {
        Vec3f contactPos;

        contactPos.x = self->actor.world.pos.x;
        contactPos.y = self->actor.world.pos.y + self->actor.yDistToWater;
        contactPos.z = self->actor.world.pos.z;
        EffectSsGSplash_Spawn(play, &contactPos, NULL, NULL, 0, 350);
        if (type == 0) {
            EffectSsGRipple_Spawn(play, &contactPos, 150, 650, 0);
            EffectSsGRipple_Spawn(play, &contactPos, 400, 800, 4);
            EffectSsGRipple_Spawn(play, &contactPos, 500, 1100, 8);
        } else {
            EffectSsGRipple_Spawn(play, &contactPos, 300, 700, 0);
            EffectSsGRipple_Spawn(play, &contactPos, 500, 900, 4);
            EffectSsGRipple_Spawn(play, &contactPos, 500, 1300, 8);
        }
        self->actor.minVelocityY = -6.0f;
        EnIshi_sRotSpeed[0] >>= 2;
        EnIshi_sRotSpeed[1] >>= 2;
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 40, 0x0100021C);
        thisx->bgCheckFlags &= ~0x40;
    }
    Math_StepToF(&self->actor.shape.yOffset, 0.0f, 2.0f);
    EnIshi_Fall(self);
    EnIshi_ApplyDrag(&self->actor.velocity, EnIshi_sDragCoefs[type]);
    FUN_0036b96c(&self->actor);
    self->actor.shape.rot.x += EnIshi_sRotSpeed[0];
    self->actor.shape.rot.y += EnIshi_sRotSpeed[1];
    Actor_UpdateBgCheckInfo(play, &self->actor, 17.5f, 37.5f, 0.0f, 0x45);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
}

// Unmatched (EnIshi_Wait, ~8 insns): ROM computes the durations address as `sub r0,r0,#0x30; ldrb r2,[r0]` from the
// sfx-id table (adjacent statics); armcc folds the -0x30 into the load. Also `bic r1,r0,#2` result register differs.
// Needs real statics (data chunk split) to reproduce.
GLOBAL_ASM("asm/EnIshi_Wait.s")

#if 0
void EnIshi_Wait(EnIshi* self, PlayState* play) {
    Actor* thisx = &self->actor;
    s32 type = thisx->params & 3;
    s32 i;

    if (Actor_HasParent(&self->actor, play) && type != 2) {
        self->actionFunc = EnIshi_LiftedUp;
        thisx->room = -1;
        {
            u32 fl = thisx->flags;
            thisx->flags = fl | 0x10;
        }
        if ((self->actor.params & 3) == 1) {
            Flags_SetSwitch(play, ((self->actor.params >> 10) & 0x3C) | ((u32)(self->actor.params << 24) >> 30));
        }
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 20, EnIshi_sLiftSfxIds[type]);
        if ((self->actor.params >> 4) & 1) {
            for (i = 0; i < 3; i++) {
                Actor* bug = Actor_Spawn(&play->actorCtx, play, 0x20, self->actor.world.pos.x, self->actor.world.pos.y,
                                         self->actor.world.pos.z, 0, (s16)(Rand_ZeroOne() * 65535.0f), 0, 1, 1);

                if (bug == NULL) {
                    break;
                }
            }
        }
    } else if ((self->collider.base.acFlags & 2) && type == 0 &&
               (self->collider.info.acHitInfo->toucher.dmgFlags & 0x40000048)) {
        EnIshi_DropCollectible(self, play);
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, EnIshi_sBreakSfxDurations[0], EnIshi_sBreakSfxIds[0]);
        EnIshi_sFragmentSpawnFuncs[0](self, play);
        EnIshi_sDustSpawnFuncs[0](self, play);
        Actor_Kill(&self->actor);
    } else if (type == 2 && (gSaveContext.eventChkInf[6] & 0x80)) {
        Actor_Kill(&self->actor);
    } else if (self->actor.xzDistToPlayer < 600.0f) {
        Collider_UpdateCylinder(&self->actor, &self->collider);
        {
            ColliderCylinder* c = &self->collider;
            c->base.acFlags &= ~2;
        }
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
        if (self->actor.xzDistToPlayer < 400.0f) {
            CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
            if (self->actor.xzDistToPlayer < 90.0f) {
                if (type == 1) {
                    GiveItem(&self->actor, play, 0, 80.0f, 20.0f);
                } else if (type == 0) {
                    GiveItem(&self->actor, play, 0, 50.0f, 10.0f);
                }
            }
        }
    }
}
#endif

#define ENISHI_DRAW_MODEL(NAME) \
    void NAME(Actor* thisx, GameState* state) { \
        EnIshi* self = (EnIshi*)thisx; \
        MTX34 modelMtx; \
        MTX34CopyAsm(&modelMtx, &thisx->modelMtx); \
        if (self->model != NULL) { \
            self->model->SetUnkAC(1); \
            self->model->FUN_003721e0(&modelMtx); \
            self->model->FUN_00372170(0); \
        } \
    }

ENISHI_DRAW_MODEL(caseD_0_003afa7c)
ENISHI_DRAW_MODEL(caseD_1_003c4d20)
ENISHI_DRAW_MODEL(caseD_2)

#define ENISHI_SPAWN_DUST(NAME, RADIUS, COUNT, SCALE, LIFE) \
    void NAME(EnIshi* self, PlayState* play) { \
        Vec3f pos; \
        Math_Vec3f_Copy(&pos, &self->actor.world.pos); \
        if (self->actor.bgCheckFlags & 1) { \
            pos.x += 2.0f * self->actor.velocity.x; \
            pos.y -= 2.0f * self->actor.velocity.y; \
            pos.z += 2.0f * self->actor.velocity.z; \
        } else if (self->actor.bgCheckFlags & 8) { \
            pos.x -= 2.0f * self->actor.velocity.x; \
            pos.y += 2.0f * self->actor.velocity.y; \
            pos.z -= 2.0f * self->actor.velocity.z; \
        } \
        FUN_0037378c(play, &pos, RADIUS, COUNT, SCALE, LIFE, 1); \
    }

ENISHI_SPAWN_DUST(FUN_00235c94, 60.0f, 3, 0x50, 0x3C)
ENISHI_SPAWN_DUST(FUN_00273dd8, 140.0f, 0xA, 0xB4, 0x5A)

extern s16 EnIshi_sFragmentScales[];

void FUN_00235acc(EnIshi* self, PlayState* play) {
    Actor* actor = &self->actor;
    Vec3f velocity;
    Vec3f pos;
    s16 phi;
    s32 i;

    for (i = 0; i < 6; i++) {
        pos.x = actor->world.pos.x + (Rand_ZeroOne() - 0.5f) * 8.0f;
        pos.y = actor->world.pos.y + (Rand_ZeroOne() * 5.0f) + 5.0f;
        pos.z = actor->world.pos.z + (Rand_ZeroOne() - 0.5f) * 8.0f;
        Math_Vec3f_Copy(&velocity, &actor->velocity);
        if (actor->bgCheckFlags & 1) {
            velocity.x *= 0.8f;
            velocity.y *= -0.8f;
            velocity.z *= 0.8f;
        } else if (actor->bgCheckFlags & 8) {
            velocity.x *= -0.8f;
            velocity.y *= 0.8f;
            velocity.z *= -0.8f;
        }
        velocity.x += (Rand_ZeroOne() - 0.5f) * 11.0f;
        velocity.y += Rand_ZeroOne() * 6.0f;
        velocity.z += (Rand_ZeroOne() - 0.5f) * 11.0f;
        if (Rand_ZeroOne() < 0.5f) {
            phi = 65;
        } else {
            phi = 33;
        }
        EffectSsKakera_Spawn(play, &pos, &velocity, &pos, -420, phi, 30, 5, 0, EnIshi_sFragmentScales[i], 3, 10, 40,
                             -1, OBJECT_GAMEPLAY_FIELD_KEEP, (void*)5);
    }
}

// Unmatched (FUN_00273b98, still GLOBAL_ASM in src/functions/functions_240000s.cpp): structure identical to ROM but
// armcc hoists a different set of the phi constants (0x421 hoisted + 0x221 derived from it, -380 loaded instead of
// r4-0x17c) -> register numbering differs.
#if 0
extern s16 EnIshi_sLargeFragmentScales[];

void FUN_00273b98(EnIshi* self, PlayState* play) {
    Actor* thisx = &self->actor;
    Vec3f velocity;
    Vec3f pos;
    s16 angle = 0x1000;
    s32 i;
    f32 rand;
    s16 phi_v0;
    s16 phi_v1;

    for (i = 0; i < 9; i++) {
        angle += 0x4E20;
        rand = Rand_ZeroOne() * 10.0f;
        pos.x = self->actor.world.pos.x + (Math_SinS(angle) * rand);
        pos.y = self->actor.world.pos.y + (Rand_ZeroOne() * 40.0f) + 5.0f;
        pos.z = self->actor.world.pos.z + (Math_CosS(angle) * rand);
        Math_Vec3f_Copy(&velocity, &thisx->velocity);
        if (thisx->bgCheckFlags & 1) {
            velocity.x *= 0.9f;
            velocity.y *= -0.8f;
            velocity.z *= 0.9f;
        } else if (thisx->bgCheckFlags & 8) {
            velocity.x *= -0.9f;
            velocity.y *= 0.8f;
            velocity.z *= -0.9f;
        }
        rand = Rand_ZeroOne() * 10.0f;
        velocity.x += rand * Math_SinS(angle);
        velocity.y += (Rand_ZeroOne() * 4.0f) + ((Rand_ZeroOne() * i) * 0.7f);
        velocity.z += rand * Math_CosS(angle);
        if (i == 0) {
            phi_v0 = 0x421;
            phi_v1 = -450;
        } else if (i < 4) {
            phi_v0 = 0x221;
            phi_v1 = -380;
        } else {
            phi_v0 = 0x241;
            phi_v1 = -320;
        }
        EffectSsKakera_Spawn(play, &pos, &velocity, &self->actor.world.pos, phi_v1, phi_v0, 30, 5, 0,
                             EnIshi_sLargeFragmentScales[i], 5, 2, 70, 0, OBJECT_GAMEPLAY_FIELD_KEEP, (void*)3);
    }
}
#endif

}
