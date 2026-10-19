extern "C" {
#include "z_shot_sun.hpp"

#define FLAGS 0x80000009

void ShotSun_Init(Actor* thisx, PlayState* play);
void ShotSun_Destroy(Actor* thisx, PlayState* play);
void ShotSun_Update(Actor* thisx, PlayState* play);
void ShotSun_UpdateHyliaSun(ShotSun* self, PlayState* play);
void FUN_00142894(ShotSun* self, PlayState* play);

GLOBAL_ASM("data/z_Shot_Sun.data.s")

ActorInit Shot_Sun_InitVars = {
    ACTOR_SHOT_SUN,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ShotSun),
    (ActorFunc)ShotSun_Init,
    (ActorFunc)ShotSun_Destroy,
    (ActorFunc)ShotSun_Update,
    NULL,
};

// Draft (not matching yet): Init compiles with the right ops but the register
// choice (r1 vs r0 for the flag temporaries) and the else-block layout differ.
#if 0
void ShotSun_Init(Actor* thisx, PlayState* play) {
    ShotSun* self = (ShotSun*)thisx;
    s32 params = self->actor.params & 0xFF;

    if (params == 0x40 || params == 0x41) {
        self->fairySpawnerState = 0;
        self->actor.flags |= 0x2000000; // ACTOR_FLAG_UPDATE_CULLING_DISABLED
        self->actor.flags |= 0x10;      // ACTOR_FLAG_UPDATE_DURING_OCARINA
        self->actionFunc = (ShotSunActionFunc)FUN_00142894;
        self->actor.flags |= 0x8000000; // ACTOR_FLAG_LOCK_ON_DISABLED
    } else {
        Collider_InitCylinder(play, &self->collider);
        Collider_SetCylinder(play, &self->collider, &self->actor, &sCylinderInit);
        self->actionFunc = ShotSun_UpdateHyliaSun;
        self->actor.flags &= ~1; // ACTOR_FLAG_ATTENTION_ENABLED
    }
}
#endif
GLOBAL_ASM("asm/ShotSun_Init.s")

void ShotSun_Destroy(Actor* thisx, PlayState* play) {
    ShotSun* self = (ShotSun*)thisx;
    s32 params = self->actor.params & 0xFF;

    if (params != 0x40 && params != 0x41) {
        Collider_DestroyCylinder(play, &self->collider);
    }
}

void ShotSun_Update(Actor* thisx, PlayState* play) {
    ShotSun* self = (ShotSun*)thisx;

    self->actionFunc(self, play);
}

// Draft (not matching yet): 3DS version differs from N64 (sfx via FUN_00372244 / Audio_PlaySoundGeneral,
// direct gSaveContext reads, dayTime check at 0x587958). Kept as asm for now.
#if 0
void ShotSun_UpdateHyliaSun(ShotSun* self, PlayState* play) {
    Vec3s cylinderPos;
    Player* player = GET_PLAYER(play);
    EnItem00* collectible;
    STACK_PAD(s32);

    if (self->collider.base.acFlags & AC_HIT) {
        Sfx_PlaySfxCentered(NA_SE_SY_CORRECT_CHIME);
        if (INV_CONTENT(ITEM_ARROW_FIRE) == ITEM_NONE) {
            Actor_Spawn(&play->actorCtx, play, ACTOR_ITEM_ETCETERA, 700.0f, -800.0f, 7261.0f, 0, 0, 0, 7);
            play->csCtx.script = SEGMENTED_TO_VIRTUAL(gLakeHyliaFireArrowsCS);
            gSaveContext.cutsceneTrigger = 1;
        } else {
            Vec3f spawnPos = { 700.0f, -800.0f, 7261.0f };

            collectible = Item_DropCollectible(play, &spawnPos, ITEM00_MAGIC_LARGE);
            if (collectible != NULL) {
                collectible->despawnTimer = 6000;
                collectible->actor.speed = 0.0f;
            }
        }
        Actor_Kill(&self->actor);
    } else {
        if (!(self->actor.xzDistToPlayer > 120.0f) && gSaveContext.save.dayTime >= CLOCK_TIME(6, 30) &&
            gSaveContext.save.dayTime < CLOCK_TIME(7, 30)) {
            cylinderPos.x = player->bodyPartsPos[PLAYER_BODYPART_HEAD].x + play->envCtx.sunPos.x * (1.0f / 6.0f);
            cylinderPos.y = player->bodyPartsPos[PLAYER_BODYPART_HEAD].y - 30.0f + play->envCtx.sunPos.y * (1.0f / 6.0f);
            cylinderPos.z = player->bodyPartsPos[PLAYER_BODYPART_HEAD].z + play->envCtx.sunPos.z * (1.0f / 6.0f);

            Collider_SetCylinderPosition(&self->collider, &cylinderPos);
            CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
        }
    }
}
#endif
GLOBAL_ASM("asm/ShotSun_UpdateHyliaSun.s")

void FUN_0038ce68(ColliderCylinder* collider, Vec3s* pos) {
    collider->dim.pos.x = pos->x;
    collider->dim.pos.y = pos->y;
    collider->dim.pos.z = pos->z;
}

}
