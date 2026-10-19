extern "C" {
#include "z_bg_spot06_objects.hpp"

#define FLAGS 0x00400200

void BgSpot06Objects_Init(Actor* actor, GameState* state);
void BgSpot06Objects_Destroy(Actor* actor, GameState* state);
void BgSpot06Objects_Update(Actor* actor, GameState* state);
void BgSpot06Objects_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Spot06_Objects.data.s")
GLOBAL_ASM("../src/game/actors/Bg_Spot06_Objects/data_syms.s")

ActorInit Bg_Spot06_Objects_InitVars = {
    ACTOR_BG_SPOT06_OBJECTS,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_SPOT06_OBJECTS,
    sizeof(BgSpot06Objects),
    (ActorFunc)BgSpot06Objects_Init,
    (ActorFunc)BgSpot06Objects_Destroy,
    (ActorFunc)BgSpot06Objects_Update,
    (ActorFunc)BgSpot06Objects_Draw,
};

// sInitChain / sInitChainWaterPlane / sJntSphInit live in the data dump above; absolute symbols from data_syms.s.
typedef struct {
    char unk_00[0x24];
} BgSpot06JntSphInit;
extern InitChainEntry BgSpot06Objects_sInitChain[];
extern InitChainEntry BgSpot06Objects_sInitChainWaterPlane[];
extern BgSpot06JntSphInit BgSpot06Objects_sJntSphInit;

// Lake Hylia water boxes (repo WaterBox only has ySurface).
typedef struct {
    /* 0x00 */ char unk_00[0x2];
    /* 0x02 */ s16 ySurface;
    /* 0x04 */ s16 zMin;
    /* 0x06 */ char unk_06[0xA];
} BgSpot06WaterBox; // size = 0x10

void Collider_InitJntSph(PlayState* play, BgSpot06ColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, BgSpot06ColliderJntSph* collider, Actor* actor, BgSpot06JntSphInit* init,
                        void* elements);
void FUN_00176b04(BgSpot06Objects* self, PlayState* play);
void FUN_003e2078(BgSpot06Objects* self, PlayState* play);
void FUN_00182158(BgSpot06Objects* self, PlayState* play);
void FUN_00159930(BgSpot06Objects* self, PlayState* play);
void FUN_0019fe24(BgSpot06Objects* self, PlayState* play);

// Draft (far, ~270 words off with 4 size words short): switch dispatch matches, but armcc hoists 0x1F (fp) and
// -1313.0f (s16) to the prologue in the ROM and uses the mixed GetCMABByIndex r2 shape. See STATUS.md.
#if 0
// 3DS: while the Lake Hylia cutscene layer is active (adult, lake not restored) the switch is forced on.
static s32 BgSpot06Objects_ForceOpenSwitch(BgSpot06Objects* self, PlayState* play) {
    if (LINK_IS_ADULT && !(gSaveContext.eventChkInf[6] & 0x200) && gSaveContext.sceneSetupIndex >= 4) {
        self->switchFlag = 0x1F;
        Flags_SetSwitch(play, 0x1F);
        return 1;
    }
    return 0;
}

void BgSpot06Objects_Init(Actor* thisx, PlayState* play) {
    BgSpot06Objects* self = (BgSpot06Objects*)thisx;
    CollisionHeader* colHeader = NULL;
    ZARInfo* zar;

    self->switchFlag = thisx->params & 0xFF;
    thisx->params = (thisx->params >> 8) & 0xFF;
    thisx->uncullZoneForward = 3000.0f;

    switch (thisx->params) {
        case 0:
            FUN_00372f38(thisx, play, &self->skelAnimModel, 2, 0);
            Actor_ProcessInitChain(thisx, BgSpot06Objects_sInitChain);
            DynaPolyActor_Init(&self->dyna, 0);
            ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0), &colHeader);
            self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);

            if (LINK_IS_ADULT && Flags_GetSwitch(play, self->switchFlag)) {
                thisx->world.pos.y = thisx->home.pos.y + 120.0f;
                self->actionFunc = FUN_003e2078;
            } else if (BgSpot06Objects_ForceOpenSwitch(self, play)) {
                thisx->world.pos.y = thisx->home.pos.y + 120.0f;
                self->actionFunc = FUN_003e2078;
            } else {
                self->actionFunc = FUN_00176b04;
            }
            break;
        case 1:
            zar = FUN_00372f38(thisx, play, &self->skelAnimModel, 4, 0);
            self->skelAnimModel->unk_0C->FUN_00372d94(zar->GetCMABByIndex(2));
            self->skelAnimModel->unk_0C->SetAnimType(1);
            Actor_ProcessInitChain(thisx, BgSpot06Objects_sInitChain);
            Collider_InitJntSph(play, &self->collider);
            Collider_SetJntSph(play, &self->collider, thisx, &BgSpot06Objects_sJntSphInit, self->unk_1E8);

            if ((LINK_IS_ADULT && Flags_GetSwitch(play, self->switchFlag)) ||
                BgSpot06Objects_ForceOpenSwitch(self, play)) {
                if (!(gSaveContext.eventChkInf[6] & 0x200)) {
                    thisx->home.pos.y = thisx->world.pos.y = -1993.0f;
                } else {
                    thisx->home.pos.y = thisx->world.pos.y = -1313.0f;
                }

                thisx->world.pos.z -= 100.0f;
                thisx->home.pos.z = thisx->world.pos.z + 16.0f;
                self->actionFunc = FUN_00182158;
                self->collider.elements[0].dim.worldSphere.radius = self->collider.elements[0].dim.modelSphere.radius * 2;
                self->collider.elements[0].dim.worldSphere.center.z = thisx->world.pos.z + 16.0f;
            } else {
                self->actionFunc = FUN_00159930;
                self->collider.elements[0].dim.worldSphere.radius = self->collider.elements[0].dim.modelSphere.radius;
                self->collider.elements[0].dim.worldSphere.center.z = thisx->world.pos.z;
            }

            self->collider.elements[0].dim.worldSphere.center.x = thisx->world.pos.x;
            self->collider.elements[0].dim.worldSphere.center.y = thisx->world.pos.y;
            thisx->colChkInfo.mass = 0xFF;
            break;
        case 2:
            self->unk_244 = 0.7f;
            zar = FUN_00372f38(thisx, play, &self->skelAnimModel1, 0, &self->skelAnimModel2, 1, 0);
            self->skelAnimModel1->unk_0C->FUN_00372d94(zar->GetCMABByIndex(0));
            self->skelAnimModel1->unk_0C->SetAnimType(1);
            self->skelAnimModel2->unk_0C->FUN_00372d94(zar->GetCMABByIndex(1));
            self->skelAnimModel2->unk_0C->SetAnimType(1);
            Actor_ProcessInitChain(thisx, BgSpot06Objects_sInitChainWaterPlane);
            thisx->flags = 0x30;

            if (LINK_IS_ADULT && !(gSaveContext.eventChkInf[6] & 0x200)) {
                if (gSaveContext.sceneSetupIndex < 4) {
                    self->lakeHyliaWaterLevel = -681.0f;
                    ((BgSpot06WaterBox*)play->colCtx.stat.colHeader->waterBoxes)[1].ySurface = -1193;
                    ((BgSpot06WaterBox*)play->colCtx.stat.colHeader->waterBoxes)[1].zMin -= 50;
                    ((BgSpot06WaterBox*)play->colCtx.stat.colHeader->waterBoxes)[2].ySurface = -1993;
                    ((BgSpot06WaterBox*)play->colCtx.stat.colHeader->waterBoxes)[3].ySurface = -1993;
                    self->actionFunc = FUN_003e2078;
                } else {
                    self->lakeHyliaWaterLevel = -681.0f;
                    thisx->world.pos.y = -1994.0f;
                    self->actionFunc = FUN_0019fe24;
                    self->unk_244 = 0.0f;
                }
            } else {
                self->lakeHyliaWaterLevel = 0.0f;
                self->actionFunc = FUN_003e2078;
            }
            break;
        case 3:
            FUN_00372f38(thisx, play, &self->skelAnimModel, 3, 0);
            Actor_ProcessInitChain(thisx, BgSpot06Objects_sInitChain);
            DynaPolyActor_Init(&self->dyna, 0);
            ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 1), &colHeader);
            self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
            self->actionFunc = FUN_003e2078;

            if (!LINK_IS_ADULT) {
                Actor_Kill(thisx);
            }
            break;
    }
}
#endif

GLOBAL_ASM("asm/BgSpot06Objects_Init.s")

void Collider_DestroyJntSph(PlayState* play, BgSpot06ColliderJntSph* collider);

void BgSpot06Objects_Destroy(Actor* thisx, GameState* state) {
    BgSpot06Objects* self = (BgSpot06Objects*)thisx;

    switch (self->dyna.actor.params) {
        case 0:
        case 3:
            DynaPoly_DeleteBgActor((PlayState*)state, &((PlayState*)state)->colCtx.dyna, self->dyna.bgId);
            FUN_00350f34(thisx, &self->skelAnimModel, 0);
            break;
        case 1:
            Collider_DestroyJntSph((PlayState*)state, &self->collider);
            FUN_00350f34(thisx, &self->skelAnimModel, 0);
            break;
        case 2:
            FUN_00350f34(thisx, &self->skelAnimModel1, &self->skelAnimModel2, 0);
            break;
    }
}

void BgSpot06Objects_Update(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgSpot06Objects* self = (BgSpot06Objects*)thisx;

    self->actionFunc(self, play);

    if (thisx->params == 1) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    }
}

void FUN_0019f87c(BgSpot06Objects* self, PlayState* play);
void FUN_003e2078(BgSpot06Objects* self, PlayState* play);

// Lake Hylia water plane: waits for the Water Temple clear flag, then starts the rise.
void FUN_0019fe24(BgSpot06Objects* self, PlayState* play) {
    if (gSaveContext.eventChkInf[6] & 0x200) {
        Audio_PlaySoundGeneral(0x0100025B, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        self->actionFunc = FUN_0019f87c;
    }
}

// Lake Hylia water plane rises after the Water Temple is cleared.
void FUN_0019f87c(BgSpot06Objects* self, PlayState* play) {
    Actor* thisx = &self->dyna.actor;
    thisx->world.pos.y = self->lakeHyliaWaterLevel + -1313.0f;

    if (self->lakeHyliaWaterLevel >= 0.0001f) {
        thisx->world.pos.y = -1313.0f;
        self->actionFunc = FUN_003e2078;
    } else {
        Math_SmoothStepToF(&self->lakeHyliaWaterLevel, 1.0f, 0.1f, 1.0f, 0.001f);
        play->colCtx.stat.colHeader->waterBoxes[1 * 4].ySurface = -1193;
        play->colCtx.stat.colHeader->waterBoxes[2 * 4].ySurface = thisx->world.pos.y;
        play->colCtx.stat.colHeader->waterBoxes[3 * 4].ySurface = thisx->world.pos.y;
    }
}

void EffectSsGRipple_Spawn(PlayState* play, Vec3f* pos, s16 radius, s16 maxRadius, s16 life);
f32 __hardfp_sinf(f32 x);

// PlayState::gameplayFrames is not in the repo header yet (3DS offset 0x5BF4).
static inline u32 BgSpot06Objects_GameplayFrames(PlayState* play) {
    return *(u32*)((u8*)play + 0x5BF4);
}

// Lake Hylia water plane floats after it has been pulled up, rippling every few frames.
void FUN_00182158(BgSpot06Objects* self, PlayState* play) {
    if (!(BgSpot06Objects_GameplayFrames(play) % 7)) {
        EffectSsGRipple_Spawn(play, &self->dyna.actor.home.pos, 300, 700, 0);
    }

    if (self->timer != 0) {
        self->timer--;
    }

    // 3DS runs the N64 phase at 2/3 speed: (M_PI / 16) * 2 / 3
    self->dyna.actor.world.pos.y = (2.0f * __hardfp_sinf(self->timer * ((M_PI / 16.0f) * 2.0f / 3.0f))) + self->dyna.actor.home.pos.y;

    if (self->timer == 0) {
        self->timer = 48;
    }
}

void FUN_001288fc(BgSpot06Objects* self, PlayState* play);
void EffectSsBubble_Spawn(PlayState* play, Vec3f* pos, f32 a, f32 b, f32 c, f32 d);
f32 Rand_ZeroOne(void);
f32 Math_SinS(s16 angle);
f32 Math_CosS(s16 angle);

// Fish shaped lock bubbles, spawned every few frames (flag forces a spawn).
static void BgSpot06Objects_LockSpawnBubbles(BgSpot06Objects* self, PlayState* play, s32 flag) {
    if (!(BgSpot06Objects_GameplayFrames(play) % 7) || flag) {
        EffectSsBubble_Spawn(play, &self->dyna.actor.world.pos, 0.0f, 40.0f, 30.0f, (Rand_ZeroOne() * 0.05f) + 0.175f);
    }
}

// Fish shaped lock is pulled outward by the hookshot before it rises to the surface.
void FUN_00182064(BgSpot06Objects* self, PlayState* play) {
    BgSpot06Objects* s2 = self;
    if (self->timer != 0) {
        self->timer--;
    }

    self->dyna.actor.world.pos.x += (0.3f * Math_SinS(self->dyna.actor.world.rot.y));
    self->dyna.actor.world.pos.z += (0.3f * Math_CosS(self->dyna.actor.world.rot.y));
    BgSpot06Objects_LockSpawnBubbles(self, play, 0);

    if (self->timer == 0) {
        self->dyna.actor.velocity.y = 0.5f;
        self->dyna.actor.flags &= ~0x2000;
        s2->actionFunc = FUN_001288fc;
    }
}

void EffectSsGSplash_Spawn(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, s16 type, s16 scale);
s32 Math_ScaledStepToS(s16* value, s16 target, s16 step);

static void BgSpot06Objects_LockSpawnWaterRipples(BgSpot06Objects* self, PlayState* play, s32 flag) {
    if (flag || !(BgSpot06Objects_GameplayFrames(play) % 7)) {
        EffectSsGRipple_Spawn(play, &self->dyna.actor.home.pos, 300, 700, 0);
    }
}

// Fish shaped lock rises to the surface creating bubbles.
void FUN_001288fc(BgSpot06Objects* self, PlayState* play) {
    f32 cos;
    BgSpot06Objects* s2 = self;

    s2->dyna.actor.world.pos.y += self->dyna.actor.velocity.y;

    if (self->dyna.actor.velocity.y <= 0.0f) {
        cos = Math_CosS(self->dyna.actor.shape.rot.x) * 4.3f;
        self->dyna.actor.world.pos.x += (cos * Math_SinS(self->dyna.actor.shape.rot.y));
        self->dyna.actor.world.pos.z += (cos * Math_CosS(self->dyna.actor.shape.rot.y));
        self->dyna.actor.world.pos.y -= 1.3f;
        BgSpot06Objects_LockSpawnWaterRipples(self, play, 0);

        if (Math_ScaledStepToS(&self->dyna.actor.shape.rot.x, 0, 0x260) != 0) {
            self->dyna.actor.home.pos.x =
                self->dyna.actor.world.pos.x - (Math_SinS(self->dyna.actor.shape.rot.y) * 16.0f);
            self->dyna.actor.home.pos.z =
                self->dyna.actor.world.pos.z - (Math_CosS(self->dyna.actor.shape.rot.y) * 16.0f);
            self->dyna.actor.world.pos.y = -1993.0f;
            self->timer = 48;
            s2->dyna.actor.flags &= ~0x10;
            self->collider.elements[0].dim.worldSphere.radius = self->collider.elements[0].dim.modelSphere.radius * 2;
            self->actionFunc = FUN_00182158;
        }
    } else {
        if (self->dyna.actor.world.pos.y >= -1973.0f) {
            self->dyna.actor.velocity.y = 0.0f;
            BgSpot06Objects_LockSpawnWaterRipples(self, play, 1);
            EffectSsGSplash_Spawn(play, &self->dyna.actor.home.pos, NULL, NULL, 1, 700);
        } else if (self->dyna.actor.shape.rot.x == -0x4000) {
            self->dyna.actor.velocity.y += 0.02f;
            self->dyna.actor.world.pos.x = Rand_CenteredFloat(1.0f) + self->dyna.actor.home.pos.x;
            self->dyna.actor.world.pos.z = Rand_CenteredFloat(1.0f) + self->dyna.actor.home.pos.z;
            self->dyna.actor.velocity.y = (self->dyna.actor.velocity.y > 10.0f) ? (10.0f) : (self->dyna.actor.velocity.y);
            BgSpot06Objects_LockSpawnBubbles(self, play, 0);
        } else {
            BgSpot06Objects_LockSpawnBubbles(self, play, 0);

            if (Math_ScaledStepToS(&self->dyna.actor.shape.rot.x, -0x4000, 0x30)) {
                self->dyna.actor.home.pos.x = self->dyna.actor.world.pos.x;
                self->dyna.actor.home.pos.y = -1993.0f;
                self->dyna.actor.home.pos.z = self->dyna.actor.world.pos.z;
            }
        }
    }
}

s32 OnePointCutscene_Init(PlayState* play, s32 csId, s32 arg2, Actor* actor, s32 camId);

// Fish shaped lock waits to be hit by the hookshot, then pops out with bubbles.
// cfg split applied in the workspace (tail FUN_00159b2c stays asm).
void FUN_00159930(BgSpot06Objects* self, PlayState* play) {
    BgSpot06Objects* s2 = self;
    s32 i;
    Vec3f effectPos;
    f32 sin;
    f32 cos;
    Actor* thisx = &self->dyna.actor;
    BgSpot06Objects* s3 = self;

    if (self->collider.base.acFlags & 0x2) {
        self->timer = 195;
        s2->dyna.actor.flags |= 0x10;
        sin = Math_SinS(self->dyna.actor.world.rot.y);
        cos = Math_CosS(self->dyna.actor.world.rot.y);
        self->dyna.actor.world.pos.x += (3.0f * sin);
        self->dyna.actor.world.pos.z += (3.0f * cos);

        for (i = 0; i < 20; i++) {
            BgSpot06Objects_LockSpawnBubbles(self, play, 1);
        }

        effectPos.x = self->dyna.actor.world.pos.x + (5.0f * sin);
        effectPos.y = s3->dyna.actor.world.pos.y;
        effectPos.z = self->dyna.actor.world.pos.z + (5.0f * cos);

        for (i = 0; i < 3; i++) {
            EffectSsBubble_Spawn(play, &effectPos, 0.0f, 20.0f, 20.0f, (Rand_ZeroOne() * 0.1f) + 0.7f);
        }

        EffectSsGSplash_Spawn(play, &thisx->world.pos, NULL, NULL, 1, 700);
        self->collider.elements[0].dim.worldSphere.radius = 45;
        ((BgSpot06Objects*)thisx)->actionFunc = FUN_00182064;
        FUN_00372244(&play->unk_5FCC, 0x1E, 0x01000487);
        Flags_SetSwitch(play, self->switchFlag);
        OnePointCutscene_Init(play, 4120, 170, &self->dyna.actor, 0);
    } else {
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
    }
}

typedef struct {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
} BgSpot06DrawColor;
extern BgSpot06DrawColor BgSpot06Objects_sDrawColor;
void FUN_00358778(void* obj, s32 arg1, s32 arg2, BgSpot06DrawColor* color, s32 arg4);
void Collider_UpdateSpheres(s32 index, BgSpot06ColliderJntSph* collider, MTX34* mtx);

// PlayState::roomCtx bytes are not in the repo header; the variable offset keeps armcc's literal + register-offset load.
static inline u8 BgSpot06Objects_PlayByte(PlayState* play, s32 off) {
    u8* q = (u8*)play + off;
    return *q;
}

typedef struct {
    void* unk_00;
    void* unk_04;
} BgSpot06RoomPair;

void BgSpot06Objects_Draw(Actor* thisx, GameState* state) {
    BgSpot06Objects* self = (BgSpot06Objects*)thisx;
    PlayState* play = (PlayState*)state;
    MTX34 modelMtx;
    f32 animSpeed;

    MTX34CopyAsm(&modelMtx, &thisx->modelMtx);
    animSpeed = 1.0f;
    if (PauseContext_GetState() != 0) {
        animSpeed = 0.0f;
    }

    switch (thisx->params) {
        case 0:
            if (self->skelAnimModel != NULL) {
                self->skelAnimModel->SetUnkAC(1);
                self->skelAnimModel->FUN_003721e0(&modelMtx);
                self->skelAnimModel->FUN_00372170(0);
            }
            break;
        case 3:
            if (self->skelAnimModel != NULL) {
                self->skelAnimModel->SetUnkAC(1);
                self->skelAnimModel->FUN_003721e0(&modelMtx);
                self->skelAnimModel->FUN_00372170(0);
            }
            break;
        case 1:
            if (self->skelAnimModel != NULL) {
                self->skelAnimModel->unk_0C->SetAnimSpeed(animSpeed);
                self->skelAnimModel->SetUnkAC(1);
                self->skelAnimModel->FUN_003721e0(&modelMtx);
                self->skelAnimModel->FUN_00372170(0);
            }
            if (self->actionFunc == FUN_001288fc) {
                Collider_UpdateSpheres(1, &self->collider, &modelMtx);
            }
            break;
        case 2: {
            MTX34 waterMtx;
            BgSpot06DrawColor color;
            f32 waterSpeed;

            MTX34CopyAsm(&waterMtx, &thisx->modelMtx);
            waterSpeed = 1.0f;
            if (PauseContext_GetState() != 0) {
                waterSpeed = 0.0f;
            }

            if (self->lakeHyliaWaterLevel < -680.0f && gSaveContext.sceneSetupIndex < 4) {
                if (self->skelAnimModel1 != NULL) {
                    self->skelAnimModel1->unk_0C->SetAnimSpeed(waterSpeed);
                    self->skelAnimModel1->SetUnkAC(1);
                    self->skelAnimModel1->FUN_003721e0(&waterMtx);
                    self->skelAnimModel1->FUN_00372170(0);
                }
            } else {
                if (self->skelAnimModel2 != NULL) {
                    self->skelAnimModel2->unk_0C->SetAnimSpeed(waterSpeed);
                    self->skelAnimModel2->SetUnkAC(1);
                    self->skelAnimModel2->FUN_003721e0(&waterMtx);
                    self->skelAnimModel2->FUN_00372170(0);
                }
            }

            if (BgSpot06Objects_PlayByte(play, 0x4C30) == 0 && BgSpot06Objects_PlayByte(play, 0x4C36) != 0) {
                BgSpot06RoomPair* pair = (BgSpot06RoomPair*)((u8*)play + 0x4C38);
                color = BgSpot06Objects_sDrawColor;
                color.a = self->unk_244;
                FUN_00358778(pair->unk_04, 0x15, 0, &color, 2);
            }
            break;
        }
    }
}
}
