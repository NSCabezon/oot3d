extern "C" {
#include "z_en_ice_hono.hpp"
#include "model_manager.hpp"

#define FLAGS 0x00000000

void EnIceHono_Init(Actor* actor, GameState* state);
void EnIceHono_Destroy(Actor* actor, GameState* state);
void EnIceHono_Update(Actor* actor, GameState* state);
void EnIceHono_Draw(Actor* actor, GameState* state);
void FUN_0034a928(Actor* actor, s32 sfxId);

static inline s32 RUR() { return SREG(30); }
#define R_UPDATE_RATE RUR()

GLOBAL_ASM("data/z_En_Ice_Hono.data.s")

ActorInit En_Ice_Hono_InitVars = {
    ACTOR_EN_ICE_HONO,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnIceHono),
    (ActorFunc)EnIceHono_Init,
    (ActorFunc)EnIceHono_Destroy,
    (ActorFunc)EnIceHono_Update,
    (ActorFunc)EnIceHono_Draw,
};

#if 0 /* EnIceHono_Init draft: far (246/309 words; ROM keeps a separate `this` copy in r5 inside each Init* helper, sl=gGameInfo, sb=0, r7=play, 1236 B) */
/* needs src/game/actors/En_Ice_Hono/data_syms.s (EnIceHono_sCylInit*/sInitChain* EQUs 0x528084/0x5280BC/0x5280F4/0x528108/0x528114) pulled by GLOBAL_ASM */
extern ColliderCylinderInit EnIceHono_sCylInitCapturableFlame;
extern ColliderCylinderInit EnIceHono_sCylInitDroppedFlame;
extern InitChainEntry EnIceHono_sInitChainCapturableFlame[];
extern InitChainEntry EnIceHono_sInitChainDroppedFlame[];
extern InitChainEntry EnIceHono_sInitChainSmallFlame[];
void Actor_ProcessInitChain(Actor* actor, InitChainEntry* chain);
void* LightContext_InsertLight(PlayState* play, void* lightCtx, void* info);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void EnIceHono_CapturableFlame(EnIceHono* self, PlayState* play);
void EnIceHono_DropFlame(EnIceHono* self, PlayState* play);
void EnIceHono_SmallFlameMove(EnIceHono* self, PlayState* play);
void Lights_PointNoGlowSetInfo(void* info, f32 x, f32 y, f32 z, u8 r, u8 g, u8 b, s16 radius, s32 pad);

static inline void EnIceHono_SetupActionCapturableFlame(EnIceHono* self) {
    self->actionFunc = EnIceHono_CapturableFlame;
    self->alpha = 255;
    self->actor.shape.yOffset = -1000.0f;
}

static inline void EnIceHono_SetupActionDroppedFlame(EnIceHono* self) {
    self->actionFunc = EnIceHono_DropFlame;
    self->timer = (s32)(600.0f / R_UPDATE_RATE + 0.5f);
    self->alpha = 255;
}

static inline void EnIceHono_SetupActionSmallFlame(EnIceHono* self) {
    self->actionFunc = EnIceHono_SmallFlameMove;
    self->timer = (s32)(44 * 3.0f / R_UPDATE_RATE + 0.5f);
    self->alpha = 255;
    if (self->actor.params == 1) {
        self->smallFlameTargetYScale = Rand_ZeroOne() * 0.005f + 0.004f;
        self->actor.speedXZ = Rand_ZeroOne() * 1.6f + 0.5f;
    } else {
        self->smallFlameTargetYScale = Rand_ZeroOne() * 0.005f + 0.003f;
        self->actor.speedXZ = Rand_ZeroOne() * 2.0f + 0.5f;
    }
}

static inline void EnIceHono_InitCapturableFlame(Actor* thisx, PlayState* play) {
    EnIceHono* self = (EnIceHono*)thisx;

    Actor_ProcessInitChain(thisx, EnIceHono_sInitChainCapturableFlame);
    Actor_SetScale(thisx, 0.0074f);
    thisx->flags |= 1;
    Actor_SetFocus(thisx, 10.0f);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, thisx, &EnIceHono_sCylInitCapturableFlame);
    Collider_UpdateCylinder(thisx, &self->collider);
    thisx->colChkInfo.mass = 255;
    EnIceHono_SetupActionCapturableFlame(self);
}

static inline void EnIceHono_InitDroppedFlame(Actor* thisx, PlayState* play) {
    EnIceHono* self = (EnIceHono*)thisx;

    Actor_ProcessInitChain(thisx, EnIceHono_sInitChainDroppedFlame);
    thisx->scale.x = thisx->scale.z = thisx->scale.y = 0.00002f;
    thisx->gravity = -0.3f;
    thisx->minVelocityY = -4.0f;
    thisx->shape.yOffset = 0.0f;
    thisx->shape.rot.x = thisx->shape.rot.y = thisx->shape.rot.z = thisx->world.rot.x =
        thisx->world.rot.y = thisx->world.rot.z = 0;
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, thisx, &EnIceHono_sCylInitDroppedFlame);
    Collider_UpdateCylinder(thisx, &self->collider);
    self->collider.dim.radius = (s16)(thisx->scale.x * 4000.4f);
    self->collider.dim.height = (s16)(thisx->scale.y * 8000.2f);
    thisx->colChkInfo.mass = 253;
    EnIceHono_SetupActionDroppedFlame(self);
}

static inline void EnIceHono_InitSmallFlame(Actor* thisx, PlayState* play) {
    EnIceHono* self = (EnIceHono*)thisx;

    Actor_ProcessInitChain(thisx, EnIceHono_sInitChainSmallFlame);
    thisx->scale.x = thisx->scale.z = thisx->scale.y = 0.0008f;
    thisx->gravity = -0.3f;
    thisx->minVelocityY = -4.0f;
    thisx->shape.yOffset = 0.0f;
    EnIceHono_SetupActionSmallFlame(self);
}

void EnIceHono_Init(Actor* thisx, GameState* state) {
    EnIceHono* self = (EnIceHono*)thisx;
    PlayState* play = (PlayState*)state;

    switch (self->actor.params) {
        case -1:
            EnIceHono_InitCapturableFlame(&self->actor, play);
            break;
        case 0:
            EnIceHono_InitDroppedFlame(&self->actor, play);
            break;
        case 1:
        case 2:
            EnIceHono_InitSmallFlame(&self->actor, play);
            break;
    }

    if ((self->actor.params == -1) || (self->actor.params == 0)) {
        Lights_PointNoGlowSetInfo(&self->lightInfo, (s16)self->actor.world.pos.x, (s16)self->actor.world.pos.y + 10,
                                  (s16)self->actor.world.pos.z, 155, 210, 255, 0, 0);
        self->lightNode = LightContext_InsertLight(play, (u8*)play + 0xA70, &self->lightInfo);
        self->unk_154 = Rand_ZeroOne() * 65535.5f;
        self->unk_156 = Rand_ZeroOne() * 65535.5f;
    }

    self->actor.unk_19A = 1;
    {
        void* zar = (u8*)play->GetObjectStatus(self->actor.objBankIndex) + 0x10;
        ModelAllocator* alloc = GetModelManager().allocator;
        SkeletonAnimationModel* model;
        alloc->owner = self->actor.unk_178;
        model = alloc->Create(ZAR_GetCMBByIndex(zar, 0x65), 1);
        self->model = (EnIceHonoModel*)model;
        alloc->owner = NULL;
        model->unk_0C->unk_00 = model->unk_10;
        model->unk_0C->FUN_00372d94(((ZARInfo*)zar)->GetCMABByIndex(0x3A));
        model->unk_0C->SetAnimType(1);
        model->unk_0C->SetAnimSpeed(2.0f);
    }
}
#endif

GLOBAL_ASM("asm/EnIceHono_Init.s")

void LightContext_RemoveLight(PlayState* play, void* lightCtx, void* lightNode);

void EnIceHono_Destroy(Actor* actor, GameState* state) {
    EnIceHono* self = (EnIceHono*)actor;
    PlayState* play = (PlayState*)state;

    if ((self->actor.params == -1) || (self->actor.params == 0)) {
        LightContext_RemoveLight(play, (void*)((u8*)play + 0xA70), self->lightNode);
        Collider_DestroyCylinder(play, &self->collider);
    }
    if (self->model != NULL) {
        self->model->Destroy();
        self->model = NULL;
    }
}

#if 0 /* EnIceHono_Update draft: 101/115 words (consts right, size ok; ROM base+0x100 in r4 / self r5 swapped, sfx call predicated in ROM) */
void EnIceHono_Update(Actor* actor, GameState* state) {
    EnIceHono* self = (EnIceHono*)actor;
    PlayState* play = (PlayState*)state;
    f32 intensity;

    if (self->timer > 0) {
        self->timer--;
    }
    if (self->actor.params == 0) {
        if (self->actor.bgCheckFlags & 1) {
            FUN_0034a928(&self->actor, 0x0100013A);
        }
    }
    if ((self->actor.params == -1) || (self->actor.params == 0)) {
        self->unk_154 += (s32)(0.5f + (R_UPDATE_RATE * 4369.0f) * (1.0f / 3.0f));
        self->unk_156 += (s32)(0.5f + (R_UPDATE_RATE * 16384.0f) * (1.0f / 3.0f));
        intensity = Math_SinS(self->unk_154) * 0.125f;
        intensity += Math_SinS(self->unk_156) * 0.1f;
        intensity += Rand_ZeroOne() * 0.05f;
        intensity += 0.425f;

        Lights_PointNoGlowSetInfo(&self->lightInfo, self->actor.world.pos.x, self->actor.world.pos.y + 10.0f,
                                  self->actor.world.pos.z, (s32)(155.0f * intensity), (s32)(210.0f * intensity),
                                  (s32)(255.0f * intensity), 1400, 0);
    }

    if (self->actionFunc != NULL) {
        self->actionFunc(self, play);
    }
}
#endif

GLOBAL_ASM("asm/EnIceHono_Update.s")


s16 Camera_GetCamDirYaw(Camera* camera);
f32 __hardfp_sinf(f32 x);
f32 __hardfp_cosf(f32 x);
void FUN_003695cc(SkeletonAnimationModel* skelAnimModel, s32 arg1, s32 arg2, s32 arg3, f32 arg4, f32 arg5, f32 arg6,
                  f32 arg7);

#define ICE_MODEL(self) ((SkeletonAnimationModel*)(self)->model)

// Inlined 3DS Matrix_RotateY(mtx, mode, angle) (mode 1 = post-multiply, else build).
static inline void EnIceHono_RotateY(MTX34* mtx, s32 mode, f32 angle) {
    f32 sinA;
    f32 cosA;
    f32* m = (f32*)mtx;

    if (mode == 1) {
        if (angle != 0.0f) {
            s32 i;
            sinA = __hardfp_sinf(angle);
            cosA = __hardfp_cosf(angle);
            for (i = 0; i < 3; i++) {
                f32 x = m[i * 4 + 0];
                f32 z = m[i * 4 + 2];
                m[i * 4 + 0] = x * cosA - z * sinA;
                m[i * 4 + 2] = x * sinA + z * cosA;
            }
        }
    } else {
        if (angle == 0.0f) {
            sinA = 0.0f;
            cosA = 1.0f;
        } else {
            sinA = __hardfp_sinf(angle);
            cosA = __hardfp_cosf(angle);
        }
        m[0] = cosA;
        m[1] = 0.0f;
        m[2] = sinA;
        m[3] = 0.0f;
        m[5] = 1.0f;
        m[4] = 0.0f;
        m[6] = 0.0f;
        m[7] = 0.0f;
        m[8] = -sinA;
        m[9] = 0.0f;
        m[10] = cosA;
        m[11] = 0.0f;
    }
}

void EnIceHono_Draw(Actor* actor, GameState* state) {
    EnIceHono* self = (EnIceHono*)actor;
    PlayState* play = (PlayState*)state;
    MTX34 mtx;
    s32 yaw;
    s32 mode1 = 1;

    MTX34CopyAsm(&mtx, &self->actor.modelMtx);
    yaw = Camera_GetCamDirYaw(play->cameraPtrs[play->activeCamera]);
    EnIceHono_RotateY(&mtx, mode1, (s16)(yaw - self->actor.shape.rot.y + 0x8000) * (2.0f * 3.14159274f / 65536.0f));
    ICE_MODEL(self)->unk_0C->SetAnimSpeed(*(f32*)((u8*)play + 0x7C00 + 0x344));
    FUN_003695cc(ICE_MODEL(self), 0, 4, 2, 0.0f, 0.0f, 0.0f, (f32)self->alpha * (1.0f / 255.0f));
    ICE_MODEL(self)->SetUnkAC(mode1);
    ICE_MODEL(self)->FUN_003721e0(&mtx);
    ICE_MODEL(self)->FUN_00372170(1);
}

void FUN_0034a928(Actor* actor, s32 sfxId);

static inline s32 EnIceHono_InBottleRange(EnIceHono* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (self->actor.xzDistToPlayer < 60.0f) {
        Vec3f tempPos;
        f32 dx;
        f32 dz;

        tempPos.x = Math_SinS(self->actor.yawTowardsPlayer + 0x8000) * 40.0f + player->actor.world.pos.x;
        tempPos.z = Math_CosS(self->actor.yawTowardsPlayer + 0x8000) * 40.0f + player->actor.world.pos.z;
        dx = tempPos.x - self->actor.world.pos.x;
        dz = tempPos.z - self->actor.world.pos.z;

        if ((SQ(dx) + SQ(dz)) <= SQ(40.0f)) {
            s16 facing = (s16)(self->actor.yawTowardsPlayer - player->actor.shape.rot.y + 0x8000);

            if ((u32)(facing + 0x3000) <= 0x6000) {
                return true;
            }
        }
    }
    return false;
}

void EnIceHono_CapturableFlame(EnIceHono* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        self->actor.parent = NULL;
    } else if (EnIceHono_InBottleRange(self, play)) {
        // GI_MAX in this case allows the player to catch the actor in a bottle
        GiveItem(&self->actor, play, GI_MAX, 60.0f, 100.0f);
    }

    if (self->actor.xzDistToPlayer < 200.0f) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    }
    FUN_0034a928(&self->actor, 0x010001FA); // NA_SE_EV_FIRE_PILLAR_S - SFX_FLAG
}

GLOBAL_ASM("../split/asm/FUN_001852e8.s")

#if 0 /* EnIceHono_DropFlame draft: 4 words (ROM loads the SpreadFlames literal into r1 before gGameInfo and stores after the vldr; guarded static Vector3f needs pin: guard 0x528080 obj 0x59B008/12 "z_en_ice_hono.o(+BSS)") */
void EnIceHono_SpreadFlames(EnIceHono* self, PlayState* play);

static void EnIceHono_SetupSpread(Actor* thisx) {
    EnIceHono* self = (EnIceHono*)thisx;
    self->actionFunc = EnIceHono_SpreadFlames;
    self->timer = (s32)(60.0f * 3.0f / SREG(30) + 0.5f);
    self->alpha = 255;
}

void EnIceHono_DropFlame(EnIceHono* self, PlayState* play) {
    static Vector3f sUnused(0.0f, 0.0f, 0.0f);
    u32 bgFlag = self->actor.bgCheckFlags & 1;
    s32 i;

    Math_StepToF(&self->actor.scale.x, 0.0017f, 0.00008f);
    self->actor.scale.z = self->actor.scale.x;
    Math_StepToF(&self->actor.scale.y, 0.0017f, 0.00008f);

    if (bgFlag != 0) {
        for (i = 0; i < 8; i++) {
            Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ICE_HONO, self->actor.world.pos.x, self->actor.world.pos.y,
                        self->actor.world.pos.z, 0, ((s32)(Rand_ZeroOne() * 1000.0f) + i * 0x2000) - 0x1F4, 0, 1, 1);
        }
        EnIceHono_SetupSpread(&self->actor);
    }
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 10.0f, self->actor.scale.x * 3500.0f, 0.0f, 5);

    Collider_UpdateCylinder(&self->actor, &self->collider);
    self->collider.dim.radius = (s16)(self->actor.scale.x * 4000.0f);
    self->collider.dim.height = (s16)(self->actor.scale.y * 8000.0f);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);

    if (self->timer <= 0) {
        Actor_Kill(&self->actor);
    }
}
#endif

GLOBAL_ASM("asm/EnIceHono_DropFlame.s")

#define ICE_FRAMES(n) ((n) * 3.0f / R_UPDATE_RATE + 0.5f)

void EnIceHono_SmallFlameMove(EnIceHono* self, PlayState* play) {
    if (self->timer > (s32)(20 * 3.0f / SREG(30) + 0.5f)) {
        Math_StepToF(&self->actor.scale.x, 0.006f, 0.00016f);
        f32 ty = self->smallFlameTargetYScale * 0.667f;
        Math_StepToF(&self->actor.scale.y, ty, 0.00014f);
    } else {
        Math_StepToF(&self->actor.scale.x, 0.0001f, 0.00015f);
        Math_StepToF(&self->actor.scale.y, 0.0001f, 0.00015f);
    }
    PlayState* play2 = play;
    self->actor.scale.z = self->actor.scale.x;
    Math_StepToF(&self->actor.speedXZ, 0, 0.06f);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play2, &self->actor, 10.0f, 10.0f, 0.0f, 5);

    if (self->timer < (s32)ICE_FRAMES(25)) {
        self->alpha -= (s32)(0.5f + (R_UPDATE_RATE * 10.0f) * (1.0f / 3.0f));
        self->alpha = CLAMP(self->alpha, 0, 255);
    }
    if (self->timer <= 0) {
        Actor_Kill(&self->actor);
    }
}


#if 0 /* EnIceHono_SpreadFlames draft: 8 words (FRAMES(20)/FRAMES(46) numerator/rate VFP regs s0/s1 swapped) */
void EnIceHono_SpreadFlames(EnIceHono* self, PlayState* play) {
    if (self->timer > (s32)(20 * 3.0f / R_UPDATE_RATE + 0.5f)) {
        Math_StepToF(&self->actor.scale.x, 0.011f, 0.00014f);
        Math_StepToF(&self->actor.scale.y, 0.006f, 0.00012f);
    } else {
        Math_StepToF(&self->actor.scale.x, 0.0001f, 0.00015f);
        Math_StepToF(&self->actor.scale.y, 0.0001f, 0.00015f);
    }
    self->actor.scale.z = self->actor.scale.x;
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 10.0f, self->actor.scale.x * 3500.0f, 0.0f, 4);
    if (self->timer < (s32)ICE_FRAMES(25)) {
        self->alpha -= (s32)(0.5f + (R_UPDATE_RATE * 10.0f) * (1.0f / 3.0f));
        self->alpha = CLAMP(self->alpha, 0, 255);
    }

    if ((self->alpha > 100) && (self->timer < (s32)(40 * 3.0f / SREG(30) + 0.5f))) {
        Collider_UpdateCylinder(&self->actor, &self->collider);
        self->collider.dim.radius = (s16)(self->actor.scale.x * 6000.0f);
        self->collider.dim.height = (s16)(self->actor.scale.y * 8000.0f);
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
    }
    if (self->timer == (s32)ICE_FRAMES(46)) {
        s32 i;
        for (i = 0; i < 10; i++) {
            Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ICE_HONO, self->actor.world.pos.x, self->actor.world.pos.y,
                        self->actor.world.pos.z, 0, ((s32)(Rand_ZeroOne() * 1000.0f) + i * 0x1999) - 0x1F4, 0, 2, 1);
        }
    }

    if (self->timer <= 0) {
        Actor_Kill(&self->actor);
    }
}
#endif

GLOBAL_ASM("asm/EnIceHono_SpreadFlames.s")

}
