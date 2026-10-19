extern "C" {
#include "z_en_viewer.hpp"
#include "model_manager.hpp"

#define FLAGS 0x00000010

void EnViewer_Init(Actor* actor, GameState* state);
void EnViewer_Destroy(Actor* actor, GameState* state);
void EnViewer_Update(Actor* actor, GameState* state);
void EnViewer_Draw(Actor* actor, GameState* state);
void FUN_00408aac(u8* skin);

typedef void (*EnViewerDrawFunc)(EnViewer*, PlayState*);

static inline void EnViewer_CallVtblFree(void* obj) {
    void** vtbl = *(void***)obj;
    ((void (*)(void*))vtbl[1])(obj);
}

GLOBAL_ASM("data/z_En_Viewer.data.s")

ActorInit En_Viewer_InitVars = {
    ACTOR_EN_VIEWER,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnViewer),
    (ActorFunc)EnViewer_Init,
    (ActorFunc)EnViewer_Destroy,
    (ActorFunc)EnViewer_Update,
    (ActorFunc)EnViewer_Draw,
};

// Unlabeled data inside the data dump above / another actor's data -> EQU symbols in data_syms.s.
GLOBAL_ASM("../src/game/actors/En_Viewer/data_syms.s")
extern u8 EnViewer_sStaticByte;
extern InitChainEntry EnViewer_sInitChain[];
extern EnViewerDrawFunc EnViewer_sDrawFuncs[];

GLOBAL_ASM("asm/EnViewer_Init.s")

// Draft of EnViewer_Init (NOT compiled): loader via GetModelManager() compiles, 114/172 words differ.
// Remaining: state/type callee-saved regs swapped (ROM: state=r4,type=r5), ROM keeps constant 1 in sl across loop,
// extra 8 bytes. Tried ~20 variants (decl order, switch, raw state, (self,play) sig, frame hoist/sink, s32 vs u32).
#if 0
void FUN_0015b794(EnViewer* self, PlayState* play);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_0047d548(SkeletonAnimationModel* model, s32 arg);
void FUN_003586ec(void* arg);
extern s32 gUnk_0054C8CC;

void EnViewer_Init(Actor* actor, GameState* state) {
    u32 frame;
    PlayState* play = (PlayState*)state;
    EnViewer* self = (EnViewer*)actor;
    u8 type;
    ZARInfo* zar;
    s32 i;
    void* cmb;
    s32 fr;

    Actor_ProcessInitChain(actor, EnViewer_sInitChain);
    self->actionFunc = FUN_0015b794;
    EnViewer_sStaticByte = 0;
    self->unused = 0;
    self->state = 0;
    self->isVisible = 0;
    self->ganondorfCape = NULL;
    type = actor->params >> 8;
    frame = type * 11;
    if (type == 3 || type == 5 || type == 7 || type == 8 || type == 9) {
        self->ganondorfCape = Actor_SpawnAsChild(&play->actorCtx, actor, play, 0x16F, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0x23);
        if (type == 5) {
            zar = &play->GetObjectStatus(actor->objBankIndex)->zarInfo;
            {
                ModelManager& mm = GetModelManager();
                mm.allocator->owner = actor->unk_178;
            }
            cmb = ZAR_GetCMBByIndex(zar, 0x29);
            for (i = 0; i < 20; i++) {
                SkeletonAnimationModel* model;
                {
                    ModelManager& mm = GetModelManager();
                    model = mm.allocator->Create(cmb, 1);
                }
                self->animModels[i] = model;
                FUN_0047d548(model, 2);
                {
                    SkeletonAnimationModel_unk_0C* m0 = self->animModels[i]->unk_0C;
                    m0->FUN_00372d94(zar->GetCMABByIndex(4));
                }
                fr = frame % 60;
                self->animModels[i]->unk_0C->SetAnimType(1);
                {
                    SkeletonAnimationModel_unk_0C* m1 = self->animModels[i]->unk_0C;
                    if (gUnk_0054C8CC == 0) {
                        *(f32*)&m1->curFrame = (f32)fr;
                        FUN_003586ec(m1);
                    }
                }
            }
            {
                ModelManager& mm = GetModelManager();
                mm.allocator->owner = NULL;
            }
            actor->unk_19A = 1;
        }
    }
}

#endif

void EnViewer_Destroy(Actor* actor, GameState* state) {
    EnViewer* self = (EnViewer*)actor;
    s32 i;

    FUN_00408aac((u8*)self->unk_1A4);
    ((FaceAnimations*)self->unk_28C)->Destroy();
    if ((u8)(actor->params >> 8) == 5) {
        for (i = 0; i < 20; i++) {
            if (self->animModels[i] != NULL) {
                EnViewer_CallVtblFree(self->animModels[i]);
                self->animModels[i] = NULL;
            }
        }
    }
}

void EnViewer_Update(Actor* actor, GameState* state) {
    EnViewer* self = (EnViewer*)actor;

    self->actionFunc(self, (PlayState*)state);
}

void EnViewer_Draw(Actor* actor, GameState* state) {
    EnViewer* self = (EnViewer*)actor;
    PlayState* play = (PlayState*)state;
    EnViewerDrawFunc* drawFuncs;
    s16 type;

    if (self->isVisible) {
        type = actor->params >> 8;
        drawFuncs = EnViewer_sDrawFuncs;
        if (type < 3) {
            if (!(play->GetCutsceneState() != 0 && *(void**)((u8*)play + 0x22DC) != NULL)) {
                goto end;
            }
        } else {
            if (!(play->GetCutsceneState() != 0 && *(void**)((u8*)play + 0x22E0) != NULL) && type != 9) {
                goto end;
            }
        }
        GraphicsContext_UnknownStub(state->gfxCtx);
        drawFuncs[self->drawFuncIndex](self, play);
    }
end:
    ;
}
}
