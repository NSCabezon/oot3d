extern "C" {
#include "z_en_po_relay.hpp"

#define FLAGS 0x00011019

void EnPoRelay_Init(Actor* actor, PlayState* play);
void EnPoRelay_Destroy(Actor* actor, PlayState* play);
void EnPoRelay_Update(Actor* actor, PlayState* play);
void EnPoRelay_Draw(Actor* actor, PlayState* play);
void EnPoRelay_Talk(EnPoRelay* self, PlayState* play);
void EnPoRelay_Talk2(EnPoRelay* self, PlayState* play);
void EnPoRelay_EndRace(EnPoRelay* self, PlayState* play);
void EnPoRelay_Idle(EnPoRelay* self, PlayState* play);
void EnPoRelay_DisappearAndReward(EnPoRelay* self, PlayState* play);
void EnPoRelay_Race(EnPoRelay* self, PlayState* play);
extern "C" void _ZN14FaceAnimations7DestroyEv(void* self);
s32 Math_ScaledStepToS(s16* value, s16 target, s16 step);
s32 FUN_0036bb28(Actor* actor, PlayState* play, f32 arg2);
void LightContext_RemoveLight(PlayState* play, void* lightCtx, void* node);
void Actor_SetTextWithPrefix(PlayState* play, Actor* actor, u16 prefix);
u8 Message_GetState(MessageContext* msgCtx);
u8 Message_ShouldAdvance(PlayState* play);
void Message_ContinueTextbox(PlayState* play, u16 textId);
u32 FUN_00369a48(Actor* actor, PlayState* play);

GLOBAL_ASM("data/z_En_Po_Relay.data.s")
GLOBAL_ASM("../src/game/actors/En_Po_Relay/data_syms.s")

extern Vec3s EnPoRelay_sPathPoints[];
extern InitChainEntry EnPoRelay_sInitChain[];
extern ColliderCylinderInit EnPoRelay_sCylinderInit[];
void FUN_00353c9c(Actor* actor, PlayState* play, SkelAnime* skel, s32 a3, s32 a4, void* a5, void* a6, s32 a7);
void* LightContext_InsertLight(PlayState* play, void* lightCtx, void* info);
void Lights_PointNoGlowSetInfo(void* info, f32 x, f32 y, f32 z, s32 r, s32 g, s32 b, s32 a, s32 e);
s32 Actor_WorldYawTowardPoint(Actor* actor, Vec3f* point);
void FUN_0035239c(s16 arg0);
void Audio_PlayActorSound2(Actor* actor, s32 arg1);

ActorInit En_Po_Relay_InitVars = {
    ACTOR_EN_PO_RELAY,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_TK,
    sizeof(EnPoRelay),
    (ActorFunc)EnPoRelay_Init,
    (ActorFunc)EnPoRelay_Destroy,
    (ActorFunc)EnPoRelay_Update,
    (ActorFunc)EnPoRelay_Draw,
};


static inline void EnPoRelay_SetupIdle(Actor* thisx) {
    ((EnPoRelay*)thisx)->bobTimer = 48;
    ((EnPoRelay*)thisx)->pathPoint = 0;
    thisx->room = -1;
    thisx->shape.rot.y = 0;
    thisx->world.rot.y = -0x8000;
    thisx->colChkInfo.mass = 0xFE;
    ((EnPoRelay*)thisx)->actionFunc = EnPoRelay_Idle;
}

void EnPoRelay_Init(Actor* thisx, PlayState* play) {
    EnPoRelay* self = (EnPoRelay*)thisx;
    s32 temp;

    Actor_ProcessInitChain(thisx, EnPoRelay_sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 42.0f);
    FUN_00372f38(thisx, play, &self->unk_BA0, 0, 0);
    FUN_00353c9c(thisx, play, &self->skelAnime, 0, 2, (u8*)self + 0x228, (u8*)self + 0x6A0, 0x16);
    SkelAnime_SetFaceAnimations((FaceAnimations*)((u8*)self + 0xBA4), &self->skelAnime, 0, -1, -1);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, thisx, EnPoRelay_sCylinderInit);
    self->lightNode = LightContext_InsertLight(play, (u8*)play + 0xA70, &self->unk_B30);
    Lights_PointNoGlowSetInfo(&self->unk_B30, (f32)(s16)self->actor.home.pos.x, (f32)(s16)self->actor.home.pos.y,
                              (f32)(s16)self->actor.home.pos.z, 255, 255, 255, 200, 0);
    self->lightColorA = 255;
    temp = true;
    if (*(s32*)((u8*)play + 0x7F48) != 0) {
        Actor_Kill(thisx);
    } else {
        *(s32*)((u8*)play + 0x7F48) = temp;
        Actor_SetTextWithPrefix(play, thisx, 0x41);
        self->textId = self->actor.textId;
        EnPoRelay_SetupIdle(thisx);
    }
    self->actor.params &= 0x3F;
}

void EnPoRelay_Destroy(Actor* thisx, PlayState* play) {
    EnPoRelay* self = (EnPoRelay*)thisx;

    LightContext_RemoveLight(play, (u8*)play + 0xA70, self->lightNode);
    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(thisx, self->unk_BA0, 0);
    SkelAnime_Free2(&self->skelAnime);
    _ZN14FaceAnimations7DestroyEv((u8*)self + 0xBA4);
}

static inline void EnPoRelay_CorrectY(EnPoRelay* self) {
    Math_StepToF(&self->actor.home.pos.y,
                 EnPoRelay_sPathPoints[(self->pathPoint > 27) ? 27 : self->pathPoint].y + 45.0f, 2.0f);
    self->actor.world.pos.y = Math_SinS(self->bobTimer * 0x800) * 8.0f + self->actor.home.pos.y;
}

void EnPoRelay_Update(Actor* thisx, PlayState* play) {
    EnPoRelay* self = (EnPoRelay*)thisx;

    FUN_003731e0(&self->skelAnime);
    self->actionFunc(self, play);
    Actor_MoveForward(&self->actor);
    EnPoRelay_CorrectY(self);
    Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 27.0f, 60.0f, 4);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    Actor_SetFocus(&self->actor, 50.0f);
    if (self->bobTimer != 0) {
        self->bobTimer -= 1;
    }
    if (self->bobTimer == 0) {
        self->bobTimer = 48;
    }
    self->eyeTextureIdx++;
    if (self->eyeTextureIdx == 3) {
        self->eyeTextureIdx = 0;
    }
}

typedef struct {
    f32 r, g, b, a;
} EnPoRelayDrawColor;

void FUN_00235d68(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void FUN_0036932c(SkeletonAnimationModel* model, s32 index);
void FUN_00358778(SkeletonAnimationModel* skelAnimModel, s32 arg1, s32 arg2, EnPoRelayDrawColor* color, s32 arg4);
extern const EnPoRelayDrawColor EnPoRelay_sDrawColor;

void EnPoRelay_Draw(Actor* thisx, PlayState* play) {
    EnPoRelay* self = (EnPoRelay*)thisx;
    EnPoRelayDrawColor color;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    { FaceAnimations* fa = (FaceAnimations*)((u8*)self + 0xBA4); fa->FUN_0035e3a4(0, self->eyeTextureIdx); }
    FUN_0036932c(self->skelAnime.unk_28, 5);
    color = EnPoRelay_sDrawColor;
    color.r = self->lightColorR * (1.0f / 255.0f);
    color.g = self->lightColorG * (1.0f / 255.0f);
    color.b = self->lightColorB * (1.0f / 255.0f);
    SkeletonAnimationModel* m = self->skelAnime.unk_28;
    FUN_00358778(m, 3, 4, &color, 1);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, NULL, FUN_00235d68, &self->actor, 0);
    { FaceAnimations* fa = (FaceAnimations*)((u8*)self + 0xBA4); fa->FUN_0035e330(); }
}

void EnPoRelay_EndRace(EnPoRelay* self, PlayState* play) {
    Math_ScaledStepToS(&self->actor.shape.rot.y, -0x4000, 0x800);
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actionFunc = EnPoRelay_Talk2;
    } else if (*((u8*)play + 0x4C30) == 5) {
        Actor_Kill(&self->actor);
        gSaveContext.timer1State = 0;
    } else if (Actor_IsFacingAndNearPlayer(&self->actor, 150.0f, 0x3000)) {
        self->actor.textId = self->textId;
        FUN_0036bb28(&self->actor, play, 250.0f);
    }
    FUN_00373264(&self->actor, 0x010002e1);
}

void EnPoRelay_Idle(EnPoRelay* self, PlayState* play) {
    Math_ScaledStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 0x100);
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actor.flags &= ~0x10000;
        self->actionFunc = EnPoRelay_Talk;
    } else if (self->actor.xzDistToPlayer < 250.0f) {
        self->actor.flags |= 0x10000;
        self->actor.textId = self->textId;
        FUN_0036bb28(&self->actor, play, 250.0f);
    }
    FUN_00373264(&self->actor, 0x010002e1);
}

void EnPoRelay_Talk2(EnPoRelay* self, PlayState* play) {
    Math_ScaledStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 0x100);
    if (Message_GetState((MessageContext*)((u8*)play + 0x28A0)) == 5) {
        if (Message_ShouldAdvance(play)) {
            if (self->hookshotSlotFull) {
                Actor_SetTextWithPrefix(play, &self->actor, 0x2E);
            } else {
                Actor_SetTextWithPrefix(play, &self->actor, 0x2D);
            }
            self->textId = self->actor.textId;
            Message_ContinueTextbox(play, self->actor.textId);
        }
    } else if (FUN_00369a48(&self->actor, play)) {
        gSaveContext.timer1State = 0;
        self->actionTimer = 0;
        self->actionFunc = EnPoRelay_DisappearAndReward;
    }
    FUN_00373264(&self->actor, 0x010002e1);
}

// Draft, not matched (float actionTimer math in the ROM): see ws/En_Po_Relay/STATUS.md.
#if 0
void EnPoRelay_Talk(EnPoRelay* self, PlayState* play) {
    Vec3f vec;

    Math_ScaledStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 0x100);
    if (FUN_00369a48(&self->actor, play)) {
        Actor_SetTextWithPrefix(play, &self->actor, 0x2F);
        self->textId = self->actor.textId;
        vec.x = EnPoRelay_sPathPoints[self->pathPoint].x;
        vec.y = EnPoRelay_sPathPoints[self->pathPoint].y;
        vec.z = EnPoRelay_sPathPoints[self->pathPoint].z;
        self->actionTimer = ((s16)(self->actor.shape.rot.y - self->actor.world.rot.y - 0x8000) >> 0xB) % 32U;
        FUN_0035239c(0);
        self->hookshotSlotFull = INV_CONTENT(ITEM_HOOKSHOT) != ITEM_NONE;
        self->yawTowardsPathPoint = Actor_WorldYawTowardPoint(&self->actor, &vec);
        self->actor.flags |= 0x8000000;
        Audio_PlayActorSound2(&self->actor, 0x0100035C);
        self->actionFunc = EnPoRelay_Race;
    }
    FUN_00373264(&self->actor, 0x010002e1);
}
#endif

GLOBAL_ASM("asm/EnPoRelay_Talk.s")

static inline void EnPoRelay_Vec3sToVec3f(Vec3f* dest, Vec3s* src) {
    dest->x = src->x;
    dest->y = src->y;
    dest->z = src->z;
}

f32 Actor_WorldDistXZToPoint(Actor* actor, Vec3f* point);

void EnPoRelay_Race(EnPoRelay* self, PlayState* play) {
    Actor* player = (Actor*)GET_PLAYER(play);
    Vec3f vec;
    f32 speedXZ;
    f32 multiplier;

    if (self->actionTimer != 0) {
        self->actionTimer--;
    }
    if (self->actionTimer == 0 && Rand_ZeroOne() < 0.03f) {
        self->actionTimer = 48;
        if (self->pathPoint < 23) {
            speedXZ = Rand_ZeroOne() * 3.0f;
            if (speedXZ < 1.0f) {
                multiplier = 1.0f;
            } else if (speedXZ < 2.0f) {
                multiplier = -1.0f;
            } else {
                multiplier = 0.0f;
            }
            Actor_Spawn(&play->actorCtx, play, 0x11C,
                        self->actor.world.pos.x + 30.0f * multiplier * Math_CosS(self->yawTowardsPathPoint),
                        self->actor.world.pos.y,
                        self->actor.world.pos.z + 30.0f * multiplier * Math_SinS(self->yawTowardsPathPoint), 0,
                        (self->yawTowardsPathPoint + 0x8000) - (0x2000 * multiplier), 0, 2, 1);
        }
    }
    Math_SmoothStepToS(&self->actor.world.rot.y, self->yawTowardsPathPoint, 2, 0x1000, 0x100);
    self->actor.shape.rot.y = self->actor.world.rot.y + (self->actionTimer * 0x800) + 0x8000;
    if (self->pathPoint < 23) {
        if ((Math3D_PointInSquare2D(660.0f, 840.0f, -4480.0f, -3760.0f, player->world.pos.x, player->world.pos.z) != 0) ||
            (Math3D_PointInSquare2D(1560.0f, 1740.0f, -4030.0f, -3670.0f, player->world.pos.x, player->world.pos.z) != 0) ||
            (Math3D_PointInSquare2D(1580.0f, 2090.0f, -3030.0f, -2500.0f, player->world.pos.x, player->world.pos.z) != 0)) {
            speedXZ = (self->hookshotSlotFull) ? player->speedXZ * 1.4f : player->speedXZ * 1.2f;
        } else if (self->actor.xzDistToPlayer < 150.0f) {
            speedXZ = (self->hookshotSlotFull) ? player->speedXZ * 1.2f : player->speedXZ;
        } else if (self->actor.xzDistToPlayer < 300.0f) {
            speedXZ = (self->hookshotSlotFull) ? player->speedXZ : player->speedXZ * 0.8f;
        } else if (self->hookshotSlotFull) {
            speedXZ = 4.5f;
        } else {
            speedXZ = 3.5f;
        }
        multiplier = 250.0f - self->actor.xzDistToPlayer;
        multiplier = (multiplier < 0.0f) ? 0.0f : multiplier;
        speedXZ += multiplier * 0.02f + 1.0f;
        Math_ApproachF(&self->actor.speedXZ, speedXZ, 0.5f, 1.5f);
    } else {
        Math_ApproachF(&self->actor.speedXZ, 3.5f, 0.5f, 1.5f);
    }
    EnPoRelay_Vec3sToVec3f(&vec, &EnPoRelay_sPathPoints[self->pathPoint]);
    if (Actor_WorldDistXZToPoint(&self->actor, &vec) < 40.0f) {
        self->pathPoint++;
        EnPoRelay_Vec3sToVec3f(&vec, &EnPoRelay_sPathPoints[self->pathPoint]);
        if (self->pathPoint == 28) {
            self->actor.speedXZ = 0.0f;
            self->actor.world.rot.y = self->actor.home.rot.y + 0xC000;
            self->actor.flags &= ~0x8000000;
            self->actionFunc = EnPoRelay_EndRace;
        } else if (self->pathPoint == 9) {
            Flags_SetSwitch(play, 0x35);
        } else if (self->pathPoint == 17) {
            Flags_SetSwitch(play, 0x36);
        } else if (self->pathPoint == 25) {
            Flags_SetSwitch(play, 0x37);
        }
    }
    self->yawTowardsPathPoint = Actor_WorldYawTowardPoint(&self->actor, &vec);
    FUN_00373264(&self->actor, 0x010002e2);
}



}
