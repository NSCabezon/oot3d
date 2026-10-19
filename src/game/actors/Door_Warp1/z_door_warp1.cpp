extern "C" {
#include "z_door_warp1.hpp"

#define FLAGS 0x00000030

void DoorWarp1_Init(Actor* actor, GameState* state);
void DoorWarp1_Destroy(Actor* actor, GameState* state);
void DoorWarp1_Update(Actor* actor, PlayState* state);
void DoorWarp1_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Door_Warp1.data.s")

ActorInit Door_Warp1_InitVars = {
    ACTOR_DOOR_WARP1,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_WARP1,
    sizeof(DoorWarp1),
    (ActorFunc)DoorWarp1_Init,
    (ActorFunc)DoorWarp1_Destroy,
    (ActorFunc)DoorWarp1_Update,
    (ActorFunc)DoorWarp1_Draw,
};

// Init draft (stage 3, Opus): 28/197 words off. thisx-> for ProcessInitChain/shape/world.pos/params(6,7 & switch)/unk_19A
// gives ROM's sl/r4 split; per-arm Setup helpers fix size. Left: armcc hoists &skelCurve/unk_178 above the 6/7 branch
// (ROM computes them per arm into fp/sb, cross-jumps the FUN_00337200 call). Needs EQU DoorWarp1_sInitChain=0x0051C748.
#if 0
extern InitChainEntry DoorWarp1_sInitChain[]; // ROM 0x0051C748 (needs EQU in an actor-dir data_syms.s)
void FUN_0015b904(DoorWarp1* self, PlayState* play);
void FUN_003357dc(void* skelCurve);
void FUN_00337200(void* skelCurve, PlayState* play, void* cmb, void* arg3, s32 arg4, void* arg5, s32 arg6);
void* ZAR_GetCMBByIndex(ZARInfo* zar, s32 index);
void* ZAR_GetCSABByIndex(ZARInfo* zar, s32 index);
void Lights_PointNoGlowSetInfo(void* info, f32 x, f32 y, f32 z, s32 a, s32 b, s32 c, s32 d, s32 e);
void* LightContext_InsertLight(PlayState* play, void* lightCtx, void* info);

static inline void DoorWarp1_SetupA(DoorWarp1* self, PlayState* play, ZARInfo* zar) {
    FUN_00337200(self->skelCurve, play, ZAR_GetCMBByIndex(zar, 2), self->actor.unk_178, 1, self->unk_3A8, 3);
}
static inline void DoorWarp1_SetupB(DoorWarp1* self, PlayState* play, ZARInfo* zar) {
    void* u = self->actor.unk_178; FUN_00337200(self->skelCurve, play, ZAR_GetCMBByIndex(zar, 1), u, 4, self->unk_3A8, 3);
}
void DoorWarp1_Init(Actor* thisx, PlayState* play) {
    DoorWarp1* self = (DoorWarp1*)thisx;
    ZARInfo* zar;
    s32 i;
    self->unk_488 = 0;
    self->unk_484 = 0.0f;
    self->unk_438 = 2.0f;
    self->unk_460 = 0;
    Actor_ProcessInitChain(thisx, DoorWarp1_sInitChain);
    ActorShape_Init(&thisx->shape, 0.0f, NULL, 0.0f);

    if (self->actor.params != 2 && self->actor.params != -2 && self->actor.params != 4 && self->actor.params != 6 &&
        self->actor.params != 7) {
        Lights_PointNoGlowSetInfo(self->upperLightInfo, thisx->world.pos.x, thisx->world.pos.y, thisx->world.pos.z, 0, 0, 0, 0, 0);
        self->upperLight = LightContext_InsertLight(play, (u8*)play + 0xA70, self->upperLightInfo);
        Lights_PointNoGlowSetInfo(self->lowerLightInfo, thisx->world.pos.x, thisx->world.pos.y, thisx->world.pos.z, 0, 0, 0, 0, 0);
        self->lowerLight = LightContext_InsertLight(play, (u8*)play + 0xA70, self->lowerLightInfo);
    }

    zar = &play->GetObjectStatus(self->actor.objBankIndex)->zarInfo;
    FUN_003357dc(self->skelCurve);
    self->zarInfo = zar;
    if (thisx->params == 6 || thisx->params == 7) {
        DoorWarp1_SetupA(self, play, zar);
    } else {
        DoorWarp1_SetupB(self, play, zar);
    }
    *((u8*)self->skelAnimModel + 0xAD) = 0;
    self->skelAnimModel->unk_0C->SetAnimType(1);
    for (i = 0; i < 9; i++) {
        self->cmabs[i] = zar->GetCMABByIndex(i);
    }
    self->unk_438 = 1.0f;
    self->skelAnimModel->unk_0C->SetAnimSpeed(1.0f);
    switch (thisx->params) {
        case 4:
            self->skelAnimModel->unk_0C->FUN_00372d94(ZAR_GetCSABByIndex(zar, 8));
            break;
        case 8:
            self->skelAnimModel->unk_0C->FUN_00372d94(ZAR_GetCSABByIndex(zar, 6));
            break;
        case 9:
            self->skelAnimModel->unk_0C->FUN_00372d94(ZAR_GetCSABByIndex(zar, 5));
            break;
        case 10:
            self->skelAnimModel->unk_0C->FUN_00372d94(ZAR_GetCSABByIndex(zar, 7));
            break;
        case 6:
        case 7:
            self->skelAnimModel->unk_0C->FUN_00372d94(self->cmabs[2]);
            break;
        default:
            self->skelAnimModel->unk_0C->FUN_00372d94(self->cmabs[4]);
            break;
    }
    thisx->unk_19A = 1;
    FUN_0015b904(self, play);
}

#endif

GLOBAL_ASM("asm/DoorWarp1_Init.s")

// Matched: `PlayState* p2 = play;` alias used only for the FUN_00374be8 arg fixes the entry reg order.
void DoorWarp1_Update(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    DoorWarp1* self = (DoorWarp1*)thisx;
    PlayState* p2 = play;
    if (gSaveContext.health <= 0) {
        return;
    }
    if (*(u16*)((u8*)play + 0x318C) != 0) {
        return;
    }
    if (FUN_00374be8(p2, 0xF) != 0) {
        return;
    }
    (*(void (**)(DoorWarp1*, PlayState*))((u8*)self + 0x490))(self, play);

    if (self->actor.params != 3) {
        Actor_SetScale(&self->actor, *(f32*)((u8*)self + 0x47C) * 0.01f);
    }
}

// Destroy draft, NOT matching (11/34 words: ROM loop counter in r0 / zero copy in r3, ours i=r1 zero=r0; store order
// fog,light1,ambient needs the chain written ambient = light1 = fog = 0). Stage 3 also tried ~400 more (inline helpers
// taking play/env/counter param, counter s32/u32, zero locals, raw/flat/sub-struct accesses): reg choice never moves.
// Tried u8/s32/s16 counters, do-while/DECR/!= forms, zero locals, env aliases, play2, GameState* signature,
// single-site thisx/self/play aliases. Needs prototype (Actor*, PlayState*) to enable.
#if 0
void FUN_0033579c(void* p);
void LightContext_RemoveLight(PlayState* play, void* lightCtx, void* light);

typedef struct DoorWarpEnvView {
    /* 0x0000 */ char pad_0000[0x31FC];
    /* 0x31FC */ s16 adjAmbientColor[3];
    /* 0x3202 */ s16 adjFogColor[3];
    /* 0x3208 */ s16 adjLight1Color[3];
} DoorWarpEnvView;

void DoorWarp1_Destroy(Actor* thisx, PlayState* play) {
    DoorWarp1* self = (DoorWarp1*)thisx;
    u8 i;

    LightContext_RemoveLight(play, (u8*)play + 0xA70, *(void**)((u8*)self + 0x494));
    LightContext_RemoveLight(play, (u8*)play + 0xA70, *(void**)((u8*)self + 0x4B0));

    DoorWarpEnvView* env = (DoorWarpEnvView*)play;

    for (i = 0; i < 3; i++) {
        env->adjAmbientColor[i] = env->adjLight1Color[i] = env->adjFogColor[i] = 0;
    }
    FUN_0033579c((u8*)self + 0x360);
    if (self->actor.params == -2 || self->actor.params == -1 || self->actor.params == 3) {
        SkelAnime_Free2((SkelAnime*)((u8*)self + 0x1A4));
    }
}
#endif

GLOBAL_ASM("asm/DoorWarp1_Destroy.s")

GLOBAL_ASM("asm/DoorWarp1_Draw.s")
}
