extern "C" {
#include "z_obj_switch.hpp"

s32 FUN_0036ae04(void);
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

#if 0 // Not matched: best draft 40/56 words; keep the original ASM below.
void ObjSwitch_Destroy(Actor* actor, GameState* state) {
    ObjSwitch* thisx = (ObjSwitch*)actor;
    PlayState* play = (PlayState*)state;
    s32 subtype = actor->params & 7;

    if (subtype == 1) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, *(s32*)((u8*)actor + 0x1A4));
    }
    if (subtype == 1 || subtype == 2) {
        Collider_DestroyTris(play, (u8*)actor + 0x1D8);
    } else if (subtype == 3 || subtype == 4) {
        Collider_DestroyJntSph(play, (u8*)actor + 0x1D8);
    }
    FUN_003685a0((u8*)thisx + 0x2C4);
    FUN_003685a0((u8*)thisx + 0x35C);
    FUN_003685a0((u8*)thisx + 0x3F4);
    FUN_003685a0((u8*)thisx + 0x48C);
    FUN_003685a0((u8*)thisx + 0x524);
    FUN_00350f34(actor, &thisx->skelAnimModel[0], &thisx->skelAnimModel[1], &thisx->skelAnimModel[2],
                         &thisx->skelAnimModel[3], &thisx->skelAnimModel[4], &thisx->skelAnimModel[5], 0);
}
#endif
GLOBAL_ASM("asm/ObjSwitch_Destroy.s")

#if 0 // Luna batch3 incomplete draft, only 44/436 B compiled; switch/table gate unresolved
void ObjSwitch_Update(Actor* actor, GameState* state) {
    ObjSwitch* thisx = (ObjSwitch*)actor;
    PlayState* play = (PlayState*)state;
    if (thisx->releaseTimer > 0) thisx->releaseTimer--;
    if (thisx->cooldownTimer > 0) thisx->cooldownTimer--;
    thisx->actionFunc(thisx, play);
}
#endif
GLOBAL_ASM("asm/ObjSwitch_Update.s")

#if 0 // Luna batch3 draft: best 84/93 words differ
void ObjSwitch_CrystalOff(ObjSwitch* thisx, PlayState* play) {
    s32 params = thisx->actor.params;
    s32 type = ((u32)(u16)params << 25) >> 29;
    s32 activate = 0;

    if (type == 0) {
        if ((play->sceneNum == 13 && *(u8*)((u8*)play + 0x500C) == 0x12) ||
            (play->sceneNum == 6 && *(u8*)((u8*)play + 0x500C) == 0x19)) {
            if (Player_InCsMode(play)) return;
        }
        if ((thisx->eyeHitFlags & 2) && thisx->cooldownTimer <= 0) activate = 1;
    } else if (type == 1) {
        if ((thisx->eyeHitFlags & 2) && !(thisx->eyeColFlags & 2) && thisx->cooldownTimer <= 0) activate = 1;
    } else if (type == 4) {
        if (((thisx->eyeHitFlags & 2) && thisx->cooldownTimer > 0) ||
            Flags_GetSwitch(play, ((u16)params >> 8) & 0x3F)) activate = 1;
    }
    if (activate) {
        thisx->actionFunc = ObjSwitch_CrystalTurnOn;
        thisx->cooldownTimer = 150;
        ObjSwitch_SetOn(thisx, play);
    }
    if (type == 1) {
        ((u8*)thisx)[0x1D0] = (((u8*)thisx)[0x1D0] - 1) & 0x7F;
        ((u8*)thisx)[0x1D1] = (((u8*)thisx)[0x1D1] + 1) & 0x7F;
        ((u8*)thisx)[0x1D2] = (((u8*)thisx)[0x1D2] + 1) & 0x7F;
        ((u8*)thisx)[0x1D3] = (((u8*)thisx)[0x1D3] - 1) & 0x7F;
    }
}
#endif
GLOBAL_ASM("asm/ObjSwitch_CrystalOff.s")

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

#if 0 // Not matched: best draft 12/47 words; keep the original ASM in functions_140000s.cpp.
void caseD_2_0016ea9c(Actor* actor, GameState* state) {
    ObjSwitch* thisx = (ObjSwitch*)actor;
    s16 params = thisx->actor.params;
    s32 modelIndex = ((u32)(u16)params << 25) >> 29;
    u8* modelSlot = (u8*)thisx + modelIndex * 4;
    void* anim;
    if (*(SkeletonAnimationModel**)(modelSlot + 0x2B0) == NULL) {
        return;
    }
    s8* row = &ObjSwitch_sTextureAnimMask[(params & 7) * 5];
    if (row[modelIndex] >= 0) {
        anim = (u8*)thisx + modelIndex * 0x98;
        *(f32*)((u8*)anim + 0x2D0) = 0.0f;
        s16 param = *(s16*)((u8*)thisx + 0x1C8);
        f32 paramFloat = (f32)param;
        if (gGameInfo == NULL) {
            *(f32*)((u8*)anim + 0x2CC) = paramFloat;
            FUN_003586ec((u8*)anim + 0x2C4);
        }
        FUN_00373bec((u8*)anim + 0x2C4);
    }
    (*(SkeletonAnimationModel**)(modelSlot + 0x2B0))->FUN_003721e0(&thisx->actor.modelMtx);
    (*(SkeletonAnimationModel**)(modelSlot + 0x2B0))->SetUnkAC(1);
    (*(SkeletonAnimationModel**)(modelSlot + 0x2B0))->FUN_00372170(0);
}
#endif

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

#if 0 // Not matched: best draft 44/52 words; keep the original ASM below.
void ObjSwitch_EyeClosed(ObjSwitch* thisx, PlayState* play) {
    s32 subtype = ((u32)(u16)thisx->actor.params << 25) >> 29;
    if (subtype == 0) {
        if (!Flags_GetSwitch(play, ((u16)thisx->actor.params >> 8) & 0x3F)) {
            thisx->actionFunc = ObjSwitch_EyeOpening;
            thisx->cooldownTimer = 150;
            thisx->actor.params &= ~0x80;
        }
    } else if (subtype == 1) {
        Actor* hit = thisx->eyeHitActor;
        s32 yawDiff = hit ? (s16)(hit->world.rot.y - thisx->actor.shape.rot.y) : 0;
        if (((thisx->eyeHitFlags & 2) && !(thisx->eyeColFlags & 2) && hit && (u32)(yawDiff + 0x5000) <= 0xA000) ||
            ((s32)((u32)(u16)thisx->actor.params << 24) < 0)) {
            thisx->actionFunc = ObjSwitch_EyeOpening;
            thisx->cooldownTimer = 150;
            ObjSwitch_SetOff(thisx, play);
            thisx->actor.params &= ~0x80;
        }
    }
}
#endif
GLOBAL_ASM("asm/ObjSwitch_EyeClosed.s")

#if 0 // Not matched: best draft 23/71 words; keep the original ASM in functions_180000s.cpp.
void caseD_3(ObjSwitch* thisx, PlayState* play) {
    s32 modelIndex = ((u32)(u16)thisx->actor.params << 25) >> 29;
    u8* modelSlot = (u8*)thisx + modelIndex * 4;
    void* anim;
    if (*(SkeletonAnimationModel**)(modelSlot + 0x2B0) == NULL) {
        return;
    }
    FUN_003695cc(*(SkeletonAnimationModel**)(modelSlot + 0x2B0), ObjSwitch_sCrystalAnimColors[modelIndex], 4, 0,
                 ((u8*)thisx)[0x1D4] * (1.0f / 255.0f),
                 ((u8*)thisx)[0x1D5] * (1.0f / 255.0f), ((u8*)thisx)[0x1D6] * (1.0f / 255.0f), 0.5019608f);
    s8* row = &ObjSwitch_sTextureAnimMask[((u16)thisx->actor.params & 7) * 5];
    if (row[modelIndex] >= 0) {
        anim = (u8*)thisx + modelIndex * 0x98;
        *(f32*)((u8*)anim + 0x2D0) = 0.0f;
        u32 alphaInt = (((u8*)thisx)[0x1D4] * 129) >> 15;
        f32 alpha = (f32)alphaInt;
        if (gGameInfo == NULL) {
            *(f32*)((u8*)anim + 0x2CC) = alpha;
            FUN_003586ec((u8*)anim + 0x2C4);
        }
        FUN_00373bec((u8*)anim + 0x2C4);
    }
    (*(SkeletonAnimationModel**)(modelSlot + 0x2B0))->FUN_003721e0(&thisx->actor.modelMtx);
    (*(SkeletonAnimationModel**)(modelSlot + 0x2B0))->SetUnkAC(1);
    (*(SkeletonAnimationModel**)(modelSlot + 0x2B0))->FUN_00372170(0);
}
#endif

GLOBAL_ASM("../split/asm/FUN_001442ec.s")

#if 0 // Luna batch3 best draft: 60/81 words differ
void ObjSwitch_FloorDown(ObjSwitch* thisx, PlayState* play) {
    s32 params = thisx->actor.params;
    s32 type = ((u32)(u16)params << 25) >> 29;

    if ((params & 0x4000) && !Flags_GetSwitch(play, ((u16)params >> 8) & 0x3F)) {
        if (type == 1) {
            Flags_SetSwitch(play, ((u16)params >> 8) & 0x3F);
            return;
        }
    } else {
        if (type == 0) {
            if (!Flags_GetSwitch(play, ((u16)params >> 8) & 0x3F)) {
                thisx->actionFunc = ObjSwitch_FloorDown;
                thisx->releaseTimer = 150;
            }
            return;
        }
        if (type == 1) {
            if (!(thisx->eyeHitFlags & 2)) return;
            if (thisx->eyeColFlags & 2) return;
        } else if (type == 2 || type == 3) {
            if (!FUN_0036a83c(&thisx->actor) && !Player_InCsMode(play) && thisx->cooldownTimer <= 0) {
                thisx->releaseTimer = 9;
                return;
            }
        } else {
            return;
        }
    }
    ObjSwitch_SetOn(thisx, play);
}
#endif
GLOBAL_ASM("asm/ObjSwitch_FloorDown.s")

GLOBAL_ASM("asm/ObjSwitch_FloorUp.s")

#if 0 // Not matched: best draft 56/73 words; keep the original ASM below.
void ObjSwitch_InitTrisCollider(ObjSwitch* thisx, PlayState* play, ObjSwitchColliderTrisInit* init) {
    ObjSwitchColliderTris* collider = (ObjSwitchColliderTris*)((u8*)thisx + 0x1D8);
    ObjSwitchColliderTrisElement* elements = (ObjSwitchColliderTrisElement*)((u8*)thisx + 0x1F8);
    Vec3f vertices[3];
    s32 i;
    s32 j;
    Collider_InitTris(play, collider);
    Collider_SetTris(play, collider, &thisx->actor, init, elements);
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            Vec3f* src = &init->elements[i].dim.vtx[j];
            f32 s = Math_SinS(thisx->actor.home.rot.y);
            f32 c = Math_CosS(thisx->actor.home.rot.y);
            vertices[j].x = src->z * s + src->x * c + thisx->actor.world.pos.x;
            vertices[j].y = src->y + thisx->actor.world.pos.y;
            vertices[j].z = src->z * c - src->x * s + thisx->actor.world.pos.z;
        }
        Collider_SetTrisVertices(collider, i, &vertices[0], &vertices[1], &vertices[2]);
    }
}
#endif
GLOBAL_ASM("asm/ObjSwitch_InitTrisCollider.s")

GLOBAL_ASM("asm/ObjSwitch_SetOff.s")

GLOBAL_ASM("asm/ObjSwitch_SetOn.s")

}
