extern "C" {
#include "z_obj_oshihiki.hpp"

#define FLAGS 0x00400010

void ObjOshihiki_Init(Actor* actor, GameState* state);
void ObjOshihiki_Destroy(Actor* actor, GameState* state);
void ObjOshihiki_Update(Actor* actor, GameState* state);
void ObjOshihiki_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Obj_Oshihiki.data.s")

ActorInit Obj_Oshihiki_InitVars = {
    ACTOR_OBJ_OSHIHIKI,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_DANGEON_KEEP,
    sizeof(ObjOshihiki),
    (ActorFunc)ObjOshihiki_Init,
    (ActorFunc)ObjOshihiki_Destroy,
    (ActorFunc)ObjOshihiki_Update,
    (ActorFunc)ObjOshihiki_Draw,
};

// Draft (~25 insns off, structure matches ROM; remaining: `mov r6,r5` play copy kept by ROM before DynaPolyActor_Init,
// model-kind switch result in r3 with `mov ip,r4` copy, scene id/compare regs). Needs actor-dir EQUs (data_syms.s):
// sScales 0x004DBB78 (f32[8]), sSceneIds 0x004DBB98 (s8[9]), sModels 0x00535604 (s8*[9], rows of 12 s8 at 0x4DBBA1..),
// sInitChain 0x00535628 (3 entries).
#if 0
extern f32 Obj_Oshihiki_sScales[];
extern s8 Obj_Oshihiki_sSceneIds[];
extern s8* Obj_Oshihiki_sModels[];
extern InitChainEntry Obj_Oshihiki_sInitChain[];
void ObjOshihiki_OnActor(ObjOshihiki* self, PlayState* play);

static void ObjOshihiki_InitDynapoly(ObjOshihiki* self, PlayState* play, void* collision, s32 moveFlag) {
    CollisionHeader* colHeader = NULL;

    PlayState* play2 = play;
    DynaPolyActor_Init(&self->dyna, moveFlag);
    ReadCollisionHeaderFromZsi(collision, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play2, &play->colCtx.dyna, &self->dyna.actor, colHeader);
}

void ObjOshihiki_Init(Actor* thisx, GameState* state) {
    ObjOshihiki* self = (ObjOshihiki*)thisx;
    PlayState* play = (PlayState*)state;

    thisx->unk_19A = 1;

    if ((thisx->params & 0xF) <= 7) {
        PlayState* play2 = play;
        ObjOshihiki_InitDynapoly(self, play2, play->GetObjectStatus(thisx->objBankIndex)->zarInfo.FUN_003532c0(1), 1);
    }

    s16 params = thisx->params;
    if (((params >> 8) & 0xFF) < 0x40) {
        if (Flags_GetSwitch(play, (params >> 8) & 0x3F)) {
            switch (thisx->params & 0xF) {
                case 0:
                case 1:
                case 2:
                case 3:
                    Actor_Kill(thisx);
                    return;
            }
        } else {
            switch (thisx->params & 0xF) {
                case 4:
                case 5:
                case 6:
                case 7:
                    Actor_Kill(thisx);
                    return;
            }
        }
    }

    Actor_SetScale(thisx, Obj_Oshihiki_sScales[thisx->params & 0xF]);

    s32 kind;
    switch (thisx->params & 0xF) {
        case 0:
        case 1:
        case 4:
        case 5:
            kind = 0;
            break;
        case 2:
        case 6:
            kind = 1;
            break;
        default:
            kind = 2;
            break;
    }
    {
        s32 color = (thisx->params >> 6) & 3;
        s32 i;
        for (i = 0; i < 9; i++) {
            if (Obj_Oshihiki_sSceneIds[i] == play->sceneNum) {
                FUN_00372f38(thisx, play, &self->skelAnimModel, Obj_Oshihiki_sModels[i][color + kind * 4], 0);
                break;
            }
        }
    }

    Actor_ProcessInitChain(thisx, Obj_Oshihiki_sInitChain);
    thisx->colChkInfo.mass = 0xFF;
    {
        s32 i;
        for (i = 0; i < 5; i++) {
            self->floorBgIds[i] = 0x32;
        }
    }
    self->stateFlags |= 8;
    thisx->gravity = -1.0f;
    thisx->velocity.x = thisx->velocity.y = thisx->velocity.z = 0.0f;
    self->actionFunc = ObjOshihiki_OnActor;
}
#endif

GLOBAL_ASM("asm/ObjOshihiki_Init.s")

void ObjOshihiki_Destroy(Actor* actor, GameState* state) {
    ObjOshihiki* self = (ObjOshihiki*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void ObjOshihiki_Update(Actor* actor, GameState* state) {
    ObjOshihiki* self = (ObjOshihiki*)actor;
    PlayState* play = (PlayState*)state;

    self->stateFlags &= ~0xFF;
    self->stateFlags |= 0x100;

    if (self->timer > 0) {
        self->timer--;
    }

    self->dyna.actor.world.rot.y = self->dyna.unk_1B0;

    self->yawSin = Math_SinS(self->dyna.actor.world.rot.y);
    self->yawCos = Math_CosS(self->dyna.actor.world.rot.y);

    if (self->actionFunc != NULL) {
        self->actionFunc(self, play);
    }
}

// Drafts for ObjOshihiki_OnActor (~90 diff lines: ROM hoists the action-fn literal into r7 and shares one
// `str r7,[r4,#0x1bc]` tail between SetupOnScene/SetupFall; reg numbering play=r6 player=r8 dyna=sb) and
// ObjOshihiki_Draw (~55-67 diff lines: ROM keeps Actor* thisx in r5 AND self in r4, uses sl for NULL, vector/matrix
// block scheduling). Callees are still GLOBAL_ASM FUN_s: FUN_00359464 = N64 SetGround, FUN_0034aaf0/FUN_0034aae0 =
// DynaPolyActor_SetActorOnTop/SetSwitchPressed, FUN_003513fc = Player_GetStrength, FUN_0034a944 = CheckWall,
// FUN_00184ec8/FUN_003bc024/FUN_003bd348 = OnScene/Fall/Push actions.
#if 0
#include "mtx34_trans.hpp"
extern __pure float _fabsf(float);
static __inline __pure float ObjOshihiki_Fabsf(float f) {
    return _fabsf(f);
}

void* DynaPoly_GetActor(void* colCtx, s32 bgId);
void FUN_00359464(ObjOshihiki* self, PlayState* play);
void FUN_0034aaf0(DynaPolyActor* dyna);
s32 FUN_003513fc(void);
s32 FUN_0034a944(PlayState* play, s16 angle, f32 direction, ObjOshihiki* self);
void FUN_00184ec8(ObjOshihiki* self, PlayState* play);
void FUN_003bc024(ObjOshihiki* self, PlayState* play);
void FUN_003bd348(ObjOshihiki* self, PlayState* play);
void FUN_0034aae0(DynaPolyActor* dyna);

static inline s32 ObjOshihiki_StrongEnough(ObjOshihiki* self) {
    s32 strength;

    if (self->cantMove) {
        return 0;
    }
    strength = FUN_003513fc();
    switch (self->dyna.actor.params & 0xF) {
        case 0:
        case 1:
        case 4:
        case 5:
            return 1;
        case 2:
        case 6:
            return strength >= 1;
        case 3:
        case 7:
            return strength >= 2;
    }
    return 0;
}

static inline s32 ObjOshihiki_NoSwitchPress(ObjOshihiki* self, DynaPolyActor* dyna, PlayState* play) {
    s16 dynaSwitchFlag;

    if (dyna == NULL) {
        return 1;
    } else if (dyna->actor.id == ACTOR_OBJ_SWITCH) {
        dynaSwitchFlag = (dyna->actor.params >> 8) & 0x3F;
        switch (dyna->actor.params & 0x33) {
            case 0x20:
                if ((dynaSwitchFlag == ((self->dyna.actor.params >> 8) & 0x3F)) && Flags_GetSwitch(play, dynaSwitchFlag)) {
                    return 0;
                }
                break;
            case 0x30:
                if ((dynaSwitchFlag == ((self->dyna.actor.params >> 8) & 0x3F)) && !Flags_GetSwitch(play, dynaSwitchFlag)) {
                    return 0;
                }
                break;
        }
    }
    return 1;
}

static inline void ObjOshihiki_SetupOnScene(ObjOshihiki* self, PlayState* play) {
    self->stateFlags |= 2;
    self->dyna.actor.gravity = 0.0f;
    self->dyna.actor.velocity.x = self->dyna.actor.velocity.y = self->dyna.actor.velocity.z = 0.0f;
    self->actionFunc = FUN_00184ec8;
}

static inline void ObjOshihiki_SetupFall(ObjOshihiki* self, PlayState* play) {
    self->stateFlags |= 0x80;
    self->dyna.actor.velocity.x = self->dyna.actor.velocity.y = self->dyna.actor.velocity.z = 0.0f;
    self->dyna.actor.gravity = -1.0f;
    FUN_00359464(self, play);
    self->actionFunc = FUN_003bc024;
}

static inline void ObjOshihiki_SetupPush(ObjOshihiki* self, PlayState* play) {
    self->stateFlags |= 0x20;
    self->actionFunc = FUN_003bd348;
    self->dyna.actor.gravity = 0.0f;
}

void ObjOshihiki_OnActor(ObjOshihiki* self, PlayState* play) {
    s32 bgId;
    Player* player = GET_PLAYER(play);
    DynaPolyActor* dynaPolyActor;

    self->stateFlags |= 4;
    Actor_MoveForward(&self->dyna.actor);

    FUN_00359464(self, play);
    if ((self->dyna.actor.floorHeight - self->dyna.actor.world.pos.y) >= -0.001f) {
        self->dyna.actor.world.pos.y = self->dyna.actor.floorHeight;
        bgId = self->floorBgIds[self->highestFloor];
        if (bgId == 0x32) {
            ObjOshihiki_SetupOnScene(self, play);
        } else {
            dynaPolyActor = (DynaPolyActor*)DynaPoly_GetActor(&play->colCtx, bgId);
            if (dynaPolyActor != NULL) {
                FUN_0034aaf0(dynaPolyActor);
                FUN_0034aae0(dynaPolyActor);

                if ((self->timer <= 0) && (ObjOshihiki_Fabsf(self->dyna.unk_1A8) > 0.001f)) {
                    if (ObjOshihiki_StrongEnough(self) && ObjOshihiki_NoSwitchPress(self, dynaPolyActor, play) &&
                        !FUN_0034a944(play, self->dyna.unk_1B0, self->dyna.unk_1A8, self)) {
                        self->direction = self->dyna.unk_1A8;
                        ObjOshihiki_SetupPush(self, play);
                    } else {
                        player->stateFlags2 &= ~0x10;
                        self->dyna.unk_1A8 = 0.0f;
                    }
                } else {
                    player->stateFlags2 &= ~0x10;
                    self->dyna.unk_1A8 = 0.0f;
                }
            } else {
                ObjOshihiki_SetupOnScene(self, play);
            }
        }
    } else {
        bgId = self->floorBgIds[self->highestFloor];
        if (bgId == 0x32) {
            ObjOshihiki_SetupFall(self, play);
        } else {
            dynaPolyActor = (DynaPolyActor*)DynaPoly_GetActor(&play->colCtx, bgId);

            if ((dynaPolyActor != NULL) && (dynaPolyActor->unk_1B4 & 1)) {
                FUN_0034aaf0(dynaPolyActor);
                FUN_0034aae0(dynaPolyActor);
                self->dyna.actor.world.pos.y = self->dyna.actor.floorHeight;
            } else {
                ObjOshihiki_SetupFall(self, play);
            }
        }
    }
}

static inline ObjOshihiki* ObjOshihiki_GetBlockUnder(ObjOshihiki* self, PlayState* play) {
    DynaPolyActor* dynaPolyActor;

    if ((self->floorBgIds[self->highestFloor] != 0x32) &&
        (ObjOshihiki_Fabsf(self->dyna.actor.floorHeight - self->dyna.actor.world.pos.y) < 0.001f)) {
        dynaPolyActor = (DynaPolyActor*)DynaPoly_GetActor(&play->colCtx, self->floorBgIds[self->highestFloor]);
        if ((dynaPolyActor != NULL) && (dynaPolyActor->actor.id == 0xFF)) {
            return (ObjOshihiki*)dynaPolyActor;
        }
    }
    return NULL;
}

static inline void ObjOshihiki_UpdateInitPos(ObjOshihiki* self) {
    if (self->dyna.actor.world.pos.x > self->dyna.actor.home.pos.x) {
        while ((self->dyna.actor.world.pos.x - self->dyna.actor.home.pos.x) >= 20.0f) {
            self->dyna.actor.home.pos.x += 20.0f;
        }
    } else {
        while ((self->dyna.actor.home.pos.x - self->dyna.actor.world.pos.x) >= 20.0f) {
            self->dyna.actor.home.pos.x -= 20.0f;
        }
    }
    if (self->dyna.actor.world.pos.z > self->dyna.actor.home.pos.z) {
        while ((self->dyna.actor.world.pos.z - self->dyna.actor.home.pos.z) >= 20.0f) {
            self->dyna.actor.home.pos.z += 20.0f;
        }
    } else {
        while ((self->dyna.actor.home.pos.z - self->dyna.actor.world.pos.z) >= 20.0f) {
            self->dyna.actor.home.pos.z -= 20.0f;
        }
    }
}

static inline s32 ObjOshihiki_MoveWithBlockUnder(ObjOshihiki* self, PlayState* play) {
    ObjOshihiki* blockUnder = ObjOshihiki_GetBlockUnder(self, play);

    if ((blockUnder != NULL) && (blockUnder->stateFlags & 0x20) &&
        !FUN_0034a944(play, blockUnder->dyna.unk_1B0, blockUnder->direction, self)) {
        self->blockUnder = blockUnder;
    }

    if ((self->stateFlags & 0x100) && (self->blockUnder != NULL)) {
        if (self->blockUnder->stateFlags & 0x10) {
            self->underDistX = self->blockUnder->dyna.actor.world.pos.x - self->blockUnder->dyna.actor.prevPos.x;
            self->underDistZ = self->blockUnder->dyna.actor.world.pos.z - self->blockUnder->dyna.actor.prevPos.z;
            self->dyna.actor.world.pos.x += self->underDistX;
            self->dyna.actor.world.pos.z += self->underDistZ;
            ObjOshihiki_UpdateInitPos(self);
            return 1;
        } else if (!(self->blockUnder->stateFlags & 0x20)) {
            self->blockUnder = NULL;
        }
    }
    return 0;
}

void ObjOshihiki_Draw(Actor* thisx, GameState* state) {
    ObjOshihiki* self = (ObjOshihiki*)thisx;
    PlayState* play = (PlayState*)state;
    MTX34 mtx;
    Vec3f pos;
    Vec3f offs;

    if (self->skelAnimModel != NULL) {
        pos = thisx->world.pos;
        FUN_00357fd0(GET_PLAYER(play), thisx->unk_178, &thisx->world.pos);
        if (ObjOshihiki_MoveWithBlockUnder(self, play)) {
            offs.x = self->underDistX * 10.0f;
            offs.y = 0.0f;
            offs.z = self->underDistZ * 10.0f;
            Matrix_UpdatePosition(&mtx, &thisx->modelMtx, &offs);
            pos.x += offs.x;
            pos.y += offs.y;
            pos.z += offs.z;
        } else {
            MTX34CopyAsm(&mtx, &thisx->modelMtx);
        }
        self->stateFlags &= ~0x100;
        self->skelAnimModel->FUN_003721e0(&mtx);
        self->skelAnimModel->SetUnkAC(1);
        if (play->sceneNum == 6) {
            FUN_00357fd0(GET_PLAYER(play), thisx->unk_178, &pos);
        }
        self->skelAnimModel->FUN_00372170(0);
    }
}
#endif

GLOBAL_ASM("asm/ObjOshihiki_Draw.s")

GLOBAL_ASM("asm/ObjOshihiki_OnActor.s")

void FUN_0034aae0(DynaPolyActor* dyna) {
    dyna->unk_1B8 |= 8;
}

}
