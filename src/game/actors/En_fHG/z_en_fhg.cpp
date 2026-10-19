extern "C" {
#include "z_en_fhg.hpp"
#include "model_manager.hpp"

#define FLAGS 0x00000010

void EnfHG_Init(Actor* actor, GameState* state);
void EnfHG_Destroy(Actor* actor, GameState* state);
void EnfHG_Update(EnfHG* self, PlayState* play);
void EnfHG_Draw(Actor* actor, GameState* state);

void FUN_00362ab4(EnfHG* self, PlayState* play, s16 paintingIndex); // N64: EnfHG_SetupApproach
void FUN_0023929c(EnfHG* self, PlayState* play);                     // N64: EnfHG_Approach
void FUN_00126438(EnfHG* self, PlayState* play);                     // N64: EnfHG_Retreat
void FUN_0013ec58(EnfHG* self, PlayState* play);                     // N64: EnfHG_Attack

void FUN_00362a4c(SkelAnime* skelAnime, void* anim, f32 morphFrames); // Animation_MorphToLoop

typedef struct EnfHGPainting {
    Vec3f pos;
    s16 yRot;
} EnfHGPainting; // size = 0x10

typedef struct EnfHGOpposite {
    s16 idx[6];
} __attribute__((aligned(4))) EnfHGOpposite;

typedef struct EnfHGRodata {
    /* 0x00 */ u8 runningAnim[0x10];
    /* 0x10 */ u8 idleAnim[0x8];
    /* 0x18 */ EnfHGOpposite oppositeIndex;
} EnfHGRodata;

extern EnfHGRodata EnfHG_sRodata;
extern EnfHGPainting EnfHG_sPaintings[];

extern __pure float _fabsf(float);
static __inline __pure float fabsf(float f) {
    return _fabsf(f);
}

#define R_UPDATE_RATE SREG(30)
#define FRAMES(n) ((n) * 3.0f / R_UPDATE_RATE + 0.5f)
#define FOG_COLOR(play, i) (*(u8*)((u8*)(play) + 0xA82 + (i)))

GLOBAL_ASM("data/z_En_fHG.data.s")
GLOBAL_ASM("../src/game/actors/En_fHG/data_syms.s")

ActorInit En_fHG_InitVars = {
    ACTOR_EN_FHG,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_FHG,
    sizeof(EnfHG),
    (ActorFunc)EnfHG_Init,
    (ActorFunc)EnfHG_Destroy,
    (ActorFunc)EnfHG_Update,
    (ActorFunc)EnfHG_Draw,
};

void FUN_00358ea8(ZARInfo* zar, PlayState* play, SkelAnime* skel, void* cmb, void* p, s32 b, void* jointTable,
                  void* morphTable, s32 limbCount);
void* ZAR_GetCMBByIndex(ZARInfo* zar, s32 index);
void* ZAR_GetCTXBByIndex(ZARInfo* zar, s32 index);
void FUN_003490e0(SkelAnime* skelAnime, void* anim); // Animation_PlayLoop
void FUN_0017a63c(EnfHG* self, PlayState* play);     // N64: EnfHG_Intro
extern InitChainEntry EnfHG_sInitChain[];            // 0x005235C4 (data_syms.s)
extern u8 gUnk_005C1878[];                           // 0x005C1878 = GetModelManager() object + 0x32C0 (shared_syms.s)

// Inlines the shared GetModelManager() static (guard 0x0055A21C, include/model_manager.hpp).
void EnfHG_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnfHG* self = (EnfHG*)actor;
    ZARInfo* zar;
    void* cmb;

    Actor_ProcessInitChain(&self->actor, EnfHG_sInitChain);
    Flags_SetSwitch(play, 0x14);
    Actor_SetScale(&self->actor, 0.011499999f);
    self->actor.gravity = -3.5f;
    ActorShape_Init(&self->actor.shape, -2600.0f, NULL, 20.0f);
    self->actor.speedXZ = 0.0f;
    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += 70.0f;
    zar = &play->GetObjectStatus(self->actor.objBankIndex)->zarInfo;
    cmb = ZAR_GetCMBByIndex(zar, 0);
    FUN_00358ea8(zar, play, &self->skelAnime, cmb, self->actor.unk_178, 1, self->unk_2F4, self->unk_2F4 + 0x514, 0x19);
    ((EnfHG*)actor)->unk_D24 = -1;
    if (self->actor.params >= 10) {
        FUN_00362ab4(self, play, self->actor.params - 10);
    } else {
        FUN_003490e0(&self->skelAnime, EnfHG_sRodata.idleAnim);
        self->actionFunc = FUN_0017a63c;
        self->actor.world.pos.x = 14.0f;
        self->actor.world.pos.y = -300.0f;
        self->actor.world.pos.z = -3315.0f;
    }
    GetModelManager();
    self->unk_D28 = ZAR_GetCTXBByIndex(zar, *(s32*)(gUnk_005C1878 + 0xF3C));
}

void EnfHG_Destroy(Actor* actor, GameState* state) {
    EnfHG* self = (EnfHG*)actor;

    SkelAnime_Free2(&self->skelAnime);
}

GLOBAL_ASM("asm/FUN_00362ab4.s")

// NON_MATCHING: 3 words, timer vdiv/vadd register choice (s1 vs s0)
// 2026-10-14 sweep: swapping the scale.z and approachRate stores fixes the vdiv (2 words left = that store order);
// timer forms, f32 locals for 300/0.5/rate, play2/self alias/statement-swap single-site mutations did not.
#if 0
// recipe_near4 2026-10-15: still 3 words (timer: ROM vdiv s1,s0,s1 + 0.5 in s0). Tried 0.5f+x, casts, 300.0f/R, f32 temp/rate locals,
// statement position, single-site mutations. Only other ROM fn with this shape: EnHorse_Update (asm).
void FUN_00362ab4(EnfHG* self, PlayState* play, s16 paintingIndex) {
    EnfHGOpposite oppositeIndex = EnfHG_sRodata.oppositeIndex;

    FUN_00362a4c(&self->skelAnime, EnfHG_sRodata.runningAnim, 0.0f);
    self->actionFunc = FUN_0023929c;
    self->curPainting = paintingIndex;
    self->targetPainting = oppositeIndex.idx[self->curPainting];

    self->actor.world.pos.x = (1.3f * EnfHG_sPaintings[self->curPainting].pos.x) + 10.0f;
    self->actor.world.pos.y = EnfHG_sPaintings[self->curPainting].pos.y + 120.0f;
    self->actor.world.pos.z = (1.3f * EnfHG_sPaintings[self->curPainting].pos.z) - 3325.0f;
    self->actor.world.rot.y = EnfHG_sPaintings[self->curPainting].yRot;

    self->inPaintingPos.x = (EnfHG_sPaintings[self->targetPainting].pos.x * 1.3f) + 10.0f;
    self->inPaintingPos.y = EnfHG_sPaintings[self->targetPainting].pos.y + 0.0f;
    self->inPaintingPos.z = (EnfHG_sPaintings[self->targetPainting].pos.z * 1.3f) - 3325.0f;
    self->inPaintingVelX = (fabsf(self->inPaintingPos.x - self->actor.world.pos.x) * 2) * 0.01f;

    if (self->inPaintingVelX < 1.0f) {
        self->inPaintingVelX = 1.0f;
    }
    self->inPaintingVelZ = (fabsf(self->inPaintingPos.z - self->actor.world.pos.z) * 2) * 0.01f;
    if (self->inPaintingVelZ < 1.0f) {
        self->inPaintingVelZ = 1.0f;
    }

    self->timers[0] = FRAMES(100);
    self->actor.scale.x = 0.002f;
    self->actor.scale.y = 0.002f;
    self->actor.scale.z = 0.001f;
    self->approachRate = 0.0f;

    self->warpColorFilterR = FOG_COLOR(play, 0);
    self->warpColorFilterG = FOG_COLOR(play, 1);
    self->warpColorFilterB = FOG_COLOR(play, 2);
    self->warpColorFilterUnk1 = 0.0f;
    self->warpColorFilterUnk2 = 0.0f;
    self->turnRot = 0;
    self->turnTarget = 0;
    self->spawnedWarp = false;
}
#endif

void FUN_0023929c(EnfHG* self, PlayState* play) {
    if (self->actor.params == 1) {
        self->hoofSfxPos = self->actor.world.pos;
        if ((self->gallopTimer % 11) == 0) {
            Audio_PlaySoundGeneral(0x0100015B, &self->hoofSfxPos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                   &DAT_0054ac24.unk_0);
        }
    }
    FUN_00370734(&self->skelAnime);
    Math_ApproachF(&self->actor.scale.x, 0.011499999f, 1.0f, self->approachRate);
    Math_ApproachF(&self->approachRate, 0.0002f, 1.0f, 0.0000015f);
    Math_ApproachF(&self->actor.world.pos.y, 60.0f, 0.1f, 1.0f);
    self->actor.scale.y = self->actor.scale.x;
    if (self->timers[0] == 0) {
        if (self->actor.params != 1) {
            self->timers[0] = FRAMES(140);
            self->actionFunc = FUN_00126438;
            FUN_00362a4c(&self->skelAnime, EnfHG_sRodata.runningAnim, 0.0f);
            self->turnTarget = -0x8000;
        } else {
            self->actionFunc = FUN_0013ec58;
            Audio_PlayActorSound2(&self->actor, 0x01000194);
            self->timers[0] = FRAMES(40);
            Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, 0x6D, self->actor.world.pos.x,
                               self->actor.world.pos.y + 50.0f, self->actor.world.pos.z, 0,
                               self->actor.shape.rot.y + 0x8000, 0, 0x27);
            self->fhgFireKillWarp = false;
        }
    }
}

// Dead alias `s2` on the bossGndInPainting store gives the ROM's unconditional `mov r0,r4` on the kill path.
void EnfHG_Update(EnfHG* self, PlayState* play) {
    u8 i;
    EnfHG* s2 = self;

    if (self->killActor) {
        Actor_Kill(&self->actor);
        return;
    }
    self->gallopTimer++;
    s2->bossGndInPainting = true;
    for (i = 0; i < 5; i++) {
        DECR(self->timers[i]);
    }

    self->actionFunc(self, play);

    if (self->hitTimer != 0) {
        self->hitTimer--;
    }

    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += 70.0f;
    self->actor.shape.rot.y = self->actor.world.rot.y;

    self->actor.shape.yOffset = Math_SinS(self->hitTimer * 0x9000) * 700.0f * (self->hitTimer / 20.0f);
    self->actor.shape.rot.z = (s16)(Math_SinS(self->hitTimer * 0x7000) * 1500.0f) * (self->hitTimer / 20.0f);
}

GLOBAL_ASM("asm/EnfHG_Draw.s")

// NON_MATCHING: ~46 words, branch float regs + clamp const copies (s6/s5), vmovle vs vmovls
#if 0
typedef struct EnfHGColor {
    f32 r, g, b, a;
} EnfHGColor;

void FUN_0036879c(void* arg);
void FUN_0034e988(void* arg);
void FUN_00357388(Actor* actor, EnfHGColor* color, s32 arg2, s32 arg3);
void FUN_003583d4(Actor* actor, PlayState* play, void* arg2, s32 arg3, s32 arg4);

static inline const f32& EnfHG_Min(const f32& a, const f32& b) {
    return (b < a) ? b : a;
}

static inline const f32& EnfHG_Max(const f32& a, const f32& b) {
    return (a < b) ? b : a;
}

static inline void EnfHG_Clamp(f32* v, f32 min, f32 max) {
    f32 t = max;
    if (!(*v > max)) t = *v;
    *v = t;
    t = min;
    if (*v > min) t = *v;
    *v = t;
}

void EnfHG_Draw(Actor* actor, GameState* state) {
    EnfHG* self = (EnfHG*)actor;
    PlayState* play = (PlayState*)state;
    u8* bossGnd = (u8*)self->actor.parent;
    EnfHGColor color;
    s32 i;

    if ((*(u16*)(bossGnd + 0x248) & 4) && (bossGnd[0x271] == 0)) {
        FUN_0036879c(self->actor.unk_178);
        color.r = 1.0f;
        color.g = 0.196f;
        color.b = 0.0f;
        color.a = 1.0f - (color.r + color.g + color.b) * (2.0f / 3.0f);
    } else {
        FUN_0034e988(self->actor.unk_178);
        color.r = self->warpColorFilterR * (1.0f / 255.0f);
        color.g = self->warpColorFilterG * (1.0f / 255.0f);
        color.b = self->warpColorFilterB * (1.0f / 255.0f);
        color.a = 1.0f - (color.r + color.g + color.b) * (2.0f / 3.0f);
    }

    EnfHG_Clamp(&color.r, 0.0f, 1.0f);
    EnfHG_Clamp(&color.g, 0.0f, 1.0f);
    EnfHG_Clamp(&color.b, 0.0f, 1.0f);
    EnfHG_Clamp(&color.a, 0.0f, 1.0f);

    ((u8*)self->actor.unk_178)[0x1B7] = ((u8*)self->actor.unk_178)[0x1B6];
    ((u8*)self->actor.unk_178)[0x1B6] = 0;
    FUN_00357388(&self->actor, &color, 5, 0);
    ((u8*)self->actor.unk_178)[0x1B6] = ((u8*)self->actor.unk_178)[0x1B7];
    self->unk_2F0 = self->turnRot;
    FUN_003583d4(&self->actor, play, self->unk_25C, 0, 0x23);
}
#endif
}
