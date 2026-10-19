extern "C" {
#include "z_en_zl1.hpp"

#define FLAGS 0x00000019

void EnZl1_Init(Actor* actor, GameState* state);
void EnZl1_Destroy(Actor* actor, GameState* state);
void EnZl1_Update(Actor* actor, GameState* state);
void EnZl1_Draw(Actor* actor, GameState* state);

s32 FUN_0036ae18(SkelAnime* skelAnime, s32 index);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);

GLOBAL_ASM("../src/game/actors/En_Zl1/data_syms.s")

ActorInit En_Zl1_InitVars = {
    ACTOR_EN_ZL1,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_ZL1,
    sizeof(EnZl1),
    (ActorFunc)EnZl1_Init,
    (ActorFunc)EnZl1_Destroy,
    (ActorFunc)EnZl1_Update,
    (ActorFunc)EnZl1_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    { 0x00, 0x00, 0x00, 0x39, 0x20, 0x01 },
    { 1, { 0x00000000, 0, 0 }, { 0x00000000, 0, 0 }, 0, 0, 1 },
    { 20.0f, 46.0f, 0.0f, { 0.0f, 0.0f, 0.0f } },
};

void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skelAnime, void* cmb, void* p, s32 a, s32 b, s32 c, s32 d);
void FUN_003adfac(EnZl1* self, PlayState* play);
void FUN_003ad010(EnZl1* self, PlayState* play);
void FUN_001610c4(EnZl1* self, PlayState* play);

void EnZl1_Init(Actor* actor, GameState* state) {
    EnZl1* self = (EnZl1*)actor;
    PlayState* play = (PlayState*)state;
    void* zar;
    SkelAnime* skel;
    void* cmb;
    void* p;
    f32 frameCount;

    actor->unk_19A = 1;
    zar = (u8*)play->GetObjectStatus(actor->objBankIndex) + 0x10;
    skel = &self->skelAnime;
    cmb = ZAR_GetCMBByIndex(zar, 0);
    p = actor->unk_178;
    FUN_00353e78(zar, play, skel, cmb, p, -1, 0, 0, 0);
    frameCount = FUN_0036ae18(&self->skelAnime, 5);
    Animation_Change_00375c08(&self->skelAnime, 5, 1.0f, 0.0f, frameCount, 0, 0.0f);
    SkelAnime_SetFaceAnimations(&self->faceAnims, &self->skelAnime, 0, 2, 1);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, actor, &sCylinderInit);
    Actor_SetScale(actor, 0.01f);
    ActorShape_Init(&actor->shape, 0.0f, ActorShadow_DrawCircle, 24.0f);
    actor->targetMode = 0;

    if (gSaveContext.sceneSetupIndex >= 4) {
        frameCount = FUN_0036ae18(&self->skelAnime, 0);
        Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, frameCount, 0, 0.0f);
        self->unk_456 = 0;
        self->actionFunc = FUN_001610c4;
    } else if (Flags_GetEventChkInf(9) && Flags_GetEventChkInf(0x25) && Flags_GetEventChkInf(0x37)) {
        Actor_Kill(actor);
    } else if ((Flags_GetEventChkInf(9) && Flags_GetEventChkInf(0x25)) ||
               (Flags_GetEventChkInf(9) && Flags_GetEventChkInf(0x37))) {
        frameCount = FUN_0036ae18(&self->skelAnime, 0);
        Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, frameCount, 0, 0.0f);
        actor->textId = 0x703D;
        self->actionFunc = FUN_003adfac;
    } else if (Flags_GetEventChkInf(0x40)) {
        frameCount = FUN_0036ae18(&self->skelAnime, 0);
        Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, frameCount, 0, 0.0f);
        actor->textId = 0x703C;
        self->actionFunc = FUN_003adfac;
    } else {
        actor->textId = 0xFFFF;
        self->actionFunc = FUN_003ad010;
    }
}

void FUN_0034f558(SkelAnime* skel);

void EnZl1_Destroy(Actor* actor, GameState* state) {
    EnZl1* self = (EnZl1*)actor;
    PlayState* play = (PlayState*)state;

    FUN_0034f558(&self->skelAnime);
    self->faceAnims.Destroy();
    Collider_DestroyCylinder(play, &self->collider);
}

void Matrix_RotateX(MTX34* mtx, s32 mode, f32 angle);
void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);
void Matrix_RotateZ(MTX34* mtx, s32 mode, f32 angle);

#define BINANG_RAD(x) ((f32)(x) * (1.0f / 32768.0f) * 3.14159265f)

s32 EnZl1_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, Actor* actor) {
    EnZl1* self = (EnZl1*)actor;

    if (limbIndex != 0xD) {
        if (limbIndex == 0xE) {
            Matrix_RotateX(mtx, 1, BINANG_RAD(self->unk_466.y));
            Matrix_RotateY(mtx, 1, BINANG_RAD(self->unk_466.z));
            Matrix_RotateZ(mtx, 1, BINANG_RAD(self->unk_466.x));
        }
    } else {
        Matrix_RotateX(mtx, 1, BINANG_RAD(self->unk_46C.y));
        Matrix_RotateY(mtx, 1, BINANG_RAD(self->unk_46C.z));
        Matrix_RotateZ(mtx, 1, BINANG_RAD(self->unk_46C.x));
    }
    return 0;
}

static inline void EnZl1_SetVec(Vec3f* v, f32 x, f32 y, f32 z) {
    v->x = x; v->y = y; v->z = z;
}
void EnZl1_PostDrawLimb(PlayState* play, s32 limbIndex, MTX34* mtx, Actor* actor) {
    if (limbIndex == 0xE) {
        Vector3f* dst = (Vector3f*)&actor->focus;
        Vec3f vec;
        EnZl1_SetVec(&vec, 0.0f, 0.0f, 0.0f);
        VEC3TransformAsm(dst, mtx, (Vector3f*)&vec);
    }
}


void FUN_003ac060(EnZl1* self, PlayState* play);
void FUN_001610c4(EnZl1* self, PlayState* play);
extern u8 EnZl1_sEyeIdxTable[];

s32 FUN_0036ae18(SkelAnime* skelAnime, s32 index);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_0036f59c(Player* player, s32 sfxId);
void FUN_003f7848(EnZl1* self, PlayState* play);

void FUN_001467dc(EnZl1* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 frameCount = FUN_0036ae18(&self->skelAnime, 0);

    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, frameCount, 0, 0.0f);
    FUN_0036e980(play, &self->actor, 1);
    FUN_0036f59c(player, 0x01000521);
    self->actor.textId = 0x7039;
    FUN_00367c7c(play, self->actor.textId, NULL);
    self->unk_452 = 0;
    self->actionFunc = FUN_003f7848;
}

void FUN_0036bcc8(PlayState* play, Actor* actor, Vec3s* headRot, Vec3s* torsoRot, Vec3f focusPos, s32 yawLimit);
s32 FUN_00369a48(Actor* actor, PlayState* play);
void FUN_00363cb8(Actor* actor, PlayState* play);

void FUN_003adfac(EnZl1* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    FUN_0036bcc8(play, &self->actor, &self->unk_466, &self->unk_46C, self->actor.focus.pos, 0x4300);

    if (self->unk_456 != 0) {
        if (FUN_00369a48(&self->actor, play) != 0) {
            self->unk_456 = 0;
        }
    } else if (Actor_ProcessTalkRequest(&self->actor, play) != 0) {
        self->unk_456 = 1;
    } else if (player->actor.world.pos.y >= self->actor.world.pos.y) {
        FUN_00363cb8(&self->actor, play);
    }

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
}

void FUN_003ad41c(EnZl1* self, PlayState* play);
void FUN_00317b28(PlayState* play, s16 camId, Player* player, s32 setting);
void FUN_00354220(PlayState* play, s16 camId, f32 fov);
void ShrinkWindow_SetVal(void* shrinkWindow);
void Interface_ChangeAlpha(s32 mode);
void FUN_0035c528(u32 seqId);
extern void* gUnk_00549600;

// FUN_003ad010 (N64 func_80B4B010): draft 28 words off (self in r4/base r5 vs ROM r5/r4, VFP numbering of the 3 Vec3f inits,
// 30.0f hoisted into s17 in ROM). ~45 variants tried (aliases, SetVec helpers, Vector3f ctor).
#if 0
void FUN_003ad010(EnZl1* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vector3f subCamAt(-460.0f, 118.0f, 0.0f);
    Vector3f subCamEye(-406.0f, 110.0f, 0.0f);
    Vector3f playerPos(-398.0f, 84.0f, 0.0f);
    s16 rotDiff;
    EnZl1* s2 = self;
    f32 fov = 30.0f;

    if (Actor_ProcessTalkRequest(&s2->actor, play) != 0) {
        Animation_Change_00375c08(&self->skelAnime, 1, 1.0f, 0.0f, FUN_0036ae18(&self->skelAnime, 1), 3, -10.0f);
        self->subCamId = Gameplay_CreateSubCamera(play);
        Gameplay_ChangeCameraStatus(play, 0, 1);
        Gameplay_ChangeCameraStatus(play, self->subCamId, 7);
        FUN_00317b28(play, self->subCamId, player, 0x21);
        *((u8*)play + 0x3262) = 255;
        *((u8*)play + 0x3263) = 255;
        *((u8*)play + 0x3264) = 255;
        *((u8*)play + 0x3265) = 24;
        *((u8*)play + 0x3261) = 1;
        Gameplay_CameraSetAtEye(play, self->subCamId, (Vec3f*)&subCamAt, (Vec3f*)&subCamEye);
        FUN_00354220(play, self->subCamId, fov);
        ShrinkWindow_SetVal(gUnk_00549600);
        Interface_ChangeAlpha(2);
        player->actor.world.pos = *(Vec3f*)&playerPos;
        player->actor.speedXZ = 0.0f;
        self->unk_452 = 0;
        self->actionFunc = FUN_003ad41c;
        FUN_0035c528(0x010005BE);
    } else {
        rotDiff = ABS(self->actor.yawTowardsPlayer - self->actor.shape.rot.y);
        if ((rotDiff < 0x238E) && !(player->actor.world.pos.y < self->actor.world.pos.y)) {
            FUN_00363cb8(&self->actor, play);
        }
    }
}
#endif

u8 Message_GetState(MessageContext* msgCtx);
u8 Message_ShouldAdvance(PlayState* play);
void Message_ContinueTextbox(PlayState* play, u16 textId);
s32 FUN_00369f3c(PlayState* play);
void FUN_00371680(PlayState* play, s32 arg, s32 arg2);
void FUN_00338654(PlayState* play, s32 camId, s32 camIdNone);
void FUN_0036963c(PlayState* play, s16 camId);
void FUN_0034be04(s32 mode);

// FUN_003f7848 (N64 func_80B4BF2C; whole function is DCB in the asm: jump-table switch). Draft ~146 words off: structure
// matches (switch table, 7 cases, fallthrough 1->2) but ROM cross-jumps ALL `strh r0,[r4,#0x52]` into one block before the
// default tail and loads `ldrsh unk_452` before the player; we cross-jump only some and load player first.
// ~15 variants (GameState param + GET_PLAYER, player in case, helpers) did not change it. Switch var s16 unk_452 at +0x452.
#if 0
void FUN_003f7848(EnZl1* self, GameState* state) {
    Player* player = GET_PLAYER((PlayState*)state);
    PlayState* play = (PlayState*)state;
    MessageContext* msgCtx = &play->msgCtx;

    switch (self->unk_452) {
        case 0:
            if ((Message_GetState(msgCtx) == 4) && Message_ShouldAdvance(play)) {
                if (FUN_00369f3c(play) == 0) {
                    self->actor.textId = 0x703B;
                    Message_ContinueTextbox(play, self->actor.textId);
                    self->unk_452++;
                } else {
                    self->actor.textId = 0x703A;
                    Message_ContinueTextbox(play, self->actor.textId);
                    self->unk_452 = 0;
                }
            }
            break;
        case 1:
            if ((Message_GetState(msgCtx) == 5) && Message_ShouldAdvance(play)) {
                self->actor.textId = 0xFFFF;
                ((void (*)(PlayState*, Actor*))(*(u32*)((u8*)play + 0x5BB0)))(play, &self->actor);
                GiveItem(&self->actor, play, 0xB, 120.0f, 10.0f);
                FUN_00371680(play, 4, 0);
                self->unk_452++;
            } else {
                break;
            }
        case 2:
            if (Actor_HasParent(&self->actor, play) != 0) {
                FUN_00338654(play, 0, self->subCamId);
                Gameplay_ChangeCameraStatus(play, 0, 7);
                FUN_0036963c(play, self->subCamId);
                self->actor.parent = NULL;
                self->unk_452++;
            } else {
                GiveItem(&self->actor, play, 0xB, 120.0f, 10.0f);
            }
            break;
        case 3:
            if ((Message_GetState(msgCtx) == 6) && Message_ShouldAdvance(play)) {
                self->actor.textId = 0x703C;
                Message_ContinueTextbox(play, self->actor.textId);
                Flags_SetEventChkInf(0x40);
                self->unk_452 = 6;
            }
            break;
        case 4:
            if (player->actor.world.pos.y < self->actor.world.pos.y) {
                break;
            } else {
                if (Actor_ProcessTalkRequest(&self->actor, play) != 0) {
                    self->unk_452++;
                } else {
                    FUN_00363cb8(&self->actor, play);
                }
            }
            break;
        case 5:
            if (FUN_00369a48(&self->actor, play) != 0) {
                self->unk_452--;
            }
            break;
        case 6:
            if (FUN_00369a48(&self->actor, play) != 0) {
                FUN_0036e980(play, &self->actor, 7);
                FUN_0034be04(0x32);
                self->actor.flags &= ~0x100;
                self->unk_452 = 4;
            }
            break;
    }
    FUN_0036bcc8(play, &self->actor, &self->unk_466, &self->unk_46C, self->actor.focus.pos, 0x4300);
}
#endif

void FUN_00367c7c(PlayState* play, u16 textId, Actor* actor);

// FUN_003ad41c (N64 func_80B4B240). Whole function is DCB in the asm (10-case jump table). Draft.
// FUN_003ad41c (N64 func_80B4B240): the whole function is a DCB blob (10-case jump table) in the asm. Draft ~266-300 words off:
// case structure and constants match but we miss the hoisted 25.0f (ROM vpush {d8}/s16), the shared `strh unk_452` tails and
// reg numbering. ~12 variants. Class: big.
#if 0
void FUN_003ad41c(EnZl1* self, PlayState* play) {
    Vector3f subCamAt(-427.0f, 108.0f, 26.0f);
    Vector3f subCamEye(-340.0f, 108.0f, 98.0f);
    f32 fov = 25.0f;
    Vector3f sp58(-434.0f, 84.0f, 0.0f);
    u8 sp54[] = { 0, 0, 2 };
    Player* player = GET_PLAYER(play);
    MessageContext* msgCtx = &play->msgCtx;
    s32 animIdx = -1;
    s32 sp3C = 0;
    f32 frameCount;

    switch (self->unk_452) {
        case 0:
            switch ((s16)self->skelAnime.curFrame) {
                case 14:
                    self->unk_454 = 0;
                    break;
                case 15:
                    if (DECR(self->unk_454) != 0) {
                        self->skelAnime.curFrame = 15.0f;
                    }
                    break;
                case 64:
                    animIdx = 2;
                    sp3C = 1;
                    self->actor.textId = 0x702E;
                    FUN_00367c7c(play, self->actor.textId, NULL);
                    self->unk_452++;
                    break;
            }
            break;
        case 1:
            if ((Message_GetState(msgCtx) == 5) && Message_ShouldAdvance(play)) {
                *((u8*)play + 0x3261) = 0;
                Gameplay_CameraSetAtEye(play, self->subCamId, (Vec3f*)&subCamAt, (Vec3f*)&subCamEye);
                FUN_00354220(play, self->subCamId, fov);
                player->actor.world.pos = *(Vec3f*)&sp58;
                self->actor.textId = 0x702F;
                Message_ContinueTextbox(play, self->actor.textId);
                self->unk_452++;
            }
            break;
        case 2:
            if ((Message_GetState(msgCtx) == 4) && Message_ShouldAdvance(play)) {
                if (FUN_00369f3c(play) == 0) {
                    animIdx = 10;
                    sp3C = 2;
                    self->unk_452++;
                } else {
                    animIdx = 3;
                    sp3C = 2;
                    self->unk_452 = 6;
                }
            }
            break;
        case 3:
            frameCount = FUN_0036ae18(&self->skelAnime, 10);
            if (self->skelAnime.curFrame == frameCount) {
                animIdx = 11;
                sp3C = 1;
                self->actor.textId = 0x7032;
                Message_ContinueTextbox(play, self->actor.textId);
                self->unk_452++;
            }
            break;
        case 4:
            if ((Message_GetState(msgCtx) == 4) && Message_ShouldAdvance(play)) {
                if (FUN_00369f3c(play) == 0) {
                    animIdx = 8;
                    sp3C = 2;
                    self->unk_452 = 9;
                } else {
                    self->actor.textId = 0x7034;
                    Message_ContinueTextbox(play, self->actor.textId);
                    self->unk_452++;
                }
            }
            break;
        case 5:
            if ((Message_GetState(msgCtx) == 5) && Message_ShouldAdvance(play)) {
                self->actor.textId = 0x7033;
                Message_ContinueTextbox(play, self->actor.textId);
                self->unk_452--;
            }
            break;
        case 6:
            frameCount = FUN_0036ae18(&self->skelAnime, 3);
            if (self->skelAnime.curFrame == frameCount) {
                animIdx = 4;
                sp3C = 1;
                self->actor.textId = 0x7031;
                Message_ContinueTextbox(play, self->actor.textId);
                self->unk_452++;
            }
            break;
        case 7:
            if ((Message_GetState(msgCtx) == 5) && Message_ShouldAdvance(play)) {
                self->actor.textId = 0x7030;
                Message_ContinueTextbox(play, self->actor.textId);
                self->unk_452++;
            }
            break;
        case 8:
            if ((Message_GetState(msgCtx) == 4) && Message_ShouldAdvance(play)) {
                if (FUN_00369f3c(play) == 0) {
                    animIdx = 9;
                    sp3C = 2;
                    self->unk_452 = 3;
                } else {
                    self->actor.textId = 0x7031;
                    Message_ContinueTextbox(play, self->actor.textId);
                    self->unk_452--;
                }
            }
            break;
        case 9:
            frameCount = FUN_0036ae18(&self->skelAnime, 8);
            if (self->skelAnime.curFrame == frameCount) {
                animIdx = 0;
                sp3C = 1;
                play->FUN_0037573c(((ZARInfo*)((u8*)play->GetObjectStatus(self->actor.objBankIndex) + 0x10))->GetQDBByIndex(0));
                gSaveContext.cutsceneTrigger = 1;
                self->actionFunc = FUN_003ac060;
                self->unk_452++;
            }
            break;
    }
    if (sp3C != 0) {
        frameCount = FUN_0036ae18(&self->skelAnime, animIdx);
        Animation_Change_00375c08(&self->skelAnime, animIdx, 1.0f, 0.0f, frameCount, sp54[sp3C], -10.0f);
    }
    FUN_0036bcc8(play, &self->actor, &self->unk_466, &self->unk_46C, self->actor.focus.pos, 0x4300);
}
#endif

void AnimationContext_SetMoveActor(PlayState* play, Actor* actor, SkelAnime* skelAnime, f32 speed);
extern s8 EnZl1_sCueAnimIdx[];

static inline void EnZl1_MoveActor(EnZl1* self, PlayState* play) {
    *((u8*)&self->skelAnime + 0x52) |= 1;
    AnimationContext_SetMoveActor(play, &self->actor, &self->skelAnime, 1.0f);
}

static inline void EnZl1_GetCueStartPos(CsCmdActorAction* cue, Vec3f* dest) {
    dest->x = cue->startPos.x;
    dest->y = cue->startPos.y;
    dest->z = cue->startPos.z;
}

static inline void EnZl1_GetCueEndPos(CsCmdActorAction* cue, Vec3f* dest) {
    dest->x = cue->endPos.x;
    dest->y = cue->endPos.y;
    dest->z = cue->endPos.z;
}

extern s8 EnZl1_sCueAnimIdx2[];

// FUN_003ac060 (N64 func_80B4B8B4): draft ~55 words off (same self/cue r4/r5 swap as FUN_001610c4 + VFP numbering of the
// hoisted 70.0f/0.0f/1.0f constants).
#if 0
void FUN_003ac060(EnZl1* self, PlayState* play) {
    u8 modes[] = { 0, 0, 0, 0, 0, 0, 0, 2, 0, 2 };
    Vector3f subCamAt(-421.0f, 143.0f, -5.0f);
    Vector3f subCamEye(-512.0f, 105.0f, -4.0f);
    f32 fov = 70.0f;
    f32 cueDuration;
    CsCmdActorAction* cue;
    Vec3f sp74;
    Vec3f sp68;
    Vector3f velocity(0.0f, 0.0f, 0.0f);
    f32 frameCount;

    FUN_003731e0(&self->skelAnime);
    EnZl1_MoveActor(self, play);

    if (play->GetCutsceneState() == 0) {
        self->actionFunc = FUN_001467dc;
        return;
    }

    cue = play->csCtx.npcActions[0];

    if (cue != NULL) {
        EnZl1_GetCueStartPos(cue, &sp74);
        EnZl1_GetCueEndPos(cue, &sp68);

        if (self->unk_456 == 0) {
            self->actor.world.pos = self->actor.home.pos = sp74;
        }

        if (self->unk_456 != cue->action) {
            frameCount = FUN_0036ae18(&self->skelAnime, EnZl1_sCueAnimIdx2[cue->action]);
            Animation_Change_00375c08(&self->skelAnime, EnZl1_sCueAnimIdx2[cue->action], 1.0f, 0.0f, frameCount,
                                      modes[cue->action], -10.0f);
            self->unk_456 = cue->action;
        }

        self->actor.velocity = *(Vec3f*)&velocity;

        if (play->csCtx.frames < cue->endFrame) {
            cueDuration = cue->endFrame - cue->startFrame;

            self->actor.velocity.x = (sp68.x - sp74.x) / cueDuration;
            self->actor.velocity.y = (sp68.y - sp74.y) / cueDuration;
            self->actor.velocity.y += self->actor.gravity;

            if (self->actor.velocity.y < self->actor.minVelocityY) {
                self->actor.velocity.y = self->actor.minVelocityY;
            }
            self->actor.velocity.z = (sp68.z - sp74.z) / cueDuration;
        }

        FUN_0036bcc8(play, &self->actor, &self->unk_466, &self->unk_46C, self->actor.focus.pos, 0x4300);
        Gameplay_CameraSetAtEye(play, self->subCamId, (Vec3f*)&subCamAt, (Vec3f*)&subCamEye);
        FUN_00354220(play, self->subCamId, fov);
    }
}
#endif

// FUN_001610c4 (N64 func_80B4BC78): draft 41 words off. Layout/constants match, but ROM keeps `cue` in r4 and `self` in r5
// (we swap them; ~25 variants incl. dead aliases). Start pos stays on the stack via the N64 helper (address taken).
#if 0
void FUN_001610c4(EnZl1* self, PlayState* play) {
    u8 modes[] = { 0, 0, 0, 0, 0, 0, 0, 2, 0, 2 };
    f32 cueDuration;
    Vec3f sp70;
    Vec3f sp64;
    Vector3f velocity(0.0f, 0.0f, 0.0f);
    CsCmdActorAction* cue;
    f32 frameCount;
    EnZl1* s2 = self;

    if (FUN_00370734(&self->skelAnime) && (self->skelAnime.animationType == 1)) {
        frameCount = FUN_0036ae18(&self->skelAnime, 2);
        Animation_Change_00375c08(&self->skelAnime, 2, 1.0f, 0.0f, frameCount, 0, -10.0f);
    }

    EnZl1_MoveActor(self, play);

    cue = play->csCtx.npcActions[0];

    if (cue != NULL) {
        EnZl1_GetCueStartPos(cue, &sp70);
        EnZl1_GetCueEndPos(cue, &sp64);

        if (self->unk_456 == 0) {
            self->actor.world.pos = self->actor.home.pos = sp70;
        }

        if (self->unk_456 != cue->action) {
            frameCount = FUN_0036ae18(&self->skelAnime, EnZl1_sCueAnimIdx[cue->action]);
            Animation_Change_00375c08(&s2->skelAnime, EnZl1_sCueAnimIdx[cue->action], 1.0f, 0.0f, frameCount,
                                      modes[cue->action], -10.0f);
            self->unk_456 = cue->action;
        }

        self->actor.velocity = *(Vec3f*)&velocity;

        if (play->csCtx.frames < cue->endFrame) {
            cueDuration = cue->endFrame - cue->startFrame;

            self->actor.velocity.x = (sp64.x - sp70.x) / cueDuration;
            self->actor.velocity.y = (sp64.y - sp70.y) / cueDuration;
            self->actor.velocity.y += self->actor.gravity;

            if (self->actor.velocity.y < self->actor.minVelocityY) {
                self->actor.velocity.y = self->actor.minVelocityY;
            }
            self->actor.velocity.z = (sp64.z - sp70.z) / cueDuration;
        }
    }
}
#endif

static inline void EnZl1_UpdateEyes(EnZl1* self) {
    if (self->skelAnime.animationType == 1 && self->skelAnime.curFrame < 26.0f) {
        self->eyeIdx1 = 7;
        self->eyeIdx2 = 6;
        self->blinkTimer = 2;
    } else {
        if (DECR(self->blinkTimer) == 0) {
            self->blinkTimer = Rand_S16Offset_003702c8(0x1E, 0xA);
        }
        if (self->blinkTimer <= 3) {
            self->blinkIdx = self->blinkTimer;
        } else {
            self->blinkIdx = 0;
        }
        self->eyeIdx1 = EnZl1_sEyeIdxTable[self->blinkIdx];
        self->eyeIdx2 = EnZl1_sEyeIdxTable[self->blinkIdx];
        self->mouthIdx = 0;
    }
}

void EnZl1_Update(Actor* actor, GameState* state) {
    EnZl1* self = (EnZl1*)actor;
    PlayState* play = (PlayState*)state;

    if ((self->actionFunc != FUN_003ac060) && (self->actionFunc != FUN_001610c4)) {
        FUN_00370734(&self->skelAnime);
    }
    Actor_UpdateBgCheckInfo(play, actor, 0.0f, 0.0f, 0.0f, 5);
    self->actionFunc(self, play);
    if (self->actionFunc != FUN_003ac060) {
        Collider_UpdateCylinder(actor, &self->collider);
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    }
    Math_SmoothStepToS(&actor->shape.rot.x, actor->world.rot.x, 10, 1000, 1);
    Math_SmoothStepToS(&actor->shape.rot.y, actor->world.rot.y, 10, 1000, 1);
    Math_SmoothStepToS(&actor->shape.rot.z, actor->world.rot.z, 10, 1000, 1);
    EnZl1_UpdateEyes(self);
}

void EnZl1_Draw(Actor* actor, GameState* state) {
    EnZl1* self = (EnZl1*)actor;
    FaceAnimations* f0 = &((EnZl1*)actor)->faceAnims;
    f0->FUN_0035e3a4(0, self->eyeIdx2);
    {
        u8 i = self->eyeIdx1;
        FaceAnimations* fa = &self->faceAnims;
        fa->FUN_0035e3a4(2, i);
    }
    {
        u8 i = self->mouthIdx;
        FaceAnimations* fa = &self->faceAnims;
        fa->FUN_0035e3a4(1, i);
    }
    self->faceAnims.FUN_0035e330();
    SkelAnime_DrawOpa(&self->skelAnime, &actor->modelMtx, (OverrideLimbDrawOpa)EnZl1_OverrideLimbDraw,
                      (PostLimbDrawOpa)EnZl1_PostDrawLimb, actor, 0);
}

}
