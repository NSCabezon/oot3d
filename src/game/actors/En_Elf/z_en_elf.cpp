extern "C" {
#include "z_en_elf.hpp"
#include "model_manager.hpp"

#define FLAGS 0x02000030

void EnElf_Init(Actor* actor, GameState* state);
void EnElf_Destroy(Actor* thisx, PlayState* play);
void EnElf_Update(Actor* thisx, PlayState* play);
void EnElf_Draw(Actor* thisx, PlayState* play);

GLOBAL_ASM("data/z_En_Elf.data_00522E74.s")
GLOBAL_ASM("data/z_En_Elf.data_005230C4.s")

ActorInit En_Elf_InitVars = {
    ACTOR_EN_ELF,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnElf),
    (ActorFunc)EnElf_Init,
    (ActorFunc)EnElf_Destroy,
    (ActorFunc)EnElf_Update,
    (ActorFunc)EnElf_Draw,
};

GLOBAL_ASM("asm/EnElf_Init.s")

extern "C++" {
class OceffStormFreer {
public:
    virtual void vf00();
    virtual void vf04();
    virtual void vf08();
    virtual void* Alloc(u32 size, const char* file, s32 line);
    virtual void Free(void* model);
};
}
extern OceffStormFreer* gUnk_0055A1F8;

void LightContext_RemoveLight(PlayState* play, void* lightCtx, void* lightNode);
void FUN_00348904(void* mgr, void* model);
void* FUN_003488e4(void* model);

void EnElf_Destroy(Actor* thisx, PlayState* play) {
    EnElf* self = (EnElf*)thisx;
    s32 i;

    LightContext_RemoveLight(play, (void*)((u8*)play + 0xA70), self->lightNodeGlow);
    LightContext_RemoveLight(play, (void*)((u8*)play + 0xA70), self->lightNodeNoGlow);
    for (i = 0; i < 2; i++) {
        FUN_00348904(GetModelManager().unk_247C, self->unk_918[i]);
        self->unk_918[i] = NULL;
        if (self->unk_910[i] != NULL) {
            void* model = FUN_003488e4(self->unk_910[i]);
            gUnk_0055A1F8->Free(model);
        }
        self->unk_910[i] = NULL;
    }
    SkelAnime_Free2(&self->skelAnime);
}

void EnElf_Update(Actor* thisx, PlayState* play) {
    EnElf* self = (EnElf*)thisx;

    self->actionFunc(self, play);
    self->actor.shape.rot.y = self->unk_9BC;
    self->timer++;
}

// EnElf_Draw draft, 173 of 282 words differ (far). 3DS draws the two glow models (unk_918[0..1]: pos/scale/colour/visible)
// instead of N64 SkelAnime_Draw. Shape is close (checks, FRAMES-style timer scaling, Vector3f pos/scale in memory,
// ldm/stm copies); remaining: ROM keeps colour in a single stack Vec4 built element-wise and copied twice, a dead
// (s32)(-1800/rate + 0.5) before the disappearTimer scaling, and VFP numbering (s17=1/3, s18=0.5, s16=copy of 0.5).
#if 0
void FUN_00341d5c(PlayState* play, SkelAnime* skel, s32 a, s32 b, Actor* actor, s16 alpha);
void FUN_00371eac(void* model, s32 arg);

#define ELF_RATE (gGameInfo->data[126])
#define ELF_KREG90 (gGameInfo->data[27 * 96 + 90])
#define ELF_FRAMES(n) \
    ((n) > 0 ? (s32)((f32)(n) * (f32)ELF_RATE * (1.0f / 3.0f) + 0.5f) : (s32)((f32)(n) * (f32)ELF_RATE * (1.0f / 3.0f) - 0.5f))

void EnElf_Draw(Actor* thisx, PlayState* play) {
    EnElf* self = (EnElf*)thisx;
    Player* player = GET_PLAYER(play);
    s32 envAlpha;
    f32 alphaScale;
    f32 sz;
    f32 sn;
    s32 t;

    if (self->type == 8) {
        return;
    }
    if (self->fairyFlags & 8) {
        return;
    }
    if ((player->stateFlags1 & 0x100000) && (thisx->unk_EC.z > (f32)ELF_KREG90)) {
    } else if (player->stateFlags1 & 0x100000) {
        return;
    }
    if (self->actor.scale.x == 0.0f) {
        return;
    }
    if ((self->shadowAlpha != 0.0f) && !(self->fairyFlags & 0x200)) {
        FUN_00341d5c(play, &self->skelAnime, 0, 0, &self->actor, (s16)(self->shadowAlpha * 255.0f));
    }
    if (self->intensity == 0.0f) {
        return;
    }
    Vector3f pos(self->actor.modelMtx.data[0][3], self->actor.modelMtx.data[1][3], self->actor.modelMtx.data[2][3]);
    t = ELF_FRAMES(self->timer);
    sn = Math_SinS((s16)(t << 12));
    sz = (1.0f + sn * 0.1f) * 0.012f;
    if (self->fairyFlags & 0x200) {
        sz *= 2.0f;
    }
    sz = self->actor.scale.x * (sz * 125.0f) * 1000.0f * 1.3f;
    Vector3f scale(sz, sz, sz);

    self->unk_918[0]->visible = 1;
    self->unk_918[0]->pos = *(Vec3f*)&pos;
    self->unk_918[0]->scale = *(Vec3f*)&scale;
    f32 half = 0.5f;
    scale.x *= half;
    scale.y *= half;
    scale.z *= half;
    self->unk_918[1]->visible = 1;
    self->unk_918[1]->pos = *(Vec3f*)&pos;
    self->unk_918[1]->scale = *(Vec3f*)&scale;
    scale.x *= half;
    scale.y *= half;
    scale.z *= half;

    t = ELF_FRAMES(self->timer);
    envAlpha = (t * 50) & 0x1FF;
    if (envAlpha >= 0x100) {
        envAlpha = 0x1FF - envAlpha;
    }
    alphaScale = 1.0f;
    if (self->disappearTimer < 0) {
        alphaScale = 1.0f + (f32)ELF_FRAMES(self->disappearTimer) * (7.0f / 6000.0f);
    }
    EnElfColor c(self->innerColor[0] / 255.0f, self->innerColor[1] / 255.0f, self->innerColor[2] / 255.0f,
                 (self->innerColor[3] * self->intensity * alphaScale) / 255.0f);
    *(EnElfColor*)self->unk_918[1]->color = c;
    c.v[0] = self->outerColor[0] / 255.0f;
    c.v[1] = self->outerColor[1] / 255.0f;
    c.v[2] = self->outerColor[2] / 255.0f;
    c.v[3] = (self->intensity * ((f32)envAlpha * alphaScale)) / 255.0f;
    *(EnElfColor*)self->unk_918[0]->color = c;
    FUN_00371eac(self->unk_918[0], 1);
    FUN_00371eac(self->unk_918[1], 1);
}
#endif

GLOBAL_ASM("asm/EnElf_Draw.s")
}
