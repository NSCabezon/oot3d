extern "C" {
#include "z_bg_haka_huta.hpp"

#define FLAGS 0x80000010


s32 Player_InCsMode(PlayState* play);
void OnePointCutscene_Init(PlayState* play, s16 csId, s16 arg, Actor* actor, s32 camId);
void FUN_0012795c(BgHakaHuta* self, PlayState* play);
// still asm (data-coupled statics, see STATUS.md)
void FUN_003df814(BgHakaHuta* self, PlayState* play);
void BgHakaHuta_Init(Actor* actor, GameState* state);


// N64: BgHakaHuta_PlaySfx (inlined by armcc on 3DS)
static void BgHakaHuta_PlaySfx(BgHakaHuta* self, PlayState* play, u32 sfxId) {
    Vec3f pos;

    pos.z = (self->dyna.actor.shape.rot.y == 0) ? self->dyna.actor.world.pos.z + 120.0f
                                                : self->dyna.actor.world.pos.z - 120.0f;
    pos.x = self->dyna.actor.world.pos.x;
    pos.y = self->dyna.actor.world.pos.y;
    Audio_PlaySoundAtPosition(play, &pos, 30, sfxId);
}

// N64: BgHakaHuta_Open
void FUN_003ebcdc(BgHakaHuta* self, PlayState* play) {
    f32 posOffset;

    if (self->counter != 0) {
        self->counter--;
    }
    posOffset = (self->dyna.actor.world.rot.y == 0) ? 4.0f : -4.0f;
    Math_StepToF(&self->dyna.actor.world.pos.x, self->dyna.actor.home.pos.x + posOffset, 2.0f);
    if (self->counter == 0) {
        self->counter = 56;
        BgHakaHuta_PlaySfx(self, play, 0x01000255); // NA_SE_EV_COFFIN_CAP_OPEN
        self->actionFunc = (BgHakaHutaActionFunc)FUN_003df814;
    }
}

// N64: BgHakaHuta_SpawnEnemies
void FUN_003e69b8(BgHakaHuta* self, PlayState* play) {
    if (Flags_GetSwitch(play, self->dyna.actor.params) && !Player_InCsMode(play)) {
        self->counter = 38;
        self->actionFunc = (BgHakaHutaActionFunc)FUN_003ebcdc;
        OnePointCutscene_Init(play, 6001, 999, &self->dyna.actor, 0);
        if (self->unk_1C2 == 2) {
            Actor_Spawn(&play->actorCtx, play, ACTOR_EN_FIREFLY,
                        (self->dyna.actor.world.pos.x + (-25.0f) * Math_CosS(self->dyna.actor.shape.rot.y) +
                         40.0f * Math_SinS(self->dyna.actor.shape.rot.y)),
                        self->dyna.actor.world.pos.y - 10.0f,
                        (self->dyna.actor.world.pos.z - (-25.0f) * Math_SinS(self->dyna.actor.shape.rot.y) +
                         Math_CosS(self->dyna.actor.shape.rot.y) * 40.0f),
                        0, self->dyna.actor.shape.rot.y + 0x8000, 0, 2, 1);

            Actor_Spawn(&play->actorCtx, play, ACTOR_EN_FIREFLY,
                        (self->dyna.actor.world.pos.x + (-25.0f) * (Math_CosS(self->dyna.actor.shape.rot.y)) +
                         Math_SinS(self->dyna.actor.shape.rot.y) * 80.0f),
                        self->dyna.actor.world.pos.y - 10.0f,
                        (self->dyna.actor.world.pos.z - (-25.0f) * (Math_SinS(self->dyna.actor.shape.rot.y)) +
                         Math_CosS(self->dyna.actor.shape.rot.y) * 80.0f),
                        0, self->dyna.actor.shape.rot.y, 0, 2, 1);
        } else if (self->unk_1C2 == 1) {
            Actor_Spawn(&play->actorCtx, play, ACTOR_EN_RD,
                        (self->dyna.actor.home.pos.x + (-25.0f) * (Math_CosS(self->dyna.actor.shape.rot.y)) +
                         Math_SinS(self->dyna.actor.shape.rot.y) * 100.0f),
                        self->dyna.actor.home.pos.y - 40.0f,
                        (self->dyna.actor.home.pos.z - (-25.0f) * (Math_SinS(self->dyna.actor.shape.rot.y)) +
                         Math_CosS(self->dyna.actor.shape.rot.y) * 100.0f),
                        0, self->dyna.actor.shape.rot.y, 0, 0xFD, 1);
        }
    }
}

void BgHakaHuta_Destroy(Actor* actor, GameState* state);
void BgHakaHuta_Update(Actor* actor, GameState* state);
void BgHakaHuta_Draw(Actor* actor, GameState* state);


GLOBAL_ASM("data/z_Bg_Haka_Huta.data.s")

ActorInit Bg_Haka_Huta_InitVars = {
    ACTOR_BG_HAKA_HUTA,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HAKACH_OBJECTS,
    sizeof(BgHakaHuta),
    (ActorFunc)BgHakaHuta_Init,
    (ActorFunc)BgHakaHuta_Destroy,
    (ActorFunc)BgHakaHuta_Update,
    (ActorFunc)BgHakaHuta_Draw,
};

#if 0 // NEAR-MATCH attempt (FUN_0012795c 6 words off, FUN_003df814 4 words off); needs ld bss rule, see STATUS.md
void FUN_003e1f24(BgHakaHuta* self, PlayState* play); // N64: BgHakaHuta_DoNothing
void FUN_00366150(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, Color_RGBA8* prim, Color_RGBA8* env,
                  s16 scale, s32 life);
void FUN_003625f8(MTX34* dst, f32 angle, Vec3f* axis);
void FUN_0036c174(MTX34* dst, MTX34* a, MTX34* b);
void FUN_003624c8(MTX34* mtx, Vec3s* rot, s32 flag);
void Quake_SetSpeed(s32 idx, s16 speed);
void Quake_SetQuakeValues(s32 idx, s16 y, s16 x, s16 zoom, s16 rollOffset);
void Quake_SetCountdown(s32 idx, s16 countdown);
s32 Quake_Add(Camera* cam, u32 type);
f32 __hardfp_sinf(f32);
f32 __hardfp_cosf(f32);

// 3DS: function-local statics with dynamic init (guarded)
struct StaticVec3f {
    f32 x, y, z;
    StaticVec3f(f32 a, f32 b, f32 c) {
        x = a;
        y = b;
        z = c;
    }
};

static u32 sInitChain = 0x00640A92; // ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP)

// N64: BgHakaHuta_SlideOpen (with BgHakaHuta_SpawnDust inlined)
void FUN_003df814(BgHakaHuta* self, PlayState* play) {
    Vec3f effectPos;
    f32 posOffset;

    if (self->counter != 0) {
        self->counter--;
    }
    posOffset = (self->dyna.actor.world.rot.y == 0) ? 24.0f : -24.0f;
    if (!Math_StepToF(&self->dyna.actor.world.pos.x, self->dyna.actor.home.pos.x + posOffset, 0.5f)) {
        // Stage 3: with `static Vector3f` this is 4 words off (s0/s1 swap in the guarded ctor).
        // It matches EXACTLY with the contrived ctor below (x/y param mapping swapped; all args are 0
        // so semantics are identical) - rejected as a register-steering fake, kept here for reference:
        //   struct ZV { f32 x, y, z; ZV(f32 a, f32 b, f32 c) : x(b), y(a), z(c) {} };
        //   static ZV sEffectAccel(0.0f, 0.0f, 0.0f);
        static Vector3f sEffectAccel(0.0f, 0.0f, 0.0f);
        static Color_RGBA8 primColor = { 30, 20, 50, 255 };
        static Color_RGBA8 envColor = { 0, 0, 0, 255 };
        f32 phi;
        Vec3f effectVel;
        s32 i;
        f32 xPosOffset;

        phi = (self->dyna.actor.world.rot.y == 0) ? 1.0f : -1.0f;
        effectVel.x = -0.5f * phi;
        effectVel.y = 0.0f;
        effectVel.z = 0.0f;
        effectPos.y = self->dyna.actor.world.pos.y;
        effectPos.z = self->dyna.actor.world.pos.z;
        xPosOffset = (50.0f - ((self->dyna.actor.world.pos.x - self->dyna.actor.home.pos.x) * phi)) * phi;
        for (i = 0; i < 4; i++) {
            if (i == 2) {
                effectPos.z += 120.0f * phi;
            }
            effectPos.x = self->dyna.actor.home.pos.x - (xPosOffset * Rand_ZeroOne());
            FUN_00366150(play, &effectPos, &effectVel, (Vec3f*)&sEffectAccel, &primColor, &envColor,
                         (s16)((Rand_ZeroOne() * 10.0f) + 50.0f), 10);
        }
    }
    if (self->counter == 0) {
        effectPos.z = (self->dyna.actor.shape.rot.y == 0) ? self->dyna.actor.world.pos.z + 120.0f
                                                          : self->dyna.actor.world.pos.z - 120.0f;
        effectPos.x = self->dyna.actor.world.pos.x;
        effectPos.y = self->dyna.actor.world.pos.y;
        Audio_PlaySoundAtPosition(play, &effectPos, 50, 0x01000248); // NA_SE_EV_COFFIN_CAP_BOUND
        self->actionFunc = (BgHakaHutaActionFunc)FUN_0012795c;
    }
}

// N64: func_8087D720
void FUN_0012795c(BgHakaHuta* self, PlayState* play) {
    MTX34 mtx;
    Vec3f vec;
    s32 quakeIndex;
    f32 angle;
    f32 sn, cs;

    MTX34CopyAsm(&mtx, &self->dyna.actor.modelMtx);
    static Vector3f D_8087D958(30.0f, 0.0f, 0.0f);
    static Vector3f D_8087D964(0.03258f, 0.3258f, -0.9449f);
    self->counter++;
    if (self->counter == 9) {
        self->actionFunc = (BgHakaHutaActionFunc)FUN_003e1f24;
        quakeIndex = Quake_Add(((Camera**)((u8*)play + 0xA54))[*(s16*)((u8*)play + 0xA64)], 3);
        Quake_SetSpeed(quakeIndex, 0x7530);
        Quake_SetQuakeValues(quakeIndex, 4, 0, 0, 0);
        Quake_SetCountdown(quakeIndex, 2);
    } else if (self->counter == 0) {
        self->counter = 9;
        self->actionFunc = (BgHakaHutaActionFunc)FUN_003e1f24;
    }

    D_8087D958.x = self->counter + 24.0f;
    if (D_8087D958.x > 30.0f) {
        D_8087D958.x = 30.0f;
    }
    angle = self->dyna.actor.world.rot.y * 9.58738E-5f;
    if (angle != 0.0f) {
        sn = __hardfp_sinf(angle);
        cs = __hardfp_cosf(angle);
    } else {
        sn = 0.0f;
        cs = 1.0f;
    }
    mtx.data[0][0] = cs;
    mtx.data[0][1] = 0.0f;
    mtx.data[0][2] = sn;
    mtx.data[0][3] = 0.0f;
    mtx.data[1][0] = 0.0f;
    mtx.data[1][1] = 1.0f;
    mtx.data[1][2] = 0.0f;
    mtx.data[1][3] = 0.0f;
    mtx.data[2][0] = -sn;
    mtx.data[2][1] = 0.0f;
    mtx.data[2][2] = cs;
    mtx.data[2][3] = 0.0f;
    {
        MTX34 rotMtx;
        FUN_003625f8(&rotMtx, 0.10667452f * self->counter, (Vec3f*)&D_8087D964);
        FUN_0036c174(&mtx, &mtx, &rotMtx);
    }
    VEC3TransformAsm((Vector3f*)&vec, &mtx, (Vector3f*)&D_8087D958);
    self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x + vec.x;
    self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y + vec.y;
    self->dyna.actor.world.pos.z = self->dyna.actor.home.pos.z + vec.z;
    FUN_003624c8(&mtx, &self->dyna.actor.shape.rot, 0);
}


#endif

void BgHakaHuta_Init(Actor* actor, GameState* state) {
    BgHakaHuta* self = (BgHakaHuta*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(actor, (InitChainEntry*)0x00511108);
    DynaPolyActor_Init(&self->dyna, 1);
    self->unk_1C8 = NULL;
    FUN_00372f38(actor, play, &self->skelAnimModel, 1, NULL);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    self->unk_1C2 = (actor->params >> 8) & 0xFF;
    actor->params &= 0xFF;
    if (Flags_GetSwitch(play, actor->params)) {
        self->counter = -1;
        self->actionFunc = (BgHakaHutaActionFunc)FUN_0012795c;
    } else {
        self->actionFunc = (BgHakaHutaActionFunc)FUN_003e69b8;
    }
}

void BgHakaHuta_Destroy(Actor* actor, GameState* state) {
    BgHakaHuta* self = (BgHakaHuta*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgHakaHuta_Update(Actor* actor, GameState* state) {
    BgHakaHuta* self = (BgHakaHuta*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
}

void BgHakaHuta_Draw(Actor* actor, GameState* state) {
    BgHakaHuta* self = (BgHakaHuta*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

}
