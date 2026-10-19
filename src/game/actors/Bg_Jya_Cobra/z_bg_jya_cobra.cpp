extern "C" {
#include "z_bg_jya_cobra.hpp"
#include "model_manager.hpp"

#define FLAGS 0x80400010

void BgJyaCobra_Init(Actor* actor, GameState* state);
void BgJyaCobra_Destroy(Actor* actor, GameState* state);
void BgJyaCobra_Update(Actor* actor, GameState* state);
void BgJyaCobra_Draw(Actor* actor, GameState* state);
void FUN_003bee08(BgJyaCobra* self, PlayState* play);
void FUN_003b342c(BgJyaCobra* self, PlayState* play);

GLOBAL_ASM("data/z_Bg_Jya_Cobra.data.s")
GLOBAL_ASM("../src/game/actors/Bg_Jya_Cobra/data_syms.s")

ActorInit Bg_Jya_Cobra_InitVars = {
    ACTOR_BG_JYA_COBRA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_JYA_OBJ,
    sizeof(BgJyaCobra),
    (ActorFunc)BgJyaCobra_Init,
    (ActorFunc)BgJyaCobra_Destroy,
    (ActorFunc)BgJyaCobra_Update,
    (ActorFunc)BgJyaCobra_Draw,
};



// Shared allocator interfaces already represented by their existing VFE keys.
extern "C++" {
struct BgJyaBigmirrorManager {
    virtual void unk_vf0(); virtual void unk_vf1(); virtual void unk_vf2(); virtual void unk_vf3();
    virtual void Remove(void* obj);
};
class OceffStormFreer {
public:
    virtual void vf00(); virtual void vf04(); virtual void vf08();
    virtual void* Alloc(u32 size, const char* file, s32 line);
    virtual void Free(void* model);
};
}
extern BgJyaBigmirrorManager* gUnk_0055A1A8;
extern OceffStormFreer* gUnk_0055A1F8;
void* BgJyaCobra_Stub(void*) __asm("GraphicsContext_UnknownStub");
void FUN_003685a0(void*);
void FUN_003445a8(void*);
void FUN_00348904(void*, void*);
void* FUN_003488e4(void*);

void BgJyaCobra_Destroy(Actor* actor, GameState* state) {
    BgJyaCobra* self = (BgJyaCobra*)actor;
    PlayState* play = (PlayState*)state;
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->models[0], &self->models[1], 0);
    FUN_00350f34(actor, &self->models[2], 0);
    if (self->object != NULL) {
        void* object = BgJyaCobra_Stub(self->object);
        gUnk_0055A1A8->Remove(object);
    }
    self->object = NULL;
    FUN_003685a0(&self->materialAnim);
    if (self->managedModel != NULL) {
        FUN_00348904(GetModelManager().unk_247C, self->managedModel);
        self->managedModel = NULL;
    }
    if (self->textureModel != NULL) {
        void* model = FUN_003488e4(self->textureModel);
        gUnk_0055A1F8->Free(model);
        self->textureModel = NULL;
    }
    FUN_003445a8(self->textureState[0]);
    FUN_003445a8(self->textureState[1]);
}

void FUN_0032b1c4(void*, void*, void*, s32);
void* FUN_00348f34(void*, void*);
void FUN_00348be4(void*);
void FUN_00348a64(void*, s32, void*, s32, s32, s32, s32);
void* FUN_0034897c(void*, void*, void*, s32);
void FUN_00278f58(Actor*, GameState*);
extern u8 BgJyaCobra_sTextureModelData[];
struct CobraManagedModel {
    u8 pad_000[0x170];
    u32 enabled;
    u32 pad_174;
    u32 flags;
};
void BgJyaCobra_Update(Actor* actor, GameState* state) {
    BgJyaCobra* self = (BgJyaCobra*)actor;
    PlayState* play = (PlayState*)state;
    FUN_003445a8(self->textureState[0]);
    FUN_003445a8(self->textureState[1]);
    FUN_0032b1c4(self->textureState[0], self->textureInfo, self->shadowTextureBuffer, 0);
    FUN_0032b1c4(self->textureState[1], self->textureInfo, self->shadowTextureBuffer, 0);
    self->textureModel = NULL;
    self->managedModel = NULL;
    {
        const char* file = "d:\\home\\queen\\dailyBuild\\game_us\\sources\\z_bg_jya_cobra.cpp";
        void* mem = gUnk_0055A1F8->Alloc(0x1B8, file, 1582);
        self->textureModel = mem != NULL ? FUN_00348f34(mem, BgJyaCobra_sTextureModelData) : NULL;
    }
    FUN_00348be4(self->textureModel);
    FUN_00348a64(self->textureModel, 0, self->textureState[self->textureIndex], 0x2601, 0x2601, 0x8370, 0x812F);
    self->managedModel = FUN_0034897c(GetModelManager().unk_247C, self->textureModel, self->dyna.actor.unk_178, 0);
    ((CobraManagedModel*)self->managedModel)->enabled = 0;
    { CobraManagedModel* m = (CobraManagedModel*)self->managedModel; m->flags |= 0x80; }
    { CobraManagedModel* m = (CobraManagedModel*)self->managedModel; m->flags |= 2; }
    play->GetObjectStatus(self->dyna.actor.objBankIndex);
    self->dyna.actor.update = FUN_00278f58;
}




void FUN_003679d0(f32, f32, f32, MTX34*, Vec3s*);
void Matrix_UpdatePosition(MTX34*, MTX34*, Vec3f*);
void FUN_00367788(void*, void*, s32);
extern Vec3s BgJyaCobra_sShadowRot0;
extern Vec3s BgJyaCobra_sShadowRot2;
extern Vec3f BgJyaCobra_sShadowScale[];
static inline void Cobra_Scale(MTX34* m, f32 x, f32 y, f32 z) {
    m->data[0][0] *= x; m->data[1][0] *= x; m->data[2][0] *= x;
    m->data[0][1] *= y; m->data[1][1] *= y; m->data[2][1] *= y;
    m->data[0][2] *= z; m->data[1][2] *= z; m->data[2][2] *= z;
}
GLOBAL_ASM("asm/FUN_001170b8.s")
#if 0 // Sol shadow draft
void FUN_001170b8(BgJyaCobra* self, PlayState* play) {
    Vec3f pos;
    Vec3s* rot;
    s16 params = self->dyna.actor.params & 3;
    MTX34 mtx;
    if (params == 0) {
        pos.x = self->dyna.actor.world.pos.x - 50.0f;
        pos.y = self->dyna.actor.world.pos.y;
        pos.z = self->dyna.actor.world.pos.z;
        rot = &BgJyaCobra_sShadowRot0;
    } else if (params == 2) {
        pos.x = self->dyna.actor.world.pos.x + 70.0f;
        pos.y = self->dyna.actor.world.pos.y;
        pos.z = self->dyna.actor.world.pos.z;
        rot = &BgJyaCobra_sShadowRot2;
    } else {
        rot = &self->dyna.actor.shape.rot;
        Math_Vec3f_Copy(&pos, &self->dyna.actor.world.pos);
    }
    pos.y += 1.0f;
    FUN_003679d0(pos.x, pos.y, pos.z, &mtx, rot);
    Cobra_Scale(&mtx, BgJyaCobra_sShadowScale[params].x, BgJyaCobra_sShadowScale[params].y, BgJyaCobra_sShadowScale[params].z);
    Vec3f trans = {0.0f, 0.0f, 40.0f};
    Matrix_UpdatePosition(&mtx, &mtx, &trans);
    if (self->managedModel != NULL) {
        *(MTX34*)((u8*)self->managedModel + 0xC) = mtx;
        ((CobraManagedModel*)self->managedModel)->enabled = 1;
        FUN_00367788((u8*)&GetModelManager() + 0x180, self->managedModel, 1);
    }
}


#endif

void FUN_001170b8(Actor*, PlayState*);
void FUN_001445e8(BgJyaCobra* self, PlayState* play) {
    self->actionFunc = FUN_003bee08;
    self->pushTimer = 0;
    self->dyna.actor.shape.rot.y = self->dyna.actor.world.rot.y =
        (self->rotationIndex * 0x2000) + self->dyna.actor.home.rot.y;
}

s32 Math_StepToS(s16*, s16, s16);
s32 Math_ScaledStepToS(s16*, s16, s16);
extern __pure float _fabsf(float);
static __inline __pure float cobra_fabsf(float x) { return _fabsf(x); }
static void BgJyaCobra_SetupRotate(BgJyaCobra* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 delta;
    s32 offset;
    self->actionFunc = (BgJyaCobraActionFunc)FUN_003b342c;
    delta = self->dyna.actor.yawTowardsPlayer - self->dyna.actor.shape.rot.y;
    offset = (s16)(self->dyna.actor.shape.rot.y - self->dyna.unk_1B0);
    offset = ABS(offset);
    if (delta > 0) {
        if (offset > 0x4000) self->direction = 1;
        else self->direction = -1;
    } else {
        if (offset > 0x4000) self->direction = -1;
        else self->direction = 1;
    }
    self->playerOffset.x = player->actor.world.pos.x - self->dyna.actor.world.pos.x;
    self->playerOffset.y = player->actor.world.pos.y - self->dyna.actor.world.pos.y;
    self->playerOffset.z = player->actor.world.pos.z - self->dyna.actor.world.pos.z;
    self->rotation = self->rotationSpeed = 0;
    self->movePlayer = 1;
}
void FUN_003bee08(BgJyaCobra* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    if (self->dyna.unk_1A8 > 0.001f) {
        s16 count = self->pushTimer + 1;
        self->pushTimer = count;
        if (count >= 15) BgJyaCobra_SetupRotate(self, play);
    } else self->pushTimer = 0;
    if (cobra_fabsf(self->dyna.unk_1A8) > 0.001f) {
        self->dyna.unk_1A8 = 0.0f;
        player->stateFlags2 &= ~0x10;
    }
}


#if 0 // Sol best: 8 words; integer range registers and player reload register.
static void BgJyaCobra_MovePlayer(BgJyaCobra* self, PlayState* play) {
    Player* target = GET_PLAYER(play);
    f32 sin = Math_SinS(self->rotation);
    f32 cos = Math_CosS(self->rotation);
    Vec3f offset;
    offset.x = self->playerOffset.z * sin + self->playerOffset.x * cos;
    offset.y = self->playerOffset.y;
    offset.z = self->playerOffset.z * cos - self->playerOffset.x * sin;
    target->actor.world.pos.x = self->dyna.actor.world.pos.x + offset.x;
    target->actor.world.pos.y = self->dyna.actor.world.pos.y + offset.y;
    target->actor.world.pos.z = self->dyna.actor.world.pos.z + offset.z;
}
void FUN_003b342c(BgJyaCobra* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 delta = (s16)(self->rotationIndex * 0x2000 + self->dyna.actor.home.rot.y) - self->dyna.actor.world.rot.y;
    if (ABS(delta) < 7424) Math_StepToS(&self->rotationSpeed, 106, 4);
    else Math_StepToS(&self->rotationSpeed, 21, 10);
    BgJyaCobra* s2 = self;
    if (Math_ScaledStepToS(&self->rotation, self->direction * 0x2000, self->rotationSpeed)) {
        self->rotationIndex = (self->rotationIndex + self->direction) & 7;
        player->stateFlags2 &= ~0x10;
        self->dyna.unk_1A8 = 0.0f;
        FUN_001445e8(self, play);
    } else {
        self->dyna.actor.shape.rot.y = self->dyna.actor.world.rot.y =
            self->rotationIndex * 0x2000 + self->dyna.actor.home.rot.y + s2->rotation;
    }
    if (player->stateFlags2 & 0x10) {
        if (s2->movePlayer) {
            BgJyaCobra_MovePlayer(self, play);
        }
    } else if (cobra_fabsf(self->dyna.unk_1A8) < 0.001f) s2->movePlayer = 0;
    self->dyna.unk_1A8 = 0.0f;
    FUN_00373264(&self->dyna.actor, 0x01000161);
}

#endif
#if 0 // Recovered unfinished draft; overlaps next ROM function.
void FUN_003bee08(Actor* actor, GameState* state) {
    u8* self = (u8*)actor;
    u8* play = (u8*)state;
    Actor* target = *(Actor**)(play + 0x20AC);
    f32 speed = *(f32*)(self + 0x1A8);
    const f32 threshold = 0.001f;
    if (speed <= threshold) {
        *(s16*)(self + 0x1C0) = 0;
    } else {
        s16 count = *(s16*)(self + 0x1C0) + 1;
        *(s16*)(self + 0x1C0) = count;
        if (count >= 15) {
            *(ActorFunc*)(self + 0x1BC) = (ActorFunc)FUN_003b342c;
            s16 delta = *(s16*)(self + 0x92) - *(s16*)(self + 0xBE);
            s16 offset = *(s16*)(self + 0xBE) - *(s16*)(self + 0x1B0);
            s16 direction = delta;
            if (offset < 0) offset = -offset;
            if (direction > 0) {
                *(s16*)(self + 0x1C2) = offset > 0x4000 ? -1 : 1;
            } else {
                *(s16*)(self + 0x1C2) = offset > 0x4000 ? 1 : -1;
            }
            *(f32*)(self + 0x1CC) = *(f32*)((u8*)target + 0x28) - *(f32*)(self + 0x28);
            *(f32*)(self + 0x1D0) = *(f32*)((u8*)target + 0x2C) - *(f32*)(self + 0x2C);
            *(f32*)(self + 0x1D4) = *(f32*)((u8*)target + 0x30) - *(f32*)(self + 0x30);
            *(s16*)(self + 0x1C6) = 0;
            *(s16*)(self + 0x1C8) = 0;
            self[0x1CA] = 1;
        }
    }
    if ((speed < 0.0f ? -speed : speed) > threshold) {
        *(f32*)(self + 0x1A8) = threshold;
        u8* flags = *(u8**)(play + 0x20AC) + 0x1714;
        *(u32*)flags &= ~0x10;
    }
}

#endif
void FUN_00373bec(void*);
void FUN_00331284(Player*, void*);
void FUN_0033e2a0(PlayState*, Actor*, void*);
void FUN_00358778(SkeletonAnimationModel*, s32, s32, void*, s32);
extern s16 BgJyaCobra_sRayRot[];
static void Cobra_UpdateMaterial(SkeletonAnimationModel_unk_0C* mat, f32 speed) { mat->SetAnimSpeed(speed); FUN_00373bec(mat); }
GLOBAL_ASM("asm/BgJyaCobra_Draw.s")
#if 0 // Sol: 11 words, VFP constant and rotation-table scheduling.
void BgJyaCobra_Draw(Actor* actor, GameState* state) {
    BgJyaCobra* self = (BgJyaCobra*)actor;
    PlayState* play = (PlayState*)state;
    Cobra_UpdateMaterial(&self->materialAnim, 1.0f);
    FUN_00331284(GET_PLAYER(play), actor->unk_178);
    FUN_0033e2a0(play, actor, self->object);
    {
        MTX34 mtx;
        MTX34CopyAsm(&mtx, &actor->modelMtx);
        if (self->models[0]) {
            self->models[0]->SetUnkAC(1);
            self->models[0]->FUN_003721e0(&mtx);
            self->models[0]->FUN_00372170(0);
        }
    }
    {
        MTX34 mtx;
        MTX34CopyAsm(&mtx, &actor->modelMtx);
        if (self->models[1]) {
            self->models[1]->SetUnkAC(1);
            self->models[1]->FUN_003721e0(&mtx);
            self->models[1]->FUN_00372170(0);
        }
    }
    if ((actor->params & 3) == 2) {
        Actor* parent = actor->parent;
        if (parent && (*(u8*)((u8*)parent + 0x1B4) & 4) && (*(u8*)((u8*)parent + 0x1B4) & 1))
            FUN_001170b8(actor, play);
    } else FUN_001170b8(actor, play);
    if (self->intensity > 0.0f) {
        Vec3s rot;
        MTX34 mtx;
        MTX34CopyAsm(&mtx, &actor->modelMtx);
        rot.x = actor->shape.rot.x + BgJyaCobra_sRayRot[actor->params & 3];
        rot.y = actor->shape.rot.y;
        rot.z = actor->shape.rot.z;
        FUN_003679d0(self->source.x, self->source.y, self->source.z, &mtx, &rot);
        Cobra_Scale(&mtx, 0.1f, 0.1f, self->length);
        if (self->models[2]) {
            struct CobraColor { f32 r,g,b,a; } rgba;
            rgba.r = 1.0f; rgba.g = 1.0f; rgba.b = 1.0f;
            rgba.a = self->intensity * (140.0f / 255.0f);
            FUN_00358778(self->models[2], 0, 5, &rgba, 2);
            self->models[2]->SetUnkAC(1);
            self->models[2]->FUN_003721e0(&mtx);
            self->models[2]->FUN_00372170(0);
        }
    }
}

#endif

GLOBAL_ASM("asm/BgJyaCobra_Init.s")
#if 0 // Sol initial draft: 237 words, 1144 vs 1128 bytes; 3DS texture init.
void* AutoClass1(void*);
ZARInfo* FUN_00340e14(Actor*, PlayState*, void*, SkeletonAnimationModel**, s32, s32);
extern InitChainEntry BgJyaCobra_sInitChain[];
extern s32 BgJyaCobra_sTopShadowAlpha[];
static void Cobra_InitTopShadow(BgJyaCobra* self) {
    f32 squared[64];
    s32 i,j;
    u16* shadow;
    for (i=0;i<64;i++) squared[i] = SQ(i - 31.5f);
    shadow = self->shadowTexture;
    Lib_MemSet((u8*)shadow, 0x2000, 0);
    for (i=0;i<64;i++) {
        for (j=0;j<64;j++) {
            f32 d = squared[j] * 0.5f + squared[i];
            if (d < 300.0f) shadow[i*64+j] = (640 - (s32)(d*2.0f) > 166 ? 166 : 640 - (s32)(d*2.0f)) >> 4;
        }
    }
    for (i=0;i<4;i++) {
        for (j=4;j<60;j++) {
            if ((shadow[0x780+i*64+j] & 15) < BgJyaCobra_sTopShadowAlpha[i]) {
                shadow[0x780+i*64+j] = (BgJyaCobra_sTopShadowAlpha[i] * 0x100) >> 8;
            }
        }
        shadow[0x780+i*64+60] = 2;
        shadow[0x780+i*64+3] = 2;
    }
    if ((self->dyna.actor.params & 3) == 1) self->dyna.actor.uncullZoneScale = 1200.0f;
}
void BgJyaCobra_Init(Actor* actor, GameState* state) {
    BgJyaCobra* self = (BgJyaCobra*)actor;
    PlayState* play = (PlayState*)state;
    self->managedModel = NULL;
    self->textureModel = NULL;
    FUN_00372f38(actor, play, &self->models[0], 15, &self->models[1], 14, 0);
    {
        const char* file = "d:\\home\\queen\\dailyBuild\\game_us\\sources\\z_bg_jya_cobra.cpp";
        // Slot 3 on the same object manager used by Destroy allocates this object.
        void* mem = ((OceffStormFreer*)gUnk_0055A1A8)->Alloc(0x234, file, 979);
        self->object = mem != NULL ? AutoClass1(mem) : NULL;
    }
    {
        ZARInfo* zar = FUN_00340e14(actor, play, self->object, &self->models[2], 16, 0);
        self->materialAnim.unk_00 = self->models[2]->unk_10;
        self->materialAnim.FUN_00372d94(zar->GetCMABByIndex(1));
    }
    self->materialAnim.SetAnimType(1);
    struct CobraTexData { u32 data[9]; } textureInfo = {{0}};
    *(CobraTexData*)self->textureInfo = textureInfo;
    // 3DS shadows use a 64x64 halfword texture, double buffered.
    Lib_MemSet((u8*)self->shadowTextureBuffer, 0x2020, 0);
    self->shadowTexture = (u16*)(((u32)self->shadowTextureBuffer + 15) & ~15);
    self->textureIndex = 0;
    *(u32*)(self->textureInfo+0) = 0x2000;
    *(u16*)(self->textureInfo+4) = 1;
    *(u8*)(self->textureInfo+6) = 0;
    *(u16*)(self->textureInfo+8) = 64;
    *(u16*)(self->textureInfo+10) = 64;
    *(u16*)(self->textureInfo+12) = 0x1908;
    *(u16*)(self->textureInfo+14) = 0x8033;
    CollisionHeader* colHeader = NULL;
    void* source = DynaPolyInfo_Alloc(&self->dyna, state, 3);
    DynaPolyActor_Init(&self->dyna, 0);
    ReadCollisionHeaderFromZsi(source, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    Actor_ProcessInitChain(actor, BgJyaCobra_sInitChain);
    if (!(actor->params & 3) && Flags_GetSwitch(play, (actor->params >> 8) & 63)) {
        actor->world.rot.y = actor->home.rot.y = actor->shape.rot.y = 0;
    }
    if (!(actor->params & 3)) {
        PlayState* play2 = play;
        Actor_SpawnAsChild(&play2->actorCtx, actor, play2, ACTOR_MIR_RAY, actor->world.pos.x, actor->world.pos.y+57.0f, actor->world.pos.z, 0,0,0,6);
    }
    FUN_001445e8(self, play);
    if ((actor->params & 3)==1 || (actor->params & 3)==2) actor->room=-1;
    if ((actor->params & 3)==1) Cobra_InitTopShadow(self);
    self->unk_2390=-1;
}

#endif
#if 0 // Sol large update draft: 488 words, 2212 vs 2196 bytes.
void Matrix_RotateX(MTX34*, f32, s32);
void Matrix_RotateY(MTX34*, f32, s32);
static inline void Math_Vec3f_Diff(Vec3f* a, Vec3f* b, Vec3f* out) { out->x=a->x-b->x; out->y=a->y-b->y; out->z=a->z-b->z; }
void FUN_0032b184(void*, u32);
extern u8 BgJyaCobra_sLightEnabled[];
extern s16 BgJyaCobra_sLightYaw[];
extern u8 BgJyaCobra_sSideMask[11][11];
extern u8 BgJyaCobra_sSmallMask[3][3];
extern Vec3f BgJyaCobra_sSidePoints[];
extern Vec3f BgJyaCobra_sBasePoints[];
struct CobraMirrorView {
    u8 pad[0x1B4];
    u8 puzzleFlags;
    u8 pad1B5[3];
    Actor* lightBeams[3];
};
struct CobraRayView {
    Actor actor;
    u8 pad[0x2E4-0x1A4];
    Vec3f source;
    Vec3f pool;
    u8 pad2FC[0x320-0x2FC];
    u8 unlit;
};
static void Cobra_UpdateLight(BgJyaCobra* self, PlayState* play) {
    s16 params = self->dyna.actor.params & 3;
    CobraMirrorView* mirror = (CobraMirrorView*)self->dyna.actor.parent;
    s16 angle;
    f32 target;
    if (params == 2 && mirror && (!(mirror->puzzleFlags&4) || !(mirror->puzzleFlags&1))) {
        Math_StepToF(&self->intensity, 0.0f, 0.05f);
    } else {
        self->intensity = 1.0f;
        if (BgJyaCobra_sLightEnabled[params]) {
            angle = self->dyna.actor.shape.rot.y - BgJyaCobra_sLightYaw[params];
            angle = ABS(angle);
            if (angle < 0x2000 && angle != -0x8000) {
                self->intensity += (angle-0x2000)*(3.0f/0x4000);
                if (self->intensity <= 0.0f) self->intensity=0.0f;
            }
        }
    }
    self->source.x=self->dyna.actor.world.pos.x;
    self->source.y=self->dyna.actor.world.pos.y+57.0f;
    self->source.z=self->dyna.actor.world.pos.z;
    if (params==0) self->length=0.1f;
    else if (params==1) {
        target=0.1f;
        angle=self->dyna.actor.shape.rot.y-0x8000;
        if (angle<0x500 && angle>-0x500) target=0.34f;
        else {
            angle=self->dyna.actor.shape.rot.y-0x4000;
            if (angle<0x500 && angle>-0x500 && mirror && (mirror->puzzleFlags&4)) target=0.34f;
        }
        Math_StepToF(&self->length,target,0.04f);
    } else if (params==2) {
        target=0.1f;
        angle=self->dyna.actor.shape.rot.y-0x8000;
        if (angle<0x500 && angle>-0x500) target=0.34f;
        else {
            angle=self->dyna.actor.shape.rot.y-0xC000;
            if (angle<0x500 && angle>-0x500) target=0.34f;
        }
        Math_StepToF(&self->length,target,0.04f);
    }
}
static void Cobra_UpdateRay(BgJyaCobra* self) {
    CobraMirrorView* mirror=(CobraMirrorView*)self->dyna.actor.parent;
    CobraRayView* ray=NULL;
    MTX34 mtx;
    MTX34CopyAsm(&mtx,&self->dyna.actor.modelMtx);
    switch (self->dyna.actor.params&3) {
        case 0:
            ray=(CobraRayView*)self->dyna.actor.child;
            if (!ray) return;
            if (!ray->actor.update) { self->dyna.actor.child=NULL; return; }
            break;
        case 1:
            ray=(CobraRayView*)mirror->lightBeams[1];
            if (!ray) return;
            break;
        case 2:
            ray=(CobraRayView*)mirror->lightBeams[2];
            if (!ray) return;
            break;
    }
    if (self->intensity<=0.0f) ray->unlit=1;
    else {
        Vec3f pos;
        f32 length;
        ray->unlit=0;
        Math_Vec3f_Copy(&ray->source,&self->source);
        Matrix_RotateY(&mtx,self->dyna.actor.shape.rot.y*(M_PI/32768.0f),0);
        Matrix_RotateX(&mtx,BgJyaCobra_sRayRot[self->dyna.actor.params&3]*(M_PI/32768.0f),1);
        length=self->length;
        if ((self->dyna.actor.params&3)==1 && length>0.21f) length=0.21f;
        pos.x=0.0f;pos.y=0.0f;pos.z=length*2800.0f;
        VEC3TransformAsm((Vector3f*)&ray->pool,&mtx,(Vector3f*)&pos);
        ray->pool.x=ray->source.x+ray->pool.x;
        ray->pool.y=ray->source.y+ray->pool.y;
        ray->pool.z=ray->source.z+ray->pool.z;
    }
}
static void Cobra_UpdateSideShadow(BgJyaCobra* self) {
    Vec3f diff, pos, transformed;
    MTX34 mtx;
    s32 x,z,tx,tz,i,j,k,l;
    s16 yaw;
    u16* texture=self->shadowTexture;
    Lib_MemSet((u8*)texture,0x2000,0);
    Matrix_RotateX(&mtx,M_PI/4.0f,0);
    yaw=!(self->dyna.actor.params&3)?self->dyna.actor.shape.rot.y+0x4000:self->dyna.actor.shape.rot.y-0x4000;
    if (self->unk_2390==yaw) return;
    self->unk_2390=yaw;
    Matrix_RotateY(&mtx,yaw*(M_PI/32768.0f),1);
    Cobra_Scale(&mtx,0.9f,0.9f,0.9f);
    for (i=0;i<25;i++) {
        Math_Vec3f_Diff(&BgJyaCobra_sSidePoints[i+1],&BgJyaCobra_sSidePoints[i],&diff);
        diff.x*=0.5f;diff.y*=0.5f;diff.z*=0.5f;
        for (j=0;j<2;j++) {
            pos.x=BgJyaCobra_sSidePoints[i].x+diff.x*j;
            pos.y=BgJyaCobra_sSidePoints[i].y+diff.y*j;
            pos.z=BgJyaCobra_sSidePoints[i].z+diff.z*j;
            VEC3TransformAsm((Vector3f*)&transformed,&mtx,(Vector3f*)&pos);
            x=(transformed.x+50.0f)*0.64f+0.5f;
            z=(88.0f-transformed.z)*0.64f+0.5f;
            for (k=0;k<11;k++) {
                tz=z-5+k;
                if (tz&~63) continue;
                tz*=64;
                for (l=0;l<11;l++) {
                    tx=x-5+l;
                    if (tx&~63) continue;
                    texture[tz+tx]|=BgJyaCobra_sSideMask[k][l];
                }
            }
        }
    }
    for (i=0;i<4;i++) {
        Math_Vec3f_Diff(&BgJyaCobra_sBasePoints[i+1],&BgJyaCobra_sBasePoints[i],&diff);
        diff.x*=0.2f;diff.y*=0.2f;diff.z*=0.2f;
        for (j=0;j<5;j++) {
            pos.x=BgJyaCobra_sBasePoints[i].x+diff.x*j;
            pos.y=BgJyaCobra_sBasePoints[i].y+diff.y*j;
            pos.z=BgJyaCobra_sBasePoints[i].z+diff.z*j;
            VEC3TransformAsm((Vector3f*)&transformed,&mtx,(Vector3f*)&pos);
            x=(transformed.x+50.0f)*0.64f+0.5f;
            z=(88.0f-transformed.z)*0.64f+0.5f;
            for (k=0;k<3;k++) {
                tz=z-1+k;
                if (tz&~63) continue;
                tz*=64;
                for (l=0;l<3;l++) {
                    tx=x-1+l;
                    if (tx&~63) continue;
                    texture[tz+tx]|=BgJyaCobra_sSmallMask[k][l];
                }
            }
        }
    }
    FUN_0032b184(texture,128);
    FUN_0032b184(texture+0xFC0,128);
    for (i=1;i<63;i++) {texture[i*64]=0;texture[i*64+63]=0;}
    self->textureIndex=1-self->textureIndex;
    FUN_0032b1c4(self->textureState[self->textureIndex],self->textureInfo,self->shadowTextureBuffer,0);
    FUN_00348a64(self->textureModel,0,self->textureState[self->textureIndex],0x2601,0x2601,0x8370,0x812F);
}
void FUN_00278f58(Actor* actor, GameState* state) {
    BgJyaCobra* self=(BgJyaCobra*)actor;
    PlayState* play=(PlayState*)state;
    self->actionFunc(self,play);
    Cobra_UpdateLight(self,play);
    Cobra_UpdateRay(self);
    if ((actor->params&3)==0 || (actor->params&3)==2) Cobra_UpdateSideShadow(self);
}

#endif

}
