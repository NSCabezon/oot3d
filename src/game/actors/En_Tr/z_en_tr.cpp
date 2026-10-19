extern "C" {
#include "z_en_tr.hpp"

#define FLAGS 0x00000010

#define TR_KOUME 0
#define TR_KOTAKE 1
#define KOTAKE_KOUME_LIMB_RIGHT_HAND 10
#define R_UPDATE_RATE SREG(30)

s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s16 Camera_GetCamDirYaw(Camera* camera);
void EnTr_ChooseAction2(EnTr* self, PlayState* play);
void EnTr_ShrinkVanish(EnTr* self, PlayState* play);
void EnTr_CrySpellcast(EnTr* self, PlayState* play);
void EnTr_TakeOff(EnTr* self, PlayState* play);
void EnTr_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnTr_Reappear(EnTr* self, PlayState* play);
void SkelAnime_SetCurrentFrame2(SkelAnime* skelAnime, f32 frame);
s32 FUN_003731e0(SkelAnime* skelAnime);
s16 Rand_S16Offset_003702c8(s16 base, s16 range);
void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);
void EnTr_DoNothing(EnTr* self, PlayState* play);
void EnTr_ChooseAction1(EnTr* self, PlayState* play);
void EnTr_TurnLookOverShoulder(EnTr* self, PlayState* play);
static void EnTr_SetStartPosRotFromCue(EnTr* self, PlayState* play, s32 cueChannel);
static void EnTr_SetStartPosRotFromCue2(EnTr* self, PlayState* play, s32 cueChannel);

void EnTr_Init(Actor* thisx, PlayState* play);
void EnTr_Destroy(Actor* actor, GameState* state);
void EnTr_Update(Actor* thisx, PlayState* play);
void EnTr_Draw(Actor* actor, PlayState* state);

GLOBAL_ASM("data/z_En_Tr.data.s")
GLOBAL_ASM("../src/game/actors/En_Tr/data_syms.s")

extern s32 EnTr_sTables[];

ActorInit En_Tr_InitVars = {
    ACTOR_EN_TR,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_TR,
    sizeof(EnTr),
    (ActorFunc)EnTr_Init,
    (ActorFunc)EnTr_Destroy,
    (ActorFunc)EnTr_Update,
    (ActorFunc)EnTr_Draw,
};

void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skelAnime, void* cmb, void* p, s32 a, void* b, void* c, s32 d);
void Animation_PlayOnce(SkelAnime* skelAnime, s32 idx);
void SkelAnime_SetFaceAnimations(FaceAnimations* param_1, SkelAnime* skelAnime, s32 param_3, s32 param_4, s32 param_5);

static inline u8* EnTr_GetObjStatus(PlayState* play, s32 idx) {
    if ((u32)idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

static inline void EnTr_SetupAction(EnTr* self, EnTrActionFunc fn) {
    self->actionFunc = fn;
}

void EnTr_Init(Actor* thisx, PlayState* play) {
    EnTr* self = (EnTr*)thisx;
    void* zar;
    void* cmb;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    EnTr_SetupAction(self, EnTr_DoNothing);
    self->unk_E84 = 0;
    self->actor.child = NULL;
    Actor_SetScale(&self->actor, 0.01f);

    zar = (void*)(EnTr_GetObjStatus(play, (u8)self->actor.objBankIndex) + 0x10);
    switch (self->actor.params) {
        case TR_KOUME:
            cmb = ZAR_GetCMBByIndex(zar, 0);
            FUN_00353e78(zar, play, &self->skelAnime, cmb, self->actor.unk_178, 6, self->unk_228, self->unk_228 + 0x548, 0x1A);
            SkelAnime_SetFaceAnimations((FaceAnimations*)self->unk_CB8, &self->skelAnime, 1, -1, -1);
            Animation_PlayOnce(&self->skelAnime, 6);
            EnTr_SetupAction(self, EnTr_ChooseAction1);
            self->animIndex = -1;
            self->cueChannel = 3;
            break;

        case TR_KOTAKE:
            cmb = ZAR_GetCMBByIndex(zar, 1);
            FUN_00353e78(zar, play, &self->skelAnime, cmb, self->actor.unk_178, 0, self->unk_228, self->unk_228 + 0x548, 0x1A);
            SkelAnime_SetFaceAnimations((FaceAnimations*)self->unk_CB8, &self->skelAnime, 0, -1, -1);
            Animation_PlayOnce(&self->skelAnime, 0);
            EnTr_SetupAction(self, EnTr_ChooseAction1);
            self->animIndex = -1;
            self->cueChannel = 2;
            break;
    }
}

void EnTr_Destroy(Actor* thisx, GameState* state) {
    EnTr* self = (EnTr*)thisx;

    SkelAnime_Free2(&self->skelAnime);
    ((FaceAnimations*)self->unk_CB8)->Destroy();
}

#if 0 // draft, 2 words off: ROM `mvn r1,#0` for the animIndex = -1 store, ours r0 (~40 variants incl. idx locals/aliases)
void EnTr_Update(Actor* thisx, PlayState* play) {
    EnTr* self = (EnTr*)thisx;

    Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 5);
    self->actionFunc(self, play);

    if (FUN_003731e0(&self->skelAnime)) {
        s32 idx;
        if (self->animIndex != -1) {
            if ((self->animIndex == 7) || (self->animIndex == 1)) {
                if (self->actor.params != TR_KOUME) {
                    Audio_PlayActorSound2(&self->actor, 0x01000421);
                } else {
                    Audio_PlayActorSound2(&self->actor, 0x01000420);
                }
                idx = self->animIndex;
                goto loop;
            }
            if (self->animIndex == 4) {
                self->actionFunc = EnTr_ChooseAction2;
                f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 4);
                Animation_Change_00375c08(&self->skelAnime, 4, 1.0f, 0.0f, lastFrame, 0, -5.0f);
            } else {
                idx = self->animIndex;
            loop:
                Animation_PlayLoop(&self->skelAnime, idx);
            }
            self->animIndex = -1;
        } else {
            SkelAnime_SetCurrentFrame2(&self->skelAnime, 0.0f);
        }
    }
    Actor_SetFocus(&self->actor, 0.0f);

    if (DECR(self->blinkTimer) == 0) {
        self->blinkTimer = Rand_S16Offset_003702c8(60, 60);
    }
    self->eyeIndex = self->blinkTimer;
    if (self->eyeIndex >= 3) {
        self->eyeIndex = 0;
    }
}
#endif

GLOBAL_ASM("asm/EnTr_Update.s")

#if 0 // draft, 7 words off: ROM keeps the +0xE00 base in r2 (ours r1) and emits `mov r0,r5; mov r1,#0` in that order
void EnTr_Draw(Actor* thisx, PlayState* play) {
    EnTr* self = (EnTr*)thisx;
    EnTr* s2 = self;

    if ((play->GetCutsceneState() == CS_STATE_IDLE) || (play->csCtx.npcActions[self->cueChannel] == NULL)) {
        s2->actor.shape.shadowDraw = NULL;
    } else {
        FaceAnimations* face = (FaceAnimations*)self->unk_CB8;

        self->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        face->FUN_0035e3a4(0, self->eyeIndex);
        face->FUN_0035e330();
        SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, NULL, EnTr_OverrideLimbDraw, &self->actor, 0);
    }
}
#endif

GLOBAL_ASM("asm/EnTr_Draw.s")

void EnTr_FlyKidnapCutscene(EnTr* self, PlayState* play);
void EnTr_WaitToReappear(EnTr* self, PlayState* play);
void FUN_00353310(EnTr* self, PlayState* play, s32 cueChannel);

#if 0 // draft, 15 words off: play/self+e00 register swap (ROM play=r7, e00=r6, -1 const in r7) + case-3 cue-copy VFP numbering (x=s1,y=s2,z=s0; EnTr_SetStartPosRotFromCue2 gives x=s1,y=s0,z=s2)
void EnTr_ChooseAction1(EnTr* self, PlayState* play) {
    u32 frames = *(u32*)((u8*)play + 0x5BF4); // PlayState::gameplayFrames (not in repo header)

    if (play->GetCutsceneState() != CS_STATE_IDLE) {
        if (play->csCtx.npcActions[self->cueChannel] != NULL) {
            switch (play->csCtx.npcActions[self->cueChannel]->action) {
                case 1:
                    EnTr_SetStartPosRotFromCue(self, play, self->cueChannel);
                    EnTr_SetupAction(self, EnTr_TurnLookOverShoulder);
                    break;

                case 3:
                    EnTr_SetStartPosRotFromCue(self, play, self->cueChannel);
                    EnTr_SetupAction(self, EnTr_ChooseAction2);
                    Animation_PlayLoop(&self->skelAnime, 4);
                    self->animIndex = -1;
                    break;

                case 4:
                    EnTr_SetupAction(self, EnTr_WaitToReappear);
                    self->actor.draw = NULL;
                    break;

                case 7:
                    EnTr_SetupAction(self, EnTr_FlyKidnapCutscene);
                    Animation_PlayLoop(&self->skelAnime, 4);
                    self->animIndex = -1;
                    self->timer = (self->actor.params != TR_KOUME)
                                      ? (s32)(((frames & 0x3F) * 0x400) * 3.0f / R_UPDATE_RATE + 0.5f) + 0x8000
                                      : (s32)(((frames & 0x3F) * 0x400) * 3.0f / R_UPDATE_RATE + 0.5f);
                    break;
            }
        }
    }
}

#endif

GLOBAL_ASM("asm/EnTr_ChooseAction1.s")

void EnTr_ChooseAction2(EnTr* self, PlayState* play) {
    if (play->GetCutsceneState() != CS_STATE_IDLE) {
        if (play->csCtx.npcActions[self->cueChannel] != NULL) {
            switch (play->csCtx.npcActions[self->cueChannel]->action) {
                case 4:
                    Actor_SetScale(&self->actor, 0.01f);
                    EnTr_SetupAction(self, EnTr_ShrinkVanish);
                    self->timer = (s32)(24 * 3.0f / R_UPDATE_RATE + 0.5f);
                    Audio_PlayActorSound2(&self->actor, 0x010002E7);
                    break;

                case 6:
                    Animation_Change_00375c08(&self->skelAnime, EnTr_sTables[8 + self->actor.params], 1.0f, 0.0f,
                                              Animation_GetLastFrame(&self->skelAnime, EnTr_sTables[8 + self->actor.params]),
                                              2, -5.0f);
                    EnTr_SetupAction(self, EnTr_CrySpellcast);
                    self->animIndex = EnTr_sTables[6 + self->actor.params];
                    self->timer = (s32)(39 * 3.0f / R_UPDATE_RATE + 0.5f);
                    Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, 0xF5, self->actor.world.pos.x,
                                       self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0,
                                       self->actor.params + 9);
                    Audio_PlayActorSound2(&self->actor, 0x01000315);
                    break;

                default: {
                    s32 rotDiff;
                    s32 rotSign;

                    FUN_00353310(self, play, self->cueChannel);
                    rotDiff = self->actor.world.rot.y - play->csCtx.npcActions[self->cueChannel]->rot.y;
                    if (rotDiff < 0) {
                        rotDiff = -rotDiff;
                        rotSign = 1;
                    } else {
                        rotSign = -1;
                    }
                    if (rotDiff >= 0x8000) {
                        rotSign = -rotSign;
                        rotDiff = 0x10000 - rotDiff;
                    }
                    rotDiff *= 0.1f;
                    self->actor.world.rot.y += rotDiff * rotSign;
                    self->actor.shape.rot.y = self->actor.world.rot.y;
                    break;
                }
            }
            FUN_00373264(&self->actor, 0x010003EB);
        }
    }
}

void EnTr_CrySpellcast(EnTr* self, PlayState* play) {
    if (self->timer == (s32)(11 * 3.0f / R_UPDATE_RATE + 0.5f)) {
        // Both cry in the title screen cutscene, but only Kotake in the in-game cutscene
        if ((self->actor.params != TR_KOUME) || (gSaveContext.sceneSetupIndex == 6)) {
            Audio_PlaySoundGeneral(0x01000423, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        }
    }

    if (self->timer != 0) {
        self->timer--;
    } else if (self->actor.child != NULL) {
        self->actor.child = NULL;
    }
    FUN_00373264(&self->actor, 0x010003EB);
}

GLOBAL_ASM("../split/asm/FUN_0010E624.s")

void EnTr_DoNothing(EnTr* self, PlayState* play) {
}

void EnTr_FlyKidnapCutscene(EnTr* self, PlayState* play) {
    Vec3f originalPos = self->actor.world.pos;

    if (play->GetCutsceneState() != CS_STATE_IDLE) {
        if (play->csCtx.npcActions[self->cueChannel] != NULL) {
            if (play->csCtx.npcActions[self->cueChannel]->action == 8) {
                FUN_00353310(self, play, self->cueChannel);
                self->actor.world.rot.y = Math_Atan2S(self->actor.velocity.z, self->actor.velocity.x);
                Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.world.rot.y, 10, 0x400, 0x100);
                self->actor.world.rot.y = self->actor.shape.rot.y;
            } else {
                EnTr_SetStartPosRotFromCue(self, play, self->cueChannel);
                self->actor.world.pos.x += Math_SinS(self->timer) * 150.0f;
                self->actor.world.pos.y += -100.0f;
                self->actor.world.pos.z += Math_CosS(self->timer) * 150.0f;

                self->actor.shape.rot.y = (s16)(self->timer) + 0x4000;
                self->timer += 0x2AB;

                self->actor.velocity.x = self->actor.world.pos.x - originalPos.x;
                self->actor.velocity.y = self->actor.world.pos.y - originalPos.y;
                self->actor.velocity.z = self->actor.world.pos.z - originalPos.z;
            }

            // PlayState::csCtx.curFrame is u16 at 0x22B8 (not in repo header)
            if ((s32)(670 * 3.0f / R_UPDATE_RATE + 0.5f) > *(u16*)((u8*)play + 0x22B8)) {
                FUN_00373264(&self->actor, 0x010003EB);
            }
        }
    }
}

static inline void EnTr_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void EnTr_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    Vector3f src(2300.0f, 0.0f, -600.0f);
    Vector3f dest(0.0f, 0.0f, 0.0f);
    EnTr* self = (EnTr*)thisx;
    Actor* child = self->actor.child;

    if ((child != NULL) && (limbIndex == KOTAKE_KOUME_LIMB_RIGHT_HAND)) {
        EnTr_MultVec3f(&src, &dest, mtx);
        dest.x -= (10.0f * Math_SinS(Camera_GetCamDirYaw(play->cameraPtrs[play->activeCamera])));
        dest.z -= (10.0f * Math_CosS(Camera_GetCamDirYaw(play->cameraPtrs[play->activeCamera])));
        child->world.pos = *(Vec3f*)&dest;
    }
}

#define EnTr_Frames(x) ((x) > 0 ? (s32)((x) * 2.0f / 3.0f + 0.5f) : (s32)((x) * 2.0f / 3.0f - 0.5f))

#if 0 // draft, ~80 words off: VFP numbering / literal fold of the rot.y step (ROM keeps *2.0, *1/3, *281.875, *1.01 as separate vmuls)
void EnTr_Reappear(EnTr* self, PlayState* play) {
    if (self->timer > (s32)(30 * 3.0f / R_UPDATE_RATE + 0.5f)) {
        s32 temp_hi = (EnTr_Frames(self->timer) * 2) % 7;

        FUN_00327b50(self, play, temp_hi, 5.0f, 1.0f);
        FUN_00327b50(self, play, (temp_hi + 1) % 7, 5.0f, 1.0f);
    } else if (self->timer == (s32)(30 * 3.0f / R_UPDATE_RATE + 0.5f)) {
        self->actor.draw = (ActorFunc)EnTr_Draw;
        self->actor.shape.rot.y = self->actor.shape.rot.y + self->timer * 2.0f * (1.0f / 3.0f) * 281.8753f * 1.01f;
    } else if (self->timer > 0) {
        self->actor.shape.rot.y = self->actor.shape.rot.y + self->timer * 2.0f * (1.0f / 3.0f) * 281.8753f * 1.01f;
        Actor_SetScale(&self->actor, (self->actor.scale.x * 0.8f) + 0.002f);
    } else {
        EnTr_SetupAction(self, EnTr_ChooseAction2);
        Actor_SetScale(&self->actor, 0.01f);
    }

    if (self->timer > 0) {
        self->timer--;
    }
    FUN_00373264(&self->actor, 0x010003EB);
}
#endif

GLOBAL_ASM("asm/EnTr_Reappear.s")

#if 0 // draft, ~65-79 words off: control flow and shared rot tail match; VFP numbering of hoisted consts (ROM 2.0->s21, 1/3->s19, 0.9->s18, 0.2->s22, 5.0->s23)
void EnTr_ShrinkVanish(EnTr* self, PlayState* play) {
    s32 temp_hi;

    if ((s32)(16 * 3.0f / R_UPDATE_RATE + 0.5f) < self->timer) {
        self->actor.shape.rot.y += (24 - EnTr_Frames(self->timer)) * 0x28F;
    } else {
        if ((s32)(4 * 3.0f / R_UPDATE_RATE + 0.5f) < self->timer) {
            Actor_SetScale(&self->actor, self->actor.scale.x * 0.9f);
            self->actor.shape.rot.y += (24 - EnTr_Frames(self->timer)) * 0x28F;
        } else if (self->timer > 0) {
            temp_hi = (EnTr_Frames(self->timer) * 2) % 7;

            FUN_00327b50(self, play, temp_hi, 5.0f, 0.2f);
            FUN_00327b50(self, play, (temp_hi + 1) % 7, 5.0f, 0.2f);
            Actor_SetScale(&self->actor, self->actor.scale.x * 0.9f);
            self->actor.shape.rot.y += (24 - EnTr_Frames(self->timer)) * 0x28F;
        } else {
            EnTr_SetupAction(self, EnTr_WaitToReappear);
            self->actor.draw = NULL;
        }
    }

    if (self->timer == (s32)(4 * 3.0f / R_UPDATE_RATE + 0.5f)) {
        Audio_PlayActorSound2(&self->actor, 0x0100033D);
    }

    if (self->timer > 0) {
        self->timer--;
    }
}
#endif

GLOBAL_ASM("asm/EnTr_ShrinkVanish.s")

void EnTr_TakeOff(EnTr* self, PlayState* play) {
    f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, EnTr_sTables[6 + self->actor.params]);

    if (play->GetCutsceneState() != CS_STATE_IDLE) {
        if ((play->csCtx.npcActions[self->cueChannel] != NULL) &&
            (play->csCtx.npcActions[self->cueChannel]->action == 3)) {
            Animation_Change_00375c08(&self->skelAnime, EnTr_sTables[6 + self->actor.params], 1.0f, 0.0f, lastFrame,
                                      0, -10.0f);
            self->animIndex = -1;
            self->actionFunc = EnTr_ChooseAction2;
        }
    }
}

void EnTr_TurnLookOverShoulder(EnTr* self, PlayState* play) {
    f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, EnTr_sTables[2 + self->actor.params]);

    if (play->GetCutsceneState() != CS_STATE_IDLE) {
        if ((play->csCtx.npcActions[self->cueChannel] != NULL) &&
            (play->csCtx.npcActions[self->cueChannel]->action == 2)) {
            Animation_Change_00375c08(&self->skelAnime, EnTr_sTables[2 + self->actor.params], 1.0f, 0.0f, lastFrame, 2,
                                      -4.0f);
            self->animIndex = EnTr_sTables[4 + self->actor.params];
            self->actionFunc = EnTr_TakeOff;
        }
    }
}

static void EnTr_SetStartPosRotFromCue(EnTr* self, PlayState* play, s32 cueChannel) {
    Vec3f startPos;

    startPos.x = play->csCtx.npcActions[cueChannel]->startPos.x;
    startPos.y = play->csCtx.npcActions[cueChannel]->startPos.y;
    startPos.z = play->csCtx.npcActions[cueChannel]->startPos.z;

    self->actor.world.pos = startPos;
    self->actor.world.rot.y = self->actor.shape.rot.y = play->csCtx.npcActions[cueChannel]->rot.y;
}

static void EnTr_SetStartPosRotFromCue2(EnTr* self, PlayState* play, s32 cueChannel) {
    Vec3f startPos;
    f32 x;
    f32 y;
    f32 z;

    x = play->csCtx.npcActions[cueChannel]->startPos.x;
    y = play->csCtx.npcActions[cueChannel]->startPos.y;
    z = play->csCtx.npcActions[cueChannel]->startPos.z;
    startPos.x = x;
    startPos.y = y;
    startPos.z = z;

    self->actor.world.pos = startPos;
    self->actor.world.rot.y = self->actor.shape.rot.y = play->csCtx.npcActions[cueChannel]->rot.y;
}

void EnTr_WaitToReappear(EnTr* self, PlayState* play) {
    if (play->GetCutsceneState() != CS_STATE_IDLE) {
        if ((play->csCtx.npcActions[self->cueChannel] != NULL) &&
            ((play->csCtx.npcActions[self->cueChannel]->action == 3) ||
             (play->csCtx.npcActions[self->cueChannel]->action == 5))) {
            Audio_PlayActorSound2(&self->actor, 0x0100037D);
            self->timer = (s32)(34 * 3.0f / R_UPDATE_RATE + 0.5f);
            EnTr_SetStartPosRotFromCue2(self, play, self->cueChannel);
            self->actionFunc = EnTr_Reappear;
            Animation_PlayLoop(&self->skelAnime, 4);
            self->animIndex = -1;
            Actor_SetScale(&self->actor, 0.003f);
        }
    }
}

#if 0 // draft FUN_00327b50 (N64 func_80B23254): 59 words off, r7/r8 (idx/yaw) swapped + pitch load scheduling; needs EnTr_sOffsets EQU 0x0052FB30 / EnTr_sColors EQU 0x0052FB68 in data_syms.s
extern f32 EnTr_sOffsets[];
extern Color_RGBA8 EnTr_sColors[];
void FUN_00366150(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, Color_RGBA8* prim, Color_RGBA8* env, s16 a, s16 b);
static inline s16 EnTr_Vec3f_Pitch(Vec3f* a, Vec3f* b) {
    return Math_Atan2S(Math_Vec3f_DistXZ(a, b), a->y - b->y);
}

void FUN_00327b50(EnTr* self, PlayState* play, s32 idx, f32 arg3, f32 scale) {
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;
    Vec3f sp58;
    Color_RGBA8* primColor;
    Color_RGBA8* envColor;
    Vec3f cameraEye = play->cameraPtrs[play->activeCamera]->eye;
    s16 yaw = Math_Vec3f_Yaw(&cameraEye, &self->actor.world.pos);
    s16 reversePitch = -Math_Atan2S(Math_Vec3f_DistXZ(&cameraEye, &self->actor.world.pos),
                                    cameraEye.y - self->actor.world.pos.y);
    f32 sp3C;

    accel.x = accel.z = 0.0f;
    sp3C = Math_SinS(yaw) * arg3;
    velocity.x = Math_CosS(reversePitch) * sp3C;
    velocity.y = Math_SinS(reversePitch) * arg3;
    sp3C = Math_CosS(yaw) * arg3;
    velocity.z = Math_CosS(reversePitch) * sp3C;
    accel.y = 0.5f;

    primColor = &EnTr_sColors[2 * self->actor.params];
    envColor = &EnTr_sColors[2 * self->actor.params + 1];

    sp58 = self->actor.world.pos;
    sp58.x -= velocity.x * 10.0f;
    sp58.y -= velocity.y * 10.0f;
    sp58.z -= velocity.z * 10.0f;

    pos.x = sp58.x + (EnTr_sOffsets[idx] * (Math_CosS(yaw) * scale));
    pos.y = sp58.y + (EnTr_sOffsets[7 + idx] * scale);
    pos.z = sp58.z - (EnTr_sOffsets[idx] * (Math_SinS(yaw) * scale));
    FUN_00366150(play, &pos, &velocity, &accel, primColor, envColor, (s32)(800.0f * scale), (s32)(80.0f * scale));
}
#endif

GLOBAL_ASM("asm/FUN_00327b50.s")

f32 FUN_00361490(u16 endFrame, u16 startFrame, u16 curFrame); // Environment_LerpWeight

static inline f32 EnTr_CueLerpWeight(PlayState* play, s32 cueChannel) {
    f32 phi = FUN_00361490(play->csCtx.npcActions[cueChannel]->endFrame,
                           play->csCtx.npcActions[cueChannel]->startFrame, *(u16*)((u8*)play + 0x22B8));
    phi = CLAMP_MAX(phi, 1.0f);
    return phi;
}

void FUN_00353310(EnTr* self, PlayState* play, s32 cueChannel) {
    f32 scale;
    Vec3f startPos;
    Vec3f endPos;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 phi_f12;

    scale = 0.1f;
    startPos.x = play->csCtx.npcActions[cueChannel]->startPos.x;
    startPos.y = play->csCtx.npcActions[cueChannel]->startPos.y;
    startPos.z = play->csCtx.npcActions[cueChannel]->startPos.z;

    endPos.x = play->csCtx.npcActions[cueChannel]->endPos.x;
    endPos.y = play->csCtx.npcActions[cueChannel]->endPos.y;
    endPos.z = play->csCtx.npcActions[cueChannel]->endPos.z;

    temp_f0 = EnTr_CueLerpWeight(play, cueChannel);

    startPos.x = ((endPos.x - startPos.x) * temp_f0) + startPos.x;
    startPos.y = ((endPos.y - startPos.y) * temp_f0) + startPos.y;
    startPos.z = ((endPos.z - startPos.z) * temp_f0) + startPos.z;

    endPos.x = (startPos.x - self->actor.world.pos.x) * scale;
    endPos.y = (startPos.y - self->actor.world.pos.y) * scale;
    endPos.z = (startPos.z - self->actor.world.pos.z) * scale;

    temp_f0_2 = sqrtf(SQ(endPos.x) + SQ(endPos.y) + SQ(endPos.z));
    phi_f12 = CLAMP(temp_f0_2, 0.0f, 20.0f);

    if ((temp_f0_2 != phi_f12) && (temp_f0_2 != 0.0f)) {
        f32 ratio = phi_f12 / temp_f0_2;
        endPos.x *= ratio;
        endPos.y *= ratio;
        endPos.z *= ratio;
    }

    Math_StepToF(&self->actor.velocity.x, endPos.x, 1.0f);
    Math_StepToF(&self->actor.velocity.y, endPos.y, 1.0f);
    Math_StepToF(&self->actor.velocity.z, endPos.z, 1.0f);
    FUN_0036b96c(&self->actor);
}

}
