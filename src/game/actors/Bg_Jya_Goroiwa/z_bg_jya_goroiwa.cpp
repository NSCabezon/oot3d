extern "C" {
#include "z_bg_jya_goroiwa.hpp"

#define FLAGS 0x80000010

// Helpers not declared in include/ yet
// 3DS ColliderJntSphInit (same layout as the N64 one; repo header lacks it)
typedef struct {
    /* 0x00 */ ColliderInit base;
    /* 0x08 */ s32 count;
    /* 0x0C */ void* elements;
} BgJyaGoroiwaJntSphInit;

void Collider_InitJntSph(PlayState* play, BgJyaGoroiwaColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, BgJyaGoroiwaColliderJntSph* collider, Actor* actor, BgJyaGoroiwaJntSphInit* init,
                        BgJyaGoroiwaColliderElement* elements);
void Collider_DestroyJntSph(PlayState* play, BgJyaGoroiwaColliderJntSph* collider);
f32 BgCheck_EntityRaycastFloor4(CollisionContext* colCtx, CollisionPoly** outPoly, s32* outBgId, Actor* actor,
                                Vec3f* pos);
void FUN_003c1124(BgJyaGoroiwa* self, PlayState* play);
void FUN_00368fc0(PlayState* play, Actor* actor, f32 speed, s16 yaw, f32 unk, s32 unk2);
void FUN_0036f59c(Player* player, s32 sfxId);
float fabsf(float arg);

void BgJyaGoroiwa_Init(Actor* actor, GameState* state);
void BgJyaGoroiwa_Destroy(Actor* actor, GameState* state);
void BgJyaGoroiwa_Update(Actor* actor, GameState* state);
void BgJyaGoroiwa_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Jya_Goroiwa.data.s")
GLOBAL_ASM("../src/game/actors/Bg_Jya_Goroiwa/data_syms.s")

// sColChkInfoInit / sJntSphInit / sInitChain live in the data dump above. They have no symbols in the
// scatter file, so they are absolute symbols (EQU) in data_syms.s.
extern CollisionCheckInfoInit BgJyaGoroiwa_sColChkInfoInit;
extern BgJyaGoroiwaJntSphInit BgJyaGoroiwa_sJntSphInit;
extern InitChainEntry BgJyaGoroiwa_sInitChain[];

ActorInit Bg_Jya_Goroiwa_InitVars = {
    ACTOR_BG_JYA_GOROIWA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GOROIWA,
    sizeof(BgJyaGoroiwa),
    (ActorFunc)BgJyaGoroiwa_Init,
    (ActorFunc)BgJyaGoroiwa_Destroy,
    (ActorFunc)BgJyaGoroiwa_Update,
    (ActorFunc)BgJyaGoroiwa_Draw,
};

static inline void BgJyaGoroiwa_UpdateCollider(BgJyaGoroiwa* self) {
    BgJyaGoroiwaSphere* worldSphere = &self->collider.elements[0].worldSphere;

    worldSphere->center.x = self->actor.world.pos.x;
    worldSphere->center.y = self->actor.world.pos.y + 59.5f;
    worldSphere->center.z = self->actor.world.pos.z;
}

void BgJyaGoroiwa_Init(Actor* actor, GameState* state) {
    BgJyaGoroiwa* self = (BgJyaGoroiwa*)actor;
    PlayState* play = (PlayState*)state;

    Actor_ProcessInitChain(actor, BgJyaGoroiwa_sInitChain);
    FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
    Collider_InitJntSph(play, &self->collider);
    Collider_SetJntSph(play, &self->collider, actor, &BgJyaGoroiwa_sJntSphInit, self->colliderElements);
    BgJyaGoroiwa_UpdateCollider(self);
    self->collider.elements[0].worldSphere.radius = 58.0f;
    self->actor.shape.rot.x = self->actor.shape.rot.y = self->actor.shape.rot.z = 0;
    CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, &BgJyaGoroiwa_sColChkInfoInit);
    ActorShape_Init(&self->actor.shape, 595.0f, ActorShadow_DrawCircle, 9.0f);
    self->actor.shape.shadowAlpha = 128;
    self->actionFunc = FUN_003c1124;
    self->collider.base.atFlags |= AT_ON;
    self->hasHit = false;
    self->speedFactor = 1.0f;
}

void BgJyaGoroiwa_Destroy(Actor* actor, GameState* state) {
    BgJyaGoroiwa* self = (BgJyaGoroiwa*)actor;
    PlayState* play = (PlayState*)state;

    Collider_DestroyJntSph(play, &self->collider);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

// SetupMove, kept as an out-of-line function by the compiler (called from the Wait action).
// Name is pinned by the link map, so it stays FUN_00144638.
__attribute__((noinline)) void FUN_00144638(BgJyaGoroiwa* self) {
    self->actionFunc = FUN_003c1124;
    self->collider.base.atFlags |= AT_ON;
    self->hasHit = false;
    self->speedFactor = 1.0f;
}

// Wait action (SetupWait target). Name pinned by the link map.
void FUN_003bf308(BgJyaGoroiwa* self, PlayState* play) {
    self->waitTimer++;
    if (self->waitTimer > 90) {
        FUN_00144638(self);
        self->speedFactor = 0.1f;
    }
}

static inline void BgJyaGoroiwa_SetupWait(BgJyaGoroiwa* self) {
    self->actionFunc = FUN_003bf308;
    self->waitTimer = 0;
}

static inline f32 BgJyaGoroiwa_Sqrtf(f32 x) { return sqrtf(x); }

static inline void BgJyaGoroiwa_Fall(Actor* thisx) {
    BgJyaGoroiwa* self = (BgJyaGoroiwa*)thisx;

    if (self->hasHit) {
        self->yOffsetSpeed -= 1.5f;
        thisx->shape.yOffset += self->yOffsetSpeed * 10.0f;
        if (thisx->shape.yOffset < 595.0f) {
            thisx->shape.yOffset = 595.0f;
            BgJyaGoroiwa_SetupWait(self);
        }
    } else {
        Math_StepToF(&self->speedFactor, 1.0f, 0.04f);
    }
}

// Move action. Name pinned by the link map.
// Note: the hasHit block must be a separate inline taking Actor* (stops armcc jump-threading the
// hasHit test after the hit block), and the 2/3 step is written `* 2.0f / 3.0f` (hoists 2.0f into s0).
void FUN_003c1124(BgJyaGoroiwa* self, PlayState* play) {
    Actor* thisx = &self->actor;
    s16 relYawTowardsPlayer;
    f32 speedXZBaseSq = (-100.0f - thisx->world.pos.y) * 2.5f;
    f32 posYfac;

    if (speedXZBaseSq < 0.01f) {
        speedXZBaseSq = 0.01f;
    }

    thisx->speedXZ = BgJyaGoroiwa_Sqrtf(speedXZBaseSq) * self->speedFactor;
    thisx->velocity.x = Math_SinS(thisx->world.rot.y) * thisx->speedXZ;
    thisx->velocity.z = Math_CosS(thisx->world.rot.y) * thisx->speedXZ;

    thisx->world.pos.x += thisx->velocity.x * 2.0f / 3.0f;
    thisx->world.pos.z += thisx->velocity.z * 2.0f / 3.0f;

    if ((thisx->world.pos.x > 1466.0f) && (thisx->world.pos.x < 1673.0f)) {
        thisx->world.pos.y = -129.5f;
    } else {
        posYfac = 1569.0f - thisx->world.pos.x;
        posYfac = fabsf(posYfac) - 103.0f;
        thisx->world.pos.y = ((35.0f / 92.0f) * posYfac) - 129.5f;
    }

    if (self->collider.base.atFlags & AT_HIT) {
        self->collider.base.atFlags &= ~(AT_ON | AT_HIT);

        relYawTowardsPlayer = thisx->yawTowardsPlayer - thisx->world.rot.y;
        if ((relYawTowardsPlayer > -0x4000) && (relYawTowardsPlayer < 0x4000)) {
            thisx->world.rot.y += 0x8000;
        }

        FUN_00368fc0(play, thisx, 2.0f, thisx->yawTowardsPlayer, 0.0f, 0);
        FUN_0036f59c(GET_PLAYER(play), 0x0100003F); // NA_SE_PL_BODY_HIT

        self->yOffsetSpeed = 10.0f;
        self->hasHit = true;
        self->speedFactor = 0.5f;
    }

    BgJyaGoroiwa_Fall(thisx);

    if (thisx->world.pos.x > 1745.0f) {
        thisx->world.rot.y = -0x4000;
    } else if (thisx->world.pos.x < 1393.0f) {
        thisx->world.rot.y = 0x4000;
    }

    Audio_PlayActorSound2(thisx, 0x0100020F); // NA_SE_EV_BIGBALL_ROLL - SFX_FLAG
}

void BgJyaGoroiwa_Update(Actor* thisx, GameState* state) {
    BgJyaGoroiwa* self = (BgJyaGoroiwa*)thisx;
    PlayState* play = (PlayState*)state;
    Player* player = GET_PLAYER(play);
    s32 bgId;
    Vec3f checkPos;

    if (!(player->stateFlags1 & 0x300000C0)) {
        self->actionFunc(self, play);
        {
            // N64 BgJyaGoroiwa_UpdateRotation, written inline so pos.x stays in a register for checkPos.x
            f32 x = self->actor.world.pos.x;
            f32 xDiff = x - self->actor.prevPos.x;
            self->actor.shape.rot.z -= 0x10000 / (119 * M_PI) * xDiff;
            checkPos.x = x;
        }
        checkPos.y = self->actor.world.pos.y + 59.5f;
        checkPos.z = self->actor.world.pos.z;
        self->actor.floorHeight =
            BgCheck_EntityRaycastFloor4(&play->colCtx, &self->actor.floorPoly, &bgId, &self->actor, &checkPos);
        BgJyaGoroiwa_UpdateCollider(self);
        if (self->collider.base.atFlags & AT_ON) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
        }
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    }
}

void BgJyaGoroiwa_Draw(Actor* actor, GameState* state) {
    BgJyaGoroiwa* self = (BgJyaGoroiwa*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->actor.modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
