extern "C" {
#include "z_bg_ice_shelter.hpp"

#define FLAGS 0x00400010

void BgIceShelter_Init(Actor* actor, GameState* state);
void BgIceShelter_Destroy(Actor* actor, PlayState* play);
void BgIceShelter_Update(Actor* actor, PlayState* play);
void BgIceShelter_Draw(Actor* actor, PlayState* play);

// Shared with the N64 layout: type in params bits 8..10, no-switch flag bit 6, switch flag bits 0..5.
#define BGICESHELTER_GET_TYPE(actor) (((u16)(actor)->params >> 8) & 7)
#define BGICESHELTER_NO_SWITCH_FLAG(actor) (((u16)(actor)->params >> 6) & 1)

enum RedIceType {
    RED_ICE_LARGE,
    RED_ICE_SMALL,
    RED_ICE_PLATFORM,
    RED_ICE_WALL,
    RED_ICE_KING_ZORA,
};

typedef void (*BgIceShelterSpawnFunc)(BgIceShelter* self, PlayState* play, f32 particleSpawningChance,
                                      f32 steamEffectScale);

// Action functions and steam spawners stay as FUN_ symbols (ld-pinned names).
void FUN_003bf214(BgIceShelter* self, PlayState* play); // N64: BgIceShelter_Idle
void FUN_003f5f00(BgIceShelter* self, PlayState* play); // N64: BgIceShelter_Melt
void FUN_001a2fa0(BgIceShelter* self, PlayState* play, f32 particleSpawningChance, f32 steamEffectScale); // N64: BgIceShelter_SpawnSteamAround
void FUN_001cf550(BgIceShelter* self, PlayState* play, f32 particleSpawningChance, f32 unusedArg); // N64: BgIceShelter_SpawnSteamAlong
void FUN_00366150(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor,
                  Color_RGBA8* envColor, s16 scale, s16 life);

// Tables of the steam/melt code: absolute symbols in BgIceShelter_data.s (keeps armcc from folding them together).
extern Color_RGBA8 BgIceShelter_sSteamColors[];
extern Color_RGBA8 BgIceShelter_sSteamPrimColor[];
extern Color_RGBA8 BgIceShelter_sSteamEnvColor[];
extern s16 BgIceShelter_sSteamCircleAngles[];
extern s16 BgIceShelter_sSteamLinePositions[];
extern f32 BgIceShelter_sMeltingRates[];
extern f32 BgIceShelter_sSteamEffectScales[];
extern BgIceShelterSpawnFunc BgIceShelter_sSteamSpawnFuncs[];
extern f32 BgIceShelter_signs[];


typedef struct BgIceShelterDrawColor {
    /* 0x0 */ f32 r;
    /* 0x4 */ f32 g;
    /* 0x8 */ f32 b;
    /* 0xC */ f32 a;
} BgIceShelterDrawColor;
extern BgIceShelterDrawColor BgIceShelter_sDrawColor;
void FUN_00358778(SkeletonAnimationModel* skelAnimModel, s32 arg1, s32 arg2, BgIceShelterDrawColor* color, s32 arg4);

GLOBAL_ASM("../src/game/actors/Bg_Ice_Shelter/BgIceShelter_data.s")

GLOBAL_ASM("data/z_Bg_Ice_Shelter.data.s")

ActorInit Bg_Ice_Shelter_InitVars = {
    ACTOR_BG_ICE_SHELTER,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_ICE_OBJECTS,
    sizeof(BgIceShelter),
    (ActorFunc)BgIceShelter_Init,
    (ActorFunc)BgIceShelter_Destroy,
    (ActorFunc)BgIceShelter_Update,
    (ActorFunc)BgIceShelter_Draw,
};

// First 8 bytes of the actor's data (0x512028); cut from the data chunk (data_chunks.txt: z_Bg_Ice_Shelter.data starts 0x512030).
// Real statics so armcc derives prim from env (`sub r0, r1, #4`) like the ROM.
static Color_RGBA8 sSteamPrimColor = { 250, 250, 250, 255 };
static Color_RGBA8 sSteamEnvColor = { 180, 180, 180, 255 };

GLOBAL_ASM("asm/BgIceShelter_Init.s")

// UNMATCHED draft (BgIceShelter_Init stays GLOBAL_ASM): ~structurally equal, but ROM keeps two copies of the actor pointer
// (mov r4,r0 / mov r5,r0 at entry) and 'self'/'actor' alias tricks did not reproduce that; callee-saved regs shift.
// Note: ReadCollisionHeaderFromZsi is link-inlined (str r0,[r1]), so fastchk shows a bl there.
#if 0
extern InitChainEntry BgIceShelter_sInitChain[];
extern f32 BgIceShelter_sRedIceScales[];
extern ColliderCylinderInit BgIceShelter_sCylinderInit1;
extern ColliderCylinderInit BgIceShelter_sCylinderInit2;
extern s16 BgIceShelter_sCylinderRadii[];
extern s16 BgIceShelter_sCylinderHeights[];
extern unsigned int BgIceShelter_sKingZoraScaleGuard;
extern Vec3f BgIceShelter_sKingZoraRedIceScale;
int __cxa_guard_acquire(unsigned int* guard_object);
void __cxa_guard_release(unsigned int* guard_object);


static inline void BgIceShelter_Vec3fCtor(Vec3f* v, f32 x, f32 y, f32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

// N64: BgIceShelter_InitDynaPoly (3DS: collision comes from the object ZSI, selected by index).
static inline void BgIceShelter_InitDynaPoly(BgIceShelter* self, PlayState* play, GameState* state, s32 index,
                                             s32 moveFlag) {
    CollisionHeader* colHeader = NULL;
    void* zsi = DynaPolyInfo_Alloc(&self->dyna, state, index);

    DynaPolyActor_Init(&self->dyna, moveFlag);
    ReadCollisionHeaderFromZsi(zsi, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
}

// N64: BgIceShelter_InitColliders
static inline void BgIceShelter_InitColliders(BgIceShelter* self, PlayState* play) {
    s32 type = BGICESHELTER_GET_TYPE(&self->dyna.actor);

    Collider_InitCylinder(play, &self->cylinder1);
    Collider_SetCylinder(play, &self->cylinder1, &self->dyna.actor, &BgIceShelter_sCylinderInit1);
    Collider_UpdateCylinder(&self->dyna.actor, &self->cylinder1);

    self->cylinder1.dim.radius = BgIceShelter_sCylinderRadii[type];
    self->cylinder1.dim.height = BgIceShelter_sCylinderHeights[type];

    if (type == RED_ICE_LARGE || type == RED_ICE_SMALL || type == RED_ICE_KING_ZORA) {
        Collider_InitCylinder(play, &self->cylinder2);
        Collider_SetCylinder(play, &self->cylinder2, &self->dyna.actor, &BgIceShelter_sCylinderInit2);
        Collider_UpdateCylinder(&self->dyna.actor, &self->cylinder2);
        self->cylinder2.dim.radius = BgIceShelter_sCylinderRadii[type];
        self->cylinder2.dim.height = BgIceShelter_sCylinderHeights[type];
    }

    // 3DS addition: a params flag doubles the cylinder heights.
    if (((u16)self->dyna.actor.params << 16) < 0) {
        self->cylinder1.dim.height *= 2.0f;
        self->cylinder2.dim.height *= 2.0f;
    }

    if (type == RED_ICE_KING_ZORA) {
        self->cylinder1.dim.pos.z += 30.0f;
        self->cylinder2.dim.pos.z += 30.0f;
    }
}

void BgIceShelter_Init(Actor* actor, GameState* state) {
    BgIceShelter* self = (BgIceShelter*)(DynaPolyActor*)actor;
    PlayState* play = (PlayState*)state;
    s16 type = BGICESHELTER_GET_TYPE(actor);
    ZARInfo* zar;

    Actor_ProcessInitChain(actor, BgIceShelter_sInitChain);

    if (type == RED_ICE_KING_ZORA) {
        actor->world.pos.y -= 45.0f;
        actor->world.pos.z -= 38.0f;
        actor->world.rot.x += 0xBB8;
        actor->shape.rot.x = actor->world.rot.x;
    }

    if (type == RED_ICE_KING_ZORA) {
        if (!(((unsigned int*)BgIceShelter_sSteamColors)[2] & 1) &&
            __cxa_guard_acquire(&BgIceShelter_sKingZoraScaleGuard)) {
            BgIceShelter_Vec3fCtor(&BgIceShelter_sKingZoraRedIceScale, 0.18f, 0.27f, 0.24f);
            __cxa_guard_release(&BgIceShelter_sKingZoraScaleGuard);
        }
        Math_Vec3f_Copy(&actor->scale, &BgIceShelter_sKingZoraRedIceScale);
    } else {
        Actor_SetScale(actor, BgIceShelter_sRedIceScales[type]);
    }

    zar = FUN_00372f38(actor, play, &self->skelAnimModel[0], 2, &self->skelAnimModel[1],
                                                     1, &self->skelAnimModel[2], 7, 0);
    self->skelAnimModel[0]->unk_0C->FUN_00372d94(zar->GetCMABByIndex(1));
    self->skelAnimModel[0]->unk_0C->SetAnimType(1);
    self->skelAnimModel[2]->unk_0C->FUN_00372d94(zar->GetCMABByIndex(0));
    self->skelAnimModel[2]->unk_0C->SetAnimType(1);

    switch (type) {
        case RED_ICE_PLATFORM:
            BgIceShelter_InitDynaPoly(self, play, state, 1, 0);
            break;
        case RED_ICE_WALL:
            BgIceShelter_InitDynaPoly(self, play, state, 4, 0);
            break;
    }

    BgIceShelter_InitColliders(self, play);

    actor->colChkInfo.mass = 0xFF;

    if (!((actor->params >> 6) & 1) && Flags_GetSwitch(play, actor->params & 0x3F)) {
        Actor_Kill(actor);
        return;
    }

    self->actionFunc = (BgIceShelterActionFunc)FUN_003bf214;
    self->alpha = 255;
    actor->unk_19B = 2;

    if ((u16)play->sceneNum == 9 && BGICESHELTER_GET_TYPE(actor) != RED_ICE_KING_ZORA) {
        actor->flags |= 0x80000000;
    }
}
#endif

void BgIceShelter_Destroy(Actor* actor, PlayState* play) {
    BgIceShelter* self = (BgIceShelter*)actor;

    switch (BGICESHELTER_GET_TYPE(&self->dyna.actor)) {
        case RED_ICE_PLATFORM:
        case RED_ICE_WALL:
            DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
            break;

        case RED_ICE_LARGE:
        case RED_ICE_SMALL:
        case RED_ICE_KING_ZORA:
            Collider_DestroyCylinder(play, &self->cylinder2);
            break;
    }

    Collider_DestroyCylinder(play, &self->cylinder1);
    // Three model slots; the include prototype of FUN_00350f34 only declares the first two.
    FUN_00350f34(
        actor, &self->skelAnimModel[0], &self->skelAnimModel[1], &self->skelAnimModel[2], 0);
}

void BgIceShelter_Update(Actor* actor, PlayState* play) {
    BgIceShelter* self = (BgIceShelter*)actor;

    self->actionFunc(self, play);
}

// Idle (N64: BgIceShelter_Idle). Checks for blue fire, also freezes King Zora.
void FUN_003bf214(BgIceShelter* self, PlayState* play) {
    s32 type = BGICESHELTER_GET_TYPE(&self->dyna.actor);

    if (type == RED_ICE_KING_ZORA) {
        if (self->dyna.actor.parent != NULL) {
            self->dyna.actor.parent->freezeTimer = 15000;
        }
    }

    if (self->cylinder1.base.acFlags & AC_HIT) {
        self->cylinder1.base.acFlags &= ~AC_HIT;

        if ((self->cylinder1.base.ac != NULL) && (self->cylinder1.base.ac->id == ACTOR_EN_ICE_HONO)) {
            if (type == RED_ICE_KING_ZORA) {
                if (self->dyna.actor.parent != NULL) {
                    self->dyna.actor.parent->freezeTimer = 75;
                }
            }

            self->actionFunc = (BgIceShelterActionFunc)FUN_003f5f00;
            self->alpha = 255;
            Audio_PlayActorSound2(&self->dyna.actor, 0x010001F9);
        }
    }

    switch (type) {
        case RED_ICE_LARGE:
        case RED_ICE_SMALL:
        case RED_ICE_KING_ZORA:
            CollisionCheck_SetOC(play, &play->colChkCtx, &self->cylinder1.base);
            CollisionCheck_SetAC(play, &play->colChkCtx, &self->cylinder2.base);
            break;
    }

    CollisionCheck_SetAC(play, &play->colChkCtx, &self->cylinder1.base);
}

// Melt (N64: BgIceShelter_Melt). Lowers alpha and height, spawns steam, kills the actor when fully melted.
void FUN_003f5f00(BgIceShelter* self, PlayState* play) {
    s32 type = BGICESHELTER_GET_TYPE(&self->dyna.actor);
    f32 particleSpawningChance;

    self->alpha -= 5;
    self->alpha = CLAMP(self->alpha, 0, 255);

    self->dyna.actor.scale.y += BgIceShelter_sMeltingRates[type];
    self->dyna.actor.scale.y = CLAMP_MIN(self->dyna.actor.scale.y, 0.0001f);

    if (self->alpha > 80) {
        switch (type) {
            case RED_ICE_LARGE:
            case RED_ICE_SMALL:
            case RED_ICE_KING_ZORA:
                CollisionCheck_SetOC(play, &play->colChkCtx, &self->cylinder1.base);
                CollisionCheck_SetAC(play, &play->colChkCtx, &self->cylinder2.base);
                break;
        }
    }

    if (self->alpha > 180) {
        particleSpawningChance = 1.0f;
    } else if (self->alpha > 60) {
        particleSpawningChance = 0.5f;
    } else {
        particleSpawningChance = 0.0f;
    }

    BgIceShelter_sSteamSpawnFuncs[type](self, play, particleSpawningChance, BgIceShelter_sSteamEffectScales[type]);

    if (self->alpha <= 0) {
        if (!((self->dyna.actor.params >> 6) & 1)) {
            Flags_SetSwitch(play, self->dyna.actor.params & 0x3F);
        }

        if (type == RED_ICE_KING_ZORA) {
            FUN_00372244((u8*)play + 0x5C00 + 0x3CC, 30, 0x01000487);
        }

        Actor_Kill(&self->dyna.actor);
    }
}

// SpawnSteamAround (N64: BgIceShelter_SpawnSteamAround). Steam in a circle around the ice.
// Needs the colors as real statics of this TU (ROM derives prim as env - 4) and the outer-loop hoisted
// distance/icePos plus a steamSize local assigned in the loop.
void FUN_001a2fa0(BgIceShelter* self, PlayState* play, f32 particleSpawningChance, f32 steamEffectScale) {
    f32 cos;
    f32 sin;
    f32 distance;
    Vec3f* icePos;
    s16 angle;
    s16 frameCounter;
    s32 i;
    Vec3f steamPos;
    Vec3f steamVel;
    Vec3f steamAccel;
    s16 steamSize;

    frameCounter = (s16)play->state.frames & 7;
    icePos = &self->dyna.actor.world.pos;
    distance = 42.0f * steamEffectScale;

    for (i = 0; i < 2; i++) {
        if (particleSpawningChance < Rand_ZeroOne()) {
            continue;
        }

        angle = BgIceShelter_sSteamCircleAngles[frameCounter] + (i * 0x8000);
        sin = Math_SinS(angle);
        cos = Math_CosS(angle);

        steamPos.x = (distance * sin) + icePos->x;
        steamPos.y = (16.0f * steamEffectScale) + icePos->y;
        steamPos.z = (distance * cos) + icePos->z;

        steamVel.x = ((Rand_ZeroOne() * 3.0f) - 1.0f) * sin;
        steamVel.y = 0.0f;
        steamVel.z = ((Rand_ZeroOne() * 3.0f) - 1.0f) * cos;

        steamAccel.x = 0.07f * sin;
        steamAccel.y = 0.8f;
        steamAccel.z = 0.07f * cos;

        steamSize = 450.0f * steamEffectScale;
        FUN_00366150(play, &steamPos, &steamVel, &steamAccel, &sSteamPrimColor, &sSteamEnvColor, steamSize,
                     (s16)((Rand_ZeroOne() * 40.0f) + 40.0f) * steamEffectScale);
    }
}

// UNMATCHED draft (FUN_001cf550 stays GLOBAL_ASM): 29 words differ (fastchk). Real color statics fix the env/450 hoisting;
// left: ROM hoists 20.0f/40.0f where ours hoists 0.2f/0.0f, plus a GPR rotation (signs/table/rot.y). The proper fix is
// signs[] + sSteamLinePositions as real statics too (ROM: signs = positions - 0xBC), which needs the whole data chunk
// up to 0x5120F0 in C (incl. Init's guard word at 0x512030) -> data split.
#if 0
// SpawnSteamAlong (N64: BgIceShelter_SpawnSteamAlong). Steam along the ice wall.
static inline void BgIceShelter_Vec3fSum(Vec3f* a, Vec3f* b, Vec3f* dest) {
    dest->x = a->x + b->x;
    dest->y = a->y + b->y;
    dest->z = a->z + b->z;
}

// plain static (late inline): 29 words; static inline: 36.
static void BgIceShelter_RotateY(Vec3f* dest, Vec3f* src, s16 angle) {
    f32 sin = Math_SinS(angle);
    f32 cos = Math_CosS(angle);

    dest->x = (src->z * sin) + (src->x * cos);
    dest->y = src->y;
    dest->z = (src->z * cos) - (src->x * sin);
}

void FUN_001cf550(BgIceShelter* self, PlayState* play, f32 particleSpawningChance, f32 unusedArg) {
    Vec3f* icePos;
    s16 frameCounter;
    Vec3f steamPos;
    Vec3f steamVel;
    Vec3f steamAccel;
    Vec3f posOffset;
    s32 i;
    f32* signs;

    frameCounter = (s16)play->state.frames & 7;
    icePos = &self->dyna.actor.world.pos;
    signs = (f32*)((u8*)BgIceShelter_sSteamLinePositions - 0xBC);

    for (i = 0; i < 2; i++) {
        if (particleSpawningChance < Rand_ZeroOne()) {
            continue;
        }

        posOffset.x = (BgIceShelter_sSteamLinePositions[frameCounter] + ((Rand_ZeroOne() * 12.0f) - 6.0f)) * signs[i];
        posOffset.y = 15.0f;
        posOffset.z = ((84.0f - posOffset.x) * 0.2f) + (Rand_ZeroOne() * 20.0f);

        BgIceShelter_RotateY(&steamPos, &posOffset, self->dyna.actor.world.rot.y);
        BgIceShelter_Vec3fSum(&steamPos, icePos, &steamPos);

        steamVel.x = (Rand_ZeroOne() * 3.0f) - 1.5f;
        steamVel.y = 0.0f;
        steamVel.z = (Rand_ZeroOne() * 3.0f) - 1.5f;

        steamAccel.x = (Rand_ZeroOne() * 0.14f) - 0.07f;
        steamAccel.y = 0.8f;
        steamAccel.z = (Rand_ZeroOne() * 0.14f) - 0.07f;

        FUN_00366150(play, &steamPos, &steamVel, &steamAccel, &sSteamPrimColor, &sSteamEnvColor, 450,
                     (s16)((Rand_ZeroOne() * 40.0f) + 40.0f));
    }
}
#endif


void BgIceShelter_Draw(Actor* actor, PlayState* play) {
    BgIceShelter* self = (BgIceShelter*)actor;
    MTX34 modelMtx;
    BgIceShelterDrawColor color;
    f32 animSpeed;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    animSpeed = 1.0f;
    if (PauseContext_GetState() != 0) {
        animSpeed = 0.0f;
    }

    color = BgIceShelter_sDrawColor;
    color.a = self->alpha * (1.0f / 255.0f);
    FUN_00358778(self->skelAnimModel[0], 0, 4, &color, 2);
    FUN_00358778(self->skelAnimModel[0], 1, 4, &color, 2);

    switch (BGICESHELTER_GET_TYPE(&self->dyna.actor)) {
        case RED_ICE_LARGE:
        case RED_ICE_SMALL:
        case RED_ICE_KING_ZORA:
            if (self->skelAnimModel[0] != NULL) {
                self->skelAnimModel[0]->unk_0C->SetAnimSpeed(animSpeed);
                self->skelAnimModel[0]->SetUnkAC(1);
                self->skelAnimModel[0]->FUN_003721e0(&modelMtx);
                self->skelAnimModel[0]->FUN_00372170(1);
            }
            break;

        case RED_ICE_PLATFORM:
            if (self->skelAnimModel[1] != NULL) {
                self->skelAnimModel[1]->SetUnkAC(1);
                self->skelAnimModel[1]->FUN_003721e0(&modelMtx);
                self->skelAnimModel[1]->FUN_00372170(1);
            }
            break;

        case RED_ICE_WALL:
            if (self->skelAnimModel[2] != NULL) {
                self->skelAnimModel[2]->unk_0C->SetAnimSpeed(animSpeed);
                self->skelAnimModel[2]->SetUnkAC(1);
                self->skelAnimModel[2]->FUN_003721e0(&modelMtx);
                self->skelAnimModel[2]->FUN_00372170(1);
            }
            break;
    }
}
}
