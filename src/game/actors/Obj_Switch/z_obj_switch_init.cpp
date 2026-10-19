extern "C" {
#include "z_obj_switch.hpp"

void ObjSwitch_FloorUp(ObjSwitch* thisx, PlayState* play);
void ObjSwitch_CrystalOff(ObjSwitch* thisx, PlayState* play);

void FUN_001eb278(ObjSwitch* thisx) {
    thisx->crystalR = 0;
    thisx->crystalG = 0;
    thisx->crystalB = 0;
    thisx->actionFunc = ObjSwitch_CrystalOff;
}

void FUN_001cf0e4(ObjSwitch* thisx) {
    thisx->actor.scale.y = 0.165f;
    thisx->actionFunc = ObjSwitch_FloorUp;
}

void FUN_001cf100(ObjSwitch* thisx) {
    thisx->actionFunc = ObjSwitch_EyeOpen;
    thisx->eyeTexIndex = 0;
}

#if 0 // Used only by the unmatched FloorPress draft.
void ObjSwitch_FloorDownInit(ObjSwitch* thisx) {
    thisx->actor.scale.y = 0.0165f;
    thisx->releaseTimer = 9;
    thisx->actionFunc = ObjSwitch_FloorDown;
}
#endif

s32 FUN_00360084(ActorContext* actorCtx, s32 actorId, s32 category, Actor** out, s32 max);
s32 FUN_0036a2dc(PlayState* play, Actor* actor, s32 arg2, s32 arg3, s32 arg4);
void ObjSwitch_SetOff(ObjSwitch* thisx, PlayState* play) {
    thisx->cooldownOn = 0;
    if (Flags_GetSwitch(play, ((u16)thisx->actor.params >> 8) & 0x3F)) {
        Flags_UnsetSwitch(play, ((u16)thisx->actor.params >> 8) & 0x3F);
        if ((((u32)(u16)thisx->actor.params << 25) >> 29) == 1) {
            Actor* target = &thisx->actor;
            s32 duration = 30;
            s32 setting = 0;
            if (gSaveContext.masterQuestFlag == 1 && play->sceneNum == 9) {
                if (thisx->actor.room == 1) {
                    Actor* targets[8];
                    targets[0] = &thisx->actor;
                    duration = 45;
                    s32 count = FUN_00360084(&play->actorCtx, 0x1B4, 6, targets, 8);
                    count = (count < 2) ? count : 2;
                    target = targets[count];
                }
            } else if (play->sceneNum == 3) {
                if (thisx->actor.room == 14) duration = -1;
            } else if (play->sceneNum == 5 && thisx->actor.room == 6) {
                Player* player = GET_PLAYER(play);
                f32 dx = ABS(player->actor.world.pos.x - -3099.0f);
                f32 dy = ABS(player->actor.world.pos.y - 910.0f);
                f32 dz = ABS(player->actor.world.pos.z - -840.0f);
                if (dx < 62.0f && dy < 110.0f && dz < 50.0f) setting = 0x139D;
            }
            if (gSaveContext.masterQuestFlag == 1 && play->sceneNum == 13 && thisx->actor.room == 10) duration = 70;
            *(s32*)((u8*)thisx + 0x5C0) = FUN_0036a2dc(play, target, 0, setting, 0);
            if (!(thisx->actor.params & 0x8000) && duration >= 0) {
                FUN_00372244(&play->unk_5FCC, (u16)duration, 0x0100048C);
            }
            thisx->cooldownOn = 1;
        }
    }
}

void ObjSwitch_SetOn(ObjSwitch* thisx, PlayState* play) {
    if (Flags_GetSwitch(play, ((u16)thisx->actor.params >> 8) & 0x3F)) {
        thisx->cooldownOn = 0;
    } else {
        s32 subtype = ((u32)(u16)thisx->actor.params << 25) >> 29;
        if (subtype == 0 || subtype == 4) {
            s32 duration = 60;
            if (play->sceneNum == 13 && thisx->actor.room == 18) duration = -1;
            if (gSaveContext.masterQuestFlag != 0 && play->sceneNum == 1 && thisx->actor.room == 0) duration = -1;
            *(s32*)((u8*)thisx + 0x5C0) = FUN_0036a2dc(play, &thisx->actor, 0, 0, 0);
            if (!(thisx->actor.params & 0x8000) && duration >= 0) {
                if (thisx->actor.params & 8) {
                    FUN_00372244(&play->unk_5FCC, (u16)duration, 0x0100048C);
                } else {
                    FUN_00372244(&play->unk_5FCC, (u16)duration, 0x01000487);
                }
            }
        } else {
            Actor* target = &thisx->actor;
            s32 duration = 60;
            s32 setting = 0;
            if (gSaveContext.masterQuestFlag == 1 && play->sceneNum == 9) {
                if (thisx->actor.room == 1) {
                    Actor* targets[8];
                    targets[0] = &thisx->actor;
                    duration = 45;
                    s32 count = FUN_00360084(&play->actorCtx, 0x1B4, 6, targets, 8);
                    count = (count < 2) ? count : 2;
                    target = targets[count];
                }
            } else if (play->sceneNum == 3) {
                if (thisx->actor.room == 14) duration = -1;
            } else if (play->sceneNum == 5 && thisx->actor.room == 6) {
                Player* player = GET_PLAYER(play);
                f32 dx = ABS(player->actor.world.pos.x - -3099.0f);
                f32 dy = ABS(player->actor.world.pos.y - 910.0f);
                f32 dz = ABS(player->actor.world.pos.z - -840.0f);
                if (dx < 62.0f && dy < 110.0f && dz < 50.0f) setting = 0x139D;
            }
            *(s32*)((u8*)thisx + 0x5C0) = FUN_0036a2dc(play, target, 0, setting, 0);
            if (!(thisx->actor.params & 0x8000) && duration >= 0) {
                FUN_00372244(&play->unk_5FCC, (u16)duration, 0x0100048C);
            }
        }
        if (!(thisx->actor.params & 0x4000)) {
            Flags_SetSwitch(play, (thisx->actor.params >> 8) & 0x3F);
        }
        thisx->cooldownOn = 1;
    }
}

#if 0 // Sol best Init draft:1188/1204B,218words; original ASM remains in main TU.
void ObjSwitch_CrystalOn(ObjSwitch* self, PlayState* play);
void ObjSwitch_FloorDown(ObjSwitch* self, PlayState* play);
void ObjSwitch_InitTrisCollider(ObjSwitch* self, PlayState* play, ObjSwitchColliderTrisInit* init);
void Collider_InitJntSph(PlayState* play, void* collider);
void Collider_SetJntSph(PlayState* play, void* collider, Actor* actor, void* init, void* elements);
void Collider_UpdateSpheres(s32 index, void* collider, MTX34* mtx);
void FUN_003679d0(f32 x, f32 y, f32 z, MTX34* mtx, Vec3s* rot);
extern s8 ObjSwitch_sModelIndices[25];
extern s8 ObjSwitch_sTextureAnimMask[64];
extern InitChainEntry ObjSwitch_sInitChain[];
extern f32 ObjSwitch_sFocusHeights[5];
extern ObjSwitchColliderTrisInit ObjSwitch_sRustyFloorTrisInit;
extern ObjSwitchColliderTrisInit ObjSwitch_sEyeTrisInit;
extern u8 ObjSwitch_sCrystalJntSphInit;
static inline void ObjSwitch_SetFloorDown(ObjSwitch* self) {
    self->actor.scale.y = 0.0165f;
    self->releaseTimer = 9;
    self->actionFunc = ObjSwitch_FloorDown;
}
static inline void ObjSwitch_InitCrystalCollider(ObjSwitch* self, PlayState* play) {
    void* collider = (u8*)self + 0x1D8;
    Collider_InitJntSph(play, collider);
    Collider_SetJntSph(play, collider, &self->actor, &ObjSwitch_sCrystalJntSphInit, (u8*)self + 0x1F8);
    FUN_003679d0(self->actor.world.pos.x, self->actor.world.pos.y + self->actor.shape.yOffset * self->actor.scale.y,
                self->actor.world.pos.z, &self->actor.modelMtx, &self->actor.shape.rot);
    f32 x = self->actor.scale.x;
    f32 y = self->actor.scale.y;
    f32 z = self->actor.scale.z;
    MTX34* m = &self->actor.modelMtx;
    m->data[0][0] *= x;
    m->data[1][0] *= x;
    m->data[2][0] *= x;
    m->data[0][1] *= y;
    m->data[1][1] *= y;
    m->data[2][1] *= y;
    m->data[0][2] *= z;
    m->data[1][2] *= z;
    m->data[2][2] *= z;
    Collider_UpdateSpheres(0, collider, &self->actor.modelMtx);
}

static inline void ObjSwitch_InitDyna(ObjSwitch* self, PlayState* play, void* collision, s32 moveFlag) {
    CollisionHeader* colHeader = NULL;
    DynaPolyActor_Init((DynaPolyActor*)&self->actor, moveFlag);
    ReadCollisionHeaderFromZsi(collision, &colHeader);
    self->bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->actor, colHeader);
}
void ObjSwitch_Init(Actor* actor, GameState* state) {
    ObjSwitch* thisx = (ObjSwitch*)actor;
    PlayState* play = (PlayState*)state;
    actor->unk_19A = 1;
    s32 invalidCamera = -1;
    thisx->unk_5BC = 1;
    thisx->cameraId = invalidCamera;
    ZARInfo* zar = &play->GetObjectStatus(actor->objBankIndex)->zarInfo;
    s32 isSet = Flags_GetSwitch(play, ((u16)actor->params >> 8) & 0x3F);
    s32 type = actor->params & 7;
    s8* models = &ObjSwitch_sModelIndices[type * 5];
    FUN_00372f38(actor, play, &thisx->skelAnimModel[0], ObjSwitch_sModelIndices[type * 5], &thisx->skelAnimModel[1], models[1],
                 &thisx->skelAnimModel[2], models[2], &thisx->skelAnimModel[3], models[3],
                 &thisx->skelAnimModel[4], models[4], 0);
    s8* animations = &ObjSwitch_sTextureAnimMask[type * 5];
    for (u32 i = 0; i < 5; i++) {
        if (thisx->skelAnimModel[i] != NULL) {
          s32 index = animations[i];
          if (index >= 0) {
            void* cmab = zar->GetCMABByIndex(index);
            thisx->textureAnim[i].unk_00 = thisx->skelAnimModel[i]->unk_10;
            thisx->textureAnim[i].FUN_00372d94(cmab);
          }
        }
    }
    if (type == 0 || type == 1) {
        void* collision = zar->FUN_003532c0(0);
        ObjSwitch_InitDyna(thisx, play, collision, 1);
    }
    Actor_ProcessInitChain(actor, ObjSwitch_sInitChain);
    if (type == 0 || type == 1) actor->world.pos.y = actor->home.pos.y + 1.0f;
    Actor_SetFocus(actor, ObjSwitch_sFocusHeights[type]);
    if (type == 1) {
        ObjSwitch_InitTrisCollider(thisx, play, &ObjSwitch_sRustyFloorTrisInit);
    } else if (type == 2) {
        ObjSwitch_InitTrisCollider(thisx, play, &ObjSwitch_sEyeTrisInit);
    } else if (type == 3 || type == 4) {
        ObjSwitch_InitCrystalCollider(thisx, play);
        if (type == 4) {
            actor->flags |= 1;
            actor->targetMode = 4;
        }
    }
    actor->colChkInfo.mass = 255;
    if ((s32)((u32)(u16)actor->params << 24) < 0) {
        Actor* ice = Actor_SpawnAsChild(&play->actorCtx, actor, play, 0x11E,
                                      actor->world.pos.x, actor->world.pos.y, actor->world.pos.z,
                                      actor->world.rot.x, actor->world.rot.y, actor->world.rot.z,
                                      actor->params & 0x3F00);
        if (ice == NULL) actor->params &= ~0x80;
    }
    if ((s32)((u32)(u16)actor->params << 24) < 0) {
        thisx->actionFunc = ObjSwitch_EyeInit;
    } else if (type == 0 || type == 1) {
        if (isSet) {
            ObjSwitch_SetFloorDown(thisx);
        } else {
            thisx->actor.scale.y = 0.165f;
            thisx->actionFunc = ObjSwitch_FloorUp;
        }
    } else if (type == 2) {
        actor->flags |= 0x400000;
        if (isSet) {
            thisx->actionFunc = ObjSwitch_EyeClosed;
            thisx->eyeTexIndex = 3;
        } else {
            thisx->actionFunc = ObjSwitch_EyeOpen;
            thisx->eyeTexIndex = 0;
        }
    } else if (type == 3 || type == 4) {
        if (isSet) {
            thisx->crystalR = 255;
            thisx->crystalG = 255;
            thisx->crystalB = 255;
            thisx->actionFunc = ObjSwitch_CrystalOn;
        } else {
            thisx->crystalR = 0;
            thisx->crystalG = 0;
            thisx->crystalB = 0;
            thisx->actionFunc = ObjSwitch_CrystalOff;
        }
    }
}

#endif

}
