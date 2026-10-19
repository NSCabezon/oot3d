extern "C" {
#include "z_en_js.hpp"

#define FLAGS 0x00000009

void EnJs_Init(EnJs* self, PlayState* play);
void EnJs_Destroy(Actor* thisx, PlayState* play);
void EnJs_Update(Actor* actor, GameState* state);
void EnJs_Draw(Actor* thisx, PlayState* play);
s32 EnJs_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnJs_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);

void FUN_0034fe20(Actor* actor, PlayState* play, SkelAnime* skelAnime, s32 cmbIndex, s32 csabIndex, void* jointTable,
                  void* morphTable, s32 limbCount);
void Animation_PlayOnce(SkelAnime* skelAnime, s32 idx);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
u32 FUN_00369a48(Actor* actor, PlayState* play); // Actor_TextboxIsClosing
void Rupees_ChangeBy(s16 rupeeChange);
f32 __hardfp_sinf(f32);
f32 __hardfp_cosf(f32);
void FUN_003f0a08(EnJs* self, PlayState* play); // func_80A89304
void FUN_003f9450(EnJs* self, PlayState* play); // func_80A891C4
void FUN_003ee2ec(EnJs* self, PlayState* play); // func_80A89160
void FUN_003f7804(EnJs* self, PlayState* play); // func_80A8910C
void FUN_001111a4(EnJs* self, PlayState* play); // func_80A890C0
void FUN_003c4d6c(EnJs* self, PlayState* play); // func_80A89078


ActorInit En_Js_InitVars = {
    ACTOR_EN_JS,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_JS,
    sizeof(EnJs),
    (ActorFunc)EnJs_Init,
    (ActorFunc)EnJs_Destroy,
    (ActorFunc)EnJs_Update,
    (ActorFunc)EnJs_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    { 0x0A, 0x00, 0x11, 0x39, 0x10, 0x01 },
    { 0, { 0x00000000, 0, 0 }, { 0xFFCFFFFF, 0, 0 }, 0, 1, 1 },
    { 30.0f, 40.0f, 0.0f, { 0.0f, 0.0f, 0.0f } },
};

void EnJs_Init(EnJs* self, PlayState* play) {
    FUN_00372f38(&self->actor, play, NULL);
    self->actor.targetArrowOffset = 1000.0f;
    ActorShape_Init(&self->actor.shape, 0.0f, NULL, 36.0f);
    FUN_0034fe20(&self->actor, play, &self->skelAnime, 0, 1, self->jointTable, self->morphTable, 11);
    Animation_PlayOnce(&self->skelAnime, 1);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &sCylinderInit);
    self->actor.colChkInfo.mass = 0xFF;
    Actor_SetScale(&self->actor, 0.01f);
    self->actionFunc = FUN_003f0a08;
    self->unk_284 = 0;
    self->actor.gravity = -1.0f;
    Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, 0x16B, self->actor.world.pos.x, self->actor.world.pos.y,
                       self->actor.world.pos.z, 0, 0, 0, 0);
}

void EnJs_Destroy(Actor* thisx, PlayState* play) {
    EnJs* self = (EnJs*)thisx;

    SkelAnime_Free2(&self->skelAnime);
    FUN_003508b8(&self->actor, NULL);
    Collider_DestroyCylinder(play, &self->collider);
}

GLOBAL_ASM("asm/EnJs_Update.s")

void EnJs_Draw(Actor* thisx, PlayState* play) {
    EnJs* self = (EnJs*)thisx;

    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnJs_OverrideLimbDraw, EnJs_PostLimbDraw, &self->actor, 0);
}

s32 EnJs_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnJs* self = (EnJs*)thisx;

    if (limbIndex == 8) {
        f32 angle = self->unk_278.y * 9.58738e-5f;

        if (angle != 0.0f) {
            f32 s = __hardfp_sinf(angle);
            f32 c = __hardfp_cosf(angle);
            s32 i;

            for (i = 0; i < 3; i++) {
                f32 a = mtx->data[i][1];
                f32 b = mtx->data[i][2];
                mtx->data[i][1] = a * c + b * s;
                mtx->data[i][2] = b * c - a * s;
            }
        }
    }
    return 0;
}

// Function-local static: guard 0x00528988 (TU .data, cut from the data chunk), object 0x0059B068 pinned in
// data/oot3d.ld (scripts/pin_static.py).
void EnJs_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    static Vector3f sMultVec(0.0f, 0.0f, 0.0f);
    EnJs* self = (EnJs*)thisx;
    Vector3f* dst;

    if (limbIndex == 8) {
        dst = (Vector3f*)&self->actor.focus.pos;
        VEC3TransformAsm(dst, mtx, &sMultVec);
    }
}


static inline void En_Js_SetupAction(EnJs* self, EnJsActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

static inline void func_80A89008(EnJs* self) {
    En_Js_SetupAction(self, FUN_003f0a08);
    Animation_Change_00375c08(&self->skelAnime, 1, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 1), 2, -4.0f);
}

// func_80A89078
void FUN_003c4d6c(EnJs* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        func_80A89008(self);
        self->actor.flags &= ~0x10000;
    }
}

// func_80A890C0
void FUN_001111a4(EnJs* self, PlayState* play) {
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        En_Js_SetupAction(self, FUN_003c4d6c);
    } else {
        FUN_0036bb28(&self->actor, play, 1000.0f);
    }
}

// func_80A8910C
void FUN_003f7804(EnJs* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        self->actor.textId = 0x6078;
        En_Js_SetupAction(self, FUN_001111a4);
        self->actor.flags |= 0x10000;
    }
}

// func_80A89160
void FUN_003ee2ec(EnJs* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        self->actor.parent = NULL;
        En_Js_SetupAction(self, FUN_003f7804);
    } else {
        GiveItem(&self->actor, play, 3, 10000.0f, 50.0f);
    }
}

// func_80A891C4
void FUN_003f9450(EnJs* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 4 && Message_ShouldAdvance(play)) {
        switch (FUN_00369f3c(play)) {
            case 0:
                if (gSaveContext.rupees < 200) {
                    Message_ContinueTextbox(play, 0x6075);
                    func_80A89008(self);
                } else {
                    Rupees_ChangeBy(-200);
                    En_Js_SetupAction(self, FUN_003ee2ec);
                }
                break;
            case 1:
                Message_ContinueTextbox(play, 0x6074);
                func_80A89008(self);
        }
    }
}

static inline u8 func_80A88F64(EnJs* self, PlayState* play, u16 textId) {
    s16 yawDiff;

    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        return 1;
    } else {
        self->actor.textId = textId;
        yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

        if (ABS(yawDiff) <= 0x1800 && self->actor.xzDistToPlayer < 100.0f) {
            self->unk_284 |= 1;
            FUN_0036bb28(&self->actor, play, 100.0f);
        }
        return 0;
    }
}

static inline void func_80A89294(EnJs* self) {
    En_Js_SetupAction(self, FUN_003f9450);
    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 0), 2, -4.0f);
}

// func_80A89304
void FUN_003f0a08(EnJs* self, PlayState* play) {
    s16 yawDiff;
    Actor* thisx = &self->actor;

    if (Actor_ProcessTalkRequest(thisx, play)) {
        func_80A89294(self);
    } else {
        thisx->textId = 0x6077;
        yawDiff = thisx->yawTowardsPlayer - thisx->shape.rot.y;
        if (ABS(yawDiff) <= 0x1800 && thisx->xzDistToPlayer < 100.0f) {
            self->unk_284 |= 1;
            FUN_0036bb28(thisx, play, 100.0f);
        }
    }
}
}
