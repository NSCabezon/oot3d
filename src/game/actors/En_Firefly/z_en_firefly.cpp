extern "C" {
#include "z_en_firefly.hpp"

#define FLAGS 0x00005005

void EnFirefly_Init(Actor* actor, GameState* state);
void EnFirefly_Destroy(Actor* actor, GameState* state);
void EnFirefly_Update(Actor* actor, GameState* state);
void EnFirefly_Draw(Actor* actor, GameState* state);
void EnFirefly_Die(EnFirefly* self, PlayState* play);
void EnFirefly_SetupFlyIdle(EnFirefly* self);
void EnFirefly_FlyIdle(EnFirefly* self, PlayState* play);
void EnFirefly_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg);
void Collider_DestroyJntSph(PlayState* play, void* collider);
void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* pos, s32 params);
void SkelAnime_Free2(SkelAnime* skelAnime);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
f32 Rand_ZeroOne(void);
s32 Actor_WorldYawTowardPoint(Actor* actor, Vec3f* point);
s32 Math_ScaledStepToS(s16* value, s16 target, s16 scale);
s32 EnFirefly_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg);
void FUN_00357a50(SkelAnime* skelAnime, s32 index, s32 count, f32* color, s32 mode);
f32 __hardfp_sinf(f32 x);
f32 __hardfp_cosf(f32 x);

typedef struct {
    u8 pad[0x208F];
    u8 lensActive;
} EnFireflyPlayLensView;
typedef struct {
    u8 pad[0x7F44];
    f32 drawFloat;
} EnFireflyPlayDrawView;
extern f32 EnFirefly_sFlameColor[4];
extern f32 EnFirefly_sNormalColor[4];

GLOBAL_ASM("../src/game/actors/En_Firefly/data_syms.s")

GLOBAL_ASM("data/z_En_Firefly.data.s")

ActorInit En_Firefly_InitVars = {
    ACTOR_EN_FIREFLY,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_FIREFLY,
    sizeof(EnFirefly),
    (ActorFunc)EnFirefly_Init,
    (ActorFunc)EnFirefly_Destroy,
    (ActorFunc)EnFirefly_Update,
    (ActorFunc)EnFirefly_Draw,
};

GLOBAL_ASM("asm/EnFirefly_Init.s")

void EnFirefly_Destroy(Actor* actor, GameState* state) {
    EnFirefly* self = (EnFirefly*)actor;
    PlayState* play = (PlayState*)state;
    Collider_DestroyJntSph(play, self->collider);
    FUN_003508b8(actor, self->model, 0);
    SkelAnime_Free2(&self->skelAnime);
}

GLOBAL_ASM("asm/EnFirefly_Update.s")

#if 0
void EnFirefly_Draw(Actor* actor, GameState* state) {
    EnFirefly* self = (EnFirefly*)actor;
    PlayState* play = (PlayState*)state;
    f32* color;
    s32 drawEnabled;
    if (self->flameType != 0) {
        color = EnFirefly_sFlameColor;
    } else {
        color = EnFirefly_sNormalColor;
    }
    FUN_00357a50(&self->skelAnime, 1, 3, color, 2);
    FUN_00357fd0(GET_PLAYER(play), actor->unk_178, &actor->world.pos);
    drawEnabled = ((EnFireflyPlayDrawView*)play)->drawFloat != 0.0f;
    self->drawEnabled = drawEnabled;
    SkelAnime_DrawOpa(&self->skelAnime, &actor->modelMtx, EnFirefly_OverrideLimbDraw,
                      EnFirefly_PostLimbDraw, actor, 0);
}
#endif
GLOBAL_ASM("asm/EnFirefly_Draw.s")

void EnFirefly_Die(EnFirefly* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
    }
    Math_StepToF(&self->actor.scale.x, 0.0f, 0.00034f);
    self->actor.scale.z = self->actor.scale.x;
    self->actor.scale.y = self->actor.scale.x;
    if (self->timer == 0) {
        Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos, 0xE0);
        Actor_Kill(&self->actor);
    }
}

GLOBAL_ASM("asm/EnFirefly_DisturbDiveAttack.s")

GLOBAL_ASM("asm/EnFirefly_DiveAttack.s")

#if 0
void EnFirefly_DrawInvisible(Actor* actor, GameState* state) {
    EnFirefly* self = (EnFirefly*)actor;
    PlayState* play = (PlayState*)state;
    if (((EnFireflyPlayLensView*)play)->lensActive != 0) {
        f32* color;
        if (self->flameType != 0) {
            color = EnFirefly_sFlameColor;
        } else {
            color = EnFirefly_sNormalColor;
        }
        FUN_00357a50(&self->skelAnime, 1, 3, color, 2);
        FUN_00357fd0(GET_PLAYER(play), actor->unk_178, &actor->world.pos);
        self->drawEnabled = ((EnFireflyPlayDrawView*)play)->drawFloat != 0.0f;
        SkelAnime_DrawOpa(&self->skelAnime, &actor->modelMtx, EnFirefly_OverrideLimbDraw,
                          EnFirefly_PostLimbDraw, actor, 0);
    }
}
#endif
GLOBAL_ASM("asm/EnFirefly_DrawInvisible.s")

GLOBAL_ASM("asm/EnFirefly_Fall.s")

GLOBAL_ASM("asm/EnFirefly_FlyAway.s")

GLOBAL_ASM("asm/EnFirefly_FlyIdle.s")

#if 0
void EnFirefly_FrozenFall(EnFirefly* self, PlayState* play) {
    if ((self->actor.bgCheckFlags & 1) == 0 && self->actor.floorHeight < 0.000128f) {
        *(s16*)((u8*)&self->actor + 0x11A) = 0x17F;
    } else {
        *(s16*)((u8*)&self->actor + 0x11A) = 0;
        self->timer = 23;
        self->actor.velocity.y = 0.0001277923583984375f;
        self->actionFunc = (ActorFunc)EnFirefly_Die;
    }
}
#endif
GLOBAL_ASM("asm/EnFirefly_FrozenFall.s")

#if 0
s32 EnFirefly_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg) {
    if (limbIndex == 0) {
        f32 angle = 0.22050973773002625f;
        f32 sinv = __hardfp_sinf(angle);
        f32 cosv = __hardfp_cosf(angle);
        f32 old0 = mtx->data[0][0];
        f32 old2 = mtx->data[0][2];
        f32 old4 = mtx->data[1][0];
        f32 old6 = mtx->data[1][2];
        f32 old8 = mtx->data[2][0];
        f32 old10 = mtx->data[2][2];
        mtx->data[0][0] = old0 * cosv - old2 * sinv;
        mtx->data[0][2] = old0 * sinv + old2 * cosv;
        mtx->data[1][0] = old4 * cosv - old6 * sinv;
        mtx->data[1][2] = old4 * sinv + old6 * cosv;
        mtx->data[2][0] = old8 * cosv - old10 * sinv;
        mtx->data[2][2] = old8 * sinv + old10 * cosv;
    }
    return 0;
}
#endif
GLOBAL_ASM("asm/EnFirefly_OverrideLimbDraw.s")

GLOBAL_ASM("asm/EnFirefly_Perch.s")

GLOBAL_ASM("asm/EnFirefly_PostLimbDraw.s")

GLOBAL_ASM("asm/EnFirefly_Rebound.s")

#if 0
void EnFirefly_SetupFlyIdle(EnFirefly* self) {
    s16 yaw;
    f32 altitude;
    f32 lastFrame;
    self->timer = Rand_S16Offset_003702c8(0x46, 0x64);
    altitude = 1.5f + Rand_ZeroOne() * 1.5f;
    self->actor.speedXZ = altitude;
    yaw = Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos);
    Math_ScaledStepToS(&self->actor.world.rot.y, yaw, 0x300);
    self->targetPitch = self->actor.world.pos.y > self->homeY ? 0xC00 : -0xC00;
    self->targetPitch += 0x1554;
    lastFrame = Animation_GetLastFrame(&self->skelAnime, 0);
    Animation_Change_00375c08(&self->skelAnime, 0, -5.0f, 0.0f, lastFrame, 2, 0.0f);
    self->actionFunc = (ActorFunc)EnFirefly_FlyIdle;
}
#endif
GLOBAL_ASM("asm/EnFirefly_SetupFlyIdle.s")

#if 0
void EnFirefly_Stunned(EnFirefly* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    Math_StepToF(&self->actor.speedXZ, 0.0f, 0.5f);
    Math_ScaledStepToS(&self->actor.shape.rot.x, 0x1554, 0x100);
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        if (self->flameType != 0) {
            self->auraType = 1;
        } else if (self->actor.params == 4) {
            self->auraType = 2;
        }
        EnFirefly_SetupFlyIdle(self);
    }
}
#endif
GLOBAL_ASM("asm/EnFirefly_Stunned.s")

}
