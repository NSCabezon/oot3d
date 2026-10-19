extern "C" {
#include "z_demo_kankyo.hpp"

#define FLAGS 0x00000030
#define R_UPDATE_RATE SREG(30)
#define DEMOKANKYO_FRAMES(n) ((s32)((n) * 3.0f / R_UPDATE_RATE + 0.5f))
s32 Flags_GetEnv(PlayState* play, s32 flag);

void FUN_0034fc7c(void* p);
void SystemArena_Free(void* p);
f32 FUN_00361490(u16 endFrame, u16 startFrame, u16 curFrame);
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 index);
void* SystemArena_Malloc_0035010c(s32 size);
void* FUN_003500c4(void* p);
void FUN_0034ff2c(void* obj, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void FUN_0034fea8(void* obj, s32 bank, PlayState* play, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);
void DemoKankyo_SetupType(DemoKankyo* self, PlayState* play);
void DemoKankyo_UpdateClouds(DemoKankyo* self, PlayState* play);
void DemoKankyo_UpdateDoorOfTime(DemoKankyo* self, PlayState* play);
void DemoKankyo_UpdateRock(DemoKankyo* self, PlayState* play);

struct DemoKankyoSharedState {
    s16 rainCount;
    s16 rainScale;
};
extern DemoKankyoSharedState DemoKankyo_sharedState;
extern s16 DemoKankyo_objectIds[19];

// Room model0x4C3C and draw parameter0x53F2 are used by Init.
struct DemoKankyoRoomModel { char unk_00[0xAD]; u8 drawEnabled; };
struct DemoKankyoRoomView {
    char unk_00[0xC];
    DemoKankyoRoomModel* model;
    char unk_10[0x7B2];
    u16 drawParam;
};

static inline void DemoKankyo_HideRoom(PlayState* play) {
    DemoKankyoRoomModel* model = ((DemoKankyoRoomView*)&play->roomCtx)->model;
    if (model != NULL) {
        model->drawEnabled = 0;
    }
}
GLOBAL_ASM("../src/game/actors/Demo_Kankyo/data_syms.s")

void DemoKankyo_Init(Actor* actor, GameState* state);
void DemoKankyo_Destroy(Actor* actor, GameState* state);
void DemoKankyo_Update(Actor* actor, GameState* state);
void DemoKankyo_Draw(DemoKankyo* self, PlayState* play);

GLOBAL_ASM("data/z_Demo_Kankyo.data.s")

ActorInit Demo_Kankyo_InitVars = {
    ACTOR_DEMO_KANKYO,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(DemoKankyo),
    (ActorFunc)DemoKankyo_Init,
    (ActorFunc)DemoKankyo_Destroy,
    (ActorFunc)DemoKankyo_Update,
    (ActorFunc)DemoKankyo_Draw,
};

#if 0
// Best Sol-direct draft:35/300 words;1200B, same size as target.
void DemoKankyo_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    DemoKankyo* self = (DemoKankyo*)actor;
    s32 objectSlot = Object_GetIndex(&play->objectCtx, DemoKankyo_objectIds[self->actor.params]);
    if (objectSlot >= 0) {
        self->requiredObjectSlot = objectSlot;
    }
    s16 i;
    s32 zero = 0;
    for (i = 0; i < 150; i++) {
        self->effects[i].mode = zero;
        self->models[i] = (SkeletonAnimationModel*)zero;
    }
    self->effectResource = (void*)zero;
    self->resourcesLoaded = zero;
    switch (self->actor.params) {
        case 0:
        case 1:
            switch (play->sceneNum) {
                case 0x47:
                    DemoKankyo_HideRoom(play);
                    DemoKankyo_sharedState.rainCount = 10;
                    DemoKankyo_sharedState.rainScale = 8;
                    break;
                case 0x43:
                    DemoKankyo_sharedState.rainCount = 14;
                    DemoKankyo_sharedState.rainScale = 8;
                    break;
                case 0x51:
                    DemoKankyo_sharedState.rainCount = 1;
                    DemoKankyo_sharedState.rainScale = 5;
                    break;
                default:
                    Actor_Kill(actor);
                    break;
            }
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            DemoKankyo_HideRoom(play);
            actor->scale.x = actor->scale.y = actor->scale.z = Rand_ZeroOne() * 0.5f + 0.5f;
            self->effects[0].velocity.x = Rand_ZeroOne() * 3.0f + 1.0f;
            self->effects[0].velocity.y = Rand_ZeroOne() * 3.0f + 1.0f;
            self->effects[0].velocity.z = Rand_ZeroOne() * 3.0f + 1.0f;
            break;
        case 7:
            for (i = 0; i < 30; i++) {
                self->effects[i].angle = Rand_ZeroOne() * 65535.0f;
                self->effects[i].translation = Rand_ZeroOne() * 100.0f + 60.0f;
            }
            break;
        case 13:
            self->actor.flags |= 0x400000;
            actor->scale.x = actor->scale.y = actor->scale.z = 1.0f;
            self->effects[0].translation = 0.0f;
            if (!(gSaveContext.eventChkInf[4] & 0x800)) {
                PlayState* play2 = play;
                Actor_SpawnAsChild(&play2->actorCtx, actor, play2, ACTOR_DOOR_TOKI,
                    actor->world.pos.x, actor->world.pos.y, actor->world.pos.z, 0, 0, 0, 0);
            } else {
                ((DemoKankyoRoomView*)&play->roomCtx)->drawParam = 0xFF;
                Actor_Kill(actor);
            }
            break;
        case 14:
            actor->flags |= 0x400000;
            actor->scale.x = actor->scale.y = actor->scale.z = 1.0f;
            self->effects[0].translation = 0.0f;
            break;
        case 15:
        case 16:
            Actor_ChangeType(play, &play->actorCtx, actor, ACTORCAT_ITEMACTION);
            actor->flags |= 0x2000000;
            self->actor.room = -1;
            self->warpTimer = (s32)(105.0f / R_UPDATE_RATE + 0.5f);
            actor->scale.x = actor->scale.y = actor->scale.z = 1.0f;
            self->sparkleCounter = zero;
            if (self->actor.params == 15) {
                Audio_PlaySoundGeneral(0x0100024B, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            }
            break;
        case 17:
        case 18: {
            self->warpTimer = (s32)(105.0f / R_UPDATE_RATE + 0.5f);
            actor->scale.x = actor->scale.y = actor->scale.z = 1.0f;
            self->sparkleCounter = zero;
            self->actor.objBankIndex = self->requiredObjectSlot;
            void* obj = SystemArena_Malloc_0035010c(0x28);
            if (obj != NULL) {
                obj = FUN_003500c4(obj);
            }
            self->effectResource = obj;
            FUN_0034ff2c(self->effectResource, 1, 1, 20, 2, 0);
            FUN_0034fea8(self->effectResource, self->actor.objBankIndex, play, 2, 0x2601, 0x2601, 0x812F, 0x812F);
            self->resourcesLoaded = 1;
            break;
        }
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
            break;
    }
    self->actionFunc = DemoKankyo_SetupType;
}

#endif
GLOBAL_ASM("asm/DemoKankyo_Init.s")

void DemoKankyo_Destroy(Actor* actor, GameState* state) {
    DemoKankyo* self = (DemoKankyo*)actor;
    s32 i;
    if (self->resourcesLoaded) {
        switch (actor->params) {
            case 0:
            case 1:
                for (i = 0; i < 150; i++) {
                    FUN_00350f34(actor, &self->models[i], 0);
                }
                break;
            case 13:
                FUN_00350f34(actor, &self->models[0], &self->models[1], 0);
                break;
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 14:
                FUN_00350f34(actor, &self->models[0], 0);
                break;
            case 7:
                for (i = 0; i < 30; i++) {
                    FUN_00350f34(actor, &self->models[i], 0);
                }
                break;
            case 15:
            case 16:
            case 17:
            case 18:
                if (self->effectResource != NULL) {
                    FUN_0034fc7c(self->effectResource);
                    SystemArena_Free(self->effectResource);
                }
                self->effectResource = NULL;
                break;
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
                break;
        }
    }
}

void DemoKankyo_Update(Actor* actor, GameState* state) {
    DemoKankyo* self = (DemoKankyo*)actor;
    self->actionFunc(self, (PlayState*)state);
}

#if 0
// Best Sol-direct draft:300/471 words;1848B vs1884B target.
#include "mtx34_trans.hpp"
void FUN_0020c728(Actor* actor, PlayState* play);
void FUN_001d27b0(Actor* actor, PlayState* play);
void FUN_001ac538(Actor* actor, PlayState* play);
void Matrix_RotateX(MTX34* mtx, s32 mode, f32 angle);
void Matrix_RotateZ(MTX34* mtx, s32 mode, f32 angle);
void FUN_00371fac(MTX34* mtx, MTX34* rotation);
f32 __hardfp_sinf(f32 angle);
f32 __hardfp_cosf(f32 angle);
struct DemoKankyoBillboardView { char unk_00[0x2FC]; MTX34 billboardMtx; };

static inline void DemoKankyo_Scale(MTX34* m, f32 x, f32 y, f32 z) {
    m->data[0][0] *= x;
    m->data[1][0] *= x;
    m->data[2][0] *= x;
    m->data[0][1] *= y;
    m->data[1][1] *= y;
    m->data[2][1] *= y;
    m->data[0][2] *= z;
    m->data[1][2] *= z;
    m->data[2][2] *= z;
}

static inline void DemoKankyo_RotateY(MTX34* m, f32 angle) {
    if (angle != 0.0f) {
        s32 i;
        f32 sinA = __hardfp_sinf(angle);
        f32 cosA = __hardfp_cosf(angle);
        for (i = 0; i < 3; i++) {
            f32 x = m->data[i][0];
            f32 z = m->data[i][2];
            m->data[i][0] = x * cosA - z * sinA;
            m->data[i][2] = x * sinA + z * cosA;
        }
    }
}

static inline void DemoKankyo_DrawRock(DemoKankyo* self, PlayState* play) {
    MTX34 mtx;
    Vec3f pos;
                if (self->effects[0].angle != 0) {
                    pos.x = self->actor.world.pos.x;
                    pos.y = self->actor.world.pos.y;
                    pos.z = self->actor.world.pos.z;
                    MTX34_MakeTrans(&mtx, &pos);
                    Matrix_RotateX(&mtx, 1, self->effects[0].offset.x * (M_PI / 180.0f));
                    DemoKankyo_RotateY(&mtx, self->effects[0].offset.y * (M_PI / 180.0f));
                    Matrix_RotateZ(&mtx, 1, self->effects[0].offset.z * (M_PI / 180.0f));
                    DemoKankyo_Scale(&mtx, self->actor.scale.x, self->actor.scale.y, self->actor.scale.z);
                    self->models[0]->unk_AC = 1;
                    self->models[0]->FUN_003721e0(&mtx);
                    FUN_003695cc(self->models[0], 0, 4, 2, 0.0f, 0.0f, 0.0f, self->effects[0].angle / 255.0f);
                    self->models[0]->FUN_00372170(0);
                }
}

static inline void DemoKankyo_DrawClouds(DemoKankyo* self, PlayState* play) {
    MTX34 mtx;
    Vec3f pos;
    s16 i;
                for (i = 0; i < 30; i++) {
                    f32 dx;
                    f32 dy;
                    f32 dz;
                    // Original cloud trig evaluations are retained by RVCT even
                    // though the 3DS path replaces their arithmetic below.
                    dx = -(Math_SinS(self->effects[i].angle - 0x8000) * 120.0f) * (30.0f + (i / 30.0f) * 10.0f);
                    dy = Math_CosS(self->effects[i].angle - 0x8000) * 5.0f + 1200.0f;
                    dz = (Math_CosS(self->effects[i].angle - 0x8000) * 120.0f) * (30.0f + (i / 30.0f) * 10.0f);
                    dx = -(Math_SinS(self->effects[i].angle - 0x8000) * 120.0f) * (30.0f + (i / 30.0f) * 10.0f);
                    dy = Math_CosS(self->effects[i].angle - 0x8000) * 5.0f + 1200.0f;
                    dz = (Math_CosS(self->effects[i].angle - 0x8000) * 120.0f) * (30.0f + (i / 30.0f) * 10.0f);
                    pos.x = play->view.eye.x + dx;
                    pos.y = play->view.eye.y + dy + (0.0f * (i - 12.0f));
                    pos.z = play->view.eye.z + dz;
                    MTX34_MakeTrans(&mtx, &pos);
                    mtx.data[0][0] *= 125.0f;
                    mtx.data[1][0] *= 125.0f;
                    mtx.data[2][0] *= 125.0f;
                    mtx.data[0][1] *= 60.0f;
                    mtx.data[1][1] *= 60.0f;
                    mtx.data[2][1] *= 60.0f;
                    mtx.data[0][2] *= 125.0f;
                    mtx.data[1][2] *= 125.0f;
                    mtx.data[2][2] *= 125.0f;
                    FUN_00371fac(&mtx, &((DemoKankyoBillboardView*)play)->billboardMtx);
                    self->models[i]->unk_AC = 1;
                    self->models[i]->FUN_003721e0(&mtx);
                    self->models[i]->FUN_00372170(0);
                }
}

static inline void DemoKankyo_DrawDoorOfTime(DemoKankyo* self, PlayState* play) {
    MTX34 mtx;
    Vec3f pos;
                MTX34CopyAsm(&mtx, &self->actor.modelMtx);
                pos.x = -self->effects[0].translation;
                pos.y = pos.z = 0.0f;
                Matrix_UpdatePosition(&mtx, &mtx, &pos);
                self->models[0]->unk_AC = 1;
                self->models[0]->FUN_003721e0(&mtx);
                self->models[0]->FUN_00372170(0);
                pos.x = self->effects[0].translation * 2.0f;
                pos.y = pos.z = 0.0f;
                Matrix_UpdatePosition(&mtx, &mtx, &pos);
                self->models[1]->unk_AC = 1;
                self->models[1]->FUN_003721e0(&mtx);
                self->models[1]->FUN_00372170(0);
}

static inline void DemoKankyo_DrawLightPlane(DemoKankyo* self, PlayState* play) {
                if (play->GetCutsceneState() != 0 && gSaveContext.sceneSetupIndex < 4) {
                    return;
                }
                f32 animSpeed = 1.0f;
                if (PauseContext_GetState() != 0) {
                    animSpeed = 0.0f;
                }
                self->models[0]->unk_AC = 1;
                self->models[0]->FUN_003721e0(&self->actor.modelMtx);
                self->models[0]->unk_0C->animSpeed = animSpeed;
                self->models[0]->FUN_00372170(0);
}
void DemoKankyo_Draw(DemoKankyo* self, PlayState* play) {
    Actor* actor = &self->actor;
    if (self->actor.objBankIndex == self->requiredObjectSlot) {
        switch (self->actor.params) {
            case 0:
            case 1:
                if (play->sceneNum == 0x43) {
                    if (!Flags_GetEnv(play, 1)) {
                        break;
                    } else if (!Actor_IsFacingAndNearPlayer(&self->actor, 300.0f, 0x7530)) {
                        break;
                    } else {
                        if (gSaveContext.linkAge != 0) {
                            if (play->csCtx.frames < DEMOKANKYO_FRAMES(170) || play->GetCutsceneState() == 0) {
                                break;
                            }
                        } else {
                            if (play->csCtx.frames < DEMOKANKYO_FRAMES(120) || play->GetCutsceneState() == 0) {
                                break;
                            }
                        }
                    }
                }
                FUN_0020c728(actor, play);
                break;
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
                DemoKankyo_DrawRock(self, play);
                break;
            case 7:
                DemoKankyo_DrawClouds(self, play);
                break;
            case 13:
                DemoKankyo_DrawDoorOfTime(self, play);
                break;
            case 14:
                DemoKankyo_DrawLightPlane(self, play);
                break;
            case 15:
            case 16:
                FUN_001d27b0(actor, play);
                break;
            case 17:
            case 18:
                FUN_001ac538(actor, play);
                break;
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
                break;
        }
    }
}


#endif
GLOBAL_ASM("asm/DemoKankyo_Draw.s")

void DemoKankyo_DoNothing(DemoKankyo* self, PlayState* play) {}

void DemoKankyo_DoNothing2(DemoKankyo* self, PlayState* play) {
    self->actionFunc = DemoKankyo_DoNothing;
}

void DemoKankyo_KillDoorOfTimeCollision(DemoKankyo* self, PlayState* play) {
    Actor_Kill(self->actor.child);
}

#if 0
// Best Sol-direct draft:290/604 words;2396B vs2416B target.
// Fields absent from the shared PlayState declaration; offsets verified from
// SetupType, which writes the screen fill at0x3261 and warp cue timer0x7F52.
struct DemoKankyoPlayView {
    char unk_00[0x3261];
    u8 fillScreen;
    u8 screenFillColor[4];
    char unk_3266[0x4CEC];
    s16 warpCueTimer;
};
struct DemoKankyoObjectContextView {
    u8 count;
    u8 unk_01;
    u8 mainKeepSlot;
};
static inline ZARInfo* DemoKankyo_GetKeepZar(PlayState* play) {
    ObjectStatus* object = &play->objectCtx.status[((DemoKankyoObjectContextView*)&play->objectCtx)->mainKeepSlot];
    return object->unk_08 != 0 ? &object->zarInfo : NULL;
}
s32 Flags_GetEnv(PlayState* play, s32 flag);
s32 FUN_00334370(PlayState* play);
s32 FUN_00296eb4(PlayState* play, Player* player);

#define DEMOKANKYO_FRAMES(n) ((s32)((n) * 3.0f / R_UPDATE_RATE + 0.5f))
#define DemoKankyo_N64Frames(n) ((n) > 0 ? (s32)(0.5f + (f32)(n) * R_UPDATE_RATE / 3.0f) : (s32)((f32)(n) * R_UPDATE_RATE / 3.0f - 0.5f))

void DemoKankyo_SetupType(DemoKankyo* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    DemoKankyoPlayView* view = (DemoKankyoPlayView*)play;
    s32 i;
    if (Object_IsLoaded(&play->objectCtx, self->requiredObjectSlot)) {
        self->actor.objBankIndex = self->requiredObjectSlot;
    }
    if (self->actor.objBankIndex == self->requiredObjectSlot) {
        self->resourcesLoaded = 1;
        switch (self->actor.params) {
            case 0:
            case 1:
                if (self->models[0] == NULL) {
                    void* cmab;
                    ZARInfo* zar = &play->GetObjectStatus(self->actor.objBankIndex)->zarInfo;
                    if (gSaveContext.entranceIndex == 0xCD) {
                        cmab = zar->GetCMABByIndex(1);
                    } else {
                        cmab = zar->GetCMABByIndex(0);
                    }
                    for (i = 0; i < 150; i++) {
                        FUN_00372f38(&self->actor, play, &self->models[i], 4, 0);
                        self->models[i]->unk_0C->FUN_00372d94(cmab);
                        self->models[i]->unk_0C->animType = 1;
                        self->models[i]->unk_0C->animSpeed = 2.0f;
                    }
                }
                break;
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
                self->actionFunc = DemoKankyo_UpdateRock;
                FUN_00372f38(&self->actor, play, &self->models[0], Rand_ZeroOne() < 0.5f ? 1 : 0, 0);
                break;
            case 7:
                self->actionFunc = DemoKankyo_UpdateClouds;
                for (i = 0; i < 30; i++) {
                    FUN_00372f38(&self->actor, play, &self->models[i], (s32)Rand_ZeroFloat(2.0f) % 2 + 5, 0);
                }
                break;
            case 13:
                if (Flags_GetEnv(play, 2)) {
                    self->actionFunc = DemoKankyo_UpdateDoorOfTime;
                }
                if (self->models[0] == NULL) {
                    FUN_00372f38(&self->actor, play, &self->models[0], 3, &self->models[1], 6, 0);
                }
                break;
            case 14:
                if (self->models[0] == NULL) {
                    ZARInfo* zar = FUN_00372f38(&self->actor, play, &self->models[0], 8, 0);
                    self->models[0]->unk_0C->FUN_00372d94(zar->GetCMABByIndex(1));
                    self->models[0]->unk_0C->animType = 1;
                }
                break;
            case 15:
                view->screenFillColor[0] = 255;
                view->screenFillColor[1] = 255;
                view->screenFillColor[2] = 255;
                view->fillScreen = 0;
                if (self->warpTimer <= DEMOKANKYO_FRAMES(20) && self->warpTimer >= DEMOKANKYO_FRAMES(15)) {
                    f32 temp = (DemoKankyo_N64Frames(self->warpTimer) - 15.0f) / 5.0f;
                    view->fillScreen = 1;
                    view->screenFillColor[3] = 255 - 255 * temp;
                }
                if (self->warpTimer <= DEMOKANKYO_FRAMES(14) && self->warpTimer >= DEMOKANKYO_FRAMES(4)) {
                    f32 temp = (DemoKankyo_N64Frames(self->warpTimer) - 4.0f) / 10.0f;
                    view->fillScreen = 1;
                    view->screenFillColor[3] = 255 * temp;
                }
                if (self->warpTimer == DEMOKANKYO_FRAMES(20)) {
                    player->actor.draw = NULL;
                }
                if ((u32)self->warpTimer != 0) {
                    self->warpTimer--;
                    if (self->warpTimer == 1) {
                        if (play->sceneNum == 0x43) {
                            view->warpCueTimer = DEMOKANKYO_FRAMES(25);
                            if (gSaveContext.linkAge != 0) {
                                play->FUN_0037573c(DemoKankyo_GetKeepZar(play)->GetQDBByIndex(8));
                            } else {
                                play->FUN_0037573c(DemoKankyo_GetKeepZar(play)->GetQDBByIndex(4));
                            }
                        } else {
                            view->warpCueTimer = DEMOKANKYO_FRAMES(32);
                            if (gSaveContext.linkAge != 0) {
                                play->FUN_0037573c(DemoKankyo_GetKeepZar(play)->GetQDBByIndex(6));
                            } else {
                                play->FUN_0037573c(DemoKankyo_GetKeepZar(play)->GetQDBByIndex(2));
                            }
                        }
                        if (FUN_00334370(play) && FUN_00296eb4(play, player)) {
                            gSaveContext.cutsceneTrigger = 1;
                        }
                        self->actionFunc = DemoKankyo_DoNothing;
                    }
                }
                if (self->effectResource == NULL) {
                    void* obj = SystemArena_Malloc_0035010c(0x28);
                    if (obj != NULL) {
                        obj = FUN_003500c4(obj);
                    }
                    self->effectResource = obj;
                    FUN_0034ff2c(self->effectResource, 1, 1, 30, 2, 0);
                    FUN_0034fea8(self->effectResource, self->actor.objBankIndex, play, 2, 0x2601, 0x2601, 0x812F, 0x812F);
                }
                break;
            case 16:
                if (play->sceneNum == 0x43) {
                    if (gSaveContext.linkAge != 0) {
                        play->FUN_0037573c(DemoKankyo_GetKeepZar(play)->GetQDBByIndex(9));
                    } else {
                        play->FUN_0037573c(DemoKankyo_GetKeepZar(play)->GetQDBByIndex(5));
                    }
                } else {
                    if (gSaveContext.linkAge != 0) {
                        play->FUN_0037573c(DemoKankyo_GetKeepZar(play)->GetQDBByIndex(7));
                    } else {
                        play->FUN_0037573c(DemoKankyo_GetKeepZar(play)->GetQDBByIndex(3));
                    }
                }
                gSaveContext.cutsceneTrigger = 1;
                self->actionFunc = DemoKankyo_DoNothing2;
                if (self->effectResource == NULL) {
                    void* obj = SystemArena_Malloc_0035010c(0x28);
                    if (obj != NULL) {
                        obj = FUN_003500c4(obj);
                    }
                    self->effectResource = obj;
                    FUN_0034ff2c(self->effectResource, 1, 1, 30, 2, 0);
                    FUN_0034fea8(self->effectResource, self->actor.objBankIndex, play, 2, 0x2601, 0x2601, 0x812F, 0x812F);
                }
                break;
            case 17:
            case 18:
                if (self->effectResource == NULL) {
                    void* obj = SystemArena_Malloc_0035010c(0x28);
                    if (obj != NULL) {
                        obj = FUN_003500c4(obj);
                    }
                    self->effectResource = obj;
                    FUN_0034ff2c(self->effectResource, 1, 1, 20, 2, 0);
                    FUN_0034fea8(self->effectResource, self->actor.objBankIndex, play, 2, 0x2601, 0x2601, 0x812F, 0x812F);
                }
                break;
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
                break;
        }
    }
}


#endif
GLOBAL_ASM("asm/DemoKankyo_SetupType.s")

void DemoKankyo_UpdateClouds(DemoKankyo* self, PlayState* play) {
    u8 i;
    for (i = 0; i < 30; i++) {
        self->effects[i].angle += (s16)(R_UPDATE_RATE * self->effects[i].translation / 3.0f);
    }
}

void DemoKankyo_UpdateDoorOfTime(DemoKankyo* self, PlayState* play) {
    Audio_PlayActorSound2(&self->actor, 0x01000175);
    self->effects[0].translation += R_UPDATE_RATE / 3.0f;
    if (self->effects[0].translation >= 102.0f) {
        Audio_PlayActorSound2(&self->actor, 0x010001EA);
        gSaveContext.eventChkInf[4] |= 0x800;
        Actor_Kill(self->actor.child);
        self->actionFunc = DemoKankyo_KillDoorOfTimeCollision;
    }
}

static inline void DemoKankyo_SetPosFromCue(DemoKankyo* self, PlayState* play, s32 cueChannel) {
    Vec3f startPos;
    Vec3f endPos;
    CsCmdActorAction* cue = play->csCtx.npcActions[cueChannel];
    f32 lerp;

    startPos.x = cue->startPos.x;
    startPos.y = cue->startPos.y;
    startPos.z = cue->startPos.z;
    endPos.x = cue->endPos.x;
    endPos.y = cue->endPos.y;
    endPos.z = cue->endPos.z;
    lerp = FUN_00361490(cue->endFrame, cue->startFrame, play->csCtx.frames);
    self->actor.world.pos.x = (endPos.x - startPos.x) * lerp + startPos.x;
    self->actor.world.pos.y = (endPos.y - startPos.y) * lerp + startPos.y;
    self->actor.world.pos.z = (endPos.z - startPos.z) * lerp + startPos.z;
}

void DemoKankyo_UpdateRock(DemoKankyo* self, PlayState* play) {
    CsCmdActorAction* cue;
    if (play->GetCutsceneState() != 0 && (cue = play->csCtx.npcActions[self->actor.params - 2]) != NULL &&
        cue->action == 2) {
        DemoKankyo_SetPosFromCue(self, play, self->actor.params - 2);
        if (play->csCtx.frames < play->csCtx.npcActions[self->actor.params - 2]->endFrame - 5) {
            self->effects[0].angle = 255;
        } else {
            self->effects[0].angle = (play->csCtx.npcActions[self->actor.params - 2]->endFrame - play->csCtx.frames) * 0.2f * 255.0f;
            if (self->effects[0].angle < 0) {
                self->effects[0].angle = 0;
            }
        }
    } else {
        self->effects[0].angle = 0;
    }
    self->effects[0].offset.x += self->effects[0].velocity.x * (R_UPDATE_RATE / 3.0f);
    self->effects[0].offset.y += self->effects[0].velocity.y * (R_UPDATE_RATE / 3.0f);
    self->effects[0].offset.z += self->effects[0].velocity.z * (R_UPDATE_RATE / 3.0f);
}

}
