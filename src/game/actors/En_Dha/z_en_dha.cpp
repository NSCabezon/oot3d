extern "C" {
#include "z_en_dha.hpp"

#define FLAGS 0x00000015

#define NA_SE_EN_DEADHAND_DAMAGE 0x01000419
#define NA_SE_EN_DEADHAND_HAND_DEAD 0x0100041A
#define NA_SE_EN_DEADHAND_HAND_AT 0x0100041B
#define NA_SE_EN_DEADHAND_GRIP 0x0100041C
#define PLAYER_STATE2_7 0x80
#define ENDH_DEATH 10
#define ENDH_WAIT_UNDERGROUND 0

void Collider_InitJntSph(PlayState* play, EnDhaColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, EnDhaColliderJntSph* collider, Actor* actor, void* init, EnDhaColliderElement* elements);
void Collider_DestroyJntSph(PlayState* play, EnDhaColliderJntSph* collider);
void Collider_UpdateSpheres(s32 index, EnDhaColliderJntSph* collider, MTX34* mtx);
Actor* Actor_FindNearby(PlayState* play, Actor* actor, s16 actorId, u8 actorCategory, f32 maxDist);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 colorIntensity, s32 bufFlag, s32 duration);
s32 Actor_ApplyDamage(Actor* actor);
void Animation_PlayLoop(SkelAnime* skelAnime, void* anim);
void Item_DropCollectibleRandom(PlayState* play, Actor* fromActor, Vec3f* pos, s16 params);
s16 Math_Vec3f_Pitch(Vec3f* a, Vec3f* b);
void Matrix_MultVec3f(Vec3f* src, Vec3f* dest);
void Matrix_Translate(f32 x, f32 y, f32 z, s32 mode);
void Audio_PlayActorSound2(Actor* actor, s32 arg1);
void FUN_0037378c(PlayState* play, Vec3f* arg1, f32 arg2, s32 arg3, s16 arg4, s16 arg5, u8 arg6);
s32 FUN_003731e0(SkelAnime* skelAnime);
s16 Math_SmoothStepToS(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep);

void EnDha_Init(Actor* thisx, PlayState* play);
void EnDha_Wait(EnDha* self, PlayState* play);
void EnDha_SetupWait(EnDha* self);
void EnDha_TakeDamage(EnDha* self, PlayState* play);
void EnDha_Die(EnDha* self, PlayState* play);
void EnDha_Destroy(Actor* thisx, PlayState* play);
void EnDha_Update(Actor* thisx, PlayState* play);
void EnDha_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Dha.data.s")

ActorInit En_Dha_InitVars = {
    ACTOR_EN_DHA,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_DH,
    sizeof(EnDha),
    (ActorFunc)EnDha_Init,
    (ActorFunc)EnDha_Destroy,
    (ActorFunc)EnDha_Update,
    (ActorFunc)EnDha_Draw,
};

void FUN_0034fe20(Actor* actor, PlayState* play, SkelAnime* skelAnime, void* a, void* b, Vec3s* jointTable, Vec3s* morphTable, s32 count);
extern u8 EnDha_sDamageTable[];
extern u8 EnDha_sJntSphInit[];
extern u8 EnDha_sInitChain[];
GLOBAL_ASM("../src/game/actors/En_Dha/data_syms.s")

void ActorShadow_DrawFeet(Actor* actor, Lights* lights, PlayState* play);

void EnDha_Init(Actor* thisx, PlayState* play) {
    EnDha* self = (EnDha*)thisx;

    Actor_ProcessInitChain(&self->actor, (InitChainEntry*)EnDha_sInitChain);
    DamageTable* dt = (DamageTable*)EnDha_sDamageTable;
    self->actor.colChkInfo.damageTable = dt;
    FUN_00372f38(&self->actor, play, 0);
    FUN_0034fe20(&self->actor, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 3);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawFeet, 90.0f);
    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += 50.0f;
    self->actor.colChkInfo.mass = 0xFE;
    self->actor.colChkInfo.health = 8;
    self->limbAngleX[0] = -0x4000;
    Collider_InitJntSph(play, &self->collider);
    Collider_SetJntSph(play, &self->collider, &self->actor, EnDha_sJntSphInit, self->colliderElements);
    thisx->flags &= ~1;

    EnDha_SetupWait(self);
}

void EnDha_Destroy(Actor* thisx, PlayState* play) {
    EnDha* self = (EnDha*)thisx;

    SkelAnime_Free2(&self->skelAnime);
    FUN_003508b8(&self->actor, 0);
    Collider_DestroyJntSph(play, &self->collider);
}

#if 1
static inline void EnDha_SetupDeath(EnDha* self) {
    self->unk_1C0 = 8;
    self->actionTimer = 450;

    if ((self->actor.parent != NULL) && (self->actor.parent->params != ENDH_DEATH)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEADHAND_HAND_DEAD);
        if (self->actor.parent->params <= ENDH_WAIT_UNDERGROUND) {
            self->actor.parent->params--;
        }
    }
    { EnDhaActionFunc f = EnDha_Die; self->actionFunc = f; }
}

void EnDha_Update(Actor* thisx, PlayState* play) {
    EnDha* self = (EnDha*)thisx;

    if (self->actor.parent == NULL) {
        self->actor.parent = Actor_FindNearby(play, &self->actor, ACTOR_EN_DH, ACTORCAT_ENEMY, 10000.0f);
    }

    if (self->unk_1C0 < 8 && (self->collider.base.acFlags & AC_HIT)) {
        self->collider.base.acFlags &= ~AC_HIT;

        if (self->actor.colChkInfo.damageEffect == 0 || self->actor.colChkInfo.damageEffect == 6) {
            goto action;
        }

        Actor_SetColorFilter(&self->actor, 0x400000, 0xFF, 0, 0x10);
        if (Actor_ApplyDamage(&self->actor) == 0) {
            { EnDha* s2 = self; EnDha_SetupDeath(s2); }
            self->actor.colChkInfo.health = 8;
            Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos, 0xE0);
        } else {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEADHAND_DAMAGE);
            self->unk_1C0 = 9;
            self->actionTimer = 23;
            self->actionFunc = EnDha_TakeDamage;
        }
    }

    if ((self->actor.parent != NULL) && (self->actor.parent->params == ENDH_DEATH)) {
        EnDha_SetupDeath(self);
    }

action:
    { PlayState* play2 = play; self->actionFunc(self, play2); }
    CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
}
#endif

s32 FUN_0025678c(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg);
void FUN_0036e88c(MTX34* mtx, s16 x, s16 y, s16 z, s32 mode);
#if 1
s32 FUN_0025678c(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnDha* self = (EnDha*)thisx;
    f32 px;
    f32 py;
    f32 pz;

    if (self->actor.home.rot.z != 0) {
        return 0;
    }

    px = mtx->data[0][3];
    py = mtx->data[1][3];
    pz = mtx->data[2][3];

    switch (limbIndex) {
        case 0:
            FUN_0036e88c(mtx, 0x4000, -(self->limbAngleX[0] + 0x4000), self->limbAngleY + 0x4000, 0);
            mtx->data[0][3] = px;
            mtx->data[1][3] = py;
            mtx->data[2][3] = pz;
            break;
        case 1:
            FUN_0036e88c(mtx, 0, -self->limbAngleY, self->limbAngleX[1], 0);
            mtx->data[0][3] = px;
            mtx->data[1][3] = py;
            mtx->data[2][3] = pz;
            break;
        case 2:
            FUN_0036e88c(mtx, -self->handAngle.x, 0, self->handAngle.z, 0);
            mtx->data[0][3] = px;
            mtx->data[1][3] = py;
            mtx->data[2][3] = pz;
            break;
    }
    return 0;
}
#endif

static inline void EnDha_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void FUN_00256884(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    Vector3f handVec(1100.0f, 0.0f, 0.0f);
    Vector3f zeroVec(0.0f, 0.0f, 0.0f);
    EnDha* self = (EnDha*)thisx;

    switch (limbIndex) {
        case 0:
            Collider_UpdateSpheres(2, &self->collider, mtx);
            Collider_UpdateSpheres(3, &self->collider, mtx);
            break;
        case 1:
            Collider_UpdateSpheres(4, &self->collider, mtx);
            Collider_UpdateSpheres(5, &self->collider, mtx);
            EnDha_MultVec3f(&zeroVec, (Vector3f*)&self->handPos[1], mtx);
            break;
        case 2:
            Collider_UpdateSpheres(1, &self->collider, mtx);
            EnDha_MultVec3f(&handVec, (Vector3f*)&self->armPos, mtx);
            EnDha_MultVec3f(&zeroVec, (Vector3f*)&self->handPos[0], mtx);
            break;
    }
}

void EnDha_Draw(Actor* actor, GameState* state) {
    EnDha* self = (EnDha*)actor;

    SkelAnime_DrawOpa(&self->skelAnime, &actor->modelMtx, FUN_0025678c, FUN_00256884, actor, 0);
}

#if 1
void EnDha_SetupWait(EnDha* self) {
    Animation_PlayLoop(&self->skelAnime, 0);
    self->unk_1C0 = 0;
    s32 t;
    if ((s16)(Rand_ZeroOne() * 10.0f + 5.0f) > 0) {
        t = (s32)(0.5f + ((s16)(Rand_ZeroOne() * 10.0f + 5.0f) * 3.0f) * 0.5f);
    } else {
        t = (s32)(((s16)(Rand_ZeroOne() * 10.0f + 5.0f) * 3.0f) * 0.5f - 0.5f);
    }
    { s16* p = &self->actionTimer; *p = t; }
    self->actor.speedXZ = 0.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->actor.home.rot.z = 1;
    { EnDhaActionFunc f = EnDha_Wait; self->actionFunc = f; }
}
#endif

void EnDha_TakeDamage(EnDha* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Player* p2 = player;
    u32 flags = player->stateFlags2;

    if ((flags & PLAYER_STATE2_7) && (&self->actor == player->actor.parent)) {
        p2->stateFlags2 &= ~PLAYER_STATE2_7;
        p2->actor.parent = NULL;
        *(s16*)((u8*)player + 0x2238) = 200;
    }

    Math_SmoothStepToS(&self->limbAngleX[1], 0, 1, 0x7D0, 0);
    Math_SmoothStepToS(&self->limbAngleY, 0, 1, 0x258, 0);
    Math_SmoothStepToS(&self->limbAngleX[0], -0x4000, 1, 0x7D0, 0);
    FUN_003731e0(&self->skelAnime);
    self->actionTimer--;

    if (self->actionTimer == 0) {
        EnDha_SetupWait(self);
    }
}
GLOBAL_ASM("../split/asm/FUN_00296e94.s")

#if 1
void EnDha_Die(EnDha* self, PlayState* play) {
    s16 angle;
    Vec3f vec;
    Player* player = GET_PLAYER(play);
    Player* p2 = player;

    u32 flags = player->stateFlags2;
    if (flags & PLAYER_STATE2_7) {
        if (&self->actor == player->actor.parent) {
            player->stateFlags2 &= ~PLAYER_STATE2_7;
            p2->actor.parent = NULL;
            *(s16*)((u8*)player + 0x2238) = 200;
        } else if (self->actor.xzDistToPlayer < 100.0f) {
            player->stateFlags2 &= ~PLAYER_STATE2_7;
            *(s16*)((u8*)player + 0x2238) = 200;
        }
    }

    Math_SmoothStepToS(&self->limbAngleX[1], 0, 1, 0x7D0, 0);
    angle = Math_SmoothStepToS(&self->limbAngleX[0], -0x4000, 1, 0x7D0, 0);
    FUN_003731e0(&self->skelAnime);

    if (angle == 0) {
        vec = self->actor.world.pos;

        if (self->actionTimer != 0) {
            if (-12000.0f < self->actor.shape.yOffset) {
                self->actor.shape.yOffset -= 1000.0f;
                FUN_0037378c(play, &vec, 7.0f, 1, 0x5A, 0x14, 1);
            } else {
                self->actionTimer--;

                if ((self->actor.parent != NULL) && (self->actor.parent->params == ENDH_DEATH)) {
                    Actor_Kill(&self->actor);
                }
            }
        } else {
            self->actor.shape.yOffset += 500.0f;
            FUN_0037378c(play, &vec, 7.0f, 1, 0x5A, 0x14, 1);

            if (self->actor.shape.yOffset == 0.0f) {
                EnDha_SetupWait(self);
            }
        }
    }
}
#endif

GLOBAL_ASM("asm/EnDha_Wait.s")

}
