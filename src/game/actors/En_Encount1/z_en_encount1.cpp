extern "C" {
#include "z_en_encount1.hpp"

#define FLAGS 0x88000010

#define PARAMS_GET_U(params, start, count) (((params) >> (start)) & ((1 << (count)) - 1))
// play->sceneId is not in the PlayState header yet (raw offset +0x104, u16)
#define PLAY_SCENE_ID(play) (*(u16*)((u8*)(play) + 0x104))

void EnEncount1_Init(Actor* actor, GameState* state);
void EnEncount1_Destroy(Actor* actor, GameState* state);
void EnEncount1_Update(Actor* actor, GameState* state);

void FUN_001cc77c(EnEncount1* self, PlayState* play); // N64: EnEncount1_SpawnLeevers
void FUN_0019f354(EnEncount1* self, PlayState* play); // N64: EnEncount1_SpawnTektites
void FUN_0021bd40(EnEncount1* self, PlayState* play); // N64: EnEncount1_SpawnStalchildOrWolfos
void FUN_001c8260(EnEncount1* self, PlayState* play); // 4-byte stub (bx lr), spawner type 4

ActorInit En_Encount1_InitVars = {
    ACTOR_EN_ENCOUNT1,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnEncount1),
    (ActorFunc)EnEncount1_Init,
    (ActorFunc)EnEncount1_Destroy,
    (ActorFunc)EnEncount1_Update,
    NULL,
};

void EnEncount1_Init(Actor* actor, GameState* state) {
    EnEncount1* self = (EnEncount1*)actor;
    PlayState* play = (PlayState*)state;

    self->actor.unk_19A = 1;
    if (self->actor.params <= 0) {
        Actor_Kill(&self->actor);
        return;
    }
    self->spawnType = PARAMS_GET_U(self->actor.params, 11, 5);
    self->maxCurSpawns = PARAMS_GET_U(self->actor.params, 6, 5);
    self->maxTotalSpawns = PARAMS_GET_U(self->actor.params, 0, 6);
    self->curNumSpawn = self->totalNumSpawn = 0;
    self->spawnRange = 120.0f + (40.0f * self->actor.world.rot.z);
    self->actor.flags &= ~1;

    switch (self->spawnType) {
        case 0:
            self->timer = 45;
            self->maxCurSpawns = 5;
            if (PLAY_SCENE_ID(play) == 0x5E) {
                self->reduceLeevers = true;
                self->maxCurSpawns = 3;
            }
            self->mqActor = NULL;
            if (gSaveContext.masterQuestFlag && (PLAY_SCENE_ID(play) == 6) && (self->actor.room == 0xE)) {
                self->mqActor = Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, 0xA7, 0.0f, 0.0f, 0.0f, 0,
                                                     0, 0, 0x2000);
                Actor_ChangeType(play, &play->actorCtx, self->mqActor, 5);
            }
            self->updateFunc = FUN_001cc77c;
            break;
        case 1:
            self->maxCurSpawns = 2;
            self->updateFunc = FUN_0019f354;
            break;
        case 2:
        case 3:
            if (PLAY_SCENE_ID(play) == 0x51) {
                self->maxTotalSpawns = 10000;
            }
            self->updateFunc = FUN_0021bd40;
            break;
        case 4:
            self->updateFunc = FUN_001c8260;
            break;
    }
}

void EnEncount1_Destroy(Actor* actor, GameState* state) {
    EnEncount1* self = (EnEncount1*)actor;

    if (self->mqActor != NULL) {
        Actor_Kill(self->mqActor);
        self->mqActor = NULL;
    }
}

void EnEncount1_Update(Actor* actor, GameState* state) {
    EnEncount1* self = (EnEncount1*)actor;
    PlayState* play = (PlayState*)state;

    if (self->timer != 0) {
        self->timer--;
    }

    self->updateFunc(self, play);
}
f32 BgCheck_EntityRaycastFloor4(CollisionContext* colCtx, CollisionPoly** outPoly, s32* bgId, Actor* actor, Vec3f* pos);

extern __pure float _fabsf(float);
static __inline __pure f32 EnEncount1_Fabsf(f32 x) {
    return _fabsf(x);
}

void FUN_0019f354(EnEncount1* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 bgId;
    CollisionPoly* floorPoly;
    Vec3f spawnPos;
    f32 floorY;

    if (self->timer == 0) {
        self->timer = 15;
        if ((EnEncount1_Fabsf(player->actor.world.pos.y - self->actor.world.pos.y) > 100.0f) ||
            (self->actor.xzDistToPlayer > self->spawnRange)) {
            self->outOfRangeTimer++;
        } else {
            self->outOfRangeTimer = 0;
            if ((self->maxCurSpawns > self->curNumSpawn) && (self->maxTotalSpawns > self->totalNumSpawn)) {
                spawnPos.x = self->actor.world.pos.x + Rand_CenteredFloat(50.0f);
                spawnPos.y = self->actor.world.pos.y + 120.0f;
                spawnPos.z = self->actor.world.pos.z + Rand_CenteredFloat(50.0f);
                floorY = BgCheck_EntityRaycastFloor4(&play->colCtx, &floorPoly, &bgId, &self->actor, &spawnPos);
                if (floorY <= -32000.0f) {
                    return;
                }
                spawnPos.y = floorY;
                if (Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, 0x1B, spawnPos.x, spawnPos.y,
                                       spawnPos.z, 0, 0, 0, -1) != NULL) {
                    self->curNumSpawn++;
                    self->totalNumSpawn++;
                }
            }
        }
    }
}

// FUN_0021bd40 (N64 EnEncount1_SpawnStalchildOrWolfos): NON_MATCHING draft, 12/223 words differ.
// Only the hoisted float constant register numbering differs: ROM s16=200 s17=100 s18=40 s19=120,
// draft gets s16=200 s17=40 s18=100 (and the 100.0f vldr is scheduled before add r7/r8).
// Control flow, stack layout (colCtx/actorCtx locals declared at top), timers (90/150) all match.
#if 0
s32 Player_GetMask(PlayState* play);

// Player fields not in the Player struct yet (raw offsets)
#define PLAYER_UNK_228C(player) (*(s32*)((u8*)(player) + 0x228C))
#define PLAYER_STATE_FLAGS1(player) (*(u32*)((u8*)(player) + 0x1710))

void FUN_0021bd40(EnEncount1* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    CollisionContext* colCtx;
    ActorContext* actorCtx;
    f32 spawnDist;
    s16 spawnAngle;
    s16 spawnId;
    s16 spawnParams;
    s16 kcOver10;
    s16 tempmod;
    Vec3f spawnPos;
    CollisionPoly* floorPoly;
    s32 bgId;
    f32 floorY;

    if (PLAY_SCENE_ID(play) != 0x51) {
        if ((EnEncount1_Fabsf(player->actor.world.pos.y - self->actor.world.pos.y) > 100.0f) ||
            (self->actor.xzDistToPlayer > self->spawnRange)) {
            self->outOfRangeTimer++;
            return;
        }
    } else if (IS_DAY || (Player_GetMask(play) == 4)) {
        self->killCount = 0;
        return;
    }

    self->outOfRangeTimer = 0;
    spawnPos = self->actor.world.pos;
    if ((self->maxCurSpawns > self->curNumSpawn) && (self->maxTotalSpawns > self->totalNumSpawn)) {
        colCtx = &play->colCtx;
        actorCtx = &play->actorCtx;
        while ((self->maxCurSpawns > self->curNumSpawn) && (self->maxTotalSpawns > self->totalNumSpawn)) {
            if (PLAY_SCENE_ID(play) == 0x51) {
                if ((PLAYER_UNK_228C(player) == 0) || (player->actor.floorBgId != 0x32) ||
                    !(player->actor.bgCheckFlags & 1) || (PLAYER_STATE_FLAGS1(player) & 0x8000000)) {
                    self->fieldSpawnTimer = 90;
                    break;
                }
                if (self->fieldSpawnTimer == 90) {
                    self->maxCurSpawns = 2;
                }
                if (self->fieldSpawnTimer != 0) {
                    self->fieldSpawnTimer--;
                    break;
                }

                spawnDist = Rand_CenteredFloat(40.0f) + 200.0f;
                spawnAngle = player->actor.shape.rot.y;
                if (self->curNumSpawn != 0) {
                    spawnAngle = -spawnAngle;
                    spawnDist = Rand_CenteredFloat(40.0f) + 100.0f;
                }
                spawnPos.x =
                    player->actor.world.pos.x + (Math_SinS(spawnAngle) * spawnDist) + Rand_CenteredFloat(40.0f);
                spawnPos.y = player->actor.floorHeight + 120.0f;
                spawnPos.z =
                    player->actor.world.pos.z + (Math_CosS(spawnAngle) * spawnDist) + Rand_CenteredFloat(40.0f);
                floorY = BgCheck_EntityRaycastFloor4(colCtx, &floorPoly, &bgId, &self->actor, &spawnPos);
                if (floorY <= -32000.0f) {
                    break;
                }
                if ((player->actor.yDistToWater != -32000.0f) &&
                    (floorY < (player->actor.world.pos.y - player->actor.yDistToWater))) {
                    break;
                }
                spawnPos.y = floorY;
            }
            if (self->spawnType == 3) {
                spawnId = 0x1AF;
                spawnParams = (0xFF << 8) | 0x00;
            } else {
                spawnId = 0x1B0;
                spawnParams = 0;

                kcOver10 = self->killCount / 10;
                if (kcOver10 > 0) {
                    tempmod = self->killCount % 10;
                    if (tempmod == 0) {
                        spawnParams = kcOver10 * 5;
                    }
                }
                self->killCount++;
            }
            if (Actor_SpawnAsChild(actorCtx, &self->actor, play, spawnId, spawnPos.x, spawnPos.y, spawnPos.z, 0,
                                   0, 0, spawnParams) != NULL) {
                self->curNumSpawn++;
                if (self->maxCurSpawns <= self->curNumSpawn) {
                    self->fieldSpawnTimer = 150;
                }
                if (PLAY_SCENE_ID(play) != 0x51) {
                    self->totalNumSpawn++;
                }
            } else {
                break;
            }
        }
    }
}
#endif


// N64 sLeeverAngles / sLeeverDists: no symbols in the scatter file -> absolute EQU symbols in data_syms.s.
GLOBAL_ASM("../src/game/actors/En_Encount1/data_syms.s")
extern s16 EnEncount1_sLeeverAngles[];
extern f32 EnEncount1_sLeeverDists[];

s32 FUN_0035ea34(CollisionContext* colCtx, CollisionPoly* poly, s32 bgId); // SurfaceType_GetFloorType

void FUN_001cc77c(EnEncount1* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 floorType;
    f32 spawnDist;
    s16 spawnParams;
    s16 spawnLimit;
    s16 spawnAngle;
    Vec3f spawnPos;
    CollisionPoly* floorPoly;
    s32 bgId;
    f32 floorY;
    Actor* leever;

    self->outOfRangeTimer = 0;
    spawnPos = self->actor.world.pos;

    if ((self->timer == 0) && (play->GetCutsceneState() == 0) && (self->maxCurSpawns >= self->curNumSpawn) &&
        (self->curNumSpawn <= 4)) {
        floorType = FUN_0035ea34(&play->colCtx, player->actor.floorPoly, player->actor.floorBgId);
        if ((floorType != 4) && (floorType != 7) && (floorType != 12)) {
            self->numLeeverSpawns = 0;
        } else if (!(self->reduceLeevers && (self->actor.xzDistToPlayer > 1300.0f))) {
            spawnLimit = 5;
            if (self->reduceLeevers) {
                spawnLimit = 3;
            }
            while ((self->maxCurSpawns > self->curNumSpawn) && (self->curNumSpawn < spawnLimit) && (self->timer == 0)) {
                spawnDist = EnEncount1_sLeeverDists[self->leeverIndex];
                spawnAngle = player->actor.shape.rot.y + EnEncount1_sLeeverAngles[self->leeverIndex];
                spawnParams = 0;

                if ((self->killCount >= 10) && (self->bigLeever == NULL)) {
                    self->killCount = self->numLeeverSpawns = 0;
                    spawnAngle = EnEncount1_sLeeverAngles[0];
                    spawnDist = EnEncount1_sLeeverDists[2];
                    spawnParams = 1;
                }

                spawnPos.x = player->actor.world.pos.x + Math_SinS(spawnAngle) * spawnDist;
                spawnPos.y = player->actor.floorHeight + 120.0f;
                spawnPos.z = player->actor.world.pos.z + Math_CosS(spawnAngle) * spawnDist;

                floorY = BgCheck_EntityRaycastFloor4(&play->colCtx, &floorPoly, &bgId, &self->actor, &spawnPos);
                if (floorY <= -32000.0f) {
                    break;
                }
                spawnPos.y = floorY;

                leever = Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, 0x1C, spawnPos.x, spawnPos.y,
                                            spawnPos.z, 0, 0, 0, spawnParams);

                if (leever != NULL) {
                    self->curNumSpawn++;
                    *(s16*)((u8*)leever + 0x4B4) = self->leeverIndex++; // EnReeba aimType
                    if (self->leeverIndex > 4) {
                        self->leeverIndex = 0;
                    }
                    self->numLeeverSpawns++;
                    if (self->numLeeverSpawns > 11) {
                        self->timer = 225;
                        self->numLeeverSpawns = 0;
                    }
                    if (spawnParams != 0) {
                        self->timer = 450;
                        if (self->mqActor != NULL) {
                            Actor_Kill(self->mqActor);
                            self->mqActor = NULL;
                        }
                        self->bigLeever = leever;
                    }
                    if (!self->reduceLeevers) {
                        self->maxCurSpawns = (s16)Rand_ZeroFloat(3.99f) + 2;
                    } else {
                        self->maxCurSpawns = (s16)Rand_ZeroFloat(2.99f) + 1;
                    }
                } else {
                    break;
                }
            }
        }
    }
}
}
