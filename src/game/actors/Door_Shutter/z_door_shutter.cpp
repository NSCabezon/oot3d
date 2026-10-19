extern "C" {
#include "z_door_shutter.hpp"

#define FLAGS 0x00400030

void DoorShutter_SetupType(DoorShutter* self, PlayState* play);
void FUN_0035021c(void* p);
void* FUN_003685a0(void* p);
void FUN_0034291c(void* unk);
void FUN_00342968(void* unk);
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);

void DoorShutter_Init(Actor* thisx, PlayState* play2);
void DoorShutter_Destroy(Actor* thisx, PlayState* play);
void DoorShutter_Update(Actor* thisx, PlayState* play);
void DoorShutter_Draw(Actor* thisx, PlayState* play);

GLOBAL_ASM("data/z_Door_Shutter.data.s")
GLOBAL_ASM("../src/game/actors/Door_Shutter/data_syms.s")

ActorInit Door_Shutter_InitVars = {
    ACTOR_DOOR_SHUTTER,
    ACTORCAT_DOOR,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(DoorShutter),
    (ActorFunc)DoorShutter_Init,
    (ActorFunc)DoorShutter_Destroy,
    (ActorFunc)DoorShutter_Update,
    (ActorFunc)DoorShutter_Draw,
};

typedef struct DoorShutterStyleInfo {
    s16 objectId;
    u8 gfxType1;
    u8 gfxType2;
} DoorShutterStyleInfo;

typedef struct DoorShutterSceneInfo {
    s16 sceneId;
    u8 styleType;
} DoorShutterSceneInfo;

typedef struct DoorShutterBossDoorInfo {
    s16 dungeonSceneId;
    s16 bossSceneId;
    u8 texIndex;
} DoorShutterBossDoorInfo;

// Data-dump tables (absolute symbols in data_syms.s; the ROM tables are not C yet)
extern s8 DoorShutter_sTypeStyles[];
extern InitChainEntry DoorShutter_sInitChain[];
extern DoorShutterBossDoorInfo DoorShutter_sBossDoorInfo[];
extern DoorShutterStyleInfo DoorShutter_sStyleInfo[];
#define DOORSHUTTER_TYPE_STYLES DoorShutter_sTypeStyles
#define DOORSHUTTER_SCENE_INFO ((DoorShutterSceneInfo*)(DoorShutter_sTypeStyles + 0x1C))
#define DOORSHUTTER_BOSS_DOOR_INFO DoorShutter_sBossDoorInfo
#define DOORSHUTTER_STYLE_INFO DoorShutter_sStyleInfo
#define DOORSHUTTER_INIT_CHAIN DoorShutter_sInitChain
#define DOORSHUTTER_SCENE_ID(play) (*(s16*)((u8*)(play) + 0x104)) // play->sceneId (not in PlayState header yet)
#define DOORSHUTTER_GET_TYPE(thisx) (((thisx)->params >> 6) & 0xF)
#define DOORSHUTTER_GET_SWITCH_FLAG(thisx) ((thisx)->params & 0x3F)



GLOBAL_ASM("asm/DoorShutter_Init.s")

typedef struct {
    /* 0x00 */ struct {
        s8 room;
        s8 bgCamIndex;
    } sides[2]; // sides[0] = front, sides[1] = back
    /* 0x04 */ s16 id;
    /* 0x06 */ u8 unk_06[0xA];
} DoorShutterTransitionEntry; // size = 0x10

#define DOORSHUTTER_TRANSITIONS(play) (*(DoorShutterTransitionEntry**)((u8*)(play) + 0x5B8C)) // play->transitionActors.list

// Object behind the global pointer at 0x0055A1C0 (slot 4 frees a model). Real virtual call. Declared under the
// EffDustFreer name so VFE uses the existing key _ZTI12EffDustFreer (src/game/shared_syms.s). A fresh key name
// (tried: _ZTI14DoorShutterFreer) keeps an extra vtable slot at 0x4EBE30 and breaks the ws_check baseline.
// The class of the 0x0055A1C0 object is not confirmed; this is only the call shape.
extern "C++" {
class EffDustFreer {
public:
    virtual void vf00();
    virtual void vf04();
    virtual void* Alloc(s32 size);
    virtual void vf0C();
    virtual void Free(void* model);
};
}
extern EffDustFreer* gUnk_0055A1C0; // src/game/shared_syms.s

void DoorShutter_Destroy(Actor* thisx, PlayState* play) {
    DoorShutter* self = (DoorShutter*)thisx;
    s32 i;

    s32 bgId = self->dyna.bgId;
    PlayState* play2 = play;

    DynaPoly_DeleteBgActor(play2, &play2->colCtx.dyna, bgId);
    if (self->dyna.actor.room >= 0) {
        s32 transitionActorId = (u16)self->dyna.actor.params >> 10;

        DOORSHUTTER_TRANSITIONS(play)[transitionActorId].id *= -1;
    }
    if (self->requiredObjectSlot >= 0) {
        FUN_00350f34(thisx, &self->skelAnimModel[0], &self->skelAnimModel[1], &self->skelAnimModel[2],
                     &self->skelAnimModel[3], &self->skelAnimModel[4], &self->skelAnimModel[5],
                     &self->skelAnimModel[6], &self->skelAnimModel[7], 0);
        for (i = 0; i < 2; i++) {
            FUN_0035021c(self->unk_1F4[i]);
        }
    }
    for (i = 0; i < 2; i++) {
        if (self->unk_1FC[i] != NULL) {
            void* model = FUN_003685a0(self->unk_1FC[i]);
            gUnk_0055A1C0->Free(model);
        }
        self->unk_1FC[i] = NULL;
    }
    FUN_0034291c(&self->unk_204);
}

void DoorShutter_SetupType(DoorShutter* self, PlayState* play);

void DoorShutter_Update(Actor* thisx, PlayState* play) {
    DoorShutter* self = (DoorShutter*)thisx;
    Player* player = GET_PLAYER(play);

    if (!(player->stateFlags1 & 0x100004C0) || (self->actionFunc == DoorShutter_SetupType)) {
        self->actionFunc(self, play);
    }
}

GLOBAL_ASM("asm/DoorShutter_Draw.s")

void FUN_00239b88(DoorShutter* self, PlayState* play);
void FUN_001ce8a4(DoorShutter* self, PlayState* play);
void FUN_002755ac(DoorShutter* self, PlayState* play);
void FUN_003cb1f4(DoorShutter* self, PlayState* play);
void FUN_003f4a3c(DoorShutter* self, PlayState* play);

GLOBAL_ASM("asm/DoorShutter_SetupDoor.s")

GLOBAL_ASM("asm/DoorShutter_SetupType.s")


void FUN_001ce8a4(DoorShutter* self, PlayState* play);
void FUN_003f4a3c(DoorShutter* self, PlayState* play);
s32 DoorShutter_SetupDoor(DoorShutter* self, PlayState* play);

void FUN_001823c0(DoorShutter* self, PlayState* play) {
    if (self->actionTimer++ > 20) {
        FUN_0036e980(play, NULL, 7);
        DoorShutter_SetupDoor(self, play);
    }
}

void FUN_003cb1f4(DoorShutter* self, PlayState* play) {
}

void FUN_002755ac(DoorShutter* self, PlayState* play) {
    if (self->isActive == 0 && !Flags_GetSwitch(play, (u16)self->dyna.actor.params & 0x3F)) {
        self->actionFunc = FUN_001ce8a4;
        self->actionTimer = 0;
    } else {
        FUN_003f4a3c(self, play);
    }
}

void FUN_0015c500(DoorShutter* self, PlayState* play) {
    f32 target;

    if (self->isActive != 0) {
        self->isActive--;
    }
    target = (self->isActive & 1) ? -3.0f : 0.0f;
    target += -34.0f;
    Math_SmoothStepToF(&self->dyna.actor.world.pos.y, target, 1.0f, 20.0f, 0.0f);
}

f32 __hardfp_sinf(f32 x);

void FUN_0021e1b4(DoorShutter* self, PlayState* play) {
    if (self->isActive != 0) {
        self->isActive--;
        self->dyna.actor.shape.yOffset = __hardfp_sinf((self->isActive * 250.0f) * 0.01f) * ((self->isActive * 3.0f) * 0.1f);
    }
}

void FUN_0036f00c(PlayState* play, Actor* actor, Vec3f* pos, f32 a, s32 b, f32 c, s32 d, s32 e, s32 f); // Actor_SpawnFloorDustRing
s32 Quake_Add(Camera* camera, s32 type);
void Quake_SetSpeed(s32 quakeIndex, s32 speed);
void Quake_SetQuakeValues(s32 quakeIndex, s32 a, s32 b, s32 c, s32 d);
void Quake_SetCountdown(s32 quakeIndex, s32 countdown);

void FUN_0021e0a4(DoorShutter* self, PlayState* play) {
    s32 quakeIndex;
    Camera* camera;

    Actor_MoveForward(&self->dyna.actor);
    Actor_UpdateBgCheckInfo(play, &self->dyna.actor, 0.0f, 0.0f, 0.0f, 4);
    if (self->dyna.actor.bgCheckFlags & 1) {
        self->actionFunc = FUN_0021e1b4;
        self->actionTimer = 0;
        if (!(gSaveContext.eventChkInf[7] & 1)) {
            Actor* parent = self->dyna.actor.parent;

            self->isActive = 10;
            Audio_PlayActorSound2(&self->dyna.actor, 0x01000174);
            camera = Gameplay_GetCamera(play, *(s16*)((u8*)parent + 0x258));
            quakeIndex = Quake_Add(camera, 3);
            Quake_SetSpeed(quakeIndex, 20000);
            Quake_SetQuakeValues(quakeIndex, 2, 0, 0, 0);
            Quake_SetCountdown(quakeIndex, 10);
            FUN_0036f00c(play, &self->dyna.actor, &self->dyna.actor.world.pos, 70.0f, 20, 8.0f, 500, 10, 1);
        }
    }
}

// Player fields used by the door (raw offsets, the Player struct has none of them yet)
#define DOORSHUTTER_PLAYER_NAVI_TEXT_ID(p) (*(s16*)((u8*)(p) + 0x1728))
#define DOORSHUTTER_PLAYER_DOOR_TIMER(p) (*(s16*)((u8*)(p) + 0x12A6))
#define DOORSHUTTER_PLAYER_DOOR_TYPE(p) (*(u8*)((u8*)(p) + 0x12A4))
#define DOORSHUTTER_PLAYER_DOOR_DIRECTION(p) (*(s8*)((u8*)(p) + 0x12A5))
#define DOORSHUTTER_PLAYER_DOOR_ACTOR(p) (*(Actor**)((u8*)(p) + 0x12A8))

s32 FUN_0033f4cc(DoorShutter* self, PlayState* play);
s32 Player_InCsMode(PlayState* play);


s32 FUN_00360084(ActorContext* actorCtx, s32 actorId, s32 arg2, Actor** out, s32 max);
void FUN_0036a2dc(PlayState* play, Actor* actor, s32 arg2, s32 arg3, s32 arg4);
void FUN_001ce768(DoorShutter* self, PlayState* play);

void FUN_00239b88(DoorShutter* self, PlayState* play) {
    if (Flags_GetClear(play, self->dyna.actor.room) || Flags_GetTempClear(play, self->dyna.actor.room)) {
        Actor* found;

        if (gSaveContext.respawnFlag > 0) {
            return;
        }
        if (DOORSHUTTER_SCENE_ID(play) == 5 && self->dyna.actor.room == 0xD) {
            if (FUN_00360084(&play->actorCtx, 0x136, 6, &found, 1) > 0 && ((DoorShutter*)found)->isActive < 0xFF) {
                return;
            }
        }
        Flags_SetClear(play, self->dyna.actor.room);
        self->actionFunc = FUN_001ce768;
        self->actionTimer = 0;
        FUN_0036a2dc(play, &self->dyna.actor, 0, 0, 0);
        FUN_0036a2dc(play, &GET_PLAYER(play)->actor, 0, 0, 0);
        FUN_00372244((u8*)play + 0x5FCC, 0x42, 0x01000487);
        self->actionTimer = -67;
    } else if (FUN_0033f4cc(self, play) != 0) {
        Player* player = GET_PLAYER(play);

        DOORSHUTTER_PLAYER_NAVI_TEXT_ID(player) = (DOORSHUTTER_SCENE_ID(play) == 2) ? -0x20B : -0x202;
    }
}

s32 FUN_0033f4cc(DoorShutter* self, PlayState* play);
void FUN_003eee7c(DoorShutter* self, PlayState* play);


typedef struct {
    u8 unk[0x3DC];
} DoorShutterSwapBlock;

void FUN_0036c520(PlayState* play, void* block);
void Gameplay_SetupRespawnPoint(PlayState* play, s32 arg1, s32 arg2);
s32 Math_StepToS(s16* pValue, s16 target, s16 step);

// FUN_001a0a34 (3DS DoorShutter_WaitPlayerSurprised?) draft: swaps two 0x3DC camera blocks at play+0x4C30 / +0x500C
void FUN_001a0a34(DoorShutter* self, PlayState* play) {
    Vec3f pos;
    DoorShutterSwapBlock saved;

    if (Math_StepToS(&self->jabuDoorClosedAmount, 100, 10)) {
        Player* player = GET_PLAYER(play);
        s8 room = self->dyna.actor.room;

        if (room >= 0) {
            s8 newRoom;

            FUN_0036c5d8(&self->dyna.actor, &pos, &player->actor.world.pos);
            newRoom = DOORSHUTTER_TRANSITIONS(play)[(u16)self->dyna.actor.params >> 10].sides[(pos.z < 0.0f) ? 0 : 1].room;
            self->dyna.actor.room = newRoom;
            if (newRoom != room) {
                saved = (*(DoorShutterSwapBlock*)((u8*)play + 0x4C30));
                (*(DoorShutterSwapBlock*)((u8*)play + 0x4C30)) = (*(DoorShutterSwapBlock*)((u8*)play + 0x500C));
                (*(DoorShutterSwapBlock*)((u8*)play + 0x500C)) = saved;
            }
            FUN_0036c520(play, (u8*)play + 0x4C30);
            Gameplay_SetupRespawnPoint(play, 0, 0xEFF);
        }
        self->isActive = 0;
        self->dyna.actor.velocity.y = 0.0f;
        if (DoorShutter_SetupDoor(self, play) != 0) {
            if (!(*(s32*)((u8*)player + 0x1710) & 0x800)) {
                self->actionFunc = FUN_001823c0;
                self->actionTimer = 0;
                FUN_0036e980(play, NULL, 2);
            }
        }
    }
}

// Drafts for the functions that are still GLOBAL_ASM (word diffs and notes in ws STATUS.md). Kept under #if 0.
// Init needs `#include <string.h>` (memset of skelAnimModel/unk_1F4 gives the ROM stm).
#if 0
static void DoorShutter_SetupAction(DoorShutter* self, DoorShutterActionFunc actionFunc) {
    self->actionFunc = actionFunc;
    self->actionTimer = 0;
}


typedef struct DoorShutterGfxRange {
    s8 unk_00[4];
    u8 rangeSides;
    u8 rangeY;
} DoorShutterGfxRange; // stride 6

extern DoorShutterGfxRange DoorShutter_sGfxRanges[];

s32 DoorShutter_SetupDoor(DoorShutter* self, PlayState* play);
void FUN_0015c500(DoorShutter* self, PlayState* play);
void FUN_0021e0a4(DoorShutter* self, PlayState* play);
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 slot);
void FUN_00352e80(void* model);
void FUN_00352ee0(Actor* actor, PlayState* play, s32 count, void** models, s32* indices);
void* FUN_0036aaa4(Actor* actor, PlayState* play, s32 index);
void FUN_00353214(void* obj, Actor* actor, PlayState* play, s32 mode);
void FUN_003586ec(void);
void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx);
extern s32 gUnk_0054C8CC; // src/game/shared_syms.s

void DoorShutter_SetupType(DoorShutter* self, PlayState* play) {
    ZARInfo* zar;
    s32 i;

    if (Object_IsLoaded(&play->objectCtx, self->requiredObjectSlot)) {
        self->dyna.actor.objBankIndex = self->requiredObjectSlot;
        zar = &play->GetObjectStatus(self->dyna.actor.objBankIndex)->zarInfo;

        if (self->doorType == 4 || self->doorType == 6) {
            CollisionHeader* colHeader = NULL;

            // N64: Actor_SetObjectDependency(play, &self->dyna.actor), empty on 3DS
            ((void (*)(PlayState*, Actor*))GraphicsContext_UnknownStub)(play, &self->dyna.actor);
            self->gfxType = DOORSHUTTER_STYLE_INFO[self->styleType].gfxType1;
            ReadCollisionHeaderFromZsi(zar->FUN_003532c0(0), &colHeader);
            self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
            if (self->doorType == 6) {
                self->dyna.actor.velocity.y = 0.0f;
                self->dyna.actor.gravity = -2.0f;
                Audio_PlayActorSound2(&self->dyna.actor, 0x01000173);
                DoorShutter_SetupAction(self, FUN_0021e0a4);
            } else {
                self->dyna.actor.flags |= 0x400000;
                DoorShutter_SetupAction(self, FUN_0015c500);
                self->isActive = 7;
            }
        } else {
            DoorShutter_SetupDoor(self, play);
        }

        if (self->gfxType == 3) {
            FUN_00372f38(&self->dyna.actor, play, &self->skelAnimModel[0], 0, &self->skelAnimModel[1], 1,
                         &self->skelAnimModel[2], 5, &self->skelAnimModel[3], 3, &self->skelAnimModel[4], 4,
                         &self->skelAnimModel[5], 3, &self->skelAnimModel[6], 2, &self->skelAnimModel[7], 1, 0);
        } else {
            s32 indices[2];
            s32 count = 0;

            for (i = 0; i < 2; i++) {
                s32 idx = DoorShutter_sGfxRanges[(&DOORSHUTTER_STYLE_INFO[self->styleType].gfxType1)[i]].unk_00[0];

                if (idx >= 0) {
                    indices[count] = idx;
                    count++;
                }
            }
            FUN_00352ee0(&self->dyna.actor, play, count, (void**)&self->skelAnimModel[0], indices);
            if (DOORSHUTTER_STYLE_INFO[self->styleType].objectId == 0xB0) {
                for (i = 0; i < count; i++) {
                    if (self->skelAnimModel[i] != NULL) {
                        SkeletonAnimationModel_unk_0C* obj = (SkeletonAnimationModel_unk_0C*)gUnk_0055A1C0->Alloc(0x98);

                        if (obj != NULL) {
                            FUN_00352e80(obj);
                        }
                        self->unk_1FC[i] = obj;
                        self->unk_1FC[i]->unk_00 = *(AnimatedMaterial**)((u8*)self->skelAnimModel[i] + 0x10);
                        self->unk_1FC[i]->FUN_00372d94(zar->GetCMABByIndex(0));
                        self->unk_1FC[i]->animSpeed = 0.0f;
                        {
                            f32 frame = (f32)self->bossDoorTexIndex;

                            if (gUnk_0054C8CC == 0) {
                                *(f32*)((u8*)self->unk_1FC[i] + 8) = frame;
                                FUN_003586ec();
                            }
                        }
                    }
                }
            }
        }

        for (i = 0; i < 2; i++) {
            s32 idx = DoorShutter_sGfxRanges[(&DOORSHUTTER_STYLE_INFO[self->styleType].gfxType1)[i]].unk_00[1];

            if (idx >= 0) {
                self->unk_1F4[i] = FUN_0036aaa4(&self->dyna.actor, play, (idx == 0x22) ? 0 : self->dyna.actor.objBankIndex);
            }
        }
        if (self->unlockTimer != 0) {
            FUN_00353214(&self->unk_204, &self->dyna.actor, play,
                         (self->doorType == 5) ? 1 : ((self->gfxType == 6) ? 2 : 0));
        }
    }
}

// DoorShutter_SetupDoor: 56 words off (ROM keeps 0x5B8C/0x4D880C literals + zero in callee-saved regs at entry)
s32 DoorShutter_SetupDoor(DoorShutter* self, PlayState* play) {
    DoorShutterTransitionEntry* transitionEntry = &DOORSHUTTER_TRANSITIONS(play)[(u16)self->dyna.actor.params >> 10];
    s32 doorType = self->doorType;
    s8 frontRoom = transitionEntry->sides[0].room;
    DoorShutterStyleInfo* styleInfo = &DOORSHUTTER_STYLE_INFO[self->styleType];

    if (doorType != 0xB) {
        if (frontRoom == transitionEntry->sides[1].room) {
            s16 d = self->dyna.actor.shape.rot.y - self->dyna.actor.yawTowardsPlayer;
            if (ABS(d) < 0x4000) {
                frontRoom = -1;
            }
        }
        if (frontRoom == self->dyna.actor.room) {
            if (doorType == 7) {
                doorType = 1;
            } else if (doorType == 5) {
                doorType = 3;
            } else {
                doorType = 0;
            }
        }
    }
    self->gfxType = (doorType == 0) ? styleInfo->gfxType1 : styleInfo->gfxType2;

    if (doorType == 1) {
        if (!Flags_GetClear(play, self->dyna.actor.room)) {
            DoorShutter_SetupAction(self, FUN_00239b88);
            self->barsClosedAmount = 1.0f;
            return true;
        }
    } else if (doorType == 2 || doorType == 7) {
        if (!Flags_GetSwitch(play, DOORSHUTTER_GET_SWITCH_FLAG(&self->dyna.actor))) {
            DoorShutter_SetupAction(self, FUN_001ce8a4);
            self->barsClosedAmount = 1.0f;
            return true;
        } else {
            DoorShutter_SetupAction(self, FUN_002755ac);
            return false;
        }
    } else if (doorType == 3) {
        DoorShutter_SetupAction(self, FUN_003cb1f4);
        return false;
    }
    DoorShutter_SetupAction(self, FUN_003f4a3c);
    return false;
}

// DoorShutter_Init: 83 words off (register roles of the doorType extraction, sceneNum base shared across branches)
void DoorShutter_Init(Actor* thisx, PlayState* play) {
    DoorShutter* self = (DoorShutter*)thisx;
    s32 styleType;
    s32 objectSlot;
    s32 i;
    DoorShutterSceneInfo* sceneInfo;
    DoorShutterBossDoorInfo* bossDoorInfo;
    void* lock;

    Actor_ProcessInitChain(&self->dyna.actor, DOORSHUTTER_INIT_CHAIN);
    self->dyna.actor.home.pos.z = self->dyna.actor.shape.yOffset;
    DynaPolyActor_Init(&self->dyna, 0);
    thisx->unk_19A = 1;
    memset(self->skelAnimModel, 0, sizeof(self->skelAnimModel));
    memset(self->unk_1F4, 0, sizeof(self->unk_1F4));
    lock = &self->unk_204;
    if (lock != NULL) {
        FUN_00342968(lock);
    }
    self->unk_21C = 0;
    self->doorType = DOORSHUTTER_GET_TYPE(&self->dyna.actor);

    styleType = DOORSHUTTER_TYPE_STYLES[self->doorType];
    if (styleType < 0) {
        for (sceneInfo = DOORSHUTTER_SCENE_INFO, i = 0; i < 17; i++, sceneInfo++) {
            if (play->sceneNum == sceneInfo->sceneId) {
                break;
            }
        }
        styleType = sceneInfo->styleType;
    } else if (styleType == 6) {
        for (bossDoorInfo = DOORSHUTTER_BOSS_DOOR_INFO, i = 0; i < 6; i++, bossDoorInfo++) {
            if (play->sceneNum == bossDoorInfo->dungeonSceneId || play->sceneNum == bossDoorInfo->bossSceneId) {
                break;
            }
        }
        self->bossDoorTexIndex = bossDoorInfo->texIndex;
    } else {
        self->dyna.actor.room = -1;
    }
    objectSlot = Object_GetIndex(&play->objectCtx, DOORSHUTTER_STYLE_INFO[styleType].objectId);
    self->requiredObjectSlot = objectSlot;
    if ((s8)objectSlot < 0) {
        Actor_Kill(&self->dyna.actor);
        return;
    }
    DoorShutter_SetupAction(self, DoorShutter_SetupType);

    self->styleType = styleType;
    if (self->doorType == 0xB || self->doorType == 5) {
        if (!Flags_GetSwitch(play, DOORSHUTTER_GET_SWITCH_FLAG(&self->dyna.actor))) {
            self->unlockTimer = 15;
        }
        Actor_SetFocus(&self->dyna.actor, 60.0f);
    } else if (styleType == 4) {
        Actor_SetScale(&self->dyna.actor, 0.1f);
        self->jabuDoorClosedAmount = 100;
        self->dyna.actor.uncullZoneScale = 280.0f;
        Actor_SetFocus(&self->dyna.actor, 0.0f);
    } else {
        Actor_SetFocus(&self->dyna.actor, 60.0f);
    }
}

// FUN_003f4a3c = DoorShutter_Idle: 16 words off (ROM keeps self+0x100 in r7 and play in r8)
void FUN_003f4a3c(DoorShutter* self, PlayState* play) {
    if (self->isActive) {
        DoorShutter_SetupAction(self, FUN_003eee7c);
        self->dyna.actor.velocity.y = 0.0f;
        if (self->unlockTimer != 0) {
            Flags_SetSwitch(play, DOORSHUTTER_GET_SWITCH_FLAG(&self->dyna.actor));
            if (self->doorType != 5) {
                gSaveContext.inventory.dungeonKeys[gSaveContext.mapIndex] -= 1;
                Audio_PlayActorSound2(&self->dyna.actor, 0x010001D3);
            } else {
                Audio_PlayActorSound2(&self->dyna.actor, 0x01000200);
            }
        }
    } else {
        s32 doorDirection = FUN_0033f4cc(self, play);

        if (doorDirection != 0) {
            Player* player = GET_PLAYER(play);

            if (self->unlockTimer != 0) {
                if (self->doorType == 5) {
                    if (!(gSaveContext.inventory.dungeonItems[gSaveContext.mapIndex] & gBitFlags[0])) {
                        DOORSHUTTER_PLAYER_NAVI_TEXT_ID(player) = -0x204;
                        return;
                    }
                } else if (gSaveContext.inventory.dungeonKeys[gSaveContext.mapIndex] <= 0) {
                    DOORSHUTTER_PLAYER_NAVI_TEXT_ID(player) = -0x203;
                    return;
                }
                DOORSHUTTER_PLAYER_DOOR_TIMER(player) = 15;
            }
            DOORSHUTTER_PLAYER_DOOR_TYPE(player) = 2;
            DOORSHUTTER_PLAYER_DOOR_DIRECTION(player) = doorDirection;
            DOORSHUTTER_PLAYER_DOOR_ACTOR(player) = &self->dyna.actor;
        }
    }
}

// FUN_0033f4cc = DoorShutter_GetPlayerSide: 7 words off (ROM: self r4, play+0x2000 r6, player r5; ours r4/r5/r6 permuted)
extern __pure float _fabsf(float);
static __inline __pure float DoorShutter_Fabsf(float f) {
    return _fabsf(f);
}

s32 FUN_0033f4cc(DoorShutter* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    DoorShutter* s2 = self;

    if (!Player_InCsMode(play)) {
        DoorShutterGfxRange* range = &DoorShutter_sGfxRanges[self->gfxType];
        f32 offsetY = (self->gfxType == 3) ? 80.0f : 0.0f;
        f32 rangeSides = range->rangeSides;
        f32 rangeY = range->rangeY;
        Vec3f playerPos;
        Vec3f local;
        f32 dist;

        Player* p2 = GET_PLAYER(play);
        playerPos.x = p2->actor.world.pos.x;
        playerPos.y = p2->actor.world.pos.y + offsetY;
        playerPos.z = p2->actor.world.pos.z;
        FUN_0036c5d8(&s2->dyna.actor, &local, &playerPos);
        if (DoorShutter_Fabsf(local.x) > rangeSides || DoorShutter_Fabsf(local.y) > rangeY) {
            dist = 3.4028235e38f;
        } else {
            dist = local.z;
        }
        if (DoorShutter_Fabsf(dist) < 50.0f) {
            s16 yawDiff = player->actor.shape.rot.y - self->dyna.actor.shape.rot.y;

            if (dist > 0.0f) {
                yawDiff = 0x8000 - yawDiff;
            }
            if (ABS(yawDiff) < 0x3000) {
                return (dist >= 0.0f) ? 1.0f : -1.0f;
            }
        }
    }
    return 0;
}
#endif

// FUN_001ce768 draft, 34/63 words off (best of ~20 variants): timer>=0 branch and the
// barsClosedAmount branch order / constant placement do not match yet.
#if 0
s32 FUN_0036ae04(void);
void FUN_00373d0c(PlayState* play);

void FUN_001ce768(DoorShutter* self, PlayState* play) {
    if (self->actionTimer != 0) {
        if (self->actionTimer >= 0) {
            self->actionTimer--;
        } else {
            self->actionTimer++;
            if ((FUN_0036ae04() == self->dyna.actor.type) || (self->actionTimer == 0)) {
                self->actionTimer = 3;
            }
        }
    } else {
        f32 one = 1.0f;
        f32 zero = 0.0f;
        s32 izero = 0;

        if (self->barsClosedAmount == 1.0f) {
            Audio_PlayActorSound2(&self->dyna.actor, (self->gfxType == 3) ? 0x010001E8 : 0x010001B0);
        }
        if (Math_StepToF(&self->barsClosedAmount, zero, 0.2f) != 0) {
            if ((self->doorType != 0) && (self->doorType != 1)) {
                self->actionFunc = FUN_002755ac;
            } else {
                self->actionFunc = FUN_003f4a3c;
            }
            self->actionTimer = izero;
            FUN_00373d0c(play);
        }
    }
}
#endif

}
