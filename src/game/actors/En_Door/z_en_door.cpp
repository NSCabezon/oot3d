extern "C" {
#include "z_en_door.hpp"

#define FLAGS 0x00400030

#define DOOR_AJAR_SLAM_RANGE 120.0f
#define DOOR_AJAR_OPEN_RANGE (2 * DOOR_AJAR_SLAM_RANGE)
#define DOOR_CHECK_RANGE 40.0f

void EnDoor_Init(Actor* actor, GameState* state);
void EnDoor_Destroy(Actor* thisx, PlayState* play);
void EnDoor_Update(Actor* thisx, PlayState* play);
void EnDoor_Draw(Actor* thisx, PlayState* play);

GLOBAL_ASM("data/z_En_Door.data.s")

s32 FUN_00369a48(Actor* actor, PlayState* play); // N64: Actor_TextboxIsClosing
s32 Math_ScaledStepToS(s16* value, s16 target, s16 step);
void SkelAnime_Free2(SkelAnime* skelAnime);
u32 Actor_ProcessTalkRequest(Actor* actor, PlayState* play);

typedef struct {
    /* 0x00 */ u8 unk_00[4];
    /* 0x04 */ s16 id;
    /* 0x06 */ u8 unk_06[0xA];
} EnDoorTransitionEntry; // size = 0x10

#define ENDOOR_TRANSITIONS(play) (*(EnDoorTransitionEntry**)((u8*)(play) + 0x5B8C)) // play->transitionActors.list

s32 EnDoor_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void FUN_0037266c(SkeletonAnimationModel* model, s32 idx);
void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);
void FUN_0033a05c(PlayState* play, s32 lockTimer, MTX34* mtx, void* pos);

#define ENDOOR_LIMB_TABLE ((u8*)0x004D9466)
#define ENDOOR_DL_TABLE ((u8*)0x004D946B)

s32 FUN_00370734(SkelAnime* skelAnime);
s32 FUN_0036e5e0(SkelAnime* skelAnime, f32 frame, f32 updateRate);
void EffectSsBubble_Spawn(PlayState* play, Vec3f* pos, f32 a, f32 b, f32 c, f32 d);

#define ENDOOR_OPEN_FRAMES ((u8*)0x004D9489)
#define ENDOOR_CLOSE_FRAMES ((u8*)0x004D949D)

typedef struct EnDoorInfo {
    /* 0x00 */ s16 sceneId;
    /* 0x02 */ u8 dListIndex;
    /* 0x04 */ s16 objectId;
} EnDoorInfo;

void EnDoor_SetupType(EnDoor* self, PlayState* play);
void FUN_00342968(void* unk);
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
void Actor_Kill(Actor* actor);

#define ENDOOR_INFO ((EnDoorInfo*)0x00522B08)
#define ENDOOR_INIT_CHAIN ((InitChainEntry*)0x00522B00)
#define ENDOOR_PARAMS_IS_DOUBLE_DOOR_MASK 0x40
#define ENDOOR_GET_IS_DOUBLE_DOOR(a) (((a)->params >> 6) & 1)

void EnDoor_AjarClose(EnDoor* self, PlayState* play);
void EnDoor_Idle(EnDoor* self, PlayState* play);
void EnDoor_AjarWait(EnDoor* self, PlayState* play);
void EnDoor_WaitForCheck(EnDoor* self, PlayState* play);
void EnDoor_Check(EnDoor* self, PlayState* play);


ActorInit En_Door_InitVars = {
    ACTOR_EN_DOOR,
    ACTORCAT_DOOR,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnDoor),
    (ActorFunc)EnDoor_Init,
    (ActorFunc)EnDoor_Destroy,
    (ActorFunc)EnDoor_Update,
    (ActorFunc)EnDoor_Draw,
};

#if 0
void EnDoor_Init(Actor* thisx, GameState* state) {
    EnDoor* self = (EnDoor*)thisx;
    PlayState* play = (PlayState*)state;
    EnDoorInfo* objectInfo;
    s32 i;
    s32 objectSlot;
    f32 xOffset;
    f32 zOffset;

    self->actor.unk_19A = 1;
    FUN_00342968((u8*)self + 0x3CC);

    objectInfo = ENDOOR_INFO;
    Actor_ProcessInitChain(&self->actor, ENDOOR_INIT_CHAIN);
    for (i = 0; i < 4; i++, objectInfo++) {
        if (play->sceneNum == objectInfo->sceneId) {
            break;
        }
    }
    if (i >= 4 && Object_GetIndex(&play->objectCtx, OBJECT_GAMEPLAY_FIELD_KEEP) >= 0) {
        objectInfo++;
    }

    self->dListIndex = objectInfo->dListIndex;
    objectSlot = Object_GetIndex(&play->objectCtx, objectInfo->objectId);
    if (objectSlot < 0) {
        Actor_Kill(&self->actor);
        return;
    }

    self->requiredObjectSlot = objectSlot;
    if (self->actor.objBankIndex == self->requiredObjectSlot) {
        EnDoor_SetupType(self, play);
    } else {
        self->actionFunc = EnDoor_SetupType;
    }

    if (ENDOOR_GET_IS_DOUBLE_DOOR(&self->actor)) {
        EnDoor* other;

        xOffset = Math_CosS(self->actor.shape.rot.y) * 30.0f;
        zOffset = Math_SinS(self->actor.shape.rot.y) * 30.0f;
        other = (EnDoor*)Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, ACTOR_EN_DOOR,
                                            self->actor.world.pos.x + xOffset, self->actor.world.pos.y,
                                            self->actor.world.pos.z - zOffset, 0, self->actor.shape.rot.y + 0x8000, 0,
                                            self->actor.params & ~ENDOOR_PARAMS_IS_DOUBLE_DOOR_MASK);
        if (other != NULL) {
            other->unk_3EC[0] = 1;
        }
        self->actor.world.pos.x -= xOffset;
        self->actor.world.pos.z += zOffset;
    }
    Actor_SetFocus(&self->actor, 70.0f);
}
#endif

GLOBAL_ASM("asm/EnDoor_Init.s")


#if 0
void EnDoor_Destroy(Actor* thisx, PlayState* play) {
    EnDoor* self = (EnDoor*)thisx;
    EnDoorTransitionEntry* transitionEntry;

    transitionEntry = &ENDOOR_TRANSITIONS(play)[((u16)self->actor.params) >> 10];
    if (transitionEntry->id < 0) {
        transitionEntry->id = -transitionEntry->id;
    }
    SkelAnime_Free2(&self->skelAnime);
}
#endif

GLOBAL_ASM("asm/EnDoor_Destroy.s")

void EnDoor_Update(Actor* thisx, PlayState* play) {
    EnDoor* self = (EnDoor*)thisx;

    self->actionFunc(self, play);
}

#if 0
void EnDoor_Draw(Actor* thisx, PlayState* play) {
    EnDoor* self = (EnDoor*)thisx;

    if (self->actor.objBankIndex == self->requiredObjectSlot) {
        SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnDoor_OverrideLimbDraw, NULL, &self->actor, 0);
        if (self->actor.world.rot.y != 0) {
            u8* dlTable = ENDOOR_DL_TABLE;

            if (self->actor.world.rot.y > 0) {
                FUN_0037266c(self->skelAnime.unk_28, dlTable[self->dListIndex * 2 + 1]);
                FUN_0036932c(self->skelAnime.unk_28, dlTable[self->dListIndex * 2]);
            } else {
                FUN_0037266c(self->skelAnime.unk_28, dlTable[self->dListIndex * 2]);
                FUN_0036932c(self->skelAnime.unk_28, dlTable[self->dListIndex * 2 + 1]);
            }
        }
        if (self->lockTimer != 0) {
            FUN_0033a05c(play, self->lockTimer, &self->actor.modelMtx, (u8*)self + 0x3CC);
        }
    }
}
#endif

GLOBAL_ASM("asm/EnDoor_Draw.s")


void EnDoor_AjarClose(EnDoor* self, PlayState* play) {
    if (Math_ScaledStepToS(&self->actor.world.rot.y, 0, 0x700)) {
        self->actionFunc = EnDoor_Idle;
    }
}

void EnDoor_AjarOpen(EnDoor* self, PlayState* play) {
    if (self->actor.xzDistToPlayer < DOOR_AJAR_SLAM_RANGE) {
        self->actionFunc = EnDoor_AjarClose;
    } else if (Math_ScaledStepToS(&self->actor.world.rot.y, -0x1800, 0x100)) {
        self->actionFunc = EnDoor_AjarWait;
    }
}

void EnDoor_AjarWait(EnDoor* self, PlayState* play) {
    if (self->actor.xzDistToPlayer < DOOR_AJAR_SLAM_RANGE) {
        self->actionFunc = EnDoor_AjarClose;
    }
}

void EnDoor_Check(EnDoor* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        self->actionFunc = EnDoor_WaitForCheck;
    }
}

GLOBAL_ASM("asm/EnDoor_Idle.s")

#if 0
void EnDoor_Open(EnDoor* self, PlayState* play) {
    if (DECR(self->lockTimer) == 0) {
        if (FUN_00370734(&self->skelAnime)) {
            self->actionFunc = EnDoor_Idle;
            self->playerIsOpening = 0;
        } else if (FUN_0036e5e0(&self->skelAnime, ENDOOR_OPEN_FRAMES[self->openAnim + self->dListIndex * 4], 1.0f)) {
            Audio_PlayActorSound2(&self->actor, (play->sceneNum == 7 || play->sceneNum == 8 || play->sceneNum == 4)
                                                    ? 0x01000232
                                                    : 0x010004E2);
            if (self->skelAnime.playSpeed < 1.0f) {
                s32 numEffects = (s32)(Rand_ZeroOne() * 30.0f) + 50;
                s32 i;

                for (i = 0; i < numEffects; i++) {
                    EffectSsBubble_Spawn(play, &self->actor.world.pos, 60.0f, 100.0f, 50.0f, 0.15f);
                }
            }
        } else if (FUN_0036e5e0(&self->skelAnime, ENDOOR_CLOSE_FRAMES[self->openAnim + self->dListIndex * 4], 1.0f)) {
            Audio_PlayActorSound2(&self->actor, (play->sceneNum == 7 || play->sceneNum == 8 || play->sceneNum == 4)
                                                    ? 0x01000233
                                                    : 0x01000158);
        }
    }
}
#endif

GLOBAL_ASM("asm/EnDoor_Open.s")


s32 EnDoor_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnDoor* self = (EnDoor*)thisx;

    if (limbIndex == ENDOOR_LIMB_TABLE[self->dListIndex]) {
        u8* dlTable = ENDOOR_DL_TABLE;
        SkeletonAnimationModel* model = self->skelAnime.unk_28;

        FUN_0037266c(model, dlTable[self->dListIndex * 2]);
        FUN_0037266c(model, dlTable[self->dListIndex * 2 + 1]);
    }
}


GLOBAL_ASM("asm/EnDoor_SetupType.s")

void EnDoor_WaitForCheck(EnDoor* self, PlayState* play) {
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actionFunc = EnDoor_Check;
    } else {
        FUN_0036bb28(&self->actor, play, DOOR_CHECK_RANGE);
    }
}

}
