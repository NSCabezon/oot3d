extern "C" {
#include "z_en_niw_girl.hpp"

#define FLAGS 0x00000019

#define RAD_TO_BINANG(rad) ((s16)((rad) * 10430.378f))

#define PLAYER_MASK_KEATON 1
#define PLAYER_MASK_SKULL 2
#define PLAYER_MASK_SPOOKY 3
#define PLAYER_MASK_BUNNY 4
#define PLAYER_MASK_GORON 5
#define PLAYER_MASK_ZORA 6
#define PLAYER_MASK_GERUDO 7
#define PLAYER_MASK_TRUTH 8

void EnNiwGirl_Init(Actor* actor, GameState* state);
void EnNiwGirl_Destroy(Actor* actor, GameState* state);
void EnNiwGirl_Update(Actor* actor, GameState* state);
void EnNiwGirl_Draw(Actor* thisx, PlayState* play);
s32 EnNiwGirl_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void FUN_0034df48(MTX34* mtx, Vec3s* rot);
void FUN_00357878(Actor* actor, Vec3f* pos, Vec3f* scale, s32 alpha, PlayState* play);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_003e6778(EnNiwGirl* self, PlayState* play);
void FUN_003ca608(EnNiwGirl* self, PlayState* play);
void FUN_001584a8(EnNiwGirl* self, PlayState* play);
void EnNiwGirl_Talk(EnNiwGirl* self, PlayState* play);
s32 Player_GetMask(PlayState* play);
void __ARM_common_memcpy2_6(void* dst, void* src);
void FUN_0034c664(Actor* actor, EnNiwGirlInteractInfo* info, s32 arg2, s32 arg3);

GLOBAL_ASM("data/z_En_Niw_Girl.data.s")

ActorInit En_Niw_Girl_InitVars = {
    ACTOR_EN_NIW_GIRL,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GR,
    sizeof(EnNiwGirl),
    (ActorFunc)EnNiwGirl_Init,
    (ActorFunc)EnNiwGirl_Destroy,
    (ActorFunc)EnNiwGirl_Update,
    (ActorFunc)EnNiwGirl_Draw,
};

GLOBAL_ASM("asm/EnNiwGirl_Init.s")

void EnNiwGirl_Destroy(Actor* thisx, GameState* state) {
    EnNiwGirl* self = (EnNiwGirl*)thisx;

    FUN_003508b8(&self->actor, self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
    ((FaceAnimations*)self->faceAnims)->Destroy();
}

void EnNiwGirl_Update(Actor* thisx, GameState* state) {
    EnNiwGirl* self = (EnNiwGirl*)thisx;
    PlayState* play = (PlayState*)state;
    Player* player = GET_PLAYER(play);
    EnNiwGirl* s2 = self;

    Player* p2 = player;
    Actor_SetScale(&self->actor, 0.013f);
    self->unkUpTimer++;
    if (self->blinkTimer == 0) {
        self->eyeIndex++;
        if (self->eyeIndex > 2) {
            self->eyeIndex = 0;
            s2->blinkTimer = (s16)Rand_ZeroFloat(60.0f) + 20;
        }
    }
    self->unk_280 = 30.0f;
    Actor_SetFocus(&self->actor, 30.0f);
    if (self->actionFunc == FUN_003e6778) {
        self->interactInfo.trackPos = p2->actor.world.pos;
        if (!LINK_IS_ADULT) {
            self->interactInfo.trackPos.y = p2->actor.world.pos.y - 10.0f;
        }
        FUN_0034c664(&self->actor, &self->interactInfo, 2, 4);
        self->unk_8AC = self->interactInfo.headRot;
        __ARM_common_memcpy2_6(&self->unk_8B2, &self->interactInfo.torsoRot);
    } else {
        Math_SmoothStepToS(&self->unk_8B2.y, 0, 5, 3000, 0);
        Math_SmoothStepToS(&self->unk_8AC.y, 0, 5, 3000, 0);
        Math_SmoothStepToS(&self->unk_8AC.z, 0, 5, 3000, 0);
    }
    if (self->blinkTimer != 0) {
        self->blinkTimer--;
    }
    if (self->jumpTimer != 0) {
        self->jumpTimer--;
    }
    self->actionFunc(self, play);
    Actor_MoveForward(&self->actor);
    if (self->unk_B24 != 0) {
        Actor_UpdateBgCheckInfo(play, &self->actor, 100.0f, 100.0f, 200.0f, 0x1C);
        if (play->sceneNum == 0x20) {
            if (self->actor.bgCheckFlags & 3) {
                self->actor.velocity.y = self->actor.gravity = 0.0f;
                self->unk_B24 = 0;
            }
        }
    }
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
}

void EnNiwGirl_Draw(Actor* thisx, PlayState* play) {
    EnNiwGirl* self = (EnNiwGirl*)thisx;
    Vector3f sp4C(0.2f, 0.2f, 0.2f);

    {
        FaceAnimations* fa = (FaceAnimations*)((EnNiwGirl*)thisx)->faceAnims;
        fa->FUN_0035e3a4(0, self->eyeIndex);
    }
    GraphicsContext_UnknownStub(play->state.gfxCtx);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, (OverrideLimbDrawOpa)EnNiwGirl_OverrideLimbDraw, NULL,
                      &self->actor, 0);
    FUN_00357878(&self->actor, &self->actor.world.pos, (Vec3f*)&sp4C, 255, play);
    ((FaceAnimations*)self->faceAnims)->FUN_0035e330();
}

s32 EnNiwGirl_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnNiwGirl* self = (EnNiwGirl*)thisx;

    if (limbIndex == 9) {
        FUN_0034df48(mtx, &self->unk_8AC);
    } else if (limbIndex == 8) {
        FUN_0034df48(mtx, &self->unk_8B2);
    }
    return 0;
}

void FUN_003e6778(EnNiwGirl* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (Message_GetState(&play->msgCtx) != 0) {
        *(s16*)((u8*)self->chasedEnNiw + 0x668) = 0;
    }
    Math_ApproachZeroF(&self->actor.speedXZ, 0.8f, 0.2f);
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        if (self->actor.textId == 0x70EA) {
            self->unk_27A = 1;
        }
    } else {
        if ((self->jumpTimer == 0) && (Message_GetState(&play->msgCtx) == 0)) {
            self->jumpTimer = Rand_ZeroFloat(100.0f) + 250.0f;
            self->actionFunc = FUN_001584a8;
        } else {
            FUN_0036bb28(&self->actor, play, 100.0f);
        }
    }
}

void FUN_003ca608(EnNiwGirl* self, PlayState* play) {
    Path* path = &play->setupPathList[self->path];
    f32 xDistBetween;
    f32 zDistBetween;

    FUN_003731e0(&self->skelAnime);
    Math_ApproachF(&self->actor.speedXZ, 3.0f, 0.2f, 0.4f);
    xDistBetween = ((Actor*)self->chasedEnNiw)->world.pos.x - self->actor.world.pos.x;
    zDistBetween = ((Actor*)self->chasedEnNiw)->world.pos.z - self->actor.world.pos.z;
    if (Message_GetState(&play->msgCtx) != 0) {
        *(s16*)((u8*)self->chasedEnNiw + 0x668) = 0;
    }
    f32 dist = sqrtf(SQ(xDistBetween) + SQ(zDistBetween));
    if (dist < 70.0f) {
        *(s16*)((u8*)self->chasedEnNiw + 0x668) = self->path + 1;
        *(s16*)((u8*)self->chasedEnNiw + 0x66C) = path->count;
    } else if (dist > 150.0f) {
        *(s16*)((u8*)self->chasedEnNiw + 0x668) = 0;
    }
    Math_SmoothStepToS(&self->actor.shape.rot.y, RAD_TO_BINANG(Math_FAtan2F(xDistBetween, zDistBetween)), 3,
                       self->unk_27C, 0);
    Math_ApproachF(&self->unk_27C, 5000.0f, 30.0f, 150.0f);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    if ((self->jumpTimer == 0) || (Player_GetMask(play) != 0)) {
        self->jumpTimer = 60;
        self->actionFunc = EnNiwGirl_Talk;
    }
}

void FUN_001584a8(EnNiwGirl* self, PlayState* play) {
    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 0), 0, -10.0f);
    self->actor.flags &= ~1;
    self->actionFunc = FUN_003ca608;
}

void EnNiwGirl_Talk(EnNiwGirl* self, PlayState* play) {
    Animation_Change_00375c08(&self->skelAnime, 1, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 1), 0, -10.0f);
    self->actor.flags |= 1;
    self->actor.textId = 0x7000;
    if ((gSaveContext.eventChkInf[8] & 1) && (self->unk_27A == 0)) {
        self->actor.textId = 0x70EA;
    }
    switch (Player_GetMask(play)) {
        case PLAYER_MASK_KEATON:
            self->actor.textId = 0x7118;
            break;
        case PLAYER_MASK_SPOOKY:
            self->actor.textId = 0x7119;
            break;
        case PLAYER_MASK_BUNNY:
        case PLAYER_MASK_ZORA:
        case PLAYER_MASK_GERUDO:
            self->actor.textId = 0x711A;
            break;
        case PLAYER_MASK_SKULL:
        case PLAYER_MASK_GORON:
        case PLAYER_MASK_TRUTH:
            self->actor.textId = 0x711B;
            break;
    }
    self->unk_8BC = 6;
    self->actionFunc = FUN_003e6778;
}

}
