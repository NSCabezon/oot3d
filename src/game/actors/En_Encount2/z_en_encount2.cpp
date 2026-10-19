extern "C" {
#include "z_en_encount2.hpp"

#define FLAGS 0x00000030

void EnEncount2_Init(Actor* actor, PlayState* play);
void EnEncount2_Destroy(Actor* actor, GameState* state);
void FUN_0034fda8(Actor* actor, s32 count, SkeletonAnimationModel** models);
void FUN_00352ee0(Actor* actor, PlayState* play, s32 count, SkeletonAnimationModel** models, s32* modelIds);
void FUN_003f4210(EnEncount2* self, PlayState* play);
void FUN_003c1924(EnEncount2* self, PlayState* play);
s32 Quake_Add(Camera* camera, s32 type);
void Quake_SetSpeed(s32 quakeIndex, s32 speed);
void Quake_SetQuakeValues(s32 quakeIndex, s32 a, s32 b, s32 c, s32 d);
void Quake_SetCountdown(s32 quakeIndex, s32 countdown);
void EnEncount2_Update(Actor* actor, GameState* state);
void EnEncount2_Draw(Actor* actor, GameState* state);

ActorInit En_Encount2_InitVars = {
    ACTOR_EN_ENCOUNT2,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_EFC_STAR_FIELD,
    sizeof(EnEncount2),
    (ActorFunc)EnEncount2_Init,
    (ActorFunc)EnEncount2_Destroy,
    (ActorFunc)EnEncount2_Update,
    (ActorFunc)EnEncount2_Draw,
};

void EnEncount2_Init(Actor* thisx, PlayState* play) {
    EnEncount2* self = (EnEncount2*)thisx;
    s32 pad;
    s32 modelIds[30];
    s32 i;

    if (play->sceneNum != 0x60) {
        self->isNotDeathMountain = true;
    }

    if (!self->isNotDeathMountain) {
        if (LINK_IS_ADULT && (gSaveContext.eventChkInf[4] & 0x200)) {
            Actor_Kill(thisx);
        }
    }

    for (i = 0; i < 30; i++) {
        if (i & 1) {
            modelIds[i] = 2;
        } else {
            modelIds[i] = 3;
        }
    }

    FUN_00352ee0(thisx, play, 30, self->models, modelIds);
    thisx->unk_19B = 3;
    self->actionFunc = FUN_003f4210;
}

void EnEncount2_Destroy(Actor* thisx, GameState* state) {
    EnEncount2* self = (EnEncount2*)thisx;

    FUN_0034fda8(thisx, 30, self->models);
}

void FUN_003f4210(EnEncount2* self, PlayState* play) {
    s32 quakeIndex;
    s16 spawnerState;
    Player* player = GET_PLAYER(play);

    spawnerState = 0;
    if (!self->isNotDeathMountain) {
        if ((player->actor.world.pos.y > 1500.0f) && (player->actor.world.pos.x > -700.0f) &&
            (player->actor.world.pos.x < 100.0f) && (player->actor.world.pos.z < -1290.0f) &&
            (player->actor.world.pos.z > -3600.0f)) {
            spawnerState = 1;
        }
    } else if ((self->actor.xzDistToPlayer < 700.0f) && (Flags_GetSwitch(play, 0x37))) {
        s16 sceneId = play->sceneNum;

        if (((sceneId == 0x4F) || (sceneId == 0x1A) || (sceneId == 0x0E) || (sceneId == 0x0F)) &&
            (!self->collapseSpawnerInactive)) {
            spawnerState = 2;
        }
    }

    switch (spawnerState) {
        case 0:
            self->isQuaking = 0;
            self->envEffectsTimer--;
            if (self->envEffectsTimer <= 0) {
                self->envEffectsTimer = 0;
            }
            break;
        case 1:
            if ((self->deathMountainSpawnerTimer == 1) || !self->isQuaking) {
                quakeIndex = Quake_Add(play->cameraPtrs[play->activeCamera], 1);
                Quake_SetSpeed(quakeIndex, 0x7FFF);
                Quake_SetQuakeValues(quakeIndex, 5, 0, 0, 0);
                Quake_SetCountdown(quakeIndex, 200);
                self->isQuaking = 1;
            }
        case 2:
            self->envEffectsTimer++;
            if (self->envEffectsTimer > 90) {
                self->envEffectsTimer = 90;
            }
            if (self->deathMountainSpawnerTimer == 0) {
                self->deathMountainSpawnerTimer = 300;
                self->numSpawnedRocks = 0;
                self->actionFunc = FUN_003c1924;
            }
            break;
    }
}

GLOBAL_ASM("asm/EnEncount2_Update.s")

GLOBAL_ASM("asm/EnEncount2_Draw.s")
}
