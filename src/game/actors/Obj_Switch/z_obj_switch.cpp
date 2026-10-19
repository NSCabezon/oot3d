extern "C" {
#include "z_obj_switch.hpp"
#include "../Door_Shutter/z_door_shutter.hpp"

void ObjSwitch_CrystalTurnOn(ObjSwitch* thisx, PlayState* play);
void ObjSwitch_InitTrisCollider(ObjSwitch* thisx, PlayState* play, ObjSwitchColliderTrisInit* init);
Actor** FUN_0033de14(s32 size);
void FUN_0033ddd4(void* ptr);
s32 FUN_00360084(ActorContext* actorCtx, s32 actorId, s32 category, Actor** out, s32 max);
s32 FUN_0036a2dc(PlayState* play, Actor* actor, s32 arg2, s32 arg3, s32 arg4);
void Collider_InitJntSph(PlayState* play, void* collider);
void Collider_SetJntSph(PlayState* play, void* collider, Actor* actor, void* init, void* elements);
void Collider_UpdateSpheres(s32 index, void* collider, MTX34* mtx);
void FUN_003679d0(f32 x, f32 y, f32 z, MTX34* mtx, Vec3s* rot);
extern s8 ObjSwitch_sModelIndices[25];
extern InitChainEntry ObjSwitch_sInitChain[];
extern f32 ObjSwitch_sFocusHeights[5];
extern ObjSwitchColliderTrisInit ObjSwitch_sRustyFloorTrisInit;
extern ObjSwitchColliderTrisInit ObjSwitch_sEyeTrisInit;
extern u8 ObjSwitch_sCrystalJntSphInit;

extern s32 gUnk_0054C8CC; // Existing shared_syms binding; animation gate, not gGameInfo.
s32 FUN_0036ae04(void);
s32 Player_InCsMode(PlayState* play);
s32 FUN_0036a83c(Actor* actor);
s32 FUN_0036adf4(Actor* actor);
void FUN_003685a0(void* arg);
void Collider_DestroyTris(PlayState* play, void* collider);
void Collider_DestroyJntSph(PlayState* play, void* collider);
void Collider_InitTris(PlayState* play, ObjSwitchColliderTris* collider);
void Collider_SetTris(PlayState* play, ObjSwitchColliderTris* collider, Actor* actor, void* src,
                      ObjSwitchColliderTrisElement* elements);
void Collider_SetTrisVertices(ObjSwitchColliderTris* collider, s32 index, Vec3f* a, Vec3f* b, Vec3f* c);
void FUN_003586ec(void* arg);
void FUN_00373bec(void* arg);
void FUN_003695cc(SkeletonAnimationModel* model, s32 arg1, s32 arg2, s32 arg3, f32 red, f32 green, f32 blue,
                  f32 alpha);
extern s8 ObjSwitch_sCrystalAnimColors[8];
extern s8 ObjSwitch_sTextureAnimMask[64];
void ObjSwitch_SetOn(ObjSwitch* thisx, PlayState* play);
void ObjSwitch_SetOff(ObjSwitch* thisx, PlayState* play);
void ObjSwitch_CrystalOn(ObjSwitch* thisx, PlayState* play);
void ObjSwitch_FloorUp(ObjSwitch* thisx, PlayState* play);
void ObjSwitch_EyeClosing(ObjSwitch* thisx, PlayState* play);
void ObjSwitch_FloorDown(ObjSwitch* thisx, PlayState* play);
void caseD_2_0016ea9c(Actor* actor, GameState* state);
void caseD_3(ObjSwitch* thisx, PlayState* play);

void ObjSwitch_EyeInit(ObjSwitch* thisx, PlayState* play) {
    if (Flags_GetSwitch(play, ((u16)thisx->actor.params >> 8) & 0x3F)) {
        thisx->actionFunc = ObjSwitch_EyeClosed;
        thisx->eyeTexIndex = 3;
    } else {
        thisx->actionFunc = ObjSwitch_EyeOpen;
        thisx->eyeTexIndex = 0;
    }
}

void ObjSwitch_EyeClosing(ObjSwitch* thisx, PlayState* play) {
    if (!thisx->cooldownOn || FUN_0036ae04() == thisx->actor.type || thisx->cooldownTimer <= 0) {
        if (play->state.frames & 1) {
            thisx->eyeTexIndex++;
        }
        if (thisx->eyeTexIndex >= 3) {
            thisx->actionFunc = ObjSwitch_EyeClosed;
            thisx->eyeTexIndex = 3;
            Audio_PlayActorSound2(&thisx->actor, 0x0100016C);
        }
    }
}

void caseD_1(ObjSwitch* thisx, PlayState* play) {
    if (thisx->skelAnimModel[0] != NULL) {
        thisx->skelAnimModel[0]->FUN_003721e0(&thisx->actor.modelMtx);
        thisx->skelAnimModel[0]->SetUnkAC(1);
        thisx->skelAnimModel[0]->FUN_00372170(0);
    }
}

void caseD_0(Actor* actor, GameState* state) {
    ObjSwitch* thisx = (ObjSwitch*)actor;
    s32 type = ((u32)(u16)actor->params << 25) >> 29;

    FUN_00357fd0(GET_PLAYER((PlayState*)state), actor->unk_178, &actor->world.pos);
    if (thisx->skelAnimModel[type] != NULL) {
        thisx->skelAnimModel[type]->FUN_003721e0(&thisx->actor.modelMtx);
        thisx->skelAnimModel[type]->SetUnkAC(1);
        thisx->skelAnimModel[type]->FUN_00372170(0);
    }
}

void ObjSwitch_EyeOpen(ObjSwitch* thisx, PlayState* play) {
    if ((thisx->eyeHitFlags & 2) && !(thisx->eyeColFlags & 2) && thisx->eyeHitActor != NULL) {
        s32 yawDiff = (s16)(thisx->eyeHitActor->world.rot.y - thisx->actor.shape.rot.y);
        if ((u32)(yawDiff + 0x5000) > 0xA000) {
            goto eyeOpenClose;
        }
    }
    if ((s32)((u32)(u16)thisx->actor.params << 24) >= 0) {
        return;
    }
eyeOpenClose:
    {
        thisx->actionFunc = ObjSwitch_EyeClosing;
        thisx->cooldownTimer = 150;
        ObjSwitch_SetOn(thisx, play);
        thisx->actor.params &= ~0x80;
    }
}

void ObjSwitch_EyeOpening(ObjSwitch* thisx, PlayState* play) {
    if ((((u16)thisx->actor.params >> 4) & 7) != 1 || !thisx->cooldownOn ||
        FUN_0036ae04() == thisx->actor.type || thisx->cooldownTimer <= 0) {
        if (play->state.frames & 1) {
            thisx->eyeTexIndex--;
        }
        if (thisx->eyeTexIndex <= 0) {
            FUN_001cf100(thisx);
            Audio_PlayActorSound2(&thisx->actor, 0x0100016C);
        }
    }
}

void ObjSwitch_FloorRelease(ObjSwitch* thisx) {
    s32 type = ((u16)thisx->actor.params >> 4) & 7;

    if ((type != 1 && type != 3) || !thisx->cooldownOn || FUN_0036ae04() == thisx->actor.type ||
        thisx->cooldownTimer <= 0) {
        thisx->actor.scale.y += 0.0495f;
        if (thisx->actor.scale.y >= 0.165f) {
            FUN_001cf0e4(thisx);
            Audio_PlayActorSound2(&thisx->actor, 0x0100016C);
        }
    }
}

static inline void ObjSwitch_SetFloorDown(ObjSwitch* thisx) {
    thisx->actor.scale.y = 0.0165f;
    thisx->releaseTimer = 9;
    thisx->actionFunc = ObjSwitch_FloorDown;
}

void ObjSwitch_FloorPress(ObjSwitch* thisx, PlayState* play) {
    if (3 == (((u32)(u16)thisx->actor.params << 25) >> 29) || !thisx->cooldownOn || FUN_0036ae04() == thisx->actor.type ||
        thisx->cooldownTimer <= 0) {
        thisx->actor.scale.y -= 0.0495f;
        if (thisx->actor.scale.y <= 0.0165f) {
            ObjSwitch_SetFloorDown(thisx);
            Audio_PlayActorSound2(&thisx->actor, 0x0100016C);
            if (thisx->actor.params & 0x4000) {
                Player* player = GET_PLAYER(play);
                player->actor.freezeTimer = thisx->cooldownTimer;
            }
        }
    }
}

static inline void ObjSwitch_SetEyeOpening(ObjSwitch* self) {
    self->actionFunc = ObjSwitch_EyeOpening;
    self->cooldownTimer = 150;
}
static inline s32 ObjSwitch_EyeHit(ObjSwitch* self) {
    if ((self->eyeHitFlags & 2) && !(self->eyeColFlags & 2)) {
        if (self->eyeHitActor != NULL) {
            s16 ownYaw, hitYaw;
            hitYaw = self->eyeHitActor->world.rot.y;
            ownYaw = self->actor.shape.rot.y;
            s32 yawDiff = (s16)(hitYaw - ownYaw);
            if ((u32)(yawDiff + 0x5000) > 0xA000) return 1;
        }
    }
    return 0;
}
static inline void ObjSwitch_SetFloorRelease(ObjSwitch* self) {
    self->actionFunc = (ObjSwitchActionFunc)ObjSwitch_FloorRelease;
    self->cooldownTimer = 150;
}
static inline void ObjSwitch_SetCrystalTurnOn(ObjSwitch* self) {
    self->actionFunc = ObjSwitch_CrystalTurnOn;
    self->cooldownTimer = 150;
}
static inline void ObjSwitch_Scroll(ObjSwitch* self) {
    ((u8*)self)[0x1D0] = (((u8*)self)[0x1D0] - 1) & 0x7F;
    ((u8*)self)[0x1D1] = (((u8*)self)[0x1D1] + 1) & 0x7F;
    ((u8*)self)[0x1D2] = (((u8*)self)[0x1D2] + 1) & 0x7F;
    ((u8*)self)[0x1D3] = (((u8*)self)[0x1D3] - 1) & 0x7F;
}
static inline void ObjSwitch_RotateY(Vec3f* dest, s16 yaw, Vec3f* src) {
    f32 s = Math_SinS(yaw);
    f32 c = Math_CosS(yaw);
    dest->x = src->z * s + src->x * c;
    dest->y = src->y;
    dest->z = src->z * c - src->x * s;
}
static inline void ObjSwitch_AddVec(Vec3f* a, Vec3f* b, Vec3f* dest) {
    dest->x = a->x + b->x;
    dest->y = a->y + b->y;
    dest->z = a->z + b->z;
}
static inline void ObjSwitch_SetFloorPress(ObjSwitch* self) {
    self->actionFunc = ObjSwitch_FloorPress;
    self->cooldownTimer = 45;
}
static inline void ObjSwitch_UpdateTextureFrame(SkeletonAnimationModel_unk_0C* anim, f32 frame) {
    if (gUnk_0054C8CC == 0) {
        *(f32*)&anim->curFrame = frame;
        FUN_003586ec(anim);
    }
}

#if 0 // Sol best disabled draft; see STATUS.md for size/score and variant budget.
static inline s32 ObjSwitch_CanUpdate(ObjSwitch* thisx, PlayState* play, Player* player) {
    s32 enable = 1;
    if (*(s32*)((u8*)thisx + 0x5BC) == 0) {
        if (player->stateFlags1 & 1) {
            enable = 0;
        } else {
            s32 max = 32;
            Actor** actors = FUN_0033de14(max * sizeof(Actor*));
            s32 count = FUN_00360084(&play->actorCtx, ACTOR_DOOR_SHUTTER, ACTORCAT_DOOR, actors, max);
            for (s32 i = 0; i < count; i++) {
                DoorShutter* other = (DoorShutter*)actors[i];
                if (other->unk_21C != 0) {
                    enable = 0;
                    break;
                }
            }
            FUN_0033ddd4(actors);
        }
    }
    return enable;
}
#endif

#define FLAGS 0x00000010

void ObjSwitch_Init(Actor* actor, GameState* state);
void ObjSwitch_Destroy(Actor* actor, GameState* state);
void ObjSwitch_Update(Actor* actor, GameState* state);
void ObjSwitch_Draw(Actor* actor, GameState* state);
typedef void (*ObjSwitchDrawFunc)(Actor*, PlayState*);
extern ObjSwitchDrawFunc ObjSwitch_sDrawFuncs[8];
GLOBAL_ASM("../src/game/actors/Obj_Switch/data_syms.s")
void ObjSwitch_Draw(Actor* actor, GameState* state) {
    ObjSwitch_sDrawFuncs[(u16)actor->params & 7](actor, (PlayState*)state);
}

GLOBAL_ASM("data/z_Obj_Switch.data.s")

ActorInit Obj_Switch_InitVars = {
    ACTOR_OBJ_SWITCH,
    ACTORCAT_SWITCH,
    FLAGS,
    OBJECT_GAMEPLAY_DANGEON_KEEP,
    sizeof(ObjSwitch),
    (ActorFunc)ObjSwitch_Init,
    (ActorFunc)ObjSwitch_Destroy,
    (ActorFunc)ObjSwitch_Update,
    (ActorFunc)ObjSwitch_Draw,
};

GLOBAL_ASM("asm/ObjSwitch_Init.s")

void ObjSwitch_Destroy(Actor* actor, GameState* state) {
    ObjSwitch* thisx = (ObjSwitch*)actor;
    PlayState* play = (PlayState*)state;
    s16 params = actor->params;
    if ((params & 7) == 0 || (params & 7) == 1) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, *(s32*)((u8*)actor + 0x1A4));
    }
    switch (actor->params & 7) {
        case 1:
        case 2:
            Collider_DestroyTris(play, (u8*)actor + 0x1D8);
            break;
        case 3:
        case 4:
            Collider_DestroyJntSph(play, (u8*)actor + 0x1D8);
            break;
    }
    FUN_003685a0((u8*)thisx + 0x2C4);
    FUN_003685a0((u8*)thisx + 0x35C);
    FUN_003685a0((u8*)thisx + 0x3F4);
    FUN_003685a0((u8*)thisx + 0x48C);
    FUN_003685a0((u8*)thisx + 0x524);
    FUN_00350f34(actor, &thisx->skelAnimModel[0], &thisx->skelAnimModel[1], &thisx->skelAnimModel[2],
                &thisx->skelAnimModel[3], &thisx->skelAnimModel[4], 0);
}


#if 0 // Sol best disabled draft; see STATUS.md for size/score and variant budget.
void ObjSwitch_Update(Actor* actor, GameState* state) {
    ObjSwitch* thisx = (ObjSwitch*)actor;
    PlayState* play = (PlayState*)state;
    if (thisx->releaseTimer > 0) thisx->releaseTimer--;
    if (thisx->cooldownTimer > 0) thisx->cooldownTimer--;
    if (ObjSwitch_CanUpdate(thisx, play, GET_PLAYER(play))) {
        *(s32*)((u8*)thisx + 0x5BC) = 0;
        thisx->actionFunc(thisx, play);
    }
    switch (actor->params & 7) {
        case 0:
        case 1:
            thisx->eyeColFlags = ((u8*)thisx)[0x1B8];
            break;
        case 2:
            thisx->eyeColFlags = thisx->eyeHitFlags;
            thisx->eyeHitFlags &= ~2;
            CollisionCheck_SetAC(play, &play->colChkCtx, (Collider*)((u8*)thisx + 0x1D8));
            break;
        case 3:
        case 4:
            if (!Player_InCsMode(play)) {
                if (*(s16*)((u8*)thisx + 0x1C2) > 0) {
                    (*(s16*)((u8*)thisx + 0x1C2))--;
                } else {
                    *(s32*)((u8*)thisx + 0x5C0) = -1;
                }
            }
            thisx->eyeColFlags = thisx->eyeHitFlags;
            thisx->eyeHitFlags &= ~2;
            if (*(s16*)((u8*)thisx + 0x1C2) <= 0) {
                CollisionCheck_SetAC(play, &play->colChkCtx, (Collider*)((u8*)thisx + 0x1D8));
            }
            CollisionCheck_SetOC(play, &play->colChkCtx, (Collider*)((u8*)thisx + 0x1D8));
            break;
    }
}
#endif
GLOBAL_ASM("asm/ObjSwitch_Update.s")

void ObjSwitch_CrystalOff(ObjSwitch* thisx, PlayState* play) {
    switch ((thisx->actor.params >> 4) & 7) {
        case 0:
            if (((play->sceneNum == 13 && *(u8*)((u8*)play + 0x500C) == 0x12) ||
                 (play->sceneNum == 6 && *(u8*)((u8*)play + 0x500C) == 0x19)) && Player_InCsMode(play)) {
                break;
            }
            if ((thisx->eyeHitFlags & 2) && *(s16*)((u8*)thisx + 0x1C2) <= 0) {
                *(s16*)((u8*)thisx + 0x1C2) = 15;
                ObjSwitch_SetOn(thisx, play);
                ObjSwitch_SetCrystalTurnOn(thisx);
            }
            break;
        case 4:
            if (((thisx->eyeHitFlags & 2) && *(s16*)((u8*)thisx + 0x1C2) <= 0) ||
                Flags_GetSwitch(play, (thisx->actor.params >> 8) & 0x3F)) {
                *(s16*)((u8*)thisx + 0x1C2) = 15;
                ObjSwitch_SetOn(thisx, play);
                ObjSwitch_SetCrystalTurnOn(thisx);
            }
            break;
        case 1:
            if ((thisx->eyeHitFlags & 2) && !(thisx->eyeColFlags & 2) && *(s16*)((u8*)thisx + 0x1C2) <= 0) {
                *(s16*)((u8*)thisx + 0x1C2) = 15;
                ObjSwitch_SetOn(thisx, play);
                ObjSwitch_SetCrystalTurnOn(thisx);
            }
            ObjSwitch_Scroll(thisx);
            break;
    }
}


void ObjSwitch_CrystalTurnOff(ObjSwitch* thisx, PlayState* play) {
    if (gSaveContext.masterQuestFlag == 1 && play->sceneNum == 9 && thisx->actor.room == 1) {
        goto transition;
    }
    if (((u32)(u16)thisx->actor.params << 25 >> 29) != 1) {
        goto transition;
    }
    if (!thisx->cooldownOn) {
        goto transition;
    }
    if (FUN_0036ae04() != thisx->actor.type && thisx->cooldownTimer > 0) {
        return;
    }
transition:
    FUN_001eb278(thisx);
    ((u8*)thisx)[0x1D0] = (((u8*)thisx)[0x1D0] - 1) & 0x7F;
    ((u8*)thisx)[0x1D1] = (((u8*)thisx)[0x1D1] + 1) & 0x7F;
    ((u8*)thisx)[0x1D2] = (((u8*)thisx)[0x1D2] + 1) & 0x7F;
    ((u8*)thisx)[0x1D3] = (((u8*)thisx)[0x1D3] - 1) & 0x7F;
    Audio_PlayActorSound2(&thisx->actor, 0x01000211);
}

void ObjSwitch_CrystalTurnOn(ObjSwitch* thisx, PlayState* play) {
    if (!(gSaveContext.masterQuestFlag == 1 && play->sceneNum == 9 && thisx->actor.room == 1) && thisx->cooldownOn &&
        FUN_0036ae04() != thisx->actor.type && thisx->cooldownTimer > 0) {
        return;
    }
    ((u8*)thisx)[0x1D4] = 0xFF;
    ((u8*)thisx)[0x1D5] = 0xFF;
    ((u8*)thisx)[0x1D6] = 0xFF;
    thisx->actionFunc = ObjSwitch_CrystalOn;
    if ((((u32)(u16)thisx->actor.params << 25) >> 29) == 1) {
        ((u8*)thisx)[0x1D0] = (((u8*)thisx)[0x1D0] - 1) & 0x7F;
        ((u8*)thisx)[0x1D1] = (((u8*)thisx)[0x1D1] + 1) & 0x7F;
        ((u8*)thisx)[0x1D2] = (((u8*)thisx)[0x1D2] + 1) & 0x7F;
        ((u8*)thisx)[0x1D3] = (((u8*)thisx)[0x1D3] - 1) & 0x7F;
    }
    Audio_PlayActorSound2(&thisx->actor, 0x01000211);
}

void caseD_2_0016ea9c(Actor* actor, GameState* state) {
    ObjSwitch* thisx = (ObjSwitch*)actor;
    s16 params = thisx->actor.params;
    s32 modelIndex = ((u32)(u16)params << 25) >> 29;
    u8* modelSlot = (u8*)actor + modelIndex * 4;
    if (*(SkeletonAnimationModel**)(modelSlot + 0x2B0) == NULL) return;
    s8* row = &ObjSwitch_sTextureAnimMask[(params & 7) * 5];
    if (row[modelIndex] >= 0) {
        thisx->textureAnim[modelIndex].SetAnimSpeed(0.0f);
        ObjSwitch_UpdateTextureFrame(&thisx->textureAnim[modelIndex], thisx->eyeTexIndex);
        FUN_00373bec(&thisx->textureAnim[modelIndex]);
    }
    (*(SkeletonAnimationModel**)(modelSlot + 0x2B0))->FUN_003721e0(&thisx->actor.modelMtx);
    (*(SkeletonAnimationModel**)(modelSlot + 0x2B0))->SetUnkAC(1);
    (*(SkeletonAnimationModel**)(modelSlot + 0x2B0))->FUN_00372170(0);
}
void ObjSwitch_CrystalOn(ObjSwitch* thisx, PlayState* play) {
    s16 params = thisx->actor.params;
    s32 subtype = ((u32)(u16)params << 25) >> 29;
    if (subtype == 0) {
        goto checkFlag;
    }
    if (subtype == 1) {
        goto checkHit;
    }
    if (subtype == 4) {
checkFlag:
        if (!Flags_GetSwitch(play, ((u32)(u16)params << 18) >> 26)) {
            thisx->actionFunc = ObjSwitch_CrystalTurnOff;
            thisx->cooldownTimer = 150;
        }
        goto updateScroll;
    }
    goto updateScroll;
checkHit:
    if (((((u8*)thisx)[0x1E9] & 2) != 0) && ((((u8*)thisx)[0x1D7] & 2) == 0) &&
        *(s16*)((u8*)thisx + 0x1C2) <= 0) {
        *(s16*)((u8*)thisx + 0x1C2) = 15;
        thisx->actionFunc = ObjSwitch_CrystalTurnOff;
        thisx->cooldownTimer = 150;
        ObjSwitch_SetOff(thisx, play);
    }
updateScroll:
    ((u8*)thisx)[0x1D0] = (((u8*)thisx)[0x1D0] - 1) & 0x7F;
    ((u8*)thisx)[0x1D1] = (((u8*)thisx)[0x1D1] + 1) & 0x7F;
    ((u8*)thisx)[0x1D2] = (((u8*)thisx)[0x1D2] + 1) & 0x7F;
    ((u8*)thisx)[0x1D3] = (((u8*)thisx)[0x1D3] - 1) & 0x7F;
}

void ObjSwitch_EyeClosed(ObjSwitch* thisx, PlayState* play) {
    s16 params = thisx->actor.params;
    switch (((u32)(u16)params << 25) >> 29) {
        case 0:
            if (!Flags_GetSwitch(play, (params >> 8) & 0x3F)) {
                ObjSwitch_SetEyeOpening(thisx);
                thisx->actor.params &= ~0x80;
            }
            break;
        case 1:
            if (ObjSwitch_EyeHit(thisx) || ((s32)((u32)params << 24) < 0)) {
                ObjSwitch_SetEyeOpening(thisx);
                ObjSwitch_SetOff(thisx, play);
                thisx->actor.params &= ~0x80;
            }
            break;
    }
}


void caseD_3(ObjSwitch* thisx, PlayState* play) {
    s32 modelIndex = ((u32)(u16)thisx->actor.params << 25) >> 29;
    if (thisx->skelAnimModel[modelIndex] != NULL) {
        FUN_003695cc(thisx->skelAnimModel[modelIndex], ObjSwitch_sCrystalAnimColors[modelIndex], 4, 0,
                     thisx->crystalR * (1.0f / 255.0f), thisx->crystalG * (1.0f / 255.0f),
                     thisx->crystalB * (1.0f / 255.0f), 128.0f / 255.0f);
        s8* row = &ObjSwitch_sTextureAnimMask[(thisx->actor.params & 7) * 5];
        if (row[modelIndex] >= 0) {
            thisx->textureAnim[modelIndex].SetAnimSpeed(0.0f);
            ObjSwitch_UpdateTextureFrame(&thisx->textureAnim[modelIndex], ((u32)thisx->crystalR * 129) >> 15);
            FUN_00373bec(&thisx->textureAnim[modelIndex]);
        }
        thisx->skelAnimModel[modelIndex]->FUN_003721e0(&thisx->actor.modelMtx);
        thisx->skelAnimModel[modelIndex]->SetUnkAC(1);
        thisx->skelAnimModel[modelIndex]->FUN_00372170(0);
    }
}
GLOBAL_ASM("../split/asm/FUN_001442ec.s")

void ObjSwitch_FloorDown(ObjSwitch* thisx, PlayState* play) {
    if ((thisx->actor.params & 0x4000) && !Flags_GetSwitch(play, (thisx->actor.params >> 8) & 0x3F)) {
        if (thisx->cooldownTimer == 1) {
            Flags_SetSwitch(play, (thisx->actor.params >> 8) & 0x3F);
        }
    } else {
        switch (((u32)thisx->actor.params << 25) >> 29) {
            case 0:
                if (!Flags_GetSwitch(play, (thisx->actor.params >> 8) & 0x3F)) {
                    ObjSwitch_SetFloorRelease(thisx);
                }
                break;
            case 1:
                if ((((u8*)thisx)[0x1B8] & 2) && !(thisx->eyeColFlags & 2)) {
                    ObjSwitch_SetFloorRelease(thisx);
                    ObjSwitch_SetOff(thisx, play);
                }
                break;
            case 2:
            case 3:
                if (!FUN_0036a83c(&thisx->actor) && !Player_InCsMode(play)) {
                    if (thisx->releaseTimer <= 0) {
                        ObjSwitch_SetFloorRelease(thisx);
                        if ((((u32)thisx->actor.params << 25) >> 29) == 2) {
                            ObjSwitch_SetOff(thisx, play);
                        } else {
                            ObjSwitch_SetOn(thisx, play);
                        }
                    }
                } else {
                    thisx->releaseTimer = 9;
                }
                break;
        }
    }
}


void ObjSwitch_FloorUp(ObjSwitch* thisx, PlayState* play) {
    s16 params = thisx->actor.params;
    if ((params & 7) == 1) {
        s32 allowed = 1;
        if (play->sceneNum == 4 && *(u8*)((u8*)play + 0x4C30) == 11 && gSaveContext.masterQuestFlag == 0) {
            Actor* nearby[5];
            s32 count = FUN_00360084(&play->actorCtx, 0x1D6, 7, nearby, 5);
            for (s32 i = 0; i < count; i++) {
                if (nearby[i] != NULL && nearby[i]->draw != NULL) {
                    Vec3f* pos = &nearby[i]->world.pos;
                    f32 dx = thisx->actor.world.pos.x - pos->x;
                    f32 dy = thisx->actor.world.pos.y - pos->y;
                    f32 dz = thisx->actor.world.pos.z - pos->z;
                    if ((dx * dx + dy * dy + dz * dz) < 2500.0f) {
                        allowed = 0;
                        break;
                    }
                }
            }
        }
        if ((thisx->eyeHitFlags & 2) && allowed) {
            ObjSwitch_SetFloorPress(thisx);
            ObjSwitch_SetOn(thisx, play);
            thisx->eyeHitFlags &= ~2;
        } else {
            CollisionCheck_SetAC(play, &play->colChkCtx, (Collider*)((u8*)thisx + 0x1D8));
        }
    } else {
        switch (((u32)(u16)params << 25) >> 29) {
            case 0:
                if (FUN_0036adf4(&thisx->actor)) {
                    ObjSwitch_SetFloorPress(thisx);
                    ObjSwitch_SetOn(thisx, play);
                }
                break;
            case 1:
                if ((((u8*)thisx)[0x1B8] & 2) && !(thisx->eyeColFlags & 2)) {
                    ObjSwitch_SetFloorPress(thisx);
                    ObjSwitch_SetOn(thisx, play);
                }
                break;
            case 2:
                if (FUN_0036a83c(&thisx->actor)) {
                    ObjSwitch_SetFloorPress(thisx);
                    ObjSwitch_SetOn(thisx, play);
                }
                break;
            case 3:
                if (FUN_0036a83c(&thisx->actor)) {
                    ObjSwitch_SetFloorPress(thisx);
                    ObjSwitch_SetOff(thisx, play);
                }
                break;
        }
    }
}



void ObjSwitch_InitTrisCollider(ObjSwitch* thisx, PlayState* play, ObjSwitchColliderTrisInit* init) {
    s32 i;
    s32 j;
    Vec3f pos[3];
    Collider_InitTris(play, (ObjSwitchColliderTris*)((u8*)thisx + 0x1D8));
    Collider_SetTris(play, (ObjSwitchColliderTris*)((u8*)thisx + 0x1D8), &thisx->actor, init, (ObjSwitchColliderTrisElement*)((u8*)thisx + 0x1F8));
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            ObjSwitch_RotateY(&pos[j], thisx->actor.home.rot.y, &init->elements[i].dim.vtx[j]);
            ObjSwitch_AddVec(&pos[j], &thisx->actor.world.pos, &pos[j]);
        }
        Collider_SetTrisVertices((ObjSwitchColliderTris*)((u8*)thisx + 0x1D8), i, &pos[0], &pos[1], &pos[2]);
    }
}






}
