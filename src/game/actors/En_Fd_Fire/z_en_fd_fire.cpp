extern "C" {
#include "z_en_fd_fire.hpp"

#define FLAGS 0x00000015

extern __pure float _fabsf(float);
f32 sinf(f32 x);
f32 cosf(f32 x);

typedef struct {
    f32 arr[12];
} EnFdFireAngles;

void EnFdFire_Init(Actor* thisx, PlayState* play);
void EnFdFire_Destroy(Actor* thisx, PlayState* play);
void EnFdFire_Update(Actor* thisx, PlayState* play);
void EnFdFire_Draw(Actor* actor, GameState* state);

void FUN_003abdcc(EnFdFire* self, PlayState* play);
void FUN_003abfb8(EnFdFire* self, PlayState* play);
void FUN_003ace80(EnFdFire* self, PlayState* play);
void FUN_003ac4c4(EnFdFire* self, PlayState* play);
void FUN_0034f94c(void* model, PlayState* play, EnFdFire* self, s32 type);
void FUN_0035046c(void* model);
void FUN_0035fb14(Actor* actor);
void FUN_0036b96c(Actor* actor);

GLOBAL_ASM("data/z_En_Fd_Fire.data.s")
GLOBAL_ASM("../src/game/actors/En_Fd_Fire/data_syms.s")

extern ColliderCylinderInit EnFdFire_sCylinderInit;
extern CollisionCheckInfoInit2 EnFdFire_sColChkInit;
extern DamageTable EnFdFire_sDamageTable;
extern EnFdFireAngles EnFdFire_sAngles;

ActorInit En_Fd_Fire_InitVars = {
    ACTOR_EN_FD_FIRE,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_GAMEPLAY_DANGEON_KEEP,
    sizeof(EnFdFire),
    (ActorFunc)EnFdFire_Init,
    (ActorFunc)EnFdFire_Destroy,
    (ActorFunc)EnFdFire_Update,
    (ActorFunc)EnFdFire_Draw,
};

// Matching: the flag clear goes through the entry `thisx` param (`thisx->flags &= ~1`), not `self->actor.flags`.
void EnFdFire_Init(Actor* thisx, PlayState* play) {
    EnFdFire* self = (EnFdFire*)thisx;
    PlayState* play2 = play;
    Player* player = GET_PLAYER(play);
    s32 type = 0xB;

    if (self->actor.params & 0x8000) {
        type = 0xD;
    }
    FUN_0034f94c(self->unk_210, play2, self, type);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 20.0f);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &EnFdFire_sCylinderInit);
    CollisionCheckInfoInit2* colChkInit = &EnFdFire_sColChkInit;

    CollisionCheck_SetInfo2(&self->actor.colChkInfo, (DamageTable*)((u8*)colChkInit + 0xA), colChkInit);
    thisx->flags &= ~0x1;
    self->actor.gravity = -0.6f;
    self->actor.speedXZ = 5.0f;
    self->actor.velocity.y = 12.0f;
    self->spawnRadius = Math_Vec3f_DistXYZ(&self->actor.world.pos, &player->actor.world.pos);
    self->scale = 3.0f;
    self->tile2Y = (s16)Rand_ZeroFloat(5.0f) - 25;
    self->actionFunc = FUN_003ac4c4;
}

void EnFdFire_Destroy(Actor* thisx, PlayState* play) {
    EnFdFire* self = (EnFdFire*)thisx;

    FUN_0035046c(self->unk_210);
    Collider_DestroyCylinder(play, &self->collider);
}

void FUN_003abdcc(EnFdFire* self, PlayState* play) {
    if (DECR(self->deathTimer) == 0) {
        self->actionFunc = FUN_003abfb8;
    }
}

void FUN_003abfb8(EnFdFire* self, PlayState* play) {
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.6f, 9.0f, 0.0f);
    FUN_0035fb14(&self->actor);
    Math_SmoothStepToF(&self->scale, 0.0f, 0.3f, 0.1f, 0.0f);
    self->actor.shape.shadowScale = 20.0f;
    self->actor.shape.shadowScale *= (self->scale / 3.0f);
    if (!(self->scale > 0.01f)) {
        Actor_Kill(&self->actor);
    }
}

void FUN_003ace80(EnFdFire* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    EnFdFireAngles angles = EnFdFire_sAngles;
    Vec3f pos;
    s16 idx;

    idx = ((play->state.frames / 10) + (self->actor.params & 0x7FFF)) % ARRAY_COUNT(angles.arr);
    pos = player->actor.world.pos;
    pos.x += 120.0f * sinf(angles.arr[idx]);
    pos.z += 120.0f * cosf(angles.arr[idx]);

    if (DECR(self->deathTimer) == 0) {
        self->actionFunc = FUN_003abfb8;
    } else {
        Math_SmoothStepToS(&self->actor.world.rot.y, Math_Vec3f_Yaw(&self->actor.world.pos, &pos), 8, 0xFA0, 1);
        Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.4f, 1.0f, 0.0f);
        if (self->actor.speedXZ < 0.1f) {
            self->actor.speedXZ = 5.0f;
        }
        FUN_0035fb14(&self->actor);
    }
}

// Diffs declared first and assigned separately (keeps ROM load interleave); float compares as early returns
// (`>= ... return` -> vstrlt, `> 0.0f ... return` -> bgt); velocity copied through a `Vec3f*` alias.
static void EnFdFire_UpdatePos(EnFdFire* self, Vec3f* targetPos) {
    f32 xDiff;
    f32 yDiff;
    f32 zDiff;
    f32 dist;

    xDiff = targetPos->x - self->actor.world.pos.x;
    yDiff = targetPos->y - self->actor.world.pos.y;
    zDiff = targetPos->z - self->actor.world.pos.z;
    dist = sqrtf(SQ(xDiff) + SQ(yDiff) + SQ(zDiff));
    if (_fabsf(dist) > _fabsf(self->actor.speedXZ)) {
        self->actor.velocity.x = (xDiff / dist) * self->actor.speedXZ;
        self->actor.velocity.z = (zDiff / dist) * self->actor.speedXZ;
    } else {
        self->actor.velocity.x = 0.0f;
        self->actor.velocity.z = 0.0f;
    }

    self->actor.velocity.y += self->actor.gravity;
    if (self->actor.velocity.y >= self->actor.minVelocityY) {
        return;
    }
    self->actor.velocity.y = self->actor.minVelocityY;
}

void FUN_003ac4c4(EnFdFire* self, PlayState* play) {
    Vector3f velocity(0.0f, 0.0f, 0.0f);
    Vec3f targetPos = self->actor.parent->world.pos;

    targetPos.x += self->spawnRadius * Math_SinS(self->actor.world.rot.y);
    targetPos.z += self->spawnRadius * Math_CosS(self->actor.world.rot.y);
    EnFdFire_UpdatePos(self, &targetPos);
    if (!(self->actor.bgCheckFlags & 0x0001) || (self->actor.velocity.y > 0.0f)) {
        return;
    }
    Vec3f* vel = &self->actor.velocity;
    *vel = *(Vec3f*)&velocity;
    self->actor.speedXZ = 0.0f;
    self->actor.bgCheckFlags &= ~0x0001;
    if (self->actor.params & 0x8000) {
        self->deathTimer = 300;
        self->actionFunc = FUN_003ace80;
    } else {
        self->deathTimer = 450;
        self->actionFunc = FUN_003abdcc;
    }
}

// Flag clears go through a second alias (`s2`) so the bic result gets a fresh reg (ROM `bic r1,r0`);
// `play2` param keeps play in r5.
static s32 EnFdFire_CheckCollider(EnFdFire* self, PlayState* play) {
    EnFdFire* s2 = self;

    if (self->actionFunc == FUN_003abfb8) {
        return false;
    }

    if (self->collider.base.acFlags & AC_HIT) {
        s2->collider.base.acFlags &= ~AC_HIT;
        return true;
    }

    if (self->collider.base.ocFlags2 & OC2_HIT_PLAYER) {
        s2->collider.base.ocFlags2 &= ~OC2_HIT_PLAYER;
        return true;
    }
    return false;
}

void EnFdFire_Update(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnFdFire* self = (EnFdFire*)thisx;

    if (self->actionFunc != FUN_003abfb8) {
        if ((self->actor.parent->update == NULL) || EnFdFire_CheckCollider(self, play)) {
            self->actionFunc = FUN_003abfb8;
        }
    }

    FUN_0036b96c(&self->actor);
    self->actionFunc(self, play);
    Actor_UpdateBgCheckInfo(play, &self->actor, 12.0f, 10.0f, 0.0f, 0x1 | 0x4);

    if (self->actionFunc != FUN_003abfb8) {
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
    }
}

GLOBAL_ASM("asm/EnFdFire_Draw.s")
}
