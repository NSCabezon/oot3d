extern "C" {
#include "z_en_bombf.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x80000011

void EnBombf_Init(Actor* thisx, GameState* state);
void EnBombf_Destroy(Actor* thisx, PlayState* play);
void EnBombf_Update(Actor* actor, GameState* state);
void EnBombf_Draw(Actor* actor, GameState* state);

void FUN_0016e398(EnBombf* self, PlayState* play); // EnBombf_Move
void FUN_003892d8(EnBombf* self, PlayState* play); // EnBombf_GrowBomb
void FUN_001ebf08(Actor* actorA, Actor* actorB, PlayState* play); // Actor_SetParentAndHeld (N64 func_8002F5C4)
void Collider_InitJntSph(PlayState* play, EnBombfColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, EnBombfColliderJntSph* collider, Actor* actor, void* init,
                        EnBombfColliderElement* elements);
void Collider_DestroyJntSph(PlayState* play, EnBombfColliderJntSph* collider);
void FUN_00193d5c(EnBombf* self, PlayState* play); // EnBombf_WaitForRelease
void Collider_UpdateSpheres(s32 index, EnBombfColliderJntSph* collider, MTX34* mtx);
Actor* Actor_FindNearby(PlayState* play, Actor* actor, s16 actorId, u8 actorCategory, f32 maxDist);

#define ACTOR_FLAG_ATTENTION_ENABLED 0x1
#define ACTOR_FLAG_DRAW_CULLING_DISABLED 0x20
#define BGCHECKFLAG_GROUND 0x1
#define BGCHECKFLAG_GROUND_TOUCH 0x2

void FUN_00357680(PlayState* play, Actor* actor); // Actor_PlaySfx_SurfaceBomb
void FUN_0034df30(Actor* actor, PlayState* play); // Actor_OfferCarry
s32 FUN_00341df0(CollisionContext* colCtx, CollisionPoly* poly, s32 bgId); // SurfaceType_GetSfxOffset

static inline void EnBombf_SetupAction(EnBombf* self, EnBombfActionFunc f) { self->actionFunc = f; }
static void EnBombf_SetupGrowBomb(EnBombf* self, s16 params) {
    EnBombf_SetupAction(self, FUN_003892d8);
}


GLOBAL_ASM("data/z_En_Bombf.data.s")
GLOBAL_ASM("../src/game/actors/En_Bombf/data_syms.s")

ActorInit En_Bombf_InitVars = {
    ACTOR_EN_BOMBF,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_BOMBF,
    sizeof(EnBombf),
    (ActorFunc)EnBombf_Init,
    (ActorFunc)EnBombf_Destroy,
    (ActorFunc)EnBombf_Update,
    (ActorFunc)EnBombf_Draw,
};

extern u8 EnBombf_sCylinderInit[];
extern u8 EnBombf_sJntSphInit[];

// FUN_003895d4 (EnBombf_Explode)
typedef struct EnBombfEnvView {
    /* 0x0000 */ char pad_0000[0x31FC];
    /* 0x31FC */ s16 adjAmbientColor[3];
    /* 0x3202 */ s16 adjFogColor[3];
} EnBombfEnvView;

void FUN_003895d4(EnBombf* self, PlayState* play) {
    EnBombfEnvView* env = (EnBombfEnvView*)play;
    Player* player;
    Actor* thisx = &self->actor;
    if (self->explosionCollider.elements[0].modelSphereRadius == 0) {
        thisx->flags |= ACTOR_FLAG_DRAW_CULLING_DISABLED;
    }

    self->explosionCollider.elements[0].modelSphereRadius += 8.0f;
    self->explosionCollider.elements[0].worldSphereRadius = self->explosionCollider.elements[0].modelSphereRadius;

    if (self->actor.params == BOMBFLOWER_EXPLOSION) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->explosionCollider.base);
    }

    if (env->adjFogColor[0] != 0) {
        env->adjFogColor[0] -= 25;
    }
    if (env->adjFogColor[1] != 0) {
        env->adjFogColor[1] -= 25;
    }
    if (env->adjFogColor[2] != 0) {
        env->adjFogColor[2] -= 25;
    }
    if (env->adjAmbientColor[0] != 0) {
        env->adjAmbientColor[0] -= 25;
    }
    if (env->adjAmbientColor[1] != 0) {
        env->adjAmbientColor[1] -= 25;
    }
    if (env->adjAmbientColor[2] != 0) {
        env->adjAmbientColor[2] -= 25;
    }

    if (self->timer == 0) {
        player = GET_PLAYER(play);

        if ((player->stateFlags1 & 0x800) && (*(Actor**)((u8*)player + 0x1224) == thisx)) {
            player->actor.child = NULL;
            *(Actor**)((u8*)player + 0x1224) = NULL;
            player->interactRangeActor = NULL;
            player->stateFlags1 &= ~0x800;
        }

        Actor_Kill(&self->actor);
    }
}

s32 FUN_003575e8(PlayState* play, Vec3f* pos, f32 radius, f32 height); // Player_IsBurningStickInRange

// FUN_003892d8 (EnBombf_GrowBomb)
void FUN_003892d8(EnBombf* self, PlayState* play) {
    EnBombf* bombFlower;
    Player* player = GET_PLAYER(play);
    PlayState* play2 = play;
    Actor* thisx = &self->actor;
    EnBombf* s2 = self;

    if (self->flowerBombScale >= 1.0f) {
        if (Actor_HasParent(&self->actor, play)) {
            bombFlower = (EnBombf*)Actor_Spawn(&play->actorCtx, play2, ACTOR_EN_BOMBF, self->actor.world.pos.x,
                                               self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0, 1);
            if (bombFlower != NULL) {
                FUN_001ebf08(&self->actor, &bombFlower->actor, play);
                self->timer = 270;
                self->flowerBombScale = 0.0f;
                Audio_PlayActorSound2(&self->actor, 0x0100006B);
                self->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                bombFlower->actor.flags &= ~0x80000000;
            } else {
                player->actor.child = NULL;
                *(Actor**)((u8*)player + 0x1224) = NULL;
                player->interactRangeActor = NULL;
                self->actor.parent = NULL;
                player->stateFlags1 &= ~0x800;
            }
        } else if (self->bombCollider.base.acFlags & 2) {
            self->bombCollider.base.acFlags &= ~2;

            if (self->bombCollider.base.ac->type != 9) {
                bombFlower = (EnBombf*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOMBF, self->actor.world.pos.x,
                                                   self->actor.world.pos.y, self->actor.world.pos.z,
                                                   self->actor.shape.rot.x, self->actor.shape.rot.y,
                                                   self->actor.shape.rot.z, 0, 1);
                if (bombFlower != NULL) {
                    bombFlower->isFuseEnabled = true;
                    bombFlower->timer = 0;
                    self->timer = 270;
                    self->flowerBombScale = 0.0f;
                    self->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                }
            }
        } else {
            if (FUN_003575e8(play, &self->actor.world.pos, 30.0f, 50.0f)) {
                bombFlower = (EnBombf*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOMBF, self->actor.world.pos.x,
                                                   self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0, 1);
                if (bombFlower != NULL) {
                    bombFlower->timer = 150;
                    self->timer = 270;
                    self->flowerBombScale = 0.0f;
                    self->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                }
            } else {
                if (!Actor_HasParent(&self->actor, play)) {
                    FUN_0034df30(&self->actor, play);
                } else {
                    player->actor.child = NULL;
                    *(Actor**)((u8*)player + 0x1224) = NULL;
                    player->interactRangeActor = NULL;
                    self->actor.parent = NULL;
                    player->stateFlags1 &= ~0x800;
                    self->actor.world.pos = self->actor.home.pos;
                }
            }
        }
    } else {
        if (self->timer == 0) {
            self->flowerBombScale += 0.05f;
            if (self->flowerBombScale >= 1.0f) {
                self->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
            }
        }

        if (Actor_HasParent(&self->actor, play)) {
            player->actor.child = NULL;
            *(Actor**)((u8*)player + 0x1224) = NULL;
            player->interactRangeActor = NULL;
            self->actor.parent = NULL;
            player->stateFlags1 &= ~0x800;
            self->actor.world.pos = self->actor.home.pos;
        }
    }
}

void FUN_0016e398(EnBombf* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        self->flowerBombScale = 0.0f;
        EnBombf_SetupAction(self, FUN_00193d5c);
        self->actor.room = -1;
        return;
    }

    self->flowerBombScale = 1.0f;

    if (!(self->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.025f, 0.0f);
    } else {
        Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 1.5f, 0.0f);
        if ((self->actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) && (self->actor.velocity.y < -6.0f)) {
            FUN_00357680(play, &self->actor);
            self->actor.velocity.y *= -0.5f;
        } else if (self->timer > 3) {
            FUN_0034df30(&self->actor, play);
        }
    }
}

void FUN_00193d5c(EnBombf* self, PlayState* play) {
    if (Actor_HasNoParent(&self->actor, play)) {
        self->actionFunc = FUN_0016e398;
        FUN_0016e398(self, play);
    } else {
        self->actor.velocity.y = 0.0f;
    }
}

void EnBombf_Init(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    f32 shapeUnk10 = 0.0f;
    EnBombf* self = (EnBombf*)thisx;

    Actor_SetScale(thisx, 0.01f);
    self->isFuseEnabled = true;
    Collider_InitCylinder(play, &self->bombCollider);
    Collider_InitJntSph(play, &self->explosionCollider);
    Collider_SetCylinder(play, &self->bombCollider, thisx, (ColliderCylinderInit*)EnBombf_sCylinderInit);
    Collider_SetJntSph(play, &self->explosionCollider, thisx, EnBombf_sJntSphInit,
                       &self->explosionColliderElements[0]);

    if (thisx->params == BOMBFLOWER_BODY) {
        shapeUnk10 = 1000.0f;
    }

    ActorShape_Init(&thisx->shape, shapeUnk10, ActorShadow_DrawCircle, 12.0f);
    thisx->focus.pos = thisx->world.pos;

    if (Actor_FindNearby(play, thisx, ACTOR_BG_DDAN_KD, ACTORCAT_BG, 10000.0f) != NULL) {
        thisx->flags |= ACTOR_FLAG_DRAW_CULLING_DISABLED;
    }

    thisx->colChkInfo.cylRadius = 10;
    thisx->colChkInfo.cylHeight = 10;
    thisx->targetMode = 0;

    if (thisx->params == BOMBFLOWER_BODY) {
        thisx->gravity = -1.5f;
        self->timer = 210;
        self->flashSpeedScale = 15;
        Actor_ChangeType(play, &play->actorCtx, thisx, ACTORCAT_EXPLOSIVES);
        thisx->colChkInfo.mass = 200;
        thisx->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
        EnBombf_SetupAction(self, FUN_0016e398);
    } else {
        thisx->colChkInfo.mass = MASS_IMMOVABLE;
        self->flowerBombScale = 1.0f;
        self->colliderSetOC = true;
        EnBombf_SetupGrowBomb(self, thisx->params);
    }

    thisx->uncullZoneScale += 31000.0f;
    thisx->uncullZoneForward += 31000.0f;
    FUN_00372f38(thisx, play, &self->skelAnimModel[0], 2, &self->skelAnimModel[1], 1, &self->skelAnimModel[2], 0, 0);
    thisx->unk_19B = 1;
}

void EnBombf_Destroy(Actor* thisx, PlayState* play) {
    EnBombf* self = (EnBombf*)thisx;

    Collider_DestroyCylinder(play, &self->bombCollider);
    Collider_DestroyJntSph(play, &self->explosionCollider);
    FUN_00350f34(thisx, &self->skelAnimModel[0], &self->skelAnimModel[1], &self->skelAnimModel[2], 0);
}

// EnBombf_Update draft: ~50 ROM words off, all of it VFP callee-saved numbering (ROM 0.0f=s16/10.0f=s17, ours swapped)
// plus prologue r5/r6 swap. Greedy play2/a2/s2 alias mutation already applied (170 -> 102 words). Kept as GLOBAL_ASM.
#if 0
void EffectSsBomb2_SpawnLayered(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep);
void FUN_00357540(PlayState* play, Actor* actor, Vec3f* pos, Vec3f* velocity, Vec3f* accel); // EffectSsGSpk_SpawnFuse
void FUN_00366150(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor,
                  Color_RGBA8* envColor, s16 scale, s16 scaleStep); // func_8002829C
void FUN_0035b9d4(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel); // EffectSsBlast_SpawnWhiteShockwave
void FUN_0036627c(void* camera, s32 a, s32 b, s32 c); // Camera_RequestQuake
void EffectSsDeadSound_SpawnStationary(PlayState* play, Vec3f* pos, s32 sfxId, s32 a, s32 b, s32 c);
struct EnBombfColor {
    u8 r, g, b, a;
} __attribute__((aligned(4)));
extern EnBombfColor EnBombf_sWhite;

#define R_DREG6 DREG(6)

// EnBombf_Update (3DS order: BgCheck before actionFunc)
void EnBombf_Update(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    Vector3f effVelocity(0.0f, 0.0f, 0.0f);
    Vector3f bomb2Accel(0.0f, 0.1f, 0.0f);
    Vector3f effAccel(0.0f, 0.0f, 0.0f);
    Vec3f effPos;
    Vector3f dustAccel(0.0f, 0.6f, 0.0f);
    EnBombfColor dustColor = EnBombf_sWhite;
    EnBombf* self = (EnBombf*)thisx;
    PlayState* play2 = play;
    Actor* a2 = thisx;
    EnBombf* s2 = self;

    if (self->isFuseEnabled && (self->timer != 0)) {
        self->timer--;
    }

    if (!self->colliderSetOC && !Actor_HasParent(thisx, play) &&
        ((thisx->xzDistToPlayer >= 20.0f) || (ABS(thisx->yDistToPlayer) >= 80.0f))) {
        self->colliderSetOC = true;
    }

    if (thisx->gravity != 0.0f) {
        DREG(6) = 1;
        Actor_UpdateBgCheckInfo(play, thisx, 5.0f, 10.0f, 15.0f, 0x1F);
        DREG(6) = 0;
    }

    self->actionFunc(self, play2);

    if (thisx->params == BOMBFLOWER_BODY) {
        Actor_MoveForward(thisx);
    }

    if (thisx->params == BOMBFLOWER_BODY) {
        if ((thisx->velocity.y > 0.0f) && (thisx->bgCheckFlags & 0x10)) {
            thisx->velocity.y = -thisx->velocity.y;
        }

        if ((thisx->speedXZ != 0.0f) && (thisx->bgCheckFlags & 8)) {
            s16 yawDiff = (s16)(thisx->wallYaw - thisx->world.rot.y);
            if ((yawDiff > 0x4000) || (yawDiff < -0x4000)) {
                thisx->world.rot.y = ((thisx->wallYaw - thisx->world.rot.y) + thisx->wallYaw) - 0x8000;
            }
            FUN_00357680(play, thisx);
            Actor_MoveForward(thisx);
            DREG(6) = 1;
            Actor_UpdateBgCheckInfo(play, thisx, 5.0f, 10.0f, 15.0f, 0x1F);
            DREG(6) = 0;
            thisx->speedXZ *= 0.7f;
            thisx->bgCheckFlags &= ~8;
        }

        if ((self->bombCollider.base.acFlags & 2) ||
            ((self->bombCollider.base.ocFlags1 & 2) && (self->bombCollider.base.oc->type == 5))) {
            self->isFuseEnabled = true;
            self->timer = 0;
        } else {
            if ((self->timer > 150) && FUN_003575e8(play, &thisx->world.pos, 30.0f, 50.0f)) {
                self->timer = 150;
            }
        }

        if (self->isFuseEnabled) {
            dustAccel.y = 0.2f;
            effPos = thisx->world.pos;
            effPos.y += 25.0f;
            if (self->timer < 191) {
                if ((*(u32*)((u8*)play + 0x5BF4) % 2) == 0) {
                    FUN_00357540(play, thisx, &effPos, (Vec3f*)&effVelocity, (Vec3f*)&effAccel);
                }
                Audio_PlayActorSound2(thisx, 0x010000FF);

                effPos.y += 3.0f;
                FUN_00366150(play, &effPos, (Vec3f*)&effVelocity, (Vec3f*)&dustAccel, (Color_RGBA8*)&dustColor, (Color_RGBA8*)&dustColor, 50, 5);
            }

            if ((self->timer == 3) || (self->timer == 30) || (self->timer == 50) || (self->timer == 70)) {
                self->flashSpeedScale >>= 1;
            }

            if ((self->timer < 150) && ((self->timer & (self->flashSpeedScale + 1)) != 0)) {
                Math_SmoothStepToF(&self->flashIntensity, 150.0f, 1.0f, 150.0f / self->flashSpeedScale, 0.0f);
            } else {
                Math_SmoothStepToF(&self->flashIntensity, 0.0f, 1.0f, 150.0f / self->flashSpeedScale, 0.0f);
            }

            if (self->timer < 3) {
                Actor_SetScale(thisx, thisx->scale.x + 0.002f);
            }

            if (self->timer == 0) {
                effPos = thisx->world.pos;

                effPos.y += 10.0f;

                if (Actor_HasParent(thisx, play)) {
                    effPos.y += 30.0f;
                }

                if (((u16)thisx->shape.rot.x - 0x7980) > 0x700) {
                    EffectSsBomb2_SpawnLayered(play, &effPos, (Vec3f*)&effVelocity, (Vec3f*)&bomb2Accel, 100, 19);
                } else {
                    bomb2Accel.y = -bomb2Accel.y;
                    EffectSsBomb2_SpawnLayered(play, &effPos, (Vec3f*)&effVelocity, (Vec3f*)&bomb2Accel, -100, 19);
                }

                effPos.y = thisx->floorHeight;
                if (thisx->floorHeight > -32000.0f) {
                    FUN_0035b9d4(play, &effPos, (Vec3f*)&effVelocity, (Vec3f*)&effAccel);
                }

                Audio_PlayActorSound2(thisx, 0x01000100);
                {
                    EnBombfEnvView* env = (EnBombfEnvView*)play;

                    env->adjFogColor[0] = env->adjFogColor[1] = env->adjFogColor[2] = 250;
                    env->adjAmbientColor[0] = env->adjAmbientColor[1] = env->adjAmbientColor[2] = 250;
                }
                FUN_0036627c((u8*)play + 0x364, 2, 11, 8);
                thisx->params = BOMBFLOWER_EXPLOSION;
                self->timer = 10;
                thisx->flags |= ACTOR_FLAG_DRAW_CULLING_DISABLED;
                EnBombf_SetupAction(self, FUN_003895d4);
            }
        }
    }

    thisx->focus.pos = thisx->world.pos;
    thisx->focus.pos.y += 10.0f;

    if (thisx->params <= BOMBFLOWER_BODY) {
        Collider_UpdateCylinder(thisx, &self->bombCollider);

        if ((self->flowerBombScale >= 1.0f) && self->colliderSetOC) {
            CollisionCheck_SetOC(play, &play->colChkCtx, &self->bombCollider.base);
        }

        CollisionCheck_SetAC(play, &play->colChkCtx, &self->bombCollider.base);
    }

    if ((thisx->scale.x >= 0.01f) && (thisx->params != BOMBFLOWER_EXPLOSION)) {
        if (thisx->yDistToWater >= 20.0f) {
            EffectSsDeadSound_SpawnStationary(play, &thisx->world.pos, 0x01000101, true, 1, 10);
            Actor_Kill(thisx);
            return;
        }
        if (thisx->bgCheckFlags & 0x40) {
            thisx->bgCheckFlags &= ~0x40;
            Audio_PlayActorSound2(thisx, 0x0100016E);
        }
    }
}
#endif

GLOBAL_ASM("asm/EnBombf_Update.s")

// EnBombf_Draw draft: ~65 words off (translate vector VFP copies, scale mult operand order, pool layout).
// Kept as GLOBAL_ASM until fixed.
#if 0
static inline void EnBombf_ScaleMtx(MTX34* mtx, f32 x, f32 y, f32 z) {
    f32* m = (f32*)mtx;

    m[0] *= x;
    m[4] *= x;
    m[8] *= x;
    m[1] *= y;
    m[5] *= y;
    m[9] *= y;
    m[2] *= z;
    m[6] *= z;
    m[10] *= z;
}

void EnBombf_Draw(Actor* thisx, GameState* state) {
    EnBombf* self = (EnBombf*)thisx;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->actor.modelMtx);
    f32 zero = 0.0f;

    if (self->actor.params <= 0) {
        if (self->actor.params != 0) {
            self->skelAnimModel[0]->SetUnkAC(1);
            self->skelAnimModel[0]->FUN_003721e0(&modelMtx);
            self->skelAnimModel[0]->FUN_00372170(0);
            self->skelAnimModel[1]->SetUnkAC(1);
            self->skelAnimModel[1]->FUN_003721e0(&modelMtx);
            self->skelAnimModel[1]->FUN_00372170(0);

            {
                Vec3f translate;

                translate.x = zero;
                translate.y = 1000.0f;
                translate.z = zero;
                Matrix_UpdatePosition(&modelMtx, &modelMtx, &translate);
            }
            EnBombf_ScaleMtx(&modelMtx, self->flowerBombScale, self->flowerBombScale, self->flowerBombScale);
        }

        FUN_003695cc(self->skelAnimModel[2], 0, 4, 0, self->flashIntensity * (1.0f / 255.0f), zero, zero, 1.0f);
        self->skelAnimModel[2]->SetUnkAC(1);
        self->skelAnimModel[2]->FUN_003721e0(&modelMtx);
        self->skelAnimModel[2]->FUN_00372170(0);
    } else {
        Collider_UpdateSpheres(0, &self->explosionCollider, &modelMtx);
    }
}
#endif

GLOBAL_ASM("asm/EnBombf_Draw.s")
}
