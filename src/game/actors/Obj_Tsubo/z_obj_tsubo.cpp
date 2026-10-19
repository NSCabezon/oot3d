extern "C" {
#include "z_obj_tsubo.hpp"

#define FLAGS 0x00800010

void ObjTsubo_Init(Actor* thisx, PlayState* play);
void ObjTsubo_Destroy(ObjTsubo* self, PlayState* play);
void ObjTsubo_Update(Actor* thisx, PlayState* play);
void FUN_002318b4(Actor* thisx, PlayState* play);
void FUN_0011055c(ObjTsubo* self, PlayState* play);
void FUN_003be8e4(ObjTsubo* self, PlayState* play);

s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 bankIndex);
f32 BgCheck_EntityRaycastFloor4(CollisionContext* colCtx, CollisionPoly** outPoly, s32* bgId, Actor* actor, Vec3f* pos);

// Dumped data (0x00535958..0x005359C4): the Init asm reads sObjectIds, sColChkInfoInit, sCylinderInit and
// sInitChain from here. Kept as asm because a C definition changed the layout of the whole .data section.
GLOBAL_ASM("data/z_Obj_Tsubo.data.s")

ActorInit Obj_Tsubo_InitVars = {
    ACTOR_OBJ_TSUBO,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ObjTsubo),
    (ActorFunc)ObjTsubo_Init,
    (ActorFunc)ObjTsubo_Destroy,
    (ActorFunc)ObjTsubo_Update,
    NULL,
};

#define PARAMS_GET_U(params, shift, count) (((params) >> (shift)) & ((1 << (count)) - 1))
#define BGCHECK_Y_MIN_3DS -32000.0f
// The Init data lives in the dump above. It has no symbols in the scatter file, so the tables are
// absolute symbols (EQU) in data_syms.s.
GLOBAL_ASM("../src/game/actors/Obj_Tsubo/data_syms.s")
extern s16 ObjTsubo_sObjectIds[];
extern CollisionCheckInfoInit ObjTsubo_sColChkInfoInit;
extern ColliderCylinderInit ObjTsubo_sCylinderInit;
extern InitChainEntry ObjTsubo_sInitChain[];
extern u8 ObjTsubo_sModelIds[]; // 0x004DBC5C, rodata outside the actor dump (model index per params bit 8)
#define sObjectIds ObjTsubo_sObjectIds
#define sColChkInfoInit (&ObjTsubo_sColChkInfoInit)
#define sCylinderInit ObjTsubo_sCylinderInit
#define sInitChain ObjTsubo_sInitChain

void ObjTsubo_InitCollider(Actor* thisx, PlayState* play) {
    ObjTsubo* self = (ObjTsubo*)thisx;

    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &sCylinderInit);
    Collider_UpdateCylinder(&self->actor, &self->collider);
}

s32 ObjTsubo_SnapToFloor(ObjTsubo* self, PlayState* play) {
    CollisionPoly* groundPoly;
    Vec3f pos;
    s32 bgId;
    f32 groundY;

    pos.x = self->actor.world.pos.x;
    pos.y = self->actor.world.pos.y + 20.0f;
    pos.z = self->actor.world.pos.z;
    groundY = BgCheck_EntityRaycastFloor4(&play->colCtx, &groundPoly, &bgId, &self->actor, &pos);
    if (groundY > BGCHECK_Y_MIN_3DS) {
        self->actor.world.pos.y = groundY;
        Math_Vec3f_Copy(&self->actor.home.pos, &self->actor.world.pos);
        return true;
    }
    return false;
}

static inline void ObjTsubo_SetupWaitForObject(Actor* thisx) {
    ObjTsubo* self = (ObjTsubo*)thisx;
    self->actionFunc = FUN_0011055c;
}
void ObjTsubo_Init(Actor* thisx, PlayState* play) {
    ObjTsubo* self = (ObjTsubo*)thisx;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ObjTsubo_InitCollider(&self->actor, play);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, sColChkInfoInit);
    if (!ObjTsubo_SnapToFloor(self, play)) {
        Actor_Kill(&self->actor);
        return;
    }
    self->requiredObjectSlot = Object_GetIndex(&play->objectCtx, sObjectIds[PARAMS_GET_U(self->actor.params, 8, 1)]);
    if (self->requiredObjectSlot < 0) {
        Actor_Kill(&self->actor);
    } else {
        ObjTsubo_SetupWaitForObject(thisx);
        self->skelAnimModel = NULL;
        self->actor.unk_19B = 1;
    }
}


// WaitForObject: once the pot's object is loaded, switch to the real draw/action and load the model.
void FUN_0011055c(ObjTsubo* self, PlayState* play) {
    if (Object_IsLoaded(&play->objectCtx, self->requiredObjectSlot)) {
        self->actor.draw = (ActorFunc)FUN_002318b4;
        self->actor.objBankIndex = self->requiredObjectSlot;
        self->actionFunc = FUN_003be8e4;
        self->actor.flags &= ~0x10;
        FUN_00372f38(&self->actor, play, &self->skelAnimModel, ObjTsubo_sModelIds[PARAMS_GET_U((u16)self->actor.params, 8, 1)], 0);
    }
}



void ObjTsubo_Destroy(ObjTsubo* self, PlayState* play) {
    Collider_DestroyCylinder(play, &self->collider);
    FUN_00350f34(&self->actor, &self->skelAnimModel, 0);
}

static inline void ObjTsubo_SetMode(Actor* actor, u8 m) {
    actor->unk_19B = m;
}
void ObjTsubo_Update(Actor* thisx, PlayState* play) {
    ObjTsubo* self = (ObjTsubo*)thisx;

    if (((u8*)GET_PLAYER(play))[0x1A9] != 6) {
        ObjTsubo_SetMode(thisx, 1);
    } else {
        ObjTsubo_SetMode(thisx, 2);
    }
    self->actionFunc(self, play);
}


// FUN_003be8e4 (ObjTsubo_Idle) stays as GLOBAL_ASM in src/functions/functions_3A0000s.cpp. Best draft (62/177 words
// differ: Item drop arg setup order, list/count register swap, AC flag store scheduling, yaw range check).
// FUN_00360084 looks like "find actors by id" into a new[]'d Actor*[16] (FUN_0033de14 = new[], FUN_0033ddd4 = delete[]).
#if 0
void FUN_003cbeb0(ObjTsubo* self, PlayState* play);
void FUN_0036f59c(Actor* actor, u32 sfxId);
void FUN_0031650c(ObjTsubo* self, PlayState* play);
void FUN_00316718(ObjTsubo* self, PlayState* play);
Actor** FUN_0033de14(s32 size);
void FUN_0033ddd4(void* ptr);
s32 FUN_00360084(ActorContext* actorCtx, s32 actorId, s32 arg2, Actor** out, s32 max);

static inline void ObjTsubo_SpawnCollectible2(ObjTsubo* self, PlayState* play) {
    PlayState* p = play;
    Actor* actor = &self->actor;
    s32 params = self->actor.params;
    s32 dropParams = params & 0x1F;

    if ((u32)dropParams < 0x1A) {
        Item_DropCollectible(p, &actor->world.pos, dropParams | ((params >> 1) & 0x3F00));
    }
}
#define ObjTsubo_SpawnCollectible ObjTsubo_SpawnCollectible2

static inline void ObjTsubo_SetupLiftedUp(ObjTsubo* self) {
    self->actionFunc = FUN_003cbeb0;
    self->actor.room = -1;
    FUN_0036f59c(&self->actor, 0x01000070);
    self->actor.flags |= 0x10;
}

// Idle
void FUN_003be8e4(ObjTsubo* self, PlayState* play) {
    s16 yawDiff;
    s32 absYawDiff;

    if (Actor_HasParent(&self->actor, play)) {
        ObjTsubo_SetupLiftedUp(self);
    } else if ((self->actor.bgCheckFlags & 0x20) && (self->actor.yDistToWater > 15.0f)) {
        FUN_0031650c(self, play);
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 20, 0x010001DE);
        ObjTsubo_SpawnCollectible(self, play);
        Actor_Kill(&self->actor);
    } else if ((self->collider.base.acFlags & 2) &&
               (self->collider.info.acHitInfo->toucher.dmgFlags & 0x4FC1FFFC)) {
        if ((*(u16*)((u8*)play + 0x104) == 9) && ((self->actor.room == 6) || (self->actor.room == 9))) {
            s32 count;
            s32 max = 16;
            Actor** list = FUN_0033de14(max * sizeof(Actor*));
            s32 i;

            count = FUN_00360084(&play->actorCtx, 0xEF, 1, list, max);

            for (i = 0; i < count; i++) {
                if (Actor_WorldDistXYZToActor(&self->actor, list[i]) < 30.0f) {
                    self->collider.base.acFlags &= ~2;
                    FUN_0033ddd4(list);
                    return;
                }
            }
            FUN_0033ddd4(list);
        }
        FUN_00316718(self, play);
        ObjTsubo_SpawnCollectible(self, play);
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 20, 0x010001DE);
        Actor_Kill(&self->actor);
    } else {
        if (self->actor.xzDistToPlayer < 600.0f) {
            Collider_UpdateCylinder(&self->actor, &self->collider);
            self->collider.base.acFlags &= ~2;
            CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
            if (self->actor.xzDistToPlayer < 150.0f) {
                CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
            }
        }
        if (self->actor.xzDistToPlayer < 100.0f) {
            yawDiff = self->actor.yawTowardsPlayer - GET_PLAYER(play)->actor.world.rot.y;
            if ((yawDiff >= 0x5556) || (yawDiff <= -0x5556)) {
                GiveItem(&self->actor, play, 0, 30.0f, 30.0f);
            }
        }
    }
}
#endif

void FUN_002318b4(Actor* thisx, PlayState* play) {
    ObjTsubo* self = (ObjTsubo*)thisx;

    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->FUN_003721e0(&self->actor.modelMtx);
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
