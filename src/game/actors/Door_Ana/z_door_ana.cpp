extern "C" {
#include "z_door_ana.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x82400000

static inline void DoorAna_SetupAction(DoorAna* self, DoorAnaActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void DoorAna_Init(Actor* thisx, PlayState* play);
void DoorAna_Destroy(Actor* thisx, PlayState* play);
void DoorAna_Update(Actor* thisx, PlayState* play);
void DoorAna_Draw(Actor* thisx, PlayState* play);

GLOBAL_ASM("data/z_Door_Ana.data.s")

GLOBAL_ASM("../src/game/actors/Door_Ana/data_syms.s")

ActorInit Door_Ana_InitVars = {
    ACTOR_DOOR_ANA,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_FIELD_KEEP,
    sizeof(DoorAna),
    (ActorFunc)DoorAna_Init,
    (ActorFunc)DoorAna_Destroy,
    (ActorFunc)DoorAna_Update,
    (ActorFunc)DoorAna_Draw,
};

void DoorAna_WaitClosed(DoorAna* self, PlayState* play);
void DoorAna_WaitOpen(DoorAna* self, PlayState* play);
void FUN_001ce864(DoorAna* self, PlayState* play);
void FUN_0027554c(DoorAna* self, PlayState* play);

// DoorAna_Init: ~100/137 words off (best draft, 15 variants). ROM stores each switch case as predicated
// `moveq r1,#K; streq r1,[switchFlag]; beq` into the shared SetScale block; ours cross-jumps the stores.
// Needs `DoorAna_sCylinderInit EQU 0x0051C538` in data_syms.s when enabled.
#if 0
extern ColliderCylinderInit DoorAna_sCylinderInit;

void DoorAna_Init(Actor* thisx, PlayState* play) {
    DoorAna* self = (DoorAna*)thisx;
    s32 flag;

    self->actor.shape.rot.z = 0;
    self->actor.shape.rot.y = self->actor.shape.rot.z;
    self->actor.unk_19A = 1;
    FUN_00372f38(&self->actor, play, &self->model, 0, 0);
    if ((self->actor.params & 0x300) != 0) {
        if (self->actor.params == 0x2E4 && gSaveContext.linkAge == 0) {
            self->switchFlag = 0x11;
        } else {
            self->switchFlag = -1;
        }
        if ((self->actor.params & 0x200) != 0) {
            Collider_InitCylinder(play, &self->collider);
            Collider_SetCylinder(play, &self->collider, &self->actor, &DoorAna_sCylinderInit);
        } else {
            self->actor.flags |= 0x10;
        }
        Actor_SetScale(&self->actor, 0.0f);
        DoorAna_SetupAction(self, DoorAna_WaitClosed);
    } else {
        flag = 0;
        switch (self->actor.params) {
            case 0x00:
                self->switchFlag = 0xE;
                break;
            case 0x14:
                self->switchFlag = 0x11;
                break;
            case 0x22:
                self->switchFlag = 8;
                break;
            case 0x7A:
                self->switchFlag = 0x1D;
                break;
            case 0xEF:
                flag = 1;
                break;
            case 0xF2:
            case 0xFD:
                self->switchFlag = 0x3C;
                break;
            case 0xF5:
                self->switchFlag = 0x1F;
                break;
            case 0xF9:
                self->switchFlag = 0x17;
                break;
            case 0x10E6:
                self->switchFlag = 5;
                break;
            case 0x10FF:
                if (play->sceneNum == 0x51) {
                    self->switchFlag = 0x10;
                    break;
                }
            default:
                DoorAna_SetupAction(self, DoorAna_WaitOpen);
                break;
        }
        if (self->switchFlag != 0) {
            Actor_SetScale(&self->actor, 0.0f);
            DoorAna_SetupAction(self, FUN_001ce864);
        } else if (flag != 0) {
            Actor_SetScale(&self->actor, 0.0f);
            DoorAna_SetupAction(self, FUN_0027554c);
        }
    }
    self->actor.targetMode = 0;
}
#endif

GLOBAL_ASM("asm/DoorAna_Init.s")

void DoorAna_Destroy(Actor* thisx, PlayState* play) {
    DoorAna* self = (DoorAna*)thisx;

    if ((self->actor.params & 0x200) != 0) {
        Collider_DestroyCylinder(play, &self->collider);
    }
    FUN_00350f34(thisx, &self->model, 0);
}

void DoorAna_Update(Actor* thisx, PlayState* play) {
    DoorAna* self = (DoorAna*)thisx;

    self->actionFunc(self, play);
}

// DoorAna_Draw: 28/46 words off with MTX34_MakeTrans (recipe R8). Prologue/shrink-wrap, VFP regs and frame match;
// left: ROM puts 0x3F800000 in r0 and 0 in r1 (one strd fewer), ours 0 in r0 -> store schedule differs.
// Tried: helper store orders/local consts, Translate param+store perms, mode-style inline, alias mutations.
#if 0
void FUN_0036c174(MTX34* dst, MTX34* a, MTX34* b);

struct DaVec {
    f32& operator[](s32 i) {
        return (&x)[i];
    }
    f32 x, y, z;
};
static inline void DoorAna_Translate(MTX34* m, f32 y, f32 x, f32 z) {
    DaVec p;
    p[0] = x;
    p[1] = y;
    p[2] = z;
    MTX34_MakeTrans(m, (Vec3f*)&p);
}

static inline void DoorAna_DrawModel(Actor* thisx) {
    DoorAna* self = (DoorAna*)thisx;
    MTX34 mtx;

    DoorAna_Translate(&mtx, 0.05f, 0.0f, 0.0f);
    FUN_0036c174(&mtx, &mtx, &self->actor.modelMtx);
    self->model->FUN_003721e0(&mtx);
    self->model->SetUnkAC(1);
    self->model->FUN_00372170(0);
}

void DoorAna_Draw(Actor* thisx, PlayState* play) {
    DoorAna* self = (DoorAna*)thisx;

    if (self->actor.scale.x == 0.0f) {
        return;
    }
    DoorAna_DrawModel(thisx);
}
#endif

GLOBAL_ASM("asm/DoorAna_Draw.s")


void DoorAna_WaitOpen(DoorAna* self, PlayState* play);

void FUN_00353484(Actor* actor, PlayState* play);
s32 FUN_00189628(Actor* actor);

void FUN_001ce864(DoorAna* self, PlayState* play) {
    if (Flags_GetSwitch(play, self->switchFlag)) {
        DoorAna_SetupAction(self, DoorAna_WaitOpen);
        FUN_00353484(&self->actor, play);
    }
}

void FUN_0027554c(DoorAna* self, PlayState* play) {
    if (self->actor.params == 0xEF) {
        Actor* actor = Actor_Find(&play->actorCtx, 0x9D, 1);
        if (actor != NULL && FUN_00189628(actor)) {
            DoorAna_SetupAction(self, DoorAna_WaitOpen);
            FUN_00353484(&self->actor, play);
        }
    }
}

void DoorAna_GrabLink(DoorAna* self, PlayState* play) {
    Player* player;

    if (self->actor.yDistToPlayer <= 0.0f && 15.0f < self->actor.xzDistToPlayer) {
        player = GET_PLAYER(play);
        player->actor.world.pos.x = Math_SinS(self->actor.yawTowardsPlayer) * 15.0f + self->actor.world.pos.x;
        player->actor.world.pos.z = Math_CosS(self->actor.yawTowardsPlayer) * 15.0f + self->actor.world.pos.z;
    }
}

s32 Flags_GetEnv(PlayState* play, s32 flag);
void FUN_00372244(void* arg0, u16 arg1, u32 sfxId);

void DoorAna_WaitClosed(DoorAna* self, PlayState* play) {
    s32 openGrotto = false;

    if (!(self->actor.params & 0x200)) {
        // opening with song of storms
        if (self->actor.xyzDistToPlayerSq < SQ(200.0f) && (Flags_GetEnv(play, 5) || Flags_GetEnv(play, 7))) {
            openGrotto = true;
            self->actor.flags &= ~0x10; // ACTOR_FLAG_UPDATE_CULLING_DISABLED
        }
    } else {
        // switch-controlled grotto, or bombing/hammering open a grotto
        if (self->switchFlag >= 0) {
            if (Flags_GetSwitch(play, self->switchFlag)) {
                self->switchFlag = -1;
            }
        } else {
            if (self->collider.base.acFlags & AC_HIT) {
                openGrotto = true;
                Collider_DestroyCylinder(play, &self->collider);
            } else {
                Collider_UpdateCylinder(&self->actor, &self->collider);
                CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
            }
        }
    }
    // open the grotto
    if (openGrotto) {
        self->actor.params &= ~0x0300;
        DoorAna_SetupAction(self, DoorAna_WaitOpen);
        FUN_00372244(&play->unk_5FCC, 0x1E, 0x01000487);
    }
    FUN_00353484(&self->actor, play);
}

void DoorAna_GrabLink(DoorAna* self, PlayState* play);
void Gameplay_SetupRespawnPoint(PlayState* play, s32 mode, s32 playerParams);
void Gameplay_TriggerVoidOut(PlayState* play);
s32 Player_InCsMode(PlayState* play);

extern s16 DoorAna_sGrottoEntrances[];

// DoorAna_WaitOpen: 4 words off (only the Math_StepToF float args: ROM loads target 0.0095f into s0 first,
// armcc emits the step load first; declaration/assignment variants and wrapper helpers tried, ~20 variants).
void DoorAna_WaitOpen(DoorAna* self, PlayState* play) {
    Player* player;
    s32 destinationIdx;

    player = GET_PLAYER(play);
    f32 a, b;

    a = 0.0095f;
    b = 0.00094999996f;
    if (Math_StepToF(&self->actor.scale.x, a, b)) {
        if ((self->actor.targetMode != 0) && (play->sceneLoadFlag == 0) && (player->stateFlags1 & 0x80000000)) {
            if (*(u8*)((u8*)player + 0x2237) != 0) {
                Gameplay_TriggerVoidOut(play);
                play->fadeOutTransition = 4;
                Audio_PlaySoundGeneral(0x010004E1, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                DoorAna_SetupAction(self, DoorAna_GrabLink);
            } else {
                destinationIdx = (((u32)(u16)self->actor.params << 17) >> 29) - 1;
                Gameplay_SetupRespawnPoint(play, 1, 0x4FF);
                gSaveContext.respawn[1].pos.y = self->actor.world.pos.y;
                gSaveContext.respawn[1].yaw = self->actor.home.rot.y;
                gSaveContext.respawn[1].data = (u16)self->actor.params;
                if (destinationIdx < 0) {
                    destinationIdx = self->actor.home.rot.z + 1;
                }
                play->nextEntranceIndex = DoorAna_sGrottoEntrances[destinationIdx];
                DoorAna_SetupAction(self, DoorAna_GrabLink);
            }
        } else {
            if (!Player_InCsMode(play) && !(player->stateFlags1 & (0x800000 | 0x8000000)) &&
                self->actor.xzDistToPlayer <= 15.0f && -50.0f <= self->actor.yDistToPlayer &&
                self->actor.yDistToPlayer <= 15.0f) {
                {
                    u32 f = player->stateFlags1;
                    player->stateFlags1 = f | 0x80000000;
                }
                self->actor.targetMode = 1;
            } else {
                self->actor.targetMode = 0;
            }
        }
    }
    Actor_SetScale(&self->actor, self->actor.scale.x);
}

}
