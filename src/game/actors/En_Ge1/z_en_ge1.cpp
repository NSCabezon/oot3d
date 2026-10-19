extern "C" {
#include "z_en_ge1.hpp"

#define FLAGS 0x00000009

void EnGe1_Init(Actor* actor, GameState* state);
void EnGe1_Destroy(Actor* thisx, PlayState* play);
void EnGe1_Update(Actor* actor, GameState* state);
void EnGe1_Draw(Actor* thisx, PlayState* play);
s32 EnGe1_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnGe1_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);

#define GE1_STATE_TALKING (1 << 0)
#define GE1_STATE_GIVE_QUIVER (1 << 1)
#define GE1_STATE_IDLE_ANIM (1 << 2)
#define GE1_STATE_STOP_FIDGET (1 << 3)

extern u8 gSaveContext_E00[];
extern u8 gSaveContext_F00[];
extern f32 FLOAT_0054ac20;
extern DAT_0054ac24_struct DAT_0054ac24;

void Animation_PlayOnce(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
s32 FUN_00369a48(Actor* actor, PlayState* play);
void FUN_003725e0(PlayState* play);
void FUN_0033c764(PlayState* play);
void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx);
void FUN_0036932c(void* model, s32 index);
void FUN_0037266c(void* model, s32 index);
void EnGe1_TalkTooPoor_Archery(EnGe1* self, PlayState* play);
Actor* Actor_FindNearby(PlayState* play, Actor* actor, s16 actorId, s32 actorCat, f32 dist);
void FUN_0012a76c(EnGe1* self, PlayState* play);
void FUN_003af608(EnGe1* self, PlayState* play);
void FUN_003ebda0(EnGe1* self, PlayState* play);
void EnGe1_GetReaction_GateGuard(EnGe1* self, PlayState* play);
void FUN_003c2e3c(EnGe1* self, PlayState* play);
void FUN_003c2ec4(EnGe1* self, PlayState* play);
void EnGe1_KickPlayer(EnGe1* self, PlayState* play);
void FUN_003c2fe0(EnGe1* self, PlayState* play);
void FUN_003c305c(EnGe1* self, PlayState* play);
void EnGe1_WaitUntilGateOpened_GateOp(EnGe1* self, PlayState* play);
void EnGe1_OpenGate_GateOp(EnGe1* self, PlayState* play);
void FUN_003f1344(EnGe1* self, PlayState* play);
s32 FUN_00369f3c(PlayState* play);
void Rupees_ChangeBy(s32 amount);
s32 EnGe1_SetTalkAction(EnGe1* self, PlayState* play, u16 textId, f32 arg3, EnGe1ActionFunc actionFunc);
void EnGe1_CueUpAnimation(EnGe1* self);
void EnGe1_StopFidget(EnGe1* self);
void EnGe1_DoNothing(EnGe1* self, PlayState* play);
void EnGe1_SetNormalText(EnGe1* self, PlayState* play);
void EnGe1_ChooseActionFromTextId(EnGe1* self, PlayState* play);
void EnGe1_GetReaction_ValleyFloor(EnGe1* self, PlayState* play);
void EnGe1_CheckGate_GateOp(EnGe1* self, PlayState* play);
void EnGe1_Wait_Archery(EnGe1* self, PlayState* play);
void EnGe1_TalkNoPrize_Archery(EnGe1* self, PlayState* play);
void EnGe1_TalkWinPrize_Archery(EnGe1* self, PlayState* play);
void EnGe1_TalkOfferPlay_Archery(EnGe1* self, PlayState* play);
void EnGe1_TalkNoHorse_Archery(EnGe1* self, PlayState* play);
void EnGe1_BeginGame_Archery(EnGe1* self, PlayState* play);
void EnGe1_BeginItemGive_Archery(EnGe1* self, PlayState* play);
void EnGe1_WaitGateOpen_GateOp(EnGe1* self, PlayState* play);
void EnGe1_SetupOpenGate_GateOp(EnGe1* self, PlayState* play);

GLOBAL_ASM("data/z_En_Ge1.data.s")

ActorInit En_Ge1_InitVars = {
    ACTOR_EN_GE1,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GE1,
    sizeof(EnGe1),
    (ActorFunc)EnGe1_Init,
    (ActorFunc)EnGe1_Destroy,
    (ActorFunc)EnGe1_Update,
    (ActorFunc)EnGe1_Draw,
};

GLOBAL_ASM("asm/EnGe1_Init.s")

void EnGe1_Destroy(Actor* thisx, PlayState* play) {
    EnGe1* self = (EnGe1*)thisx;

    Collider_DestroyCylinder(play, (ColliderCylinder*)((u8*)self + 0x1A4));
    FUN_003508b8(thisx, self->model, 0);
    SkelAnime_Free2((SkelAnime*)((u8*)self + 0x1FC));
    ((FaceAnimations*)((u8*)self + 0x8BC))->Destroy();
}

GLOBAL_ASM("asm/EnGe1_Update.s")

#if 0 // draft: not matched, 28 words off (face-pointer and eye-offset register order, 0x8AA switch sharing; GraphicsContext_UnknownStub is link-inlined)
void EnGe1_Draw(Actor* thisx, PlayState* play) {
    EnGe1* self = (EnGe1*)thisx;
    FaceAnimations* face;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    face = (FaceAnimations*)((u8*)self + 0x8BC);
    face->FUN_0035e3a4(0, *(s16*)((u8*)self + 0x8A4));
    FUN_0036932c(*(void**)((u8*)self + 0x224), 1);
    FUN_0036932c(*(void**)((u8*)self + 0x224), 2);
    FUN_0036932c(*(void**)((u8*)self + 0x224), 3);

    if (*(u8*)((u8*)self + 0x8AA) == 0) {
        FUN_0037266c(*(void**)((u8*)self + 0x224), 2);
    } else if (*(u8*)((u8*)self + 0x8AA) == 1) {
        FUN_0037266c(*(void**)((u8*)self + 0x224), 1);
    } else if (*(u8*)((u8*)self + 0x8AA) == 2) {
        FUN_0037266c(*(void**)((u8*)self + 0x224), 3);
    }

    SkelAnime_DrawOpa((SkelAnime*)((u8*)self + 0x1FC), (MTX34*)((u8*)self + 0x148),
                      (OverrideLimbDrawOpa)EnGe1_OverrideLimbDraw, (PostLimbDrawOpa)EnGe1_PostLimbDraw, thisx, 0);
    face->FUN_0035e330();
}
#endif
GLOBAL_ASM("asm/EnGe1_Draw.s")

void EnGe1_BeginGame_Archery(EnGe1* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Actor* horse;
    s32 choice;

    if (Message_GetState(&play->msgCtx) == 4 && Message_ShouldAdvance(play)) {
        self->actor.flags &= ~0x10000;

        choice = FUN_00369f3c(play);
        if (choice == 0) {
            goto rupees;
        }
        if (choice != 1) {
            return;
        }
        self->actionFunc = EnGe1_Wait_Archery;
        FUN_003725e0(play);
        return;

rupees:
        if (gSaveContext.rupees < 20) {
            Message_ContinueTextbox(play, 0x85);
            self->actionFunc = EnGe1_TalkTooPoor_Archery;
        } else {
            Rupees_ChangeBy(-20);
            FUN_003716f0(play, 0x129, 0x14, 0x26);
            u8* miscCtx = (u8*)0x00588E58;

            *(s16*)(miscCtx + 0xA0) = -16;
            *(u16*)(miscCtx + 0x8A) |= 0x100;
            *(u16*)(miscCtx - 0x700 + 0xF8) |= 0x100;

            if (!(player->stateFlags1 & 0x800000)) {
                FUN_0036e980(play, &self->actor, 1);
            } else {
                horse = Actor_FindNearby(play, &player->actor, 0x14, 1, 1200.0f);
                player->actor.freezeTimer = 0x708;

                if (horse != NULL) {
                    horse->freezeTimer = 0x708;
                }
            }
            self->actionFunc = EnGe1_DoNothing;
        }
    }
}

void EnGe1_WaitTillItemGiven_Archery(EnGe1* self, PlayState* play);
void EnGe1_BeginItemGive_Archery(EnGe1* self, PlayState* play) {
    s32 getItemId = 0;
    EnGe1* s2 = self;

    if (FUN_00369a48(&self->actor, play)) {
        EnGe1ActionFunc next = EnGe1_WaitTillItemGiven_Archery;
        u32 flags = self->actor.flags;
        self->actionFunc = next;
        s2->actor.flags = flags & ~0x10000;
    }

    if (!(self->stateFlags & GE1_STATE_GIVE_QUIVER)) {
        getItemId = 0x3E;
    } else {
        s32 upg = CUR_UPG_VALUE(UPG_QUIVER);

        if (upg == 1) {
            getItemId = 0x30;
        } else if (upg == 2) {
            getItemId = 0x31;
        }
    }

    GiveItem(&self->actor, play, getItemId, 10000.0f, 50.0f);
}


void EnGe1_CheckGate_GateOp(EnGe1* self, PlayState* play) {
    if (Flags_GetSwitch(play, (self->actor.params >> 8) & 0x3F)) {
        EnGe1_SetTalkAction(self, play, 0x6018, 100.0f, EnGe1_WaitGateOpen_GateOp);
    } else {
        EnGe1_SetTalkAction(self, play, 0x6017, 100.0f, EnGe1_SetupOpenGate_GateOp);
    }
}

void EnGe1_ChooseActionFromTextId(EnGe1* self, PlayState* play) {
    self->stateFlags |= GE1_STATE_TALKING;

    if (FUN_00369a48(&self->actor, play)) {
        switch (self->actor.textId) {
            case 0x6001:
                self->actionFunc = EnGe1_SetNormalText;
                break;

            case 0x601A:
            case 0x6019:
                self->actionFunc = EnGe1_GetReaction_ValleyFloor;
                break;

            case 0x6018:
                self->actionFunc = EnGe1_CheckGate_GateOp;
                break;

            default:
                self->actionFunc = EnGe1_GetReaction_ValleyFloor;
                break;
        }
    }
}

void EnGe1_CueUpAnimation(EnGe1* self) {
    if (FUN_003731e0((SkelAnime*)((u8*)self + 0x1FC))) {
        Animation_PlayOnce((SkelAnime*)((u8*)self + 0x1FC), self->animation);
    }
}

static inline void EnGe1_SetAnimationIdle(EnGe1* self) {
    Animation_Change_00375c08((SkelAnime*)((u8*)self + 0x1FC), 2, -1.0f,
                              Animation_GetLastFrame((SkelAnime*)((u8*)self + 0x1FC), 2), 0.0f, 2, 8.0f);
    self->animation = 2;
    self->animFunc = EnGe1_CueUpAnimation;
}

void EnGe1_DoNothing(EnGe1* self, PlayState* play) {
}

void FUN_003af608(EnGe1* self, PlayState* play) {
    self->stateFlags |= GE1_STATE_TALKING;

    if (FUN_00369a48(&self->actor, play)) {
        self->actionFunc = EnGe1_GetReaction_GateGuard;
        EnGe1_SetAnimationIdle(self);
    }
}

void EnGe1_GetReaction_GateGuard(EnGe1* self, PlayState* play) {
    u16 textId = Text_GetFaceReaction(play, 0x22);

    if (textId == 0) {
        textId = 0x6069;
    }

    if (EnGe1_SetTalkAction(self, play, textId, 100.0f, FUN_003af608)) {
        self->animFunc = EnGe1_CueUpAnimation;
        self->animation = 1;
        Animation_Change_00375c08((SkelAnime*)((u8*)self + 0x1FC), 1, 1.0f, 0.0f,
                                  Animation_GetLastFrame((SkelAnime*)((u8*)self + 0x1FC), 1), 2, -8.0f);
    }
}

void FUN_0012a76c(EnGe1* self, PlayState* play) {
    if (CHECK_QUEST_ITEM(22)) {
        EnGe1_SetTalkAction(self, play, 0x6014, 100.0f, FUN_003c2ec4);
    } else {
        EnGe1_SetTalkAction(self, play, 0x6013, 100.0f, FUN_003c2e3c);
    }
}

void EnGe1_GetReaction_ValleyFloor(EnGe1* self, PlayState* play) {
    u16 textId = Text_GetFaceReaction(play, 0x22);

    if (textId == 0) {
        textId = 0x6019;
    }

    EnGe1_SetTalkAction(self, play, textId, 100.0f, EnGe1_ChooseActionFromTextId);
}

void FUN_003c2e3c(EnGe1* self, PlayState* play) {
    self->stateFlags |= GE1_STATE_TALKING;

    if (FUN_00369a48(&self->actor, play)) {
        self->actionFunc = FUN_0012a76c;
        EnGe1_SetAnimationIdle(self);
    }
}

void FUN_003c2ec4(EnGe1* self, PlayState* play) {
    self->stateFlags |= GE1_STATE_TALKING;

    if (Message_GetState(&play->msgCtx) == 4 && Message_ShouldAdvance(play)) {
        FUN_003725e0(play);

        s32 choice = FUN_00369f3c(play);
        if (choice == 0) {
            goto rupees;
        }
        if (choice != 1) {
            return;
        }
        self->actionFunc = FUN_0012a76c;
        EnGe1_SetAnimationIdle(self);
        return;
rupees:
        if (gSaveContext.rupees < 10) {
            Message_ContinueTextbox(play, 0x6016);
            self->actionFunc = FUN_003c2fe0;
        } else {
            Rupees_ChangeBy(-10);
            Message_ContinueTextbox(play, 0x6015);
            self->actionFunc = FUN_003c305c;
        }
    }
}

void FUN_003c2fe0(EnGe1* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 2) {
        self->actionFunc = FUN_0012a76c;
        EnGe1_SetAnimationIdle(self);
    }
}

void FUN_003c305c(EnGe1* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
        self->actionFunc = FUN_003ebda0;
        Animation_Change_00375c08((SkelAnime*)((u8*)self + 0x1FC), 0, 1.0f, 0.0f,
                                  Animation_GetLastFrame((SkelAnime*)((u8*)self + 0x1FC), 0), 2, -3.0f);
        self->animation = 0;
        self->animFunc = EnGe1_StopFidget;
        self->stateFlags &= ~GE1_STATE_IDLE_ANIM;
    }
}

void FUN_003f1344(EnGe1* self, PlayState* play) {
    if (self->cutsceneTimer > 0) {
        self->cutsceneTimer--;
    } else {
        EnGe1_SetAnimationIdle(self);
        self->actionFunc = EnGe1_SetNormalText;
    }
    self->stateFlags |= GE1_STATE_STOP_FIDGET;
}

void EnGe1_KickPlayer(EnGe1* self, PlayState* play) {
    self->stateFlags |= GE1_STATE_TALKING;

    if (self->cutsceneTimer > 0) {
        self->cutsceneTimer--;
    } else {
        FUN_0033c764(play);

        if ((INV_CONTENT(ITEM_HOOKSHOT) == ITEM_NONE) || (INV_CONTENT(ITEM_LONGSHOT) == ITEM_NONE)) {
            FUN_003716f0(play, 0x1A5, 0x14, 0x26);
        } else if (*(u16*)&gSaveContext_F00[4] & 0x80) {
            FUN_003716f0(play, 0x5F8, 0x14, 0x26);
        } else {
            FUN_003716f0(play, 0x3B4, 0x14, 0x26);
        }

        play->fadeOutTransition = 0x26;
        play->sceneLoadFlag = 0x14;
    }
}

void FUN_003ebda0(EnGe1* self, PlayState* play) {
    if (self->stateFlags & GE1_STATE_IDLE_ANIM) {
        self->actionFunc = FUN_003f1344;
        Flags_SetSwitch(play, (self->actor.params >> 8) & 0x3F);
        self->cutsceneTimer = 0x4B;
        FUN_003725e0(play);
    } else if (Animation_OnFrameImpl((SkelAnime*)((u8*)self + 0x1FC), 15.0f, 1.0f) ||
               Animation_OnFrameImpl((SkelAnime*)((u8*)self + 0x1FC), 19.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, 0x0100013F);
    }
}

void EnGe1_OpenGate_GateOp(EnGe1* self, PlayState* play) {
    if (self->stateFlags & GE1_STATE_IDLE_ANIM) {
        self->actionFunc = EnGe1_WaitUntilGateOpened_GateOp;
        Flags_SetSwitch(play, (self->actor.params >> 8) & 0x3F);
        self->cutsceneTimer = 0x4B;
        FUN_003725e0(play);
    } else if (Animation_OnFrameImpl((SkelAnime*)((u8*)self + 0x1FC), 15.0f, 1.0f) ||
               Animation_OnFrameImpl((SkelAnime*)((u8*)self + 0x1FC), 19.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, 0x0100013F);
    }
}

void FUN_0034e01c(MTX34* mtx, Vec3s* rot); // rotates mtx by rot (limb helper)

s32 EnGe1_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnGe1* self = (EnGe1*)thisx;

    if (limbIndex == 0xE) {
        FUN_0034e01c(mtx, &self->limbRot);
    }
    return 0;
}

// N64 Matrix_MultVec3f shape: (src, dest) order drives the arg scheduling of the tail call
static inline void EnGe1_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void EnGe1_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnGe1* self = (EnGe1*)thisx;
    static Vector3f sFocusOffset(600.0f, 700.0f, 0.0f);

    if (limbIndex == 0xE) {
        EnGe1_MultVec3f(&sFocusOffset, (Vector3f*)&self->actor.focus.pos, mtx);
    }
}

void EnGe1_SetNormalText(EnGe1* self, PlayState* play) {
    EnGe1_SetTalkAction(self, play, 0x6001, 100.0f, EnGe1_ChooseActionFromTextId);
}

s32 EnGe1_SetTalkAction(EnGe1* self, PlayState* play, u16 textId, f32 arg3, EnGe1ActionFunc actionFunc) {
    Actor* thisx = &self->actor;
    if (Actor_ProcessTalkRequest(thisx, play)) {
        self->actionFunc = actionFunc;
        self->animFunc = EnGe1_StopFidget;
        self->stateFlags &= ~GE1_STATE_IDLE_ANIM;
        self->animation = 2;
        Animation_Change_00375c08((SkelAnime*)((u8*)self + 0x1FC), 2, 1.0f, 0.0f,
                                  Animation_GetLastFrame((SkelAnime*)((u8*)self + 0x1FC), 2), 2, -8.0f);
        return 1;
    }

    thisx->textId = textId;

    if (thisx->xzDistToPlayer < arg3) {
        FUN_0036bb28(thisx, play, arg3);
    }

    return 0;
}

void EnGe1_SetupOpenGate_GateOp(EnGe1* self, PlayState* play) {
    self->stateFlags |= GE1_STATE_TALKING;
    if (Message_GetState(&play->msgCtx) == 5 && Message_ShouldAdvance(play)) {
        self->actionFunc = EnGe1_OpenGate_GateOp;
        Animation_Change_00375c08((SkelAnime*)((u8*)self + 0x1FC), 0, 1.0f, 0.0f,
                                  Animation_GetLastFrame((SkelAnime*)((u8*)self + 0x1FC), 0), 2, -3.0f);
        self->animation = 0;
        self->animFunc = EnGe1_StopFidget;
        self->stateFlags &= ~GE1_STATE_IDLE_ANIM;
    }
}

void EnGe1_SetupWait_Archery(EnGe1* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        self->actionFunc = EnGe1_Wait_Archery;
        EnGe1_SetAnimationIdle(self);
    }
}

void EnGe1_StopFidget(EnGe1* self) {
    if (!(self->stateFlags & GE1_STATE_IDLE_ANIM)) {
        if (FUN_003731e0((SkelAnime*)((u8*)self + 0x1FC))) {
            self->stateFlags |= GE1_STATE_IDLE_ANIM;
        }
        self->stateFlags |= GE1_STATE_STOP_FIDGET;
    }
}

#if 0 // draft: not matched, 52 words off (armcc shares the 0x588E58 save base differently, action ptr hoisting tried)
void EnGe1_TalkAfterGame_Archery(EnGe1* self, PlayState* play) {
    u8* base = (u8*)&gSaveContext + 0x1500;
    u8* saveCtx = base - 0x1500;
    u32 highScore;
    u16 minigameScore;

    *(u16*)(base + 0x8A) &= ~0x100;
    self->actor.flags |= 0x10000;

    highScore = *(u32*)(saveCtx + 0xED0);
    minigameScore = *(u16*)(base + 0x96);
    *(u32*)(saveCtx + 0xED0) = (highScore > minigameScore) ? highScore : minigameScore;

    if (minigameScore < 1000) {
        self->actor.textId = 0x6045;
        self->actionFunc = EnGe1_TalkNoPrize_Archery;
    } else if (!(*(u16*)&gSaveContext_F00[0x42] & 1)) {
        self->actor.textId = 0x6046;
        self->actionFunc = EnGe1_TalkWinPrize_Archery;
        self->stateFlags &= ~GE1_STATE_GIVE_QUIVER;
    } else if (minigameScore < 1500) {
        self->actor.textId = 0x6047;
        self->actionFunc = EnGe1_TalkNoPrize_Archery;
    } else if (*(u16*)&gSaveContext_F00[0x8] & 0x8000) {
        self->actor.textId = 0x6047;
        self->actionFunc = EnGe1_TalkNoPrize_Archery;
    } else {
        self->actor.textId = 0x6044;
        self->actionFunc = EnGe1_TalkWinPrize_Archery;
        self->stateFlags |= GE1_STATE_GIVE_QUIVER;
    }
}
#endif
GLOBAL_ASM("asm/EnGe1_TalkAfterGame_Archery.s")

void EnGe1_TalkNoHorse_Archery(EnGe1* self, PlayState* play) {
    self->stateFlags |= GE1_STATE_TALKING;

    if (FUN_00369a48(&self->actor, play)) {
        self->actionFunc = EnGe1_Wait_Archery;
        EnGe1_SetAnimationIdle(self);
    }
}

void EnGe1_TalkNoPrize_Archery(EnGe1* self, PlayState* play) {
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actionFunc = EnGe1_TalkOfferPlay_Archery;
        return;
    }
    FUN_0036bb28(&self->actor, play, 300.0f);
}

void EnGe1_TalkOfferPlay_Archery(EnGe1* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 5 && Message_ShouldAdvance(play)) {
        Message_ContinueTextbox(play, 0x6041);
        self->actionFunc = EnGe1_BeginGame_Archery;
    }
}

void EnGe1_TalkTooPoor_Archery(EnGe1* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
        FUN_003725e0(play);
        self->actionFunc = EnGe1_Wait_Archery;
        EnGe1_SetAnimationIdle(self);
    }
}

void EnGe1_TalkWinPrize_Archery(EnGe1* self, PlayState* play) {
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actionFunc = EnGe1_BeginItemGive_Archery;
        self->actor.flags &= ~0x10000;
        return;
    }
    FUN_0036bb28(&self->actor, play, 200.0f);
}

void EnGe1_WaitGateOpen_GateOp(EnGe1* self, PlayState* play) {
    self->stateFlags |= GE1_STATE_TALKING;

    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
        FUN_003725e0(play);
        self->actionFunc = EnGe1_CheckGate_GateOp;
        EnGe1_SetAnimationIdle(self);
    }
}

void EnGe1_WaitTillItemGiven_Archery(EnGe1* self, PlayState* play) {
    s32 getItemId = 0;

    if (Actor_HasParent(&self->actor, play)) {
        self->actionFunc = EnGe1_SetupWait_Archery;
        if (self->stateFlags & GE1_STATE_GIVE_QUIVER) {
            *(u16*)&gSaveContext_F00[0x8] |= 0x8000;
        } else {
            *(u16*)&gSaveContext_F00[0x42] |= 1;
        }
    } else {
        if (!(self->stateFlags & GE1_STATE_GIVE_QUIVER)) {
            getItemId = 0x3E;
        } else {
            s32 upg = CUR_UPG_VALUE(UPG_QUIVER);

            if (upg == 1) {
                getItemId = 0x30;
            } else if (upg == 2) {
                getItemId = 0x31;
            }
        }
        GiveItem(&self->actor, play, getItemId, 10000.0f, 50.0f);
    }
}

void EnGe1_WaitUntilGateOpened_GateOp(EnGe1* self, PlayState* play) {
    if (self->cutsceneTimer > 0) {
        self->cutsceneTimer--;
    } else {
        EnGe1_SetAnimationIdle(self);
        self->actionFunc = EnGe1_CheckGate_GateOp;
    }
    self->stateFlags |= GE1_STATE_STOP_FIDGET;
}
GLOBAL_ASM("../split/asm/FUN_0010fd24.s")

#if 0 // draft: 22 words off (vldr 100.0f hoisted to entry, ROM loads it per arm)
static void EnGe1_Wait_TalkArm(EnGe1* self, PlayState* play, u16 textId, EnGe1ActionFunc actionFunc) {
    EnGe1_SetTalkAction(self, play, textId, 100.0f, actionFunc);
}

void EnGe1_Wait_Archery(EnGe1* self, PlayState* play) {
    if (*(u32*)((u8*)GET_PLAYER(play) + 0x1710) & 0x800000) {
        if (*(u16*)&gSaveContext_E00[0xF8] & 0x100) {
            if (*(u16*)&gSaveContext_F00[0x42] & 1) {
                EnGe1_Wait_TalkArm(self, play, 0x6042, EnGe1_TalkOfferPlay_Archery);
            } else {
                EnGe1_Wait_TalkArm(self, play, 0x6043, EnGe1_TalkOfferPlay_Archery);
            }
        } else {
            EnGe1_Wait_TalkArm(self, play, 0x6040, EnGe1_TalkOfferPlay_Archery);
        }
    } else {
        f32 dist = 100.0f;
        EnGe1_SetTalkAction(self, play, 0x603F, dist, EnGe1_TalkNoHorse_Archery);
    }
}
#endif

GLOBAL_ASM("asm/EnGe1_Wait_Archery.s")

static void EnGe1_SpotPlayer(EnGe1* self, PlayState* play) {
    self->cutsceneTimer = 45;
    self->actionFunc = EnGe1_KickPlayer;
    FUN_0036e980(play, &self->actor, 0x5F);
    Audio_PlaySoundGeneral(0x010004B1, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    FUN_00367c7c(play, 0x6000, &self->actor);
}

void EnGe1_WatchForAndSensePlayer(EnGe1* self, PlayState* play) {
    s16 angleDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    if ((self->actor.xzDistToPlayer < 50.0f) || ((ABS(angleDiff) <= 0x4300) && (self->actor.xzDistToPlayer < 400.0f))) {
        EnGe1_SpotPlayer(self, play);
    }

    if (((ColliderCylinder*)((u8*)self + 0x1A4))->base.acFlags & AC_HIT) {
        EnGe1_SpotPlayer(self, play);
    }

    {
        PlayState* play2 = play;
        CollisionCheck_SetAC(play, &play2->colChkCtx, &((ColliderCylinder*)((u8*)self + 0x1A4))->base);
    }
}


void EnGe1_WatchForPlayerFrontOnly(EnGe1* self, PlayState* play) {
    s16 angleDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    if ((ABS(angleDiff) <= 0x4300) && (self->actor.xzDistToPlayer < 100.0f)) {
        EnGe1_SpotPlayer(self, play);
    }

    if (((ColliderCylinder*)((u8*)self + 0x1A4))->base.acFlags & AC_HIT) {
        EnGe1_SpotPlayer(self, play);
    }

    {
        PlayState* play2 = play;
        CollisionCheck_SetAC(play, &play2->colChkCtx, &((ColliderCylinder*)((u8*)self + 0x1A4))->base);
    }
}

}
