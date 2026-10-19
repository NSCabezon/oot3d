extern "C" {
#include "z_oceff_wipe3.hpp"
#include "model_manager.hpp"

#define FLAGS 0x02000010

void OceffWipe3_Init(Actor* actor, GameState* state);
void OceffWipe3_Destroy(Actor* thisx, PlayState* play);
void OceffWipe3_Update(Actor* thisx, PlayState* play);
void OceffWipe3_Draw(Actor* actor, GameState* state);

ActorInit Oceff_Wipe3_InitVars = {
    ACTOR_OCEFF_WIPE3,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(OceffWipe3),
    (ActorFunc)OceffWipe3_Init,
    (ActorFunc)OceffWipe3_Destroy,
    (ActorFunc)OceffWipe3_Update,
    (ActorFunc)OceffWipe3_Draw,
};

void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_0047d548(SkeletonAnimationModel* model, s32 arg);
void FUN_00367c54(void* obj);
void FUN_00367c60(void* obj, f32 arg);

void OceffWipe3_Init(Actor* actor, GameState* state) {
    OceffWipe3* self = (OceffWipe3*)actor;
    PlayState* play = (PlayState*)state;
    ZARInfo* zar;
    Camera* cam;

    Actor_SetScale(&self->actor, 0.1f);
    self->counter = 0;
    self->actor.world.pos = play->cameraPtrs[play->activeCamera]->eye;
    zar = &play->GetObjectStatus(self->actor.objBankIndex)->zarInfo;
    {
        ModelManager& mm = GetModelManager();
        mm.allocator->owner = self->actor.unk_178;
    }
    self->cmb = ZAR_GetCMBByIndex(zar, 0x36);
    {
        ModelManager& mm = GetModelManager();
        self->model = mm.allocator->Create(self->cmb, 1);
    }
    self->model->unk_AD[0] = 0;
    {
        ModelManager& mm = GetModelManager();
        mm.allocator->owner = NULL;
    }
    self->model->unk_0C->FUN_00372d94(zar->GetCMABByIndex(0x21));
    self->model->unk_0C->SetAnimType(1);
    FUN_0047d548(self->model, 2);
    self->actor.unk_19A = 1;
    cam = Gameplay_GetCamera(play, -1);
    FUN_00367c54(cam);
    FUN_00367c60(cam, *(f32*)((u8*)play + 0x1B0));
    self->camObj = cam;
}

void FUN_0034708c(PlayState* play); // Magic_Reset
void FUN_00367c48(void* obj);

void OceffWipe3_Destroy(Actor* thisx, PlayState* play) {
    Player* player = GET_PLAYER(play);
    void* obj;

    FUN_0034708c(play);
    if (gSaveContext.nayrusLoveTimer != 0) {
        u8* playerBase = (u8*)player + 0x1000;

        playerBase[0x72A] |= 0x40;
    }
    obj = *(void**)((u8*)thisx + 0x1A8);
    if (obj != NULL) {
        void** vtbl = *(void***)obj;

        ((void (*)(void*))vtbl[1])(obj);
        *(void**)((u8*)thisx + 0x1A8) = NULL;
    }
    FUN_00367c48(*(void**)((u8*)thisx + 0x1B0));
}

#define R_UPDATE_RATE SREG(30)

void OceffWipe3_Update(Actor* thisx, PlayState* play) {
    thisx->world.pos = play->cameraPtrs[play->activeCamera]->eye;
    if (*(s16*)((u8*)thisx + 0x1A4) < 3.0f / R_UPDATE_RATE * 100.0f) {
        (*(s16*)((u8*)thisx + 0x1A4))++;
    } else {
        Actor_Kill(thisx);
    }
}

// OceffWipe3_Draw: (structural) SetUnkAC `1` in callee-saved reg + inline translate/rotate/scale MTX34 on stack.
// Draft only (NOT compiled, not checked), N64 shape with 3DS layout.
#if 0
void OceffWipe3_Draw(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    OceffWipe3* self = (OceffWipe3*)actor;
    Vec3f eye = play->cameraPtrs[play->activeCamera]->eye;
    f32 z;
    u8 alpha;

    Vec3f quakeOffset = Camera_GetQuakeOffset(play->cameraPtrs[play->activeCamera]);
    f32 frames = (f32)self->counter * (f32)SREG(30) * (3.0f / 100.0f); // scaled to 3DS frame rate
    if (frames < 32.0f) {
        z = Math_SinS((s16)(frames * 512.0f)) * 1330.0f;
    } else {
        z = 1330.0f;
    }
    if (frames >= 80.0f) {
        alpha = (u8)((100.0f - frames) * 12.0f);
    } else {
        alpha = 255;
    }
    eye.x += quakeOffset.x;
    eye.y += quakeOffset.y;
    eye.z += quakeOffset.z;
    // MTX34 m = billboard * translate(eye) * scale(0.1) * translate(0,0,-z);
    // self->model->SetUnkAC(1); model->unk_0C->... ; FUN_003721e0(&m); FUN_00372170(...)
}
#endif

GLOBAL_ASM("asm/OceffWipe3_Draw.s")
}
