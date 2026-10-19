extern "C" {
#include "z_bg_bdan_switch.hpp"

#define FLAGS 0x80400010

void BgBdanSwitch_Init(Actor* actor, GameState* state);
void BgBdanSwitch_Destroy(Actor* thisx, PlayState* play);
void BgBdanSwitch_Update(Actor* thisx, PlayState* play);
void BgBdanSwitch_Draw(Actor* thisx, PlayState* play);

void Collider_InitJntSph(PlayState* play, BgBdanSwitchColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, BgBdanSwitchColliderJntSph* collider, Actor* actor,
                        BgBdanSwitchJntSphInit* init, BgBdanSwitchColliderElement* elements);

// Action functions still in functions_*.cpp (N64: func_8086D5E0, D754, D888, DB40, DB68, DCE8).
void FUN_0012b38c(BgBdanSwitch* self, PlayState* play);
void FUN_00110c88(BgBdanSwitch* self, PlayState* play);
void FUN_0012b36c(BgBdanSwitch* self, PlayState* play);
void FUN_00110c84(BgBdanSwitch* self, PlayState* play);
void FUN_0012b4d8(BgBdanSwitch* self, PlayState* play);
void FUN_00144074(BgBdanSwitch* self, PlayState* play);

extern InitChainEntry BgBdanSwitch_sInitChain[];
extern BgBdanSwitchJntSphInit BgBdanSwitch_sJntSphInit;

GLOBAL_ASM("data/z_Bg_Bdan_Switch.data.s")
GLOBAL_ASM("data/z_Bg_Bdan_Switch.data_00510A9C.s")
GLOBAL_ASM("../src/game/actors/Bg_Bdan_Switch/data_syms.s")

ActorInit Bg_Bdan_Switch_InitVars = {
    ACTOR_BG_BDAN_SWITCH,
    ACTORCAT_SWITCH,
    FLAGS,
    OBJECT_BDAN_OBJECTS,
    sizeof(BgBdanSwitch),
    (ActorFunc)BgBdanSwitch_Init,
    (ActorFunc)BgBdanSwitch_Destroy,
    (ActorFunc)BgBdanSwitch_Update,
    (ActorFunc)BgBdanSwitch_Draw,
};

#if 0
// BgBdanSwitch_Init: UNMATCHED, 27/172 words differ (688 B == ROM). Everything structural matches: GetCMAB shapes (scoped
// cmab/m0 locals on call 0, plain r2 shape on call 1), chain stores, predicated type-1 setup (unk, then action arms),
// InitCollision as `(Actor* thisx, play)` helper (ROM `mov r6,r4`). Left: ROM copies the play/state param twice at entry
// (`mov r7,r1; mov r8,r1`, r8 only feeds Flags_GetSwitch, zar lives in sb); ours makes that copy mid-function
// (`mov sb,r7`) and puts zar in r8. play2 at 4 positions x ~2000 use-site combos, decl orders, helper shapes: no effect.
static inline void BgBdanSwitch_InitCollision(Actor* thisx, PlayState* play) {
    BgBdanSwitch* self = (BgBdanSwitch*)thisx;
    Collider_InitJntSph(play, &self->collider);
    Collider_SetJntSph(play, &self->collider, &self->dyna.actor, &BgBdanSwitch_sJntSphInit, self->colliderElements);
}

void BgBdanSwitch_Init(Actor* actor, GameState* state) {
    BgBdanSwitch* self = (BgBdanSwitch*)actor;
    PlayState* play = (PlayState*)state;
    ZARInfo* zarInfo;
    s16 type;
    s32 flag;
    PlayState* play2;

    type = actor->params & 0xFF;
    Actor_ProcessInitChain(actor, BgBdanSwitch_sInitChain);
    if (type == YELLOW_TALL_1 || type == YELLOW_TALL_2) {
        actor->scale.z = actor->scale.x = 0.05f;
    } else {
        actor->scale.z = actor->scale.x = 0.1f;
    }
    actor->scale.y = 0.0f;
    Actor_SetFocus(actor, 10.0f);
    play2 = play;
    zarInfo = FUN_00372f38(actor, state, &self->skelAnimModel[0], 10, &self->skelAnimModel[1], 9, 0);
    {
        void* cmab = zarInfo->GetCMABByIndex(6);
        SkeletonAnimationModel* m0 = self->skelAnimModel[0];
        m0->unk_0C->FUN_00372d94(cmab);
    }
    self->skelAnimModel[0]->unk_0C->SetAnimType(1);
    self->skelAnimModel[1]->unk_0C->FUN_00372d94(zarInfo->GetCMABByIndex(5));
    self->skelAnimModel[1]->unk_0C->SetAnimType(1);

    switch (type) {
        case BLUE:
        case YELLOW_HEAVY:
        case YELLOW:
            {
                void* zsi = DynaPolyInfo_Alloc(&self->dyna, state, 2);
                CollisionHeader* colHeader = NULL;
                DynaPolyActor_Init(&self->dyna, 1);
                ReadCollisionHeaderFromZsi(zsi, &colHeader);
                self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
            }
            break;
        case YELLOW_TALL_1:
        case YELLOW_TALL_2:
            BgBdanSwitch_InitCollision(actor, play);
            actor->flags |= 1;
            actor->targetMode = 4;
            break;
    }
    flag = Flags_GetSwitch(play2, (actor->params >> 8) & 0x3F);

    switch (type) {
        case BLUE:
        case YELLOW:
            if (flag) {
                self->unk_1C8 = 0.1f;
                self->actionFunc = FUN_00110c88;
                self->unk_1D8 = 9;
            } else {
                self->actionFunc = FUN_0012b38c;
                self->unk_1C8 = 1.0f;
            }
            break;
        case YELLOW_HEAVY:
            if (flag) {
                self->unk_1C8 = 0.1f;
                self->actionFunc = FUN_00110c84;
            } else {
                self->unk_1C8 = 1.0f;
                self->actionFunc = FUN_0012b36c;
            }
            break;
        case YELLOW_TALL_1:
        case YELLOW_TALL_2:
            if (flag) {
                self->actionFunc = FUN_0012b4d8;
                self->unk_1C8 = 1.0f;
            } else {
                self->unk_1C8 = 2.0f;
                self->actionFunc = FUN_00144074;
            }
            break;
        default:
            Actor_Kill(actor);
            return;
    }
}
#endif

GLOBAL_ASM("asm/BgBdanSwitch_Init.s")

void Collider_DestroyJntSph(PlayState* play, BgBdanSwitchColliderJntSph* collider);

void BgBdanSwitch_Destroy(Actor* thisx, PlayState* play) {
    BgBdanSwitch* self = (BgBdanSwitch*)thisx;

    switch (self->dyna.actor.params & 0xFF) {
        case BLUE:
        case YELLOW_HEAVY:
        case YELLOW:
            DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
            break;
        case YELLOW_TALL_1:
        case YELLOW_TALL_2:
            Collider_DestroyJntSph(play, &self->collider);
            break;
    }
    FUN_00350f34(thisx, &self->skelAnimModel[0], &self->skelAnimModel[1], 0);
}

// BgBdanSwitch_Update: UNMATCHED, 80/136 words differ (draft 512 B vs ROM 544 B). Switch cases are inlined with
// different per-case float scheduling (vmla/vmul operand + reg order), jump table layout differs. Not pursued further.
s32 Player_InCsMode(PlayState* play);

// Inlined in ROM (N64 func_8086D0EC): timer step, per-type scale/offset, shape.yOffset.
static void BgBdanSwitch_UpdateScale(BgBdanSwitch* self) {
    if (self->unk_1C8 != 0.1f) {
        if (self->unk_1CC > 0) {
            self->unk_1CC += 0x5DC;
        } else {
            self->unk_1CC += 0xFA0;
        }
    }

    switch (self->dyna.actor.params & 0xFF) {
        case BLUE:
        case YELLOW_HEAVY:
        case YELLOW:
            self->unk_1D4 = ((Math_CosS(self->unk_1CC) * 0.5f) + (53.000004f / 6.0f)) * 0.012f;
            self->unk_1D0 = ((Math_CosS(self->unk_1CC) * 0.5f) + 20.5f) * 0.0050000004f * self->unk_1C8;
            self->unk_1D0 *= 0.95f;
            self->dyna.actor.scale.y = self->unk_1C8 * 0.1f;
            break;
        case YELLOW_TALL_1:
        case YELLOW_TALL_2:
            self->unk_1D4 = ((Math_CosS(self->unk_1CC) * 0.5f) + (43.0f / 6.0f)) * 0.0075000003f;
            self->unk_1D0 = ((Math_CosS(self->unk_1CC) * 0.5f) + 20.5f) * 0.0050000004f * self->unk_1C8;
            self->dyna.actor.scale.y = self->unk_1C8 * 0.1f;
    }
    self->dyna.actor.shape.yOffset = 1.2f / self->unk_1D0;
}

void BgBdanSwitch_Update(Actor* thisx, PlayState* play) {
    BgBdanSwitch* self = (BgBdanSwitch*)thisx;
    s32 type;

    if (self->unk_1DA > 0) {
        self->unk_1DA--;
    }
    self->actionFunc(self, play);
    BgBdanSwitch_UpdateScale(self);
    type = self->dyna.actor.params & 0xFF;
    if (type != 3 && type != 4) {
        self->unk_1D8--;
    } else {
        if (!Player_InCsMode(play) && self->unk_1D8 > 0) {
            self->unk_1D8--;
        }
        self->unk_1DC = self->collider.base.acFlags;
        self->collider.base.acFlags &= ~AC_HIT;
        self->collider.elements[0].modelSphere.radius = (s16)(self->unk_1D4 * 370.0f);
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    }
}



}
