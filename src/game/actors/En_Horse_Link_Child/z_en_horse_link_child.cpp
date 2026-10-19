extern "C" {
#include "z_en_horse_link_child.hpp"
#include "regs.hpp"

#define FLAGS 0x02000010

void EnHorseLinkChild_Init(Actor* actor, GameState* state);
void EnHorseLinkChild_Destroy(Actor* actor, GameState* state);
void EnHorseLinkChild_Update(Actor* actor, GameState* state);
void EnHorseLinkChild_Draw(EnHorseLinkChild* self, PlayState* play);

GLOBAL_ASM("data/z_En_Horse_Link_Child.data.s")
GLOBAL_ASM("../src/game/actors/En_Horse_Link_Child/data_syms.s")

extern void* EnHorseLinkChild_sAnimations[];
extern u8 EnHorseLinkChild_sEyeIndexOrder[];
extern f32 EnHorseLinkChild_sSpeedScale[];
extern void* EnHorseLinkChild_sInitChain[];
void ActorShadow_DrawHorse(Actor* actor, Lights* lights, PlayState* play);
s32 Collider_InitCylinder(PlayState* play, ColliderCylinder* collider);
void Collider_SetCylinderType1(PlayState* play, ColliderCylinder* collider, Actor* actor, void* src);
void Collider_InitJntSph(PlayState* play, EnHorseLinkChildJntSph* collider);
void Collider_SetJntSph(PlayState* play, EnHorseLinkChildJntSph* collider, Actor* actor, void* src, void* elements);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_00358ea8(void* zar, PlayState* play, SkelAnime* skelAnime, void* cmb, void* p, s32 a, void* b, void* c, s32 d);

s16 Actor_WorldYawTowardActor(Actor* a, Actor* b);
void EnHorse_RotateToPoint(Actor* actor, Vec3f* pos, s16 turnAmount);
f32 Rand_ZeroOne(void);

void Collider_DestroyJntSph(PlayState* play, EnHorseLinkChildJntSph* collider);
void Animation_PlayOnce(SkelAnime* skelAnime, void* anim);
void SkelAnime_SetPlaySpeed(SkelAnime* skelAnime, f32 speed);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, void* anim);
void Animation_Change_00375c08(SkelAnime* skelAnime, void* anim, f32 speed, f32 startFrame, f32 endFrame, s32 mode,
                               f32 morphFrames);
f32 Actor_WorldDistXZToActor(Actor* a, Actor* b);
void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx); // N64: Gfx_SetupDL_25Opa (empty on 3DS)
void FUN_00341e68(Actor* actor, PlayState* play, void* skin, void* postDraw, void* overrideLimbDraw, s32 flag);
void FUN_003409ac(void* skin, s32 limbIndex, Vec3f* src, Vec3f* dst);
f32 FUN_00348854(Actor* actor);
void FUN_001e1dfc(Actor* thisx, PlayState* play, void* skin);
void FUN_002874b0(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
f32* EnHorse_CurFramePtr(SkelAnime* skelAnime);

// FUN_003731e0 returns SkelAnime_Update's result; include declares it void.
static inline s32 EnHorseLinkChild_SkelAnimeUpdate(SkelAnime* skelAnime) {
    return FUN_003731e0(skelAnime);
}

ActorInit En_Horse_Link_Child_InitVars = {
    ACTOR_EN_HORSE_LINK_CHILD,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HORSE_LINK_CHILD,
    sizeof(EnHorseLinkChild),
    (ActorFunc)EnHorseLinkChild_Init,
    (ActorFunc)EnHorseLinkChild_Destroy,
    (ActorFunc)EnHorseLinkChild_Update,
    (ActorFunc)EnHorseLinkChild_Draw,
};

static inline u8* EnHorseLinkChild_GetObjBank(PlayState* play, s32 idx) {
    if ((u32)idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

// N64 func_80A69EC0
static inline void EnHorseLinkChild_SetupIdle(EnHorseLinkChild* self) {
    self->action = 3;
    self->actor.speedXZ = 0.0f;
    self->animationIdx = 0;
    Animation_Change_00375c08(&self->skin.skelAnime, EnHorseLinkChild_sAnimations[self->animationIdx],
                              FUN_00348854(&self->actor), 0.0f,
                              Animation_GetLastFrame(&self->skin.skelAnime,
                                                     EnHorseLinkChild_sAnimations[self->animationIdx]),
                              2, -5.0f);
}

void EnHorseLinkChild_Init(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnHorseLinkChild* self = (EnHorseLinkChild*)thisx;
    u8* zar;

    Actor_ProcessInitChain(&self->actor, (InitChainEntry*)EnHorseLinkChild_sInitChain);
    Actor_SetScale(&self->actor, 0.005f);
    self->actor.gravity = -3.5f;
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawHorse, 20.0f);
    self->action = 1;
    self->actor.speedXZ = 0.0f;
    thisx->focus.pos = thisx->world.pos;
    thisx->focus.pos.y += 70.0f;
    zar = EnHorseLinkChild_GetObjBank(play, (u8)self->actor.objBankIndex) + 0x10;
    FUN_00358ea8(zar, play, &self->skin.skelAnime, ZAR_GetCMBByIndex(zar, 0), self->actor.unk_178, 0,
                 &self->unk_23C[0x240 - 0x23C], &self->unk_23C[0x720 - 0x23C], 0x18);
    SkelAnime_SetFaceAnimations((FaceAnimations*)self->unk_C00, &self->skin.skelAnime, 0, -1, -1);
    self->animationIdx = 0;
    Animation_PlayOnce(&self->skin.skelAnime, EnHorseLinkChild_sAnimations[0]);
    Collider_InitCylinder(play, &self->bodyCollider);
    Collider_SetCylinderType1(play, &self->bodyCollider, &self->actor, (u8*)EnHorseLinkChild_sAnimations + 0x14);
    Collider_InitJntSph(play, &self->headCollider);
    Collider_SetJntSph(play, &self->headCollider, &self->actor, (u8*)EnHorseLinkChild_sAnimations + 0x4C,
                        &self->unk_E54[0]);
    // sCylinderInit / sJntSphInit / sColCheckInfoInit sit at fixed offsets from sAnimations (ROM addresses them via the same base register).
    CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL,
                           (CollisionCheckInfoInit*)((u8*)EnHorseLinkChild_sAnimations - 0x10));
    self->unk_1F0 = 0;
    self->eyeTexIndex = 0;

    if (gSaveContext.sceneSetupIndex >= 4) {
        EnHorseLinkChild_SetupIdle(self);
    } else if (play->sceneNum == 0x63) {
        if (!Flags_GetEventChkInf(0x14)) {
            Actor_Kill(&self->actor);
            return;
        }
        self->unk_2A0 = gSaveContext.eventChkInf[1] & 0x40;
        EnHorseLinkChild_SetupIdle(self);
    } else {
        EnHorseLinkChild_SetupIdle(self);
    }

    self->actor.shape.rot.z = 0;
    self->actor.world.rot.z = 0;
    self->actor.home.rot.z = 0;
}

void EnHorseLinkChild_Destroy(Actor* thisx, GameState* state) {
    EnHorseLinkChild* self = (EnHorseLinkChild*)thisx;
    PlayState* play = (PlayState*)state;

    SkelAnime_Free2(&self->skin.skelAnime);
    ((FaceAnimations*)self->unk_C00)->Destroy();
    Collider_DestroyCylinder(play, &self->bodyCollider);
    Collider_DestroyJntSph(play, &self->headCollider);
}

extern s32 EnHorseLinkChild_sStepFrames[];
extern void (*EnHorseLinkChild_sActionFuncs[])(EnHorseLinkChild*, PlayState*);

// N64 func_80A693D0
static inline void EnHorseLinkChild_StepSfx(EnHorseLinkChild* self) {
    if ((*EnHorse_CurFramePtr(&self->skin.skelAnime) > EnHorseLinkChild_sStepFrames[self->unk_1F0]) &&
        !((self->unk_1F0 == 0) && (*EnHorse_CurFramePtr(&self->skin.skelAnime) > EnHorseLinkChild_sStepFrames[1]))) {
        Audio_PlaySoundGeneral(0x01000199, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
        self->unk_1F0++;
        if (self->unk_1F0 >= 2) {
            self->unk_1F0 = 0;
        }
    }
}

// N64 func_80A6948C (as an inline helper: picks ROM's cross-jump target for the shared sfx tail)
static inline void EnHorseLinkChild_Sfx(EnHorseLinkChild* self) {
    if (self->animationIdx == 2) {
        EnHorseLinkChild_StepSfx(self);
    } else if (*EnHorse_CurFramePtr(&self->skin.skelAnime) == 0.0f) {
        if ((self->animationIdx == 3) || (self->animationIdx == 4)) {
            Audio_PlaySoundGeneral(0x0100019A, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                   &DAT_0054ac24.unk_0);
        } else if (self->animationIdx == 1) {
            if (Rand_ZeroOne() > 0.5f) {
                Audio_PlaySoundGeneral(0x0100019C, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                       &DAT_0054ac24.unk_0);
            } else {
                Audio_PlaySoundGeneral(0x0100019B, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                       &DAT_0054ac24.unk_0);
            }
        }
    }
}

void EnHorseLinkChild_Update(Actor* thisx, GameState* state) {
    EnHorseLinkChild* self = (EnHorseLinkChild*)thisx;
    PlayState* play = (PlayState*)state;

    EnHorseLinkChild_sActionFuncs[self->action](self, play);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 20.0f, 55.0f, 100.0f, 0x1D);

    if (play->sceneNum == 0x63) {
        f32 z = self->actor.world.pos.z;
        self->actor.world.pos.z = z <= -2400.0f ? -2400.0f : z;
    }

    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += 70.0f;

    if ((Rand_ZeroOne() < 0.025f) && (self->eyeTexIndex == 0)) {
        self->eyeTexIndex++;
    } else if (self->eyeTexIndex > 0) {
        self->eyeTexIndex++;
        if (self->eyeTexIndex > 3) {
            self->eyeTexIndex = 0;
        }
    }

    // thisx (not &self->actor) here: gives ROM's r1 for the eyeTexIndex = 1 store above
    Collider_UpdateCylinder(thisx, &self->bodyCollider);
    CollisionCheck_SetOC(play, &play->colChkCtx, (Collider*)&self->bodyCollider);
    EnHorseLinkChild_Sfx(self);
}

void FUN_0037e568(EnHorseLinkChild* self, PlayState* play) {
    self->actor.speedXZ = 0.0f;
    if (EnHorseLinkChild_SkelAnimeUpdate(&self->skin.skelAnime)) {
        self->action = 0;
        self->animationIdx++;
        if (self->animationIdx >= 5) {
            self->animationIdx = 0;
        }

        Animation_PlayOnce(&self->skin.skelAnime, EnHorseLinkChild_sAnimations[self->animationIdx]);
        SkelAnime_SetPlaySpeed(&self->skin.skelAnime, FUN_00348854(&self->actor));
    }
}

// N64 func_80A699FC (u8 newAnimationIdx declared in the else block: strb/and order + float regs)
void FUN_0039099c(EnHorseLinkChild* self, PlayState* play) {
    f32 distFromLink;

    distFromLink = Actor_WorldDistXZToActor(&self->actor, &GET_PLAYER(play)->actor);

    if (EnHorseLinkChild_SkelAnimeUpdate(&self->skin.skelAnime)) {
        if ((distFromLink < 1000.0f) && (distFromLink > 70.0f)) {
            self->action = 1;
            self->actor.speedXZ = 0.0f;
            self->animationIdx = 0;
            Animation_Change_00375c08(&self->skin.skelAnime, EnHorseLinkChild_sAnimations[self->animationIdx],
                                      FUN_00348854(&self->actor), 0.0f,
                                      Animation_GetLastFrame(&self->skin.skelAnime,
                                                             EnHorseLinkChild_sAnimations[self->animationIdx]),
                                      2, -5.0f);
        } else {
            u8 newAnimationIdx;

            if (self->animationIdx == 1) {
                newAnimationIdx = 0;
            } else {
                newAnimationIdx = 1;
            }
            if (self->animationIdx != newAnimationIdx) {
                self->animationIdx = newAnimationIdx;
                Animation_Change_00375c08(&self->skin.skelAnime, EnHorseLinkChild_sAnimations[self->animationIdx],
                                          FUN_00348854(&self->actor), 0.0f,
                                          Animation_GetLastFrame(&self->skin.skelAnime,
                                                                 EnHorseLinkChild_sAnimations[self->animationIdx]),
                                          2, -5.0f);
            } else {
                Animation_Change_00375c08(&self->skin.skelAnime, EnHorseLinkChild_sAnimations[self->animationIdx],
                                          FUN_00348854(&self->actor), 0.0f,
                                          Animation_GetLastFrame(&self->skin.skelAnime,
                                                                 EnHorseLinkChild_sAnimations[self->animationIdx]),
                                          2, 0.0f);
            }
        }
    }
}

// N64 EnHorseLinkChild_OverrideLimbDraw (FaceAnimations* local declared after the index local, used only for the
// first call: gives ROM's add r0 / mov r1,#0 order)
void FUN_002874b0(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnHorseLinkChild* self = (EnHorseLinkChild*)thisx;

    if (limbIndex == 0xD) {
        u8 index = EnHorseLinkChild_sEyeIndexOrder[self->eyeTexIndex];
        FaceAnimations* fa = (FaceAnimations*)self->unk_C00;

        fa->FUN_0035e3a4(0, index);
        ((FaceAnimations*)self->unk_C00)->FUN_0035e330();
    }
}

// Temporary built member-wise from a Vec3f (interleaved load/store), then struct-copied into the local.
class EnHorseLinkChildVecCopy {
  public:
    EnHorseLinkChildVecCopy(const Vec3f* p) : x(p->x), y(p->y), z(p->z) {}
    f32 x;
    f32 y;
    f32 z;
};

void FUN_001e1dfc(Actor* thisx, PlayState* play, void* skin) {
    EnHorseLinkChild* self = (EnHorseLinkChild*)thisx;
    Vec3f center;
    Vec3f newCenter;
    s32 i;

    for (i = 0; i < self->headCollider.count; i++) {
        center = *(Vec3f*)&EnHorseLinkChildVecCopy(&self->headCollider.elements[i].modelCenter);
        FUN_003409ac(skin, self->headCollider.elements[i].limb, &center, &newCenter);
        self->headCollider.elements[i].worldCenter = newCenter;
        self->headCollider.elements[i].worldRadius =
            self->headCollider.elements[i].modelRadius * self->headCollider.elements[i].scale;
    }

    CollisionCheck_SetOC(play, &play->colChkCtx, (Collider*)&self->headCollider);
}

// N64 func_80A6A5A4
void FUN_00390760(EnHorseLinkChild* self, PlayState* play) {
    s16 yawDiff;

    if (DREG(53)) {
        DREG(53) = 0;
        Audio_PlaySoundGeneral(0x0100019B, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
        self->timer = 0;
        self->action = 4;
        self->animationIdx = 2;
        self->actor.speedXZ = 2.0f;
        self->unk_1E8 = 0;
        Animation_Change_00375c08(&self->skin.skelAnime, EnHorseLinkChild_sAnimations[self->animationIdx],
                                  FUN_00348854(&self->actor), 0.0f,
                                  Animation_GetLastFrame(&self->skin.skelAnime,
                                                         EnHorseLinkChild_sAnimations[self->animationIdx]),
                                  2, -5.0f);
    } else {
        self->actor.speedXZ = 0.0f;
        yawDiff = Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(play)->actor) - self->actor.world.rot.y;
        if ((Math_CosS(yawDiff) < 0.7071f) && (self->animationIdx == 2)) {
            EnHorse_RotateToPoint(&self->actor, &GET_PLAYER(play)->actor.world.pos, 200);
        }

        if (EnHorseLinkChild_SkelAnimeUpdate(&self->skin.skelAnime)) {
            if (Math_CosS(yawDiff) < 0.0f) {
                self->animationIdx = 2;
                Animation_Change_00375c08(&self->skin.skelAnime, EnHorseLinkChild_sAnimations[self->animationIdx],
                                          EnHorseLinkChild_sSpeedScale[self->animationIdx], 0.0f,
                                          Animation_GetLastFrame(&self->skin.skelAnime,
                                                                 EnHorseLinkChild_sAnimations[self->animationIdx]),
                                          2, -5.0f);
            } else {
                self->action = 5;
                self->animationIdx = Rand_ZeroOne() > 0.5f ? 0 : 1;
                DREG(53) = 0;
                Animation_Change_00375c08(&self->skin.skelAnime, EnHorseLinkChild_sAnimations[self->animationIdx],
                                          FUN_00348854(&self->actor), 0.0f,
                                          Animation_GetLastFrame(&self->skin.skelAnime,
                                                                 EnHorseLinkChild_sAnimations[self->animationIdx]),
                                          2, 0.0f);
            }
        }
    }
}

// N64 func_80A6993C
static inline void EnHorseLinkChild_SetupWait(EnHorseLinkChild* self, u8 newAnimationIdx) {
    self->action = 2;
    self->actor.speedXZ = 0.0f;

    if (!((newAnimationIdx == 0) || (newAnimationIdx == 1))) {
        newAnimationIdx = 0;
    }

    if (self->animationIdx != newAnimationIdx) {
        self->animationIdx = newAnimationIdx;        Animation_Change_00375c08(&self->skin.skelAnime, EnHorseLinkChild_sAnimations[self->animationIdx],
                                  FUN_00348854(&self->actor), 0.0f,
                                  Animation_GetLastFrame(&self->skin.skelAnime,
                                                         EnHorseLinkChild_sAnimations[self->animationIdx]),
                                  2, -5.0f);
    }
}

// N64 func_80A69C18
void FUN_0039c5e0(EnHorseLinkChild* self, PlayState* play) {
    s16 yawDiff;
    f32 distFromLink;
    f32 distFromHome;
    u8 newAnimationIdx;

    if ((self->animationIdx == 4) || (self->animationIdx == 3) || (self->animationIdx == 2)) {
        yawDiff = Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(play)->actor) - self->actor.world.rot.y;

        if (yawDiff > 200) {
            self->actor.world.rot.y += 200;
        } else if (yawDiff < -200) {
            self->actor.world.rot.y -= 200;
        } else {
            self->actor.world.rot.y += yawDiff;
        }

        self->actor.shape.rot.y = self->actor.world.rot.y;
    }

    if (EnHorseLinkChild_SkelAnimeUpdate(&self->skin.skelAnime)) {
        distFromLink = Actor_WorldDistXZToActor(&self->actor, &GET_PLAYER(play)->actor);

        distFromHome = Math3D_Vec3f_DistXYZ(&self->actor.world.pos, &self->actor.home.pos);

        if (distFromLink > 1000.0f || distFromHome > 600.0f) {
            EnHorseLinkChild_SetupWait(self, 0);
            return;
        } else if ((distFromLink < 1000.0f) && (distFromLink >= 300.0f)) {
            newAnimationIdx = 4;
            self->actor.speedXZ = 5.0f;
        } else if ((distFromLink < 300.0f) && (distFromLink >= 150.0f)) {
            newAnimationIdx = 3;
            self->actor.speedXZ = 4.0f;
        } else if ((distFromLink < 150.0f) && (distFromLink >= 70.0f)) {
            newAnimationIdx = 2;
            self->actor.speedXZ = 2.0f;
            self->unk_1F0 = 0;
        } else {
            EnHorseLinkChild_SetupWait(self, 1);
            return;
        }

        if (self->animationIdx != newAnimationIdx) {
            self->animationIdx = newAnimationIdx;
            Animation_Change_00375c08(&self->skin.skelAnime, EnHorseLinkChild_sAnimations[self->animationIdx],
                                      FUN_00348854(&self->actor), 0.0f,
                                      Animation_GetLastFrame(&self->skin.skelAnime,
                                                             EnHorseLinkChild_sAnimations[self->animationIdx]),
                                      2, -5.0f);
        } else {
            Animation_Change_00375c08(&self->skin.skelAnime, EnHorseLinkChild_sAnimations[self->animationIdx],
                                      FUN_00348854(&self->actor), 0.0f,
                                      Animation_GetLastFrame(&self->skin.skelAnime,
                                                             EnHorseLinkChild_sAnimations[self->animationIdx]),
                                      2, 0.0f);
        }
    }
}


// N64 func_80A69F5C
static inline void EnHorseLinkChild_TurnTowardHome(EnHorseLinkChild* self, PlayState* play) {
    Player* player;
    s16 yawDiff;
    s32 yawSign;
    s32 yawOffset;

    if ((self->animationIdx == 4) || (self->animationIdx == 3) || (self->animationIdx == 2)) {
        player = GET_PLAYER(play);

        if (Math3D_Vec3f_DistXYZ(&player->actor.world.pos, &self->actor.home.pos) < 250.0f) {
            yawDiff = player->actor.shape.rot.y;
            yawSign = Actor_WorldYawTowardActor(&self->actor, &player->actor) > 0 ? 1 : -1;
            yawOffset = yawSign << 0xE;
            yawDiff += yawOffset;
        } else {
            yawDiff = Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.home.pos) - self->actor.world.rot.y;
        }

        if (yawDiff > 200) {
            self->actor.world.rot.y += 200;
        } else if (yawDiff < -200) {
            self->actor.world.rot.y -= 200;
        } else {
            self->actor.world.rot.y += yawDiff;
        }

        self->actor.shape.rot.y = self->actor.world.rot.y;
    }
}

// N64 func_80A6A4DC
static inline void EnHorseLinkChild_SetupGreet(EnHorseLinkChild* self) {
    self->action = 5;
    self->animationIdx = Rand_ZeroOne() > 0.5f ? 0 : 1;
    DREG(53) = 0;
    Animation_Change_00375c08(&self->skin.skelAnime, EnHorseLinkChild_sAnimations[self->animationIdx],
                              FUN_00348854(&self->actor), 0.0f,
                              Animation_GetLastFrame(&self->skin.skelAnime,
                                                     EnHorseLinkChild_sAnimations[self->animationIdx]),
                              2, 0.0f);
}

// N64 func_80A6A724 (3DS stores speedXZ before unk_1E8)
static inline void EnHorseLinkChild_SetupFollow(EnHorseLinkChild* self) {
    self->timer = 0;
    self->action = 4;
    self->animationIdx = 2;
    self->actor.speedXZ = 2.0f;
    self->unk_1E8 = 0;
    Animation_Change_00375c08(&self->skin.skelAnime, EnHorseLinkChild_sAnimations[self->animationIdx],
                              FUN_00348854(&self->actor), 0.0f,
                              Animation_GetLastFrame(&self->skin.skelAnime,
                                                     EnHorseLinkChild_sAnimations[self->animationIdx]),
                              2, -5.0f);
}

// N64 func_80A6A068. N64 helpers as static inlines (block layout), idle toggles as if/else (not ternaries, for
// jump threading), distLinkFromHome via Math_Vec3f_DistXYZ directly (float regs).
void FUN_00292b50(EnHorseLinkChild* self, PlayState* play) {
    Player* player;
    f32 distFromLink;
    s32 animFinished;
    u8 newAnimationIdx;

    EnHorseLinkChild_TurnTowardHome(self, play);
    player = GET_PLAYER(play);
    distFromLink = Actor_WorldDistXZToActor(&self->actor, &player->actor);

    if (gSaveContext.entranceIndex == 0x2AE) {
        Audio_PlaySoundGeneral(0x0100019B, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
        EnHorseLinkChild_SetupFollow(self);
        return;
    }

    if (((gSaveContext.eventChkInf[1] & 0x40) && DREG(53)) ||
        ((play->sceneNum == 0x63) && (gSaveContext.cutsceneIndex == (s32)0xFFF1))) {
        EnHorseLinkChild_SetupGreet(self);
    } else {
        self->unk_2A0 = gSaveContext.eventChkInf[1] & 0x40;
    }

    newAnimationIdx = self->animationIdx;
    animFinished = EnHorseLinkChild_SkelAnimeUpdate(&self->skin.skelAnime);
    if (animFinished || (self->animationIdx == 1) || (self->animationIdx == 0)) {
        if (gSaveContext.eventChkInf[1] & 0x20) {
            f32 distFromHome = Math3D_Vec3f_DistXYZ(&self->actor.world.pos, &self->actor.home.pos);
            f32 distLinkFromHome = Math_Vec3f_DistXYZ(&player->actor.world.pos, &self->actor.home.pos);

            if (distLinkFromHome > 250.0f) {
                if (distFromHome >= 300.0f) {
                    newAnimationIdx = 4;
                    self->actor.speedXZ = 5.0f;
                } else if ((distFromHome < 300.0f) && (distFromHome >= 150.0f)) {
                    newAnimationIdx = 3;
                    self->actor.speedXZ = 4.0f;
                } else if ((distFromHome < 150.0f) && (distFromHome >= 70.0f)) {
                    newAnimationIdx = 2;
                    self->actor.speedXZ = 2.0f;
                    self->unk_1F0 = 0;
                } else {
                    self->actor.speedXZ = 0.0f;
                    if (self->animationIdx == 0) {
                        if (animFinished == 1) {
                            newAnimationIdx = 1;
                        } else {
                            newAnimationIdx = 0;
                        }
                    } else {
                        if (animFinished == 1) {
                            newAnimationIdx = 0;
                        } else {
                            newAnimationIdx = 1;
                        }
                    }
                }
            } else {
                if (distFromLink < 200.0f) {
                    newAnimationIdx = 4;
                    self->actor.speedXZ = 5.0f;
                } else if (distFromLink < 300.0f) {
                    newAnimationIdx = 3;
                    self->actor.speedXZ = 4.0f;
                } else if (distFromLink < 400.0f) {
                    newAnimationIdx = 2;
                    self->actor.speedXZ = 2.0f;
                    self->unk_1F0 = 0;
                } else {
                    self->actor.speedXZ = 0.0f;
                    if (self->animationIdx == 0) {
                        if (animFinished == 1) {
                            newAnimationIdx = 1;
                        } else {
                            newAnimationIdx = 0;
                        }
                    } else {
                        if (animFinished == 1) {
                            newAnimationIdx = 0;
                        } else {
                            newAnimationIdx = 1;
                        }
                    }
                }
            }
        } else {
            self->actor.speedXZ = 0.0f;
            if (self->animationIdx == 0) {
                if (animFinished == 1) {
                    newAnimationIdx = 1;
                } else {
                    newAnimationIdx = 0;
                }
            } else {
                if (animFinished == 1) {
                    newAnimationIdx = 0;
                } else {
                    newAnimationIdx = 1;
                }
            }
        }
    }

    if ((self->animationIdx != newAnimationIdx) || (animFinished == 1)) {
        self->animationIdx = newAnimationIdx;
        Animation_Change_00375c08(&self->skin.skelAnime, EnHorseLinkChild_sAnimations[self->animationIdx],
                                  FUN_00348854(&self->actor), 0.0f,
                                  Animation_GetLastFrame(&self->skin.skelAnime,
                                                         EnHorseLinkChild_sAnimations[self->animationIdx]),
                                  2, -5.0f);
    } else {
        Animation_Change_00375c08(&self->skin.skelAnime, EnHorseLinkChild_sAnimations[self->animationIdx],
                                  FUN_00348854(&self->actor), *EnHorse_CurFramePtr(&self->skin.skelAnime),
                                  Animation_GetLastFrame(&self->skin.skelAnime,
                                                         EnHorseLinkChild_sAnimations[self->animationIdx]),
                                  2, 0.0f);
    }
}

// N64 func_80A6A7D0 (u8 newAnimationIdx is needed for the strb/and order)
void FUN_003a0ffc(EnHorseLinkChild* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 dist;
    f32 distFromHome;
    u8 newAnimationIdx;

    self->timer++;
    if (self->timer > 300) {
        self->unk_1E8 = 1;
    }

    distFromHome = Math3D_Vec3f_DistXYZ(&self->actor.world.pos, &self->actor.home.pos);
    if (distFromHome > 600.0f) {
        self->unk_1E8 = 1;
    }

    if ((self->animationIdx == 4) || (self->animationIdx == 3) || (self->animationIdx == 2)) {
        if (!self->unk_1E8) {
            EnHorse_RotateToPoint(&self->actor, &player->actor.world.pos, 200);
        } else {
            EnHorse_RotateToPoint(&self->actor, &self->actor.home.pos, 200);
        }
    }

    if (EnHorseLinkChild_SkelAnimeUpdate(&self->skin.skelAnime)) {
        if (!self->unk_1E8) {
            dist = Actor_WorldDistXZToActor(&self->actor, &GET_PLAYER(play)->actor);
        } else {
            dist = Math3D_Vec3f_DistXYZ(&self->actor.world.pos, &self->actor.home.pos);
        }

        if (!self->unk_1E8) {
            if (dist >= 300.0f) {
                newAnimationIdx = 4;
                self->actor.speedXZ = 5.0f;
                if (distFromHome > 500.0f) {
                    self->actor.speedXZ = 150.0f;
                }
            } else if (dist >= 150.0f) {
                newAnimationIdx = 3;
                self->actor.speedXZ = 4.0f;
            } else {
                newAnimationIdx = 2;
                self->actor.speedXZ = 2.0f;
                self->unk_1F0 = 0;
            }
        } else {
            if (dist >= 300.0f) {
                newAnimationIdx = 4;
                self->actor.speedXZ = 5.0f;
            } else if (dist >= 150.0f) {
                newAnimationIdx = 3;
                self->actor.speedXZ = 4.0f;
            } else if (dist >= 70.0f) {
                newAnimationIdx = 2;
                self->actor.speedXZ = 2.0f;
                self->unk_1F0 = 0;
            } else {
                self->action = 5;
                self->animationIdx = Rand_ZeroOne() > 0.5f ? 0 : 1;
                DREG(53) = 0;
                Animation_Change_00375c08(&self->skin.skelAnime, EnHorseLinkChild_sAnimations[self->animationIdx],
                                          FUN_00348854(&self->actor), 0.0f,
                                          Animation_GetLastFrame(&self->skin.skelAnime,
                                                                 EnHorseLinkChild_sAnimations[self->animationIdx]),
                                          2, 0.0f);
                return;
            }
        }

        if (self->animationIdx != newAnimationIdx) {
            self->animationIdx = newAnimationIdx;
            Animation_Change_00375c08(&self->skin.skelAnime, EnHorseLinkChild_sAnimations[self->animationIdx],
                                      FUN_00348854(&self->actor), 0.0f,
                                      Animation_GetLastFrame(&self->skin.skelAnime,
                                                             EnHorseLinkChild_sAnimations[self->animationIdx]),
                                      2, -5.0f);
        } else {
            Animation_Change_00375c08(&self->skin.skelAnime, EnHorseLinkChild_sAnimations[self->animationIdx],
                                      FUN_00348854(&self->actor), 0.0f,
                                      Animation_GetLastFrame(&self->skin.skelAnime,
                                                             EnHorseLinkChild_sAnimations[self->animationIdx]),
                                      2, 0.0f);
        }
    }
}

void EnHorseLinkChild_Draw(EnHorseLinkChild* self, PlayState* play) {
    GraphicsContext_UnknownStub(play->state.gfxCtx);
    FUN_00341e68(&self->actor, play, &self->skin, (void*)FUN_001e1dfc, (void*)FUN_002874b0, 1);
}
}
