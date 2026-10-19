extern "C" {
#include "z_en_in.hpp"

#define FLAGS 0x00000019

void EnIn_Init(Actor* thisx, PlayState* play);
void EnIn_Destroy(Actor* thisx, PlayState* play);
void EnIn_Update(Actor* thisx, PlayState* play);
void EnIn_Draw(Actor* thisx, PlayState* play);

s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
void FUN_003b8684(EnIn* self, PlayState* play);
void FUN_0037266c(SkeletonAnimationModel* model, s32 arg);
void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);
s32 EnIn_OverrideLimbDraw(void* unused, s32 limbIndex, PlayState* play, Actor* thisx);
void EnIn_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, Actor* thisx);

#define EVENTINF_INDEX_HORSES 0
#define PLAYER_STATE1_23 (1 << 23)

GLOBAL_ASM("data/z_En_In.data.s")

GLOBAL_ASM("../src/game/actors/En_In/data_syms.s")
extern s32 EnIn_sHorsebackAnimations[8];
extern void* gUnk_00549600;
extern ColliderCylinderInit EnIn_sCylinderInit;
extern CollisionCheckInfoInit2 EnIn_sColChkInfoInit;

ActorInit En_In_InitVars = {
    ACTOR_EN_IN,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_IN,
    sizeof(EnIn),
    (ActorFunc)EnIn_Init,
    (ActorFunc)EnIn_Destroy,
    (ActorFunc)EnIn_Update,
    (ActorFunc)EnIn_Draw,
};

// near-miss draft (27 words off, zero kept in r6 + extra frame slot); see notes
void EnIn_Init(Actor* thisx, PlayState* play) {
    EnIn* self = (EnIn*)thisx;
    Vec3f respawnPos;

    self->requiredObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_IN);
    if (self->requiredObjectSlot < 0 && thisx->params >= 1) {
        self->actionFunc = NULL;
        Actor_Kill(&self->actor);
        return;
    }
    respawnPos = *(Vec3f*)&gSaveContext.respawn[RESPAWN_MODE_DOWN].pos;
    if (*(s32*)((u8*)play + 0x7F90) == 0 && respawnPos.x == 1107.0f && respawnPos.z == -3740.0f) {
        gSaveContext.eventInf[EVENTINF_INDEX_HORSES] = 0;
        *(s32*)((u8*)play + 0x7F90) = 1;
    }
    self->actionFunc = FUN_003b8684;
}


void EnIn_Destroy(Actor* thisx, PlayState* play) {
    EnIn* self = (EnIn*)thisx;

    if (self->actionFunc != NULL && self->actionFunc != FUN_003b8684) {
        Collider_DestroyCylinder(play, (ColliderCylinder*)((u8*)self + 0x400));
        SkelAnime_Free2((SkelAnime*)((u8*)self + 0x1a4));
        ((FaceAnimations*)((u8*)self + 0x228))->Destroy();
    }
}

void FUN_003c54f0(EnIn* self, PlayState* play);
void FUN_00105440(EnIn* self, PlayState* play);
void FUN_003fd1b8(PlayState* play, Actor* actor, SkelAnime* skelAnime, f32 arg);
s32 FUN_0036bc98(Actor* actor, PlayState* play);
void FUN_0034c664(Actor* actor, EnInInteractInfo* info, s32 arg2, s32 mode); // Npc_TrackPoint
void FUN_00342714(PlayState* play, Actor* actor, void* talkState, f32 range, void* getTextId, void* updateTalkState);
s16 FUN_00171370(PlayState* play, Actor* thisx);
u16 FUN_00171660(PlayState* play, Actor* thisx);
void FUN_003b8bd4(EnIn* self, PlayState* play);

#if 0
// FAR draft (22 words off): Stagger jointTable reg (r0 vs r1) + Fidget loop register numbering (f/i/ptr rotated: ROM f=r0,
// i=r1,ptr=ip). R5: early-return `if (jt <= base) return;` fixed the bls; Fidget decl/f-init/pointer-loop perms no change.
static inline void EnIn_UpdateTracking(EnIn* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 trackingMode;

    if (self->skelAnime.animationType == 0 || self->skelAnime.animationType == 2 ||
        self->skelAnime.animationType == 5) {
        trackingMode = 1;
    } else {
        trackingMode = 0;
    }
    if (self->actionFunc == FUN_003c54f0) {
        trackingMode = 4;
    }
    if (self->actionFunc == FUN_00105440) {
        self->interactInfo.trackPos = *(Vec3f*)((u8*)play + 0x1b8);
        self->interactInfo.yOffset = 60.0f;
    } else {
        self->interactInfo.trackPos = player->actor.world.pos;
        self->interactInfo.yOffset = 16.0f;
    }
    FUN_0034c664(&self->actor, &self->interactInfo, 1, trackingMode);
}

static inline void EnIn_Stagger(SkelAnime* skelAnime, EnIn* self, PlayState* play) {
    f32 jt, base;
    s32* p = &self->skelAnime.animationType;
    base = *(f32*)((u8*)skelAnime + 0x68);
    jt = *(f32*)((u8*)skelAnime->jointTable + 0x1c);
    if (jt <= base) {
        return;
    }
    {
        u8 flags = *((u8*)skelAnime + 0x52);
        *((u8*)p + 0x22) = flags | 3;
        FUN_003fd1b8(play, &self->actor, skelAnime, 1.0f);
    }
}

static inline void EnIn_UpdateEyes(EnIn* self) {
    if (self->eyeIndex != 3) {
        if (DECR(self->blinkTimer) == 0) {
            self->eyeIndex++;
            if (self->eyeIndex >= 3) {
                self->blinkTimer = Rand_S16Offset_003702c8(30, 30);
                self->eyeIndex = 0;
            }
        }
    }
}

static inline void EnIn_Fidget(EnIn* self, PlayState* play) {
    s32 i;
    u32 f = 0;
    PlayState* play2 = play;

    if (self->skelAnime.animationType != 7) {
        f = *(u32*)((u8*)play2 + 0x5bf4);
    }
    for (i = 0; i < 22; i++) {
        self->fidgetVec[i].y = (0x814 + 50 * i) * f;
        self->fidgetVec[i].z = (0x940 + 50 * i) * f;
    }
}

void EnIn_Update(Actor* thisx, PlayState* play) {
    EnIn* self = (EnIn*)thisx;

    if (self->actionFunc == FUN_003b8684) {
        self->actionFunc(self, play);
        return;
    }
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, (Collider*)&self->collider);
    if (self->actionFunc != FUN_003b8bd4) {
        FUN_003731e0(&self->skelAnime);
        if (self->skelAnime.animationType == 2 && (gSaveContext.eventInf[0] & 0xF) != 6) {
            EnIn_Stagger(&self->skelAnime, self, play);
        }
        Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    }
    EnIn_UpdateEyes(self);
    self->actionFunc(self, play);
    if (self->actionFunc != FUN_003b8bd4) {
        EnIn_Fidget(self, play);
        if ((gSaveContext.timer2Value <= 5) && (gSaveContext.timer2State != 0) && self->interactInfo.talkState == 0) {
            if (FUN_0036bc98(&self->actor, play)) {}
        } else {
            FUN_00342714(play, &self->actor, &self->interactInfo.talkState,
                         ((self->actor.targetMode == 6) ? 80.0f : 320.0f) + self->collider.dim.radius,
                         (void*)FUN_00171660, (void*)FUN_00171370);
            if (self->interactInfo.talkState != 0) {
                self->prevTextState = self->textState;
                self->textState = Message_GetState(&play->msgCtx);
            }
        }
        EnIn_UpdateTracking(self, play);
    }
}
#endif

GLOBAL_ASM("asm/EnIn_Update.s")

void EnIn_Draw(Actor* thisx, PlayState* play) {
    EnIn* self = (EnIn*)thisx;

    if (self->actionFunc != FUN_003b8684) {
        if (self->actor.params >= 1) {
            FUN_0037266c(*(SkeletonAnimationModel**)((u8*)self + 0x1cc), 1);
            FUN_0036932c(*(SkeletonAnimationModel**)((u8*)self + 0x1cc), 0);
        } else {
            FUN_0037266c(*(SkeletonAnimationModel**)((u8*)self + 0x1cc), 0);
            FUN_0036932c(*(SkeletonAnimationModel**)((u8*)self + 0x1cc), 1);
        }
        FaceAnimations* face = (FaceAnimations*)((u8*)self + 0x228);
        face->FUN_0035e3a4(0, *(s16*)((u8*)self + 0x466));
        FaceAnimations* f2 = (FaceAnimations*)((u8*)self + 0x228);
        f2->FUN_0035e3a4(2, 0);
        ((FaceAnimations*)((u8*)self + 0x228))->FUN_0035e330();
        SkelAnime_DrawOpa((SkelAnime*)((u8*)self + 0x1a4), &self->actor.modelMtx, (OverrideLimbDrawOpa)EnIn_OverrideLimbDraw,
                          (PostLimbDrawOpa)EnIn_PostLimbDraw, thisx, 0);
    }
}


void FUN_0034e01c(PlayState* play, void* p);
void FUN_0037266c(SkeletonAnimationModel* model, s32 arg);
void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);

void FUN_003b8e48(Actor* thisx, PlayState* play) {}

u16 FUN_00171660(PlayState* play, Actor* thisx) {
    u16 textId = Text_GetFaceReaction(play, 0x19);

    if (textId != 0) {
        return textId;
    }
    if (!LINK_IS_ADULT) {
        if (gSaveContext.eventChkInf[1] & 0x10) {
            if (gSaveContext.infTable[9] & 0x80) {
                return 0x2046;
            } else {
                return 0x2045;
            }
        }
        if (gSaveContext.infTable[9] & 0x10) {
            return 0x2040;
        } else {
            return 0x203F;
        }
    } else {
        Player* player = GET_PLAYER(play);

        textId = Text_GetFaceReaction(play, 0x19);
        if (textId != 0) {
            return textId;
        }
        if (gSaveContext.eventChkInf[1] & 0x100) {
            if (IS_DAY) {
                return 0x205F;
            } else {
                return 0x2057;
            }
        }
        if (IS_NIGHT) {
            return 0x204E;
        }
        switch (gSaveContext.eventInf[0] & 0xF) {
            case 1:
                if (!(player->stateFlags1 & PLAYER_STATE1_23)) {
                    return 0x2036;
                } else if (gSaveContext.eventChkInf[1] & 0x800) {
                    if (gSaveContext.infTable[10] & 4) {
                        return 0x2036;
                    } else {
                        return 0x2038;
                    }
                } else {
                    return 0x2037;
                }
            case 3:
                if ((gSaveContext.eventInf[0] & (1 << 6)) || (gSaveContext.eventInf[0] & (1 << 5))) {
                    return 0x203E;
                } else {
                    return 0x203D;
                }
            case 4:
                return 0x203A;
            case 5:
            case 6:
                return 0x203C;
            case 7:
                return 0x205B;
            case 2:
            default:
                if (gSaveContext.infTable[9] & 0x400) {
                    return 0x2031;
                } else {
                    return 0x2030;
                }
        }
    }
}


void FUN_003715d0(void* actor, PlayState* play, s32 arg2, s32 arg3);
void FUN_00371680(PlayState* play, s32 arg, s32 arg2);

void FUN_00105440(EnIn* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Actor* held = player->rideActor;

    if (held != NULL) {
        held->freezeTimer = 10;
    }
    player->actor.freezeTimer = 10;

    if (*(u16*)((u8*)self + 0xd88) == 2) {
        if (!(gSaveContext.eventChkInf[1] & 0x800)) {
            if (gSaveContext.infTable[10] & 0x800) {
                u16 a = gSaveContext.eventChkInf[1] | 0x800;
                gSaveContext.eventChkInf[1] = a;
                u16 b = gSaveContext.infTable[10] | 0x800;
                gSaveContext.infTable[10] = b;
            }
        }
        FUN_003715d0(self, play, 0, 0x26);
        gSaveContext.eventInf[0] &= ~0xF;
        gSaveContext.eventInf[0] |= 0x8000;
        FUN_00371680(play, 4, 0);
        *(u16*)((u8*)self + 0xd88) = 0;
        player->stateFlags1 &= ~0x20000000;
    }
}

s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);

#if 0
// near-miss draft (ROM is 1 insn longer: the animationType == 0xF block is NOT if-converted in ROM (bne;..;b), ours predicates it)
s32 FUN_0036b1e0(SkelAnime* skelAnime, f32 frame);
void FUN_00319ae8(Vec3f* pos, u32 sfxId, s32 count);
void __ARM_common_memcpy2_6(void* dst, void* src);

typedef struct {
    char pad0[0xc];
    f32 x0;
    char pad1[0xc];
    f32 x1;
    char pad2[0xc];
    f32 x2;
    s32 pad3;
} EnInJointEntry;
static inline void ZeroRootTransl(EnIn* self) {
    s32 i = ((u8*)&self->skelAnime)[0x75];
    f32* p = (f32*)self->skelAnime.jointTable + i * 13;
    p[3] = 0.0f;
    p = (f32*)self->skelAnime.jointTable + i * 13;
    p[7] = 0.0f;
    p = (f32*)self->skelAnime.jointTable + i * 13;
    p[11] = 0.0f;
}

void FUN_003b8bd4(EnIn* self, PlayState* play) {
    f32 one = 1.0f;
    f32 zero = 0.0f;

    if (self->skelAnime.animationType == 9 || self->skelAnime.animationType == 0xb) {
        f32 speed = self->skelAnime.playSpeed;
        f32 d = self->skelAnime.curFrame - self->unk_478;

        if (!(d > zero)) {
            d = one;
        }
        self->skelAnime.playSpeed = d;
        if (FUN_0036b1e0(&self->skelAnime, 8.0f)) {
            if (self->unk_476 != 0) {
                FUN_00319ae8(&self->actor.world.pos, 0x01000541, 2);
            }
            self->unk_476 = 1 - (s16)self->unk_476;
        }
        self->skelAnime.playSpeed = speed;
        self->unk_478 = self->skelAnime.curFrame;
    }
    if (self->skelAnime.animationType == 0xf) {
        if (FUN_0036b1e0(&self->skelAnime, 20.0f)) {
            Audio_PlayActorSound2(&self->actor, 0x0100053f);
        }
    }
    if (self->skelAnime.animationType == 0xf) {
        self->unk_3F8 = self->skelAnime.curFrame;
        self->skelAnime.playSpeed = one;
    } else {
        self->unk_3F8 = self->skelAnime.curFrame;
        self->skelAnime.curFrame = self->unk_3F4;
        self->skelAnime.playSpeed = zero;
    }
    s32 done = FUN_003731e0(&self->skelAnime);
    ZeroRootTransl(self);
    if (!done && self->skelAnime.curFrame > self->unk_3F8) {
        return;
    }
    self->horsebackAnimationIdx %= 8;
    self->unusedAnimationIdx = self->horsebackAnimationIdx;
    if (self->horsebackAnimationIdx == 3 || self->horsebackAnimationIdx == 4) {
        Audio_PlaySoundGeneral(0x01000110, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
        if (Rand_ZeroOne() < 0.3f) {
            Audio_PlaySoundGeneral(0x01000136, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                   &DAT_0054ac24.unk_0);
        }
    }
    Animation_Change_00375c08(&self->skelAnime, EnIn_sHorsebackAnimations[self->horsebackAnimationIdx], one, zero,
                              Animation_GetLastFrame(&self->skelAnime, EnIn_sHorsebackAnimations[self->horsebackAnimationIdx]), 2, -10.0f);
    ZeroRootTransl(self);
}
#endif

void FUN_001520b0(EnIn* self, PlayState* play);
void Rupees_ChangeBy(s16 rupeeChange);

static inline s16 EnIn_UpdateTalkStateOnClosing(PlayState* play, Actor* thisx) {
    s32 talkState = 0;

    switch (thisx->textId) {
        case 0x2045:
            gSaveContext.infTable[9] |= 0x80;
            break;
        case 0x203E:
            talkState = 2;
            break;
        case 0x203F:
            gSaveContext.eventChkInf[1] |= 2;
            gSaveContext.infTable[9] |= 0x10;
            break;
    }
    return talkState;
}

static inline s16 EnIn_UpdateTalkStateOnChoice(PlayState* play, Actor* thisx) {
    EnIn* self = (EnIn*)thisx;
    s32 talkState = 1;

    switch (self->actor.textId) {
        case 0x2030:
        case 0x2031:
            if (FUN_00369f3c(play) == 1) {
                self->actor.textId = 0x2032;
            } else if (gSaveContext.rupees < 10) {
                self->actor.textId = 0x2033;
            } else {
                self->actor.textId = 0x2034;
            }
            Message_ContinueTextbox(play, self->actor.textId);
            gSaveContext.infTable[9] |= 0x400;
            break;
        case 0x2034:
            if (FUN_00369f3c(play) == 1) {
                Rupees_ChangeBy(-10);
                self->actor.textId = 0x205C;
            } else {
                self->actor.textId = 0x2035;
            }
            Message_ContinueTextbox(play, self->actor.textId);
            break;
        case 0x2036:
        case 0x2037:
            if (FUN_00369f3c(play) == 1) {
                talkState = 2;
            } else {
                self->actor.textId = 0x201F;
                Message_ContinueTextbox(play, self->actor.textId);
            }
            break;
        case 0x2038:
            if (FUN_00369f3c(play) == 0 && gSaveContext.rupees >= 50) {
                talkState = 2;
            } else {
                self->actor.textId = 0x2039;
                Message_ContinueTextbox(play, self->actor.textId);
                gSaveContext.infTable[10] |= 4;
            }
            break;
        case 0x205B:
            if (FUN_00369f3c(play) == 0 && gSaveContext.rupees >= 50) {
                talkState = 2;
            } else {
                Message_ContinueTextbox(play, self->actor.textId = 0x2039);
                gSaveContext.eventInf[0] &= ~0xF;
                gSaveContext.eventInf[0] &= ~0x20;
                gSaveContext.eventInf[0] &= ~0x40;
                self->actionFunc = FUN_001520b0;
            }
            break;
    }
    return talkState;
}

static inline s16 EnIn_UpdateTalkStateOnEvent(PlayState* play, Actor* thisx) {
    s32 talkState = 1;

    switch (thisx->textId) {
        case 0x2035:
            Rupees_ChangeBy(-10);
            thisx->textId = 0x205C;
            Message_ContinueTextbox(play, thisx->textId);
            break;
        default:
            talkState = 2;
            break;
    }
    return talkState;
}

s16 FUN_00171370(PlayState* play, Actor* thisx) {
    s16 talkState = 1;

    switch (Message_GetState(&play->msgCtx)) {
        case 0:
        case 1:
            break;
        case 2:
            talkState = EnIn_UpdateTalkStateOnClosing(play, thisx);
            break;
        case 3:
            break;
        case 4:
            if (Message_ShouldAdvance(play)) {
                talkState = EnIn_UpdateTalkStateOnChoice(play, thisx);
            }
            break;
        case 5:
            if (Message_ShouldAdvance(play)) {
                talkState = EnIn_UpdateTalkStateOnEvent(play, thisx);
            }
            break;
    }
    return talkState;
}

void FUN_00367c7c(PlayState* play, u16 textID, Actor* actor);
void ShrinkWindow_SetVal(void* shrinkWindow);
void Interface_ChangeAlpha(s32 mode);
void FUN_00105440(EnIn* self, PlayState* play);
void FUN_003715d0(void* actor, PlayState* play, s32 arg2, s32 arg3);
void FUN_00371680(PlayState* play, s32 arg, s32 arg2);
void FUN_00319ae8(Vec3f* pos, u32 sfxId, s32 count);

void __ARM_common_memcpy2_6(void* dst, void* src);

static inline void EnIn_StartTransition(EnIn* self, PlayState* play, s32 index, u32 transitionType) {
    FUN_003715d0(self, play, index, transitionType);
}

static inline void EnIn_RentalTimeExpired(EnIn* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vec3f subCamAt;
    Vec3f subCamEye;
    Vec3s zeroVec = { 0, 0, 0 };
    Vec3s* zp = &zeroVec; // kept in a callee-saved reg for the helper copy below

    *(s16*)((u8*)self + 0x468) = Gameplay_CreateSubCamera(play);
    Gameplay_ChangeCameraStatus(play, 0, 1);
    Gameplay_ChangeCameraStatus(play, *(s16*)((u8*)self + 0x468), 7);
    subCamAt.x = self->actor.world.pos.x;
    subCamAt.y = self->actor.world.pos.y + 60.0f;
    subCamAt.z = self->actor.world.pos.z;
    subCamEye.x = subCamAt.x;
    subCamEye.y = subCamAt.y - 22.0f;
    subCamEye.z = subCamAt.z + 40.0f;
    Gameplay_CameraSetAtEye(play, *(s16*)((u8*)self + 0x468), &subCamAt, &subCamEye);
    self->actor.shape.rot.y = Math_Vec3f_Yaw(&self->actor.world.pos, &subCamEye);
    self->interactInfo.torsoRot = zeroVec;
    __ARM_common_memcpy2_6(&self->interactInfo.headRot, zp); // Vec3s copy (struct copy would emit a duplicate helper)
    FUN_00367c7c(play, 0x2025, NULL);
    self->interactInfo.talkState = 1;
    player->actor.world.pos = self->actor.world.pos;
    player->actor.world.pos.x += 100.0f * Math_SinS(self->actor.shape.rot.y);
    player->actor.world.pos.z += 100.0f * Math_CosS(self->actor.shape.rot.y);
    if (player->rideActor != NULL) {
        player->rideActor->world.pos = player->actor.world.pos;
        player->rideActor->freezeTimer = 10;
    }
    player->actor.freezeTimer = 10;
    player->stateFlags1 |= 0x20000000;
    self->actor.flags &= ~1;
    ShrinkWindow_SetVal(gUnk_00549600);
    Interface_ChangeAlpha(2);
}

void FUN_003c54f0(EnIn* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 entranceIndex;
    s32 transitionType;
    s32 raceState; // eventInf[0] with the new race state, stored once at the join (keeps the horse-type store)

    if (!(gSaveContext.eventChkInf[1] & 0x800) && (player->stateFlags1 & 0x800000)) {
        gSaveContext.infTable[10] |= 0x800;
    }
    if (gSaveContext.timer1State == 10) {
        Audio_PlaySoundGeneral(0x010004B1, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        EnIn_RentalTimeExpired(self, play);
        self->actionFunc = FUN_00105440;
        gSaveContext.timer1State = 0;
    } else if (self->interactInfo.talkState == 2) {
        if (FUN_00369f3c(play) == 0) {
            if (gSaveContext.rupees < 50) {
                FUN_00371680(play, 4, 0);
                self->interactInfo.talkState = 0;
                return;
            }
            gSaveContext.eventInf[0] = (gSaveContext.eventInf[0] & ~0x10) | (*(u8*)((u8*)GET_PLAYER(play)->rideActor + 0x1b0) << 4);
            raceState = (gSaveContext.eventInf[0] & ~0xF) | 2;
            entranceIndex = 2;
            transitionType = 2;
        } else {
            Audio_PlaySoundGeneral(0x010004B1, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            if (!(gSaveContext.eventChkInf[1] & 0x800)) {
                u16 chk = gSaveContext.eventChkInf[1];
                u16 inf = gSaveContext.infTable[10];
                if (inf & 0x800) {
                    gSaveContext.eventChkInf[1] = chk | 0x800;
                    gSaveContext.infTable[10] = inf | 0x800;
                }
            }
            raceState = gSaveContext.eventInf[0] & ~0xF;
            entranceIndex = 0;
            transitionType = 0x20;
        }
        gSaveContext.eventInf[0] = raceState;
        EnIn_StartTransition(self, play, entranceIndex, transitionType);
        gSaveContext.eventInf[0] |= 0x8000;
        FUN_00371680(play, 0, 0);
        self->interactInfo.talkState = 0;
    }
}

s32 Object_IsLoaded(ObjectContext* objectCtx, s32 slot);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skel, void* cmb, void* p, s32 b, void* jointTable,
                  void* morphTable, s32 limbCount);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_0035c414(Actor* actor, s32 index); // EnIn_ChangeAnim
void FUN_0035239c(s16 arg0);                // Interface_SetTimer
void FUN_001520b0(EnIn* self, PlayState* play);
void FUN_00134590(EnIn* self, PlayState* play);
void FUN_00151fe4(EnIn* self, PlayState* play);
void FUN_001ff980(EnIn* self, PlayState* play);
void FUN_001959f8(EnIn* self, PlayState* play);
void FUN_003b8e48(Actor* thisx, PlayState* play);

static inline u8* EnIn_GetObjBank(PlayState* play, u32 idx) {
    if (idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

static inline s32 EnIn_StartingRace(EnIn* self, PlayState* play) {
    if (self->actor.params != 1 || self->actor.shape.rot.z != 1 || !LINK_IS_ADULT) {
        return false;
    }
    self->collider.base.ocFlags1 &= ~1;
    self->horsebackAnimationIdx = 1;
    {
        f32 frameCount = (f32)Animation_GetLastFrame(&self->skelAnime, EnIn_sHorsebackAnimations[self->horsebackAnimationIdx]);
        Animation_Change_00375c08(&self->skelAnime, EnIn_sHorsebackAnimations[self->horsebackAnimationIdx], 1.0f, 0.0f,
                                  frameCount, 2, 0.0f);
    }
    self->unk_3F8 = 0.0f;
    self->actionFunc = FUN_003b8bd4;
    return true;
}

static inline s32 EnIn_GetStartMode(EnIn* self, PlayState* play) {
    if (play->sceneNum == 0x63 && LINK_IS_CHILD && IS_DAY && self->actor.shape.rot.z == 1 &&
        !(gSaveContext.eventChkInf[1] & 0x10)) {
        return 1;
    }
    if (play->sceneNum == 0x36 && LINK_IS_CHILD && IS_DAY && self->actor.shape.rot.z == 3 &&
        (gSaveContext.eventChkInf[1] & 0x10)) {
        return 1;
    }
    if (play->sceneNum == 0x36 && LINK_IS_CHILD && IS_NIGHT) {
        if ((self->actor.shape.rot.z == 2) && !(gSaveContext.eventChkInf[1] & 0x10)) {
            return 1;
        }
        if ((self->actor.shape.rot.z == 4) && (gSaveContext.eventChkInf[1] & 0x10)) {
            return 1;
        }
    }
    if (play->sceneNum == 0x63 && LINK_IS_ADULT && IS_DAY) {
        if ((self->actor.shape.rot.z == 5) && !(gSaveContext.eventChkInf[1] & 0x100)) {
            return 2;
        }
        if ((self->actor.shape.rot.z == 7) && (gSaveContext.eventChkInf[1] & 0x100)) {
            return 4;
        }
    }
    if (play->sceneNum == 0x4C && LINK_IS_ADULT && IS_NIGHT) {
        if (self->actor.shape.rot.z == 6 && !(gSaveContext.eventChkInf[1] & 0x100)) {
            return 3;
        }
        if (self->actor.shape.rot.z == 8 && (gSaveContext.eventChkInf[1] & 0x100)) {
            return 3;
        }
    }
    return 0;
}

void FUN_003b8684(EnIn* self, PlayState* play) {
    s32 staySpawned = false;

    if (Object_IsLoaded(&play->objectCtx, self->requiredObjectSlot) || self->actor.params < 1) {
        u8* zar;

        ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
        zar = EnIn_GetObjBank(play, (u8)self->actor.objBankIndex) + 0x10;
        FUN_00353e78(zar, play, &self->skelAnime, ZAR_GetCMBByIndex(zar, 0), self->actor.unk_178, -1,
                     (u8*)self + 0x47C, (u8*)self + 0x8F4, 22);
        SkelAnime_SetFaceAnimations((FaceAnimations*)((u8*)self + 0x228), &self->skelAnime, 0, -1, 1);
        ((u8*)&self->skelAnime)[0x75] = 0;
        Collider_InitCylinder(play, &self->collider);
        Collider_SetCylinder(play, &self->collider, &self->actor, &EnIn_sCylinderInit);
        CollisionCheck_SetInfo2(&self->actor.colChkInfo, NULL, &EnIn_sColChkInfoInit);
        if (EnIn_StartingRace(self, play)) {
            gSaveContext.eventInf[0] &= ~0x8000;
            return;
        }
        Actor_SetScale(&self->actor, 0.01f);
        self->actor.targetMode = 6;
        self->interactInfo.talkState = 0;
        self->unk_476 = 1;
        self->unk_478 = 0.0f;
        self->actionFunc = (EnInActionFunc)FUN_003b8e48;

        switch (EnIn_GetStartMode(self, play)) {
            case 1:
                FUN_0035c414(&self->actor, 9);
                self->actionFunc = (EnInActionFunc)FUN_003b8e48;
                break;
            case 3:
                FUN_0035c414(&self->actor, 7);
                self->actionFunc = (EnInActionFunc)FUN_003b8e48;
                if (!(gSaveContext.eventChkInf[1] & 0x100)) {
                    self->actor.params = 5;
                }
                break;
            case 4:
                FUN_0035c414(&self->actor, 8);
                self->eyeIndex = 3;
                self->actionFunc = (EnInActionFunc)FUN_003b8e48;
                break;
            case 0:
                Actor_Kill(&self->actor);
                break;
            default:
                switch (gSaveContext.eventInf[0] & 0xF) {
                    case 0:
                    case 2:
                    case 3:
                    case 4:
                    case 7:
                        if (self->actor.params == 2) {
                            staySpawned = true;
                        }
                        break;
                    case 1:
                        if (self->actor.params == 3) {
                            staySpawned = true;
                        }
                        break;
                    case 5:
                    case 6:
                        if (self->actor.params == 4) {
                            staySpawned = true;
                        }
                        break;
                }
                if (staySpawned != true) {
                    Actor_Kill(&self->actor);
                    return;
                }
                switch (gSaveContext.eventInf[0] & 0xF) {
                    case 0:
                    case 2:
                        FUN_0035c414(&self->actor, 2);
                        self->actionFunc = FUN_001520b0;
                        gSaveContext.eventInf[0] = 0;
                        break;
                    case 1:
                        self->actor.targetMode = 3;
                        FUN_0035c414(&self->actor, 2);
                        self->actionFunc = FUN_003c54f0;
                        FUN_0035239c(60);
                        break;
                    case 3:
                        FUN_0035c414(&self->actor, 4);
                        self->actionFunc = FUN_00134590;
                        break;
                    case 4:
                        FUN_0035c414(&self->actor, 6);
                        self->losingScreamTimer = 8;
                        self->actionFunc = FUN_00151fe4;
                        break;
                    case 5:
                    case 6:
                        self->actor.targetMode = 3;
                        FUN_0035c414(&self->actor, 6);
                        self->losingScreamTimer = 8;
                        self->actionFunc = FUN_001ff980;
                        break;
                    case 7:
                        FUN_0035c414(&self->actor, 2);
                        self->actionFunc = FUN_001959f8;
                        break;
                }
        }
    }
}

s32 EnIn_OverrideLimbDraw(void* unused, s32 limbIndex, PlayState* play, Actor* thisx) {
    if (limbIndex == 9) {
        FUN_0034e01c(play, (u8*)thisx + 0xd90);
    } else if (limbIndex == 8) {
        FUN_0034e01c(play, (u8*)thisx + 0xd96);
    }
    return 0;
}

void EnIn_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, Actor* thisx) {
    EnIn* self = (EnIn*)thisx;
    Vector3f focusPos(1600.0f, 0.0f, 0.0f);

    if (limbIndex == 9) {
        Vector3f* dst = (Vector3f*)&self->actor.focus.pos;
        VEC3TransformAsm(dst, mtx, &focusPos);
        self->actor.focus.rot = self->actor.world.rot;
    } else if (limbIndex == 0x11) {
        if (*(s32*)((u8*)self + 0x1d4) == 7) {
            FUN_0037266c(*(SkeletonAnimationModel**)((u8*)self + 0x1cc), 2);
        } else {
            FUN_0036932c(*(SkeletonAnimationModel**)((u8*)self + 0x1cc), 2);
        }
    } else if (limbIndex == 0x15) {
        if (*(s32*)((u8*)self + 0x1d4) == 7) {
            FUN_0037266c(*(SkeletonAnimationModel**)((u8*)self + 0x1cc), 3);
        } else {
            FUN_0036932c(*(SkeletonAnimationModel**)((u8*)self + 0x1cc), 3);
        }
    }
}

}
