extern "C" {
#include "z_demo_shd.hpp"

#define FLAGS 0x00000030
#define R_UPDATE_RATE SREG(30)
#define FRAMES(n) ((n) * 3.0f / R_UPDATE_RATE + 0.5f)

extern s32 gUnk_0054C8CC; // src/game/shared_syms.s

void DemoShd_Init(Actor* thisx, PlayState* play);
void DemoShd_Destroy(Actor* thisx, PlayState* play);
void DemoShd_Update(Actor* thisx, PlayState* play);
void DemoShd_Draw(Actor* thisx, PlayState* play);

void FUN_003f1fd8(DemoShd* self, PlayState* play);
void FUN_003674e4(s32 seqId);
void FUN_003586ec(SkeletonAnimationModel_unk_0C* anim);
u64 FUN_00368d94(s32 a, s32 b);

ActorInit Demo_Shd_InitVars = {
    ACTOR_DEMO_SHD,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(DemoShd),
    (ActorFunc)DemoShd_Init,
    (ActorFunc)DemoShd_Destroy,
    (ActorFunc)DemoShd_Update,
    (ActorFunc)DemoShd_Draw,
};

void DemoShd_Init(Actor* thisx, PlayState* play) {
    DemoShd* self = (DemoShd*)thisx;
    ZARInfo* zarInfo;

    self->unk_14C = 0;
    self->actionFunc = FUN_003f1fd8;
    Actor_SetScale(&self->actor, 0.4f);
    self->actor.world.pos.y = 0.0f;
    self->actor.world.pos.x = 0.0f;
    zarInfo = FUN_00372f38(thisx, play, &self->skelAnimModel[0], 0x5E,
                                  &self->skelAnimModel[1], 0x5F, 0);
    self->cmab[0] = zarInfo->GetCMABByIndex(0x2F);
    self->cmab[1] = zarInfo->GetCMABByIndex(0x30);
    self->skelAnimModel[0]->unk_0C->FUN_00372d94(self->cmab[0]);
    self->skelAnimModel[1]->unk_0C->FUN_00372d94(self->cmab[1]);
    self->skelAnimModel[0]->unk_0C->SetAnimSpeed(0.0f);
    self->skelAnimModel[1]->unk_0C->SetAnimSpeed(0.0f);
    self->skelAnimModel[0]->unk_0C->SetAnimType(1);
    self->skelAnimModel[1]->unk_0C->SetAnimType(1);
}

void DemoShd_Destroy(Actor* thisx, PlayState* play) {
    DemoShd* self = (DemoShd*)thisx;

    FUN_00350f34(thisx, &self->skelAnimModel[0], &self->skelAnimModel[1], 0);
}

void DemoShd_Update(Actor* thisx, PlayState* play) {
    DemoShd* self = (DemoShd*)thisx;

    self->actionFunc(self, play);
}

void DemoShd_Draw(Actor* thisx, PlayState* play) {
    DemoShd* self = (DemoShd*)thisx;
    u16 scroll = self->unk_14E;
    s32 gate = gUnk_0054C8CC;
    u8* p;
    s32 frame;
    f32 fFrame;
    SkeletonAnimationModel_unk_0C* anim;

    if (self->unk_14C & 1) {
        p = *(u8**)self->cmab[0];
        p += *(s32*)(p + 0x14);
        frame = (s32)(FUN_00368d94((s32)scroll, *(s32*)(p + 4)) >> 32);
        fFrame = (f32)frame;
        anim = self->skelAnimModel[0]->unk_0C;
        if (gate == 0) {
            *(f32*)&anim->curFrame = fFrame;
            FUN_003586ec(anim);
        }
        self->skelAnimModel[0]->FUN_00372170(0);
    } else if (self->unk_14C & 2) {
        p = *(u8**)self->cmab[1];
        p += *(s32*)(p + 0x14);
        frame = (s32)(FUN_00368d94((s32)scroll, *(s32*)(p + 4)) >> 32);
        fFrame = (f32)frame;
        anim = self->skelAnimModel[1]->unk_0C;
        if (gate == 0) {
            *(f32*)&anim->curFrame = fFrame;
            FUN_003586ec(anim);
        }
        self->skelAnimModel[1]->FUN_00372170(0);
    }
}

void FUN_003f1fd8(DemoShd* self, PlayState* play) {
    if ((play->GetCutsceneState() != 0 && play->csCtx.npcActions[0] != NULL) ||
        (play->GetCutsceneState() != 0 && play->csCtx.npcActions[1] != NULL)) {
        if (play->csCtx.frames == (s32)FRAMES(800)) {
            FUN_003674e4(9);
        }
        if (play->csCtx.frames == (s32)FRAMES(1069)) {
            FUN_003674e4(8);
        }
    }

    if (play->GetCutsceneState() != 0 && play->csCtx.npcActions[0] != NULL) {
        if (*(u16*)play->csCtx.npcActions[0] == 2) {
            if (!(self->unk_14C & 1)) {
                self->unk_14E = 0xFFFF;
            }
            self->unk_14C |= 1;
        } else {
            self->unk_14C &= ~1;
        }
    }

    if (play->GetCutsceneState() != 0 && play->csCtx.npcActions[1] != NULL) {
        if (*(u16*)play->csCtx.npcActions[1] == 2) {
            if (!(self->unk_14C & 2)) {
                self->unk_14E = 0xFFFF;
            }
            self->unk_14C |= 2;
        } else {
            self->unk_14C &= ~2;
        }
    }

    self->unk_14E++;
}
}
