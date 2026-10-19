extern "C" {
#include "z_bg_spot16_bombstone.hpp"

#define FLAGS 0x80400010

#define PARAMS_GET_U(params, start, count) (((params) >> (start)) & ((1 << (count)) - 1))

void BgSpot16Bombstone_Init(Actor* actor, GameState* state);
void BgSpot16Bombstone_Destroy(Actor* actor, GameState* state);
void BgSpot16Bombstone_Update(Actor* actor, GameState* state);
void BgSpot16Bombstone_Draw(Actor* actor, GameState* state);

void FUN_00357248(SkeletonAnimationModel** skelAnimModel);
void Collider_DestroyJntSph(PlayState* play, BgSpot16BombstoneColliderJntSph* collider);

GLOBAL_ASM("data/z_Bg_Spot16_Bombstone.data.s")

ActorInit Bg_Spot16_Bombstone_InitVars = {
    ACTOR_BG_SPOT16_BOMBSTONE,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_SPOT16_OBJ,
    sizeof(BgSpot16Bombstone),
    (ActorFunc)BgSpot16Bombstone_Init,
    (ActorFunc)BgSpot16Bombstone_Destroy,
    (ActorFunc)BgSpot16Bombstone_Update,
    (ActorFunc)BgSpot16Bombstone_Draw,
};

// Data lives in the data dump above (no labels). Absolute symbols in data_syms.s (EQU).
GLOBAL_ASM("../src/game/actors/Bg_Spot16_Bombstone/data_syms.s")
typedef struct {
    /* 0x0 */ s16 timer;
    /* 0x4 */ Actor* playerBomb;
} BgSpot16BombstoneStatics;
extern BgSpot16BombstoneStatics BgSpot16Bombstone_sStatics;
extern s16 BgSpot16Bombstone_sTimer[];
extern void* BgSpot16Bombstone_sPlayerBomb[];
extern s16 BgSpot16Bombstone_sDebrisParams[][10];
extern InitChainEntry BgSpot16Bombstone_sInitChainBoulder[];
extern InitChainEntry BgSpot16Bombstone_sInitChainDebris[];
extern void* BgSpot16Bombstone_sJntSphInit[];

extern ColliderCylinderInit BgSpot16Bombstone_sCylinderInit[];
extern f32 BgSpot16Bombstone_sFragmentScales[];
extern s16 BgSpot16Bombstone_sFragmentParams[];

void Collider_InitJntSph(PlayState* play, BgSpot16BombstoneColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, BgSpot16BombstoneColliderJntSph* collider, Actor* actor, void* init,
                        BgSpot16BombstoneColliderElement* elements);
s32 Object_GetIndex(ObjectContext* objectCtx, s32 id);
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 slot);
SkeletonAnimationModel* FUN_0036a924(Actor* actor, GameState* state, s32 arg2, s32 arg3);
void FUN_0011093c(BgSpot16Bombstone* self, PlayState* play);
void FUN_003bfb70(BgSpot16Bombstone* self, PlayState* play);
void FUN_003bcf7c(BgSpot16Bombstone* self, PlayState* play);
s32 OnePointCutscene_Init(PlayState* play, s32 csId, s32 arg2, Actor* actor, s32 camId);


// Pushes the player out of the boulder (N64 func_808B56BC).
static void BgSpot16Bombstone_PushPlayer(BgSpot16Bombstone* self, PlayState* play) {
    Actor* actor = &self->actor;
    Player* player = GET_PLAYER(play);
    f32 sinValue;
    s16 adjustedYawDiff;
    s32 absYawDiff;

    if (actor->xzDistToPlayer < 130.0f && actor->yDistToPlayer < 160.0f &&
        actor->yDistToPlayer >= -10.0f) {
        absYawDiff = ABS(actor->yawTowardsPlayer - actor->shape.rot.y);

        adjustedYawDiff = absYawDiff - 0x3FFF;

        if (adjustedYawDiff > 0) {
            sinValue = Math_SinS(adjustedYawDiff) * self->actor.xzDistToPlayer;

            if (sinValue >= 0.0f) {
                player->actor.world.pos.x += sinValue * self->sinRotation;
                player->actor.world.pos.z += sinValue * self->cosRotation;
            }
        }
    }
}

static __inline f32 BgSpot16Bombstone_Dist1DSq(f32 x, f32 z) {
    return x * x + z * z;
}

// Tracks the bomb the player is carrying to start the camera cutscene (N64 func_808B57E0).
static void BgSpot16Bombstone_TrackBomb(BgSpot16Bombstone* self, PlayState* play) {
    Actor* playerHeldActor;
    Player* player = GET_PLAYER(play);
    Actor* currentBomb;

    if (BgSpot16Bombstone_sStatics.timer > 0) {
        BgSpot16Bombstone_sStatics.timer--;
    }

    if (BgSpot16Bombstone_sStatics.playerBomb != NULL) {
        if (BgSpot16Bombstone_sStatics.playerBomb->update == NULL) {
            BgSpot16Bombstone_sStatics.playerBomb = NULL;
        } else if (BgSpot16Bombstone_sStatics.timer <= 0 && BgSpot16Bombstone_sStatics.playerBomb->world.pos.y < 1400.0f &&
                   BgSpot16Bombstone_Dist1DSq(BgSpot16Bombstone_sStatics.playerBomb->world.pos.x + 1579.0f,
                                              BgSpot16Bombstone_sStatics.playerBomb->world.pos.z + 790.0f) < 160000.0f &&
                   BgSpot16Bombstone_sStatics.playerBomb->params == 0) {
            currentBomb = BgSpot16Bombstone_sStatics.playerBomb;
            if (*(s16*)((u8*)currentBomb + 0x26C) > 0) {
                BgSpot16Bombstone_sStatics.timer = *(s16*)((u8*)currentBomb + 0x26C) + 20;
                OnePointCutscene_Init(play, 4180, BgSpot16Bombstone_sStatics.timer, NULL, 0);
            }
        }
    } else if (player->stateFlags1 & 0x800) {
        playerHeldActor = *(Actor**)((u8*)player + 0x1224);
        if (playerHeldActor != NULL && playerHeldActor->type == 3 && playerHeldActor->id == 0x4C) {
            BgSpot16Bombstone_sStatics.playerBomb = playerHeldActor;
        }
    }
}

// N64 func_808B561C.
static void BgSpot16Bombstone_SpawnRocks(BgSpot16Bombstone* self, PlayState* play) {
    s32 index;
    PosRot* world;

    world = &self->actor.world;
    for (index = 0; index < 5; index++) {
        if (Actor_Spawn(&play->actorCtx, play, ACTOR_BG_SPOT16_BOMBSTONE, world->pos.x, world->pos.y, world->pos.z, 0,
                        0, 0, BgSpot16Bombstone_sFragmentParams[index], 1) == NULL) {
            break;
        }
    }
}

// Boulder intact action (N64 func_808B5950).
void FUN_003bfb70(BgSpot16Bombstone* self, PlayState* play) {
    PlayState* play2 = play;
    BgSpot16Bombstone_PushPlayer(self, play);
    BgSpot16Bombstone_TrackBomb(self, play2);

    if (self->colliderCylinder.base.acFlags & AC_HIT) {
        self->colliderCylinder.base.acFlags &= ~AC_HIT;

        BgSpot16Bombstone_SpawnRocks(self, play);

        OnePointCutscene_Init(play, 4180, 20, NULL, 0);

        Flags_SetSwitch(play, self->switchFlag);
        gSaveContext.eventChkInf[2] |= 8;

        self->unk_154 = 0;
        self->unk_158 = 0;
        self->actor.draw = NULL;
        self->actionFunc = FUN_003bcf7c;
    } else {
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->colliderCylinder.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->colliderJntSph.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->colliderJntSph.base);
    }
}

// Init is still GLOBAL_ASM. Best draft below (stage 3, Opus): 836 B = ROM size; whole intact path incl. the
// `mov r5,r4` self copy matches (helper takes `Actor* actor` + local `self` cast: ProcessInitChain/final stores via
// self, the rest via actor). Debris path matches once shifted. Remaining: shouldLive phi coalesces with the zero
// constant (ours zero=r6/one=r8, ROM zero=r8/shouldLive=r6 init 1, failure `mov r6,r8`), and the 0x1aa pool load is
// scheduled later than ROM. ~35 variants tried (return types, if/else shapes, local order, Actor* params, play local).
#if 0
static void BgSpot16Bombstone_InitJntSph(BgSpot16Bombstone* self, PlayState* play, f32 yOff) {
    Collider_InitJntSph(play, &self->colliderJntSph);
    Collider_SetJntSph(play, &self->colliderJntSph, &self->actor, BgSpot16Bombstone_sJntSphInit,
                       self->colliderJntSphElements);
    self->colliderJntSph.elements[0].worldCenter.x = self->actor.world.pos.x;
    self->colliderJntSph.elements[0].worldCenter.y = self->actor.world.pos.y + yOff;
    self->colliderJntSph.elements[0].worldCenter.z = self->actor.world.pos.z;
    self->colliderJntSph.elements[0].worldRadius = 120.0f;
}

static void BgSpot16Bombstone_InitCylinder(BgSpot16Bombstone* self, PlayState* play) {
    Collider_InitCylinder(play, &self->colliderCylinder);
    Collider_SetCylinder(play, &self->colliderCylinder, &self->actor, BgSpot16Bombstone_sCylinderInit);
    self->colliderCylinder.dim.pos.x += self->actor.world.pos.x;
    self->colliderCylinder.dim.pos.y += self->actor.world.pos.y;
    self->colliderCylinder.dim.pos.z += self->actor.world.pos.z;
}

static void BgSpot16Bombstone_SetupIntact(BgSpot16Bombstone* self) {
    self->actor.draw = BgSpot16Bombstone_Draw;
    self->actionFunc = FUN_003bfb70;
}

static s32 BgSpot16Bombstone_InitIntact(Actor* actor, PlayState* play, s32 switchFlag, f32 sinCosPosFactor) {
    BgSpot16Bombstone* self = (BgSpot16Bombstone*)actor;

    if (Flags_GetSwitch(play, switchFlag)) {
        return false;
    }
    Actor_ProcessInitChain(&self->actor, BgSpot16Bombstone_sInitChainBoulder);
    Actor_SetScale(actor, 0.4f);
    actor->colChkInfo.mass = MASS_IMMOVABLE;
    BgSpot16Bombstone_InitJntSph(((BgSpot16Bombstone*)actor), play, sinCosPosFactor);
    BgSpot16Bombstone_InitCylinder(((BgSpot16Bombstone*)actor), play);
    self->sinRotation = Math_SinS(actor->shape.rot.y);
    self->cosRotation = Math_CosS(actor->shape.rot.y);
    self->skelAnimModel = FUN_0036a924(actor, (GameState*)play, 0xC2, 1);
    BgSpot16Bombstone_SetupIntact(self);
    return true;
}

static s32 BgSpot16Bombstone_InitDebris(BgSpot16Bombstone* self, PlayState* play, f32 sinCosPosFactor) {
    Actor* actor = &self->actor;
    f32 sinValue;
    f32 cosValue;

    Actor_ProcessInitChain(actor, BgSpot16Bombstone_sInitChainDebris);

    {
        s16* row = BgSpot16Bombstone_sDebrisParams[actor->params];
        actor->speedXZ = row[0];
        actor->velocity.y = row[1];
        Actor_SetScale(actor, row[2] * (1.0f / 600.0f));
    }

    self->unk_210 = BgSpot16Bombstone_sDebrisParams[actor->params][3];
    self->unk_212 = BgSpot16Bombstone_sDebrisParams[actor->params][4];

    actor->world.rot.y = BgSpot16Bombstone_sDebrisParams[actor->params][5];

    sinValue = Math_SinS(self->actor.world.rot.y);
    cosValue = Math_CosS(self->actor.world.rot.y);

    actor->world.pos.x = (sinValue * sinCosPosFactor) + actor->home.pos.x;
    {
        s16* row = BgSpot16Bombstone_sDebrisParams[actor->params];
        actor->world.pos.y = row[6] + actor->home.pos.y;
        actor->world.pos.z = (cosValue * sinCosPosFactor) + actor->home.pos.z;

        actor->shape.rot.x = row[7];
        actor->shape.rot.y = row[8];
        actor->shape.rot.z = row[9];
    }

    self->requiredObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_BOMBIWA);

    if (self->requiredObjectSlot < 0) {
        return false;
    }

    self->actionFunc = FUN_0011093c;
    self->actor.draw = NULL;
    return true;
}

void BgSpot16Bombstone_Init(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    s16 shouldLive = true;
    BgSpot16Bombstone* self = (BgSpot16Bombstone*)thisx;
    s32 switchFlag;
    f32 sinCosPosFactor;

    BgSpot16Bombstone_sStatics.playerBomb = NULL;
    BgSpot16Bombstone_sStatics.timer = 0;
    sinCosPosFactor = 50.0f;
    switchFlag = PARAMS_GET_U(self->actor.params, 8, 6);
    self->switchFlag = switchFlag;
    self->actor.params = PARAMS_GET_U(self->actor.params, 0, 8);

    switch (self->actor.params) {
        case 0xFF:
            shouldLive = BgSpot16Bombstone_InitIntact(thisx, play, switchFlag, sinCosPosFactor);
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            shouldLive = BgSpot16Bombstone_InitDebris(self, play, sinCosPosFactor);
            break;
    }

    if (!shouldLive) {
        Actor_Kill(&self->actor);
        return;
    }
}
#endif

GLOBAL_ASM("asm/BgSpot16Bombstone_Init.s")

void BgSpot16Bombstone_Destroy(Actor* actor, GameState* state) {
    BgSpot16Bombstone* self = (BgSpot16Bombstone*)actor;
    PlayState* play = (PlayState*)state;

    if (self->actor.params == 0xFF) {
        // Boulder is intact so remove its collider
        Collider_DestroyJntSph(play, &self->colliderJntSph);
        Collider_DestroyCylinder(play, &self->colliderCylinder);
    }
    FUN_00357248(&self->skelAnimModel);
}

void BgSpot16Bombstone_Update(Actor* actor, GameState* state) {
    BgSpot16Bombstone* self = (BgSpot16Bombstone*)actor;
    PlayState* play = (PlayState*)state;

    self->unk_154++;
    if (self->actionFunc != NULL) {
        self->actionFunc(self, play);
    }
}

void BgSpot16Bombstone_Draw(Actor* actor, GameState* state) {
    BgSpot16Bombstone* self = (BgSpot16Bombstone*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->actor.modelMtx);
    if (self->actor.params == 0xFF) {
        // The boulder is intact
        if (self->skelAnimModel != NULL) {
            self->skelAnimModel->SetUnkAC(1);
            self->skelAnimModel->FUN_003721e0(&modelMtx);
            self->skelAnimModel->FUN_00372170(0);
        }
    } else {
        // The boulder is debris
        if (self->skelAnimModel != NULL) {
            self->skelAnimModel->SetUnkAC(1);
            self->skelAnimModel->FUN_003721e0(&modelMtx);
            self->skelAnimModel->FUN_00372170(0);
        }
    }
}

// Wait for the Bombiwa object to load, then switch to the debris action (N64 func_808B5B04).
void FUN_003cb7cc(BgSpot16Bombstone* self, PlayState* play);

static __inline __pure float BgSpot16Bombstone_Fabsf(float x) {
    return __fabsf(x);
}

static void BgSpot16Bombstone_SpawnFragments(BgSpot16Bombstone* self, PlayState* play) {
    Vec3f pos;
    Vec3f velocity;
    s32 index;
    s16 scale;

    if (self->actor.params == 0) {
        Actor_Spawn(&play->actorCtx, play, ACTOR_BG_SPOT16_BOMBSTONE, self->actor.world.pos.x, self->actor.world.pos.y,
                    self->actor.world.pos.z, 0, 0, 0, 5, 1);
        index = 3;
    } else {
        index = 0;
    }
    index = index + 0; // no-op: stops armcc folding the loop guard (ROM keeps cmp #5)

    for (; index < 5; index++) {
        {
            pos.x = ((Rand_ZeroOne() - 0.5f) * 8.0f) + self->actor.world.pos.x;
            pos.y = ((Rand_ZeroOne() * 5.0f) + self->actor.world.pos.y) + 8.0f;
            pos.z = ((Rand_ZeroOne() - 0.5f) * 8.0f) + self->actor.world.pos.z;

            velocity.x = (Rand_ZeroOne() - 0.5f) * 16.0f;
            velocity.y = (Rand_ZeroOne() * 14.0f) + (BgSpot16Bombstone_Fabsf(self->actor.velocity.y) * 1.3f);
            velocity.z = (Rand_ZeroOne() - 0.5f) * 16.0f;

            scale = BgSpot16Bombstone_sFragmentScales[index] * self->actor.scale.x * 3;

            EffectSsKakera_Spawn(play, &pos, &velocity, &self->actor.world.pos, -420, 0x131, 0xF, 0xF, 0, scale, 0x20,
                                 0x40, 0x53, -1, OBJECT_BOMBIWA, NULL);
        }
    }
}

static void BgSpot16Bombstone_SpawnDust(BgSpot16Bombstone* self, PlayState* play) {
    f32 scaleX1 = self->actor.scale.x * 150;
    s16 scaleX2 = self->actor.scale.x * 250;
    Vec3f world;

    world.x = self->actor.world.pos.x;
    world.y = self->actor.world.pos.y + 50.0f;
    world.z = self->actor.world.pos.z;

    FUN_0037378c(play, &world, scaleX1, 2, scaleX2, 0xA0, 1);
}

// Debris falling action (N64 func_808B5B6C).
void FUN_003cb7cc(BgSpot16Bombstone* self, PlayState* play) {
    Actor_MoveForward(&self->actor);
    self->actor.shape.rot.x += self->unk_210;
    self->actor.shape.rot.z += self->unk_212;

    if ((s16)self->unk_154 > 90) {
        Actor_Kill(&self->actor);
        return;
    }

    if ((self->actor.bgCheckFlags & 8) || ((self->actor.bgCheckFlags & 1) && self->actor.velocity.y < 0.0f)) {
        BgSpot16Bombstone_SpawnFragments(self, play);
        BgSpot16Bombstone_SpawnDust(self, play);
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 20, 0x010001A9);
        Actor_Kill(&self->actor);
        return;
    }

    Actor_UpdateBgCheckInfo(play, &self->actor, 17.5f, 35.0f, 0.0f, 5);
}

void FUN_0011093c(BgSpot16Bombstone* self, PlayState* play) {
    if (Object_IsLoaded(&play->objectCtx, self->requiredObjectSlot)) {
        self->unk_154 = 0;
        self->actionFunc = FUN_003cb7cc;
        self->actor.draw = BgSpot16Bombstone_Draw;
        self->skelAnimModel = FUN_0036a924(&self->actor, (GameState*)play, OBJECT_BOMBIWA, 0);
    }
}
}
