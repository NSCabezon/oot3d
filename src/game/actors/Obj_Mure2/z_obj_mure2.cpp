extern "C" {
#include "z_obj_mure2.hpp"

#define FLAGS 0x00000000
#define PARAMS_GET_U(params, start, count) (((params) >> (start)) & ((1 << (count)) - 1))

typedef void (*ObjMure2SetPosFunc)(Vec3f* vec, ObjMure2* self);

typedef struct Mure2sScatteredShrubInfo {
    s16 radius;
    s16 angle;
} Mure2sScatteredShrubInfo;

void ObjMure2_Init(ObjMure2* self, PlayState* play);
void ObjMure2_Update(ObjMure2* self, PlayState* play);
void ObjMure2_Wait(ObjMure2* self, PlayState* play);
__attribute__((noinline)) void FUN_0012b6a8(ObjMure2* self);
void FUN_003b30ac(ObjMure2* self, PlayState* play);
void FUN_003aef48(ObjMure2* self, PlayState* play);
void FUN_0038a4f0(Vec3f* vec, ObjMure2* self);
void FUN_002882f4(Vec3f* vec, ObjMure2* self);
void ObjMure2_SetPosShrubCircle(Vec3f* vec, ObjMure2* self);

ActorInit Obj_Mure2_InitVars = {
    ACTOR_OBJ_MURE2,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ObjMure2),
    (ActorFunc)ObjMure2_Init,
    (ActorFunc)Actor_Noop,
    (ActorFunc)ObjMure2_Update,
    NULL,
};

// Child count per param variant, actor id per variant, and spawn params per variant (all zero), in one table
// because the zero part must not become a standalone .bss array.
__attribute__((used)) static s16 D_80B9A818[] = { 9, 12, 8 };
__attribute__((used)) static s16 sActorSpawnIDs[] = { ACTOR_EN_KUSA, ACTOR_EN_KUSA, ACTOR_EN_ISHI };
__attribute__((used)) static s16 sActorSpawnParams[] = { 0, 0, 0 };

__attribute__((used)) static f32 sDistSquared1[] = { SQ(1600.0f), SQ(1600.0f), SQ(1600.0f) };

__attribute__((used)) static f32 sDistSquared2[] = { SQ(1705.0f), SQ(1705.0f), SQ(1705.0f) };

__attribute__((used)) static Mure2sScatteredShrubInfo sScatteredShrubInfo[] = {
    { 40, 0x0666 }, { 40, 0x2CCC }, { 40, 0x5999 }, { 40, 0x8666 }, { 20, 0xC000 }, { 80, 0x1333 },
    { 80, 0x4000 }, { 80, 0x6CCC }, { 80, 0x9333 }, { 80, 0xACCC }, { 80, 0xC666 }, { 60, 0xE000 },
};

__attribute__((used)) static ObjMure2SetPosFunc sSetPosFuncs[] = { ObjMure2_SetPosShrubCircle, FUN_0038a4f0, FUN_002882f4 };

__attribute__((used)) static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 2100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 100, ICHAIN_STOP),
};

void ObjMure2_SetPosShrubCircle(Vec3f* vec, ObjMure2* self) {
    s32 i;

    Math_Vec3f_Copy(vec, &self->actor.world.pos);
    for (i = 1; i < D_80B9A818[PARAMS_GET_U(self->actor.params, 0, 2)]; i++) {
        Math_Vec3f_Copy(vec + i, &self->actor.world.pos);
        (vec + i)->x += (80.0f * Math_SinS((i - 1) * 0x2000));
        (vec + i)->z += (80.0f * Math_CosS((i - 1) * 0x2000));
    }
}

void FUN_0038a4f0(Vec3f* vec, ObjMure2* self) {
    s32 i;

    for (i = 0; i < D_80B9A818[PARAMS_GET_U(self->actor.params, 0, 2)]; i++) {
        Math_Vec3f_Copy(vec + i, &self->actor.world.pos);
        (vec + i)->x += (sScatteredShrubInfo[i].radius * Math_CosS(sScatteredShrubInfo[i].angle));
        (vec + i)->z -= (sScatteredShrubInfo[i].radius * Math_SinS(sScatteredShrubInfo[i].angle));
    }
}

void FUN_002882f4(Vec3f* vec, ObjMure2* self) {
    s32 i;

    for (i = 0; i < D_80B9A818[PARAMS_GET_U(self->actor.params, 0, 2)]; i++) {
        Math_Vec3f_Copy(vec + i, &self->actor.world.pos);
        (vec + i)->x += (80.0f * Math_SinS(i * 0x2000));
        (vec + i)->z += (80.0f * Math_CosS(i * 0x2000));
    }
}

static void ObjMure2_SetActorSpawnParams(s16* params, ObjMure2* self) {
    s16 dropTable = PARAMS_GET_U(self->actor.params, 8, 4);

    if (dropTable >= 13) {
        dropTable = 0;
    }
    *params = sActorSpawnParams[PARAMS_GET_U(self->actor.params, 0, 2)] & 0xF0FF;
    *params |= (dropTable << 8);
}

static void ObjMure2_SpawnActors(ObjMure2* self, PlayState* play) {
    s32 actorNum = PARAMS_GET_U(self->actor.params, 0, 2);
    s32 i;
    Vec3f spawnPos[12];
    s16 params;

    sSetPosFuncs[actorNum](spawnPos, self);
    ObjMure2_SetActorSpawnParams(&params, self);

    for (i = 0; i < D_80B9A818[actorNum]; i++) {
        if (self->actorSpawnPtrList[i] == NULL && ((self->currentActorNum >> i) & 1) == 0) {
            self->actorSpawnPtrList[i] =
                Actor_Spawn(&play->actorCtx, play, sActorSpawnIDs[actorNum], spawnPos[i].x, spawnPos[i].y,
                            spawnPos[i].z, self->actor.world.rot.x, 0, self->actor.world.rot.z, params, 1);
            if (self->actorSpawnPtrList[i] != NULL) {
                self->actorSpawnPtrList[i]->room = self->actor.room;
            }
        }
    }
}

static void ObjMure2_CleanupAndDie(ObjMure2* self, PlayState* play) {
    s32 i;

    for (i = 0; i < D_80B9A818[PARAMS_GET_U(self->actor.params, 0, 2)]; i++) {
        if (((self->currentActorNum >> i) & 1) == 0) {
            if (self->actorSpawnPtrList[i] != NULL) {
                if (Actor_HasParent(self->actorSpawnPtrList[i], play)) {
                    self->currentActorNum |= (1 << i);
                } else {
                    Actor_Kill(self->actorSpawnPtrList[i]);
                }
                self->actorSpawnPtrList[i] = NULL;
            }
        } else {
            self->actorSpawnPtrList[i] = NULL;
        }
    }
}

static void ObjMure2_ReleaseFinishedChildren(ObjMure2* self) {
    s32 i;

    for (i = 0; i < D_80B9A818[PARAMS_GET_U(self->actor.params, 0, 2)]; i++) {
        if (self->actorSpawnPtrList[i] != NULL && (((self->currentActorNum >> i) & 1) == 0) &&
            (self->actorSpawnPtrList[i]->update == NULL)) {
            self->currentActorNum |= (1 << i);
            self->actorSpawnPtrList[i] = NULL;
        }
    }
}

void FUN_003b30ac(ObjMure2* self, PlayState* play) {
    f32 x = self->actor.unk_EC.x;
    f32 z = self->actor.unk_EC.z;
    f32 distSq = SQ(x) + SQ(z);
    s32 actorNum = PARAMS_GET_U(self->actor.params, 0, 2);
    f32 thr = sDistSquared1[actorNum] * self->unk_1DC;

    if (distSq < thr) {
        self->actor.flags |= 0x10;
        ObjMure2_SpawnActors(self, play);
        self->actionFunc = FUN_003aef48;
    }
}

void FUN_003aef48(ObjMure2* self, PlayState* play) {
    ObjMure2_ReleaseFinishedChildren(self);

    if (SQ(self->actor.unk_EC.x) + SQ(self->actor.unk_EC.z) >=
        sDistSquared2[PARAMS_GET_U(self->actor.params, 0, 2)] * self->unk_1DC) {
        self->actor.flags &= ~0x10;
        ObjMure2_CleanupAndDie(self, play);
        FUN_0012b6a8(self);
    }
}

void FUN_0012b6a8(ObjMure2* self) {
    self->actionFunc = FUN_003b30ac;
}

// ObjMure2_Wait and the Obj_Mure3 stub (0x0015ddc4) share one pinned span; kept as asm (see report).
GLOBAL_ASM("asm/ObjMure2_Wait.s")

void ObjMure2_Init(ObjMure2* self, PlayState* play) {
    Actor_ProcessInitChain(&self->actor, sInitChain);
    if (play->GetCutsceneState() != CS_STATE_IDLE) {
        self->actor.uncullZoneForward += 1200.0f;
    }
    self->actionFunc = ObjMure2_Wait;
}

void ObjMure2_Update(ObjMure2* self, PlayState* play) {
    if (play->GetCutsceneState() == CS_STATE_IDLE) {
        self->unk_1DC = 1.0f;
    } else {
        self->unk_1DC = 4.0f;
    }
    self->actionFunc(self, play);
}

}
