extern "C" {
#include "z_en_tubo_trap.hpp"

#define FLAGS 0x80000010

void EnTuboTrap_Init(Actor* thisx, PlayState* play);
void EnTuboTrap_Destroy(Actor* thisx, PlayState* play);
void EnTuboTrap_Update(Actor* thisx, PlayState* play);
void EnTuboTrap_Draw(Actor* thisx, PlayState* play);

void EnTuboTrap_WaitForProximity(EnTuboTrap* self, PlayState* play);
void EnTuboTrap_WaitForProximity_003e6f88(EnTuboTrap* self, PlayState* play);
void EnTuboTrap_Levitate(EnTuboTrap* self, PlayState* play);
void EnTuboTrap_Fly(EnTuboTrap* self, PlayState* play);
void EnTuboTrap_SpawnFragments(EnTuboTrap* self, PlayState* play);
void EffectSsGSplash_Spawn(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, s16 type, s16 scale);

GLOBAL_ASM("data/z_En_Tubo_Trap.data.s")
GLOBAL_ASM("../src/game/actors/En_Tubo_Trap/data_syms.s")

extern ColliderCylinderInit EnTuboTrap_sCylinderInit;

ActorInit En_Tubo_Trap_InitVars = {
    ACTOR_EN_TUBO_TRAP,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_DANGEON_KEEP,
    sizeof(EnTuboTrap),
    (ActorFunc)EnTuboTrap_Init,
    (ActorFunc)EnTuboTrap_Destroy,
    (ActorFunc)EnTuboTrap_Update,
    (ActorFunc)EnTuboTrap_Draw,
};

void EnTuboTrap_Init(Actor* thisx, PlayState* play) {
    EnTuboTrap* self = (EnTuboTrap*)thisx;

    ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, 2.0f);
    FUN_00372f38(thisx, play, &self->skelAnimModel, 13, 0);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, thisx, &EnTuboTrap_sCylinderInit);
    Actor_SetScale(thisx, 0.1f);
    self->actionFunc = EnTuboTrap_WaitForProximity_003e6f88;
}

void EnTuboTrap_Destroy(Actor* thisx, PlayState* play) {
    EnTuboTrap* self = (EnTuboTrap*)thisx;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_00350f34(thisx, &self->skelAnimModel, 0);
}

void EnTuboTrap_Update(Actor* thisx, PlayState* play) {
    EnTuboTrap* self = (EnTuboTrap*)thisx;

    self->actionFunc(self, play);
    Actor_MoveForward(thisx);
    Actor_UpdateBgCheckInfo(play, thisx, 10.0f, 10.0f, 20.0f, 0x1F);
    Actor_SetFocus(thisx, 0.0f);
    Collider_UpdateCylinder(thisx, &self->collider);
    CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
    CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
}

void EnTuboTrap_Draw(Actor* thisx, PlayState* play) {
    EnTuboTrap* self = (EnTuboTrap*)thisx;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &self->actor.modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&mtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

void EnTuboTrap_WaitForProximity(EnTuboTrap* self, PlayState* play) {
    self->actor.shape.rot.y += self->spinSpeed;
    if (self->actor.xzDistToPlayer > 5000.0f) {
        Actor_Kill(&self->actor);
    } else {
        FUN_00373264(&self->actor, 0x010003E9);
    }
}

void EnTuboTrap_WaitForProximity_003e6f88(EnTuboTrap* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 targetHeight;
    s32 yDiff = (s32)player->actor.world.pos.y - (s32)self->actor.world.pos.y;

    if (play->sceneNum == 6 && self->actor.room == 0xF) {
        if (yDiff > 450) {
            return;
        }
    }

    if (self->actor.xzDistToPlayer < 200.0f && yDiff >= 0) {
        Actor_ChangeType(play, &play->actorCtx, &self->actor, ACTORCAT_ENEMY);
        self->actor.flags |= 1; // ACTOR_FLAG_ATTENTION_ENABLED
        targetHeight = 40.0f + -10.0f * gSaveContext.linkAge;

        self->targetY = player->actor.world.pos.y + targetHeight;
        if (self->targetY < self->actor.world.pos.y) {
            self->targetY = self->actor.world.pos.y + targetHeight;
        }

        self->originPos = self->actor.world.pos;
        Audio_PlayActorSound2(&self->actor, 0x0100021B); // NA_SE_EV_POT_MOVE_START
        self->actionFunc = EnTuboTrap_Levitate;
    }
}

static __inline __pure float EnTuboTrap_Fabsf(float f) {
    return __fabsf(f);
}

void EnTuboTrap_Levitate(EnTuboTrap* self, PlayState* play) {
    self->actor.shape.rot.y += 5000;
    Math_ApproachF(&self->actor.world.pos.y, self->targetY, 0.8f, 3.0f);

    if (EnTuboTrap_Fabsf(self->actor.world.pos.y - self->targetY) < 10.0f) {
        self->actor.speedXZ = 10.0f;
        self->actor.world.rot.y = self->actor.yawTowardsPlayer;
        self->actionFunc = EnTuboTrap_Fly;
    }
}

void EnTuboTrap_SpawnFragments(EnTuboTrap* self, PlayState* play) {
    f32 rand;
    f32 cos;
    f32 sin;
    Vec3f pos;
    Vec3f velocity;
    s16 var;
    s16 arg5;
    Vec3f* actorPos = &self->actor.world.pos;
    s32 i;
    PlayState* play2 = play;

    for (i = 0, var = 0; i < 15; i++, var += 20000) {
        sin = Math_SinS(var);
        cos = Math_CosS(var);
        pos.x = sin * 8.0f;
        pos.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        pos.z = cos * 8.0f;

        velocity.x = pos.x * 0.23f;
        velocity.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        velocity.z = pos.z * 0.23f;

        pos.x += actorPos->x;
        pos.y += actorPos->y;
        pos.z += actorPos->z;

        rand = Rand_ZeroOne();
        if (rand < 0.2f) {
            arg5 = 96;
        } else if (rand < 0.6f) {
            arg5 = 64;
        } else {
            arg5 = 32;
        }

        EffectSsKakera_Spawn(play2, &pos, &velocity, actorPos, -240, arg5 | 0x200, 10, 10, 0,
                             (Rand_ZeroOne() * 65.0f) + 15.0f, 0, 32, 60, -1,
                             OBJECT_GAMEPLAY_DANGEON_KEEP, (void*)12);
    }

    FUN_0037378c(play2, actorPos, 30.0f, 4, 20, 50, 0);
}

static void EnTuboTrap_DropCollectible(EnTuboTrap* self, PlayState* play) {
    s16 dropType = (self->actor.params >> 6) & 0x3FF;

    if (dropType >= 0 && dropType < 0x1A) {
        Item_DropCollectible(play, &self->actor.world.pos, dropType | ((self->actor.params & 0x3F) << 8));
    }
}

static void EnTuboTrap_SpawnEffectsInWater(EnTuboTrap* self, PlayState* play) {
    f32 rand;
    f32 cos;
    f32 sin;
    Vec3f pos;
    Vec3f velocity;
    s16 var;
    s16 arg5;
    s32 i;
    Vec3f* actorPos = &self->actor.world.pos;

    pos = *actorPos;
    pos.y += self->actor.yDistToWater;

    EffectSsGSplash_Spawn(play, &pos, NULL, NULL, 0, 400);

    for (i = 0, var = 0; i < 15; i++, var += 20000) {
        sin = Math_SinS(var);
        cos = Math_CosS(var);
        pos.x = sin * 8.0f;
        pos.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        pos.z = cos * 8.0f;

        velocity.x = pos.x * 0.20f;
        velocity.y = (Rand_ZeroOne() * 4.0f) + 2.0f;
        velocity.z = pos.z * 0.20f;

        pos.x += actorPos->x;
        pos.y += actorPos->y;
        pos.z += actorPos->z;

        rand = Rand_ZeroOne();
        if (rand < 0.2f) {
            arg5 = 64;
        } else {
            arg5 = 32;
        }

        EffectSsKakera_Spawn(play, &pos, &velocity, actorPos, -180, arg5 | 0x200, 30, 30, 0,
                             (Rand_ZeroOne() * 65.0f) + 15.0f, 0, 32, 70, -1,
                             OBJECT_GAMEPLAY_DANGEON_KEEP, (void*)12);
    }
}

static void EnTuboTrap_HandleImpact(EnTuboTrap* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Player* player2 = GET_PLAYER(play);

    if ((self->actor.bgCheckFlags & 0x20) && (self->actor.yDistToWater > 15.0f)) {
        EnTuboTrap_SpawnEffectsInWater(self, play);
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 40, 0x0100016E);
        EnTuboTrap_DropCollectible(self, play);
        Actor_Kill(&self->actor);
        return;
    }

    if (self->collider.base.atFlags & AT_BOUNCED) {
        self->collider.base.atFlags &= ~AT_BOUNCED;
        EnTuboTrap_SpawnFragments(self, play);
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 40, 0x010001DE);
        EnTuboTrap_DropCollectible(self, play);
        Actor_Kill(&self->actor);
        return;
    }

    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        EnTuboTrap_SpawnFragments(self, play);
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 40, 0x01000159);
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 40, 0x010001DE);
        EnTuboTrap_DropCollectible(self, play);
        Actor_Kill(&self->actor);
        return;
    }

    if (self->collider.base.atFlags & AT_HIT) {
        self->collider.base.atFlags &= ~AT_HIT;
        if (self->collider.base.at == &player->actor) {
            EnTuboTrap_SpawnFragments(self, play);
            Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 40, 0x010001DE);
            Audio_PlaySoundAtPosition(play, &player2->actor.world.pos, 40, 0x0100003F);
            EnTuboTrap_DropCollectible(self, play);
            Actor_Kill(&self->actor);
            return;
        }
    }

    if (self->actor.bgCheckFlags & 0x19) {
        EnTuboTrap_SpawnFragments(self, play);
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 40, 0x010001DE);
        EnTuboTrap_DropCollectible(self, play);
        Actor_Kill(&self->actor);
        return;
    }
}

void EnTuboTrap_Fly(EnTuboTrap* self, PlayState* play) {
    f32 dx = self->originPos.x - self->actor.world.pos.x;
    f32 dy = self->originPos.y - self->actor.world.pos.y;
    f32 dz = self->originPos.z - self->actor.world.pos.z;

    Audio_PlayActorSound2(&self->actor, 0x010002A7);

    if (240.0f < sqrtf(SQ(dx) + SQ(dy) + SQ(dz))) {
        Math_ApproachF(&self->actor.gravity, -3.0f, 0.2f, 0.5f);
    }

    self->actor.shape.rot.y += 5000;
    EnTuboTrap_HandleImpact(self, play);
}

}
