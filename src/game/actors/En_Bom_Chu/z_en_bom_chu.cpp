extern "C" {
#include "z_en_bom_chu.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x00000010
#define BOMBCHU_SCALE 0.01f

void EnBomChu_Init(Actor* actor, GameState* state);
void EnBomChu_Destroy(Actor* actor, GameState* state);
void EnBomChu_Update(Actor* actor, GameState* state);
void EnBomChu_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Bom_Chu.data.s")

ActorInit En_Bom_Chu_InitVars = {
    ACTOR_EN_BOM_CHU,
    ACTORCAT_EXPLOSIVES,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnBomChu),
    (ActorFunc)EnBomChu_Init,
    (ActorFunc)EnBomChu_Destroy,
    (ActorFunc)EnBomChu_Update,
    (ActorFunc)EnBomChu_Draw,
};

GLOBAL_ASM("../src/game/actors/En_Bom_Chu/data_syms.s")

// 3DS EffectBlure init params (N64 EffectBlureInit1). The vertex array element has an out-of-line empty constructor in
// the ROM (FUN_003fd780, aliased as _ZN15BlureInitVertexC1Ev in shared_syms.s).
struct BlureInitVertex {
    BlureInitVertex();
    u8 data[0x24];
};

typedef struct {
    /* 0x000 */ s32 unk_00;
    /* 0x004 */ BlureInitVertex vtx[16];
    /* 0x244 */ u8 p1StartColor[4];
    /* 0x248 */ u8 p2StartColor[4];
    /* 0x24C */ u8 p1EndColor[4];
    /* 0x250 */ u8 p2EndColor[4];
    /* 0x254 */ s32 elemDuration;
    /* 0x258 */ s32 unkFlag;
    /* 0x25C */ u8 calcMode;
    /* 0x260 */ s32 unk_260;
    /* 0x264 */ u8 unk_264;
} EnBomChuBlureInit; // size = 0x268

extern InitChainEntry EnBomChu_sInitChain[];
extern EnBomChuJntSphInit EnBomChu_sJntSphInit;
extern u8 EnBomChu_sColors[16]; // p1Start, p2Start, p1End, p2End

void Collider_InitJntSph(PlayState* play, EnBomChuColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, EnBomChuColliderJntSph* collider, Actor* actor, EnBomChuJntSphInit* init,
                        EnBomChuColliderElement* elements);
s32 Effect_Add(PlayState* play, s32* pIndex, s32 type, u8 arg3, u8 arg4, void* initParams);

void FUN_003e4ea4(EnBomChu* self, PlayState* play); // N64: EnBomChu_WaitForRelease
void FUN_00333ddc(EnBomChu* self, PlayState* play); // N64: EnBomChu_Explode

#if 0
void EnBomChu_Init(Actor* actor, GameState* state) {
    EnBomChu* self = (EnBomChu*)actor;
    PlayState* play = (PlayState*)state;
    EnBomChuBlureInit blureInit;
    s32 i;

    Actor_ProcessInitChain(&self->actor, EnBomChu_sInitChain);
    FUN_00372f38(actor, play, &self->model, 2, 0);
    Collider_InitJntSph(play, &self->collider);
    Collider_SetJntSph(play, &self->collider, &self->actor, &EnBomChu_sJntSphInit, self->colliderElements);

    self->collider.elements[0].worldSphere.radius = self->collider.elements[0].modelSphere.radius;

    {
        u8* p1Start = EnBomChu_sColors;
        u8* p2Start = p1Start + 4;
        u8* p1End = p2Start + 4;
        u8* p2End = p1End + 4;

        for (i = 0; i < 4; i++) {
            blureInit.p1StartColor[i] = p1Start[i];
            blureInit.p2StartColor[i] = p2Start[i];
            blureInit.p1EndColor[i] = p1End[i];
            blureInit.p2EndColor[i] = p2End[i];
        }
    }

    blureInit.elemDuration = 16;
    blureInit.unkFlag = 0;
    blureInit.calcMode = 0;
    blureInit.unk_264 = 14;

    Effect_Add(play, &self->blure1Index, 1, 0, 0, &blureInit);
    Effect_Add(play, &self->blure2Index, 1, 0, 0, &blureInit);

    self->actor.room = -1;
    self->timer = 180;
    self->actionFunc = FUN_003e4ea4;
    self->unk_1AA = 0;
}
#endif

GLOBAL_ASM("asm/EnBomChu_Init.s")


void Effect_Delete(PlayState* play, s32 index);
void Collider_DestroyJntSph(PlayState* play, EnBomChuColliderJntSph* collider);

GLOBAL_ASM("../split/asm/FUN_0020eb70.s")

void EnBomChu_Destroy(Actor* actor, GameState* state) {
    EnBomChu* self = (EnBomChu*)actor;
    PlayState* play = (PlayState*)state;

    Effect_Delete(play, self->blure1Index);
    Effect_Delete(play, self->blure2Index);
    Collider_DestroyJntSph(play, &self->collider);
    FUN_00350f34(actor, &self->model, 0);
}

GLOBAL_ASM("asm/EnBomChu_Update.s")

#if 0
struct EnBomChuVecIdx {
    f32& operator[](s32 i) { return (&x)[i]; }
    f32 x, y, z;
};

void EnBomChu_Draw(Actor* actor, GameState* state) {
    EnBomChu* self = (EnBomChu*)actor;
    MTX34 mtx;
    EnBomChuVecIdx jitter;
    f32 colorIntensity;
    s32 blinkHalfPeriod;
    s32 blinkTime;

    MTX34CopyAsm(&mtx, &actor->modelMtx);

    if (self->timer >= 40) {
        blinkTime = self->timer % 20;
        blinkHalfPeriod = 10;
    } else if (self->timer >= 10) {
        blinkTime = self->timer % 10;
        blinkHalfPeriod = 5;
    } else {
        blinkTime = self->timer & 1;
        blinkHalfPeriod = 1;
    }

    if (blinkTime > blinkHalfPeriod) {
        blinkTime = 2 * blinkHalfPeriod - blinkTime;
    }

    colorIntensity = blinkTime / (f32)blinkHalfPeriod;

    jitter[0] = self->visualJitter * (1.0f / BOMBCHU_SCALE);
    jitter[1] = 0.0f;
    jitter[2] = 0.0f;
    Matrix_UpdatePosition(&mtx, &mtx, (Vec3f*)&jitter);

    if (self->model != NULL) {
        FUN_003695cc(self->model, 1, 4, 0, colorIntensity * 255.0f * (1.0f / 255.0f), 0.0f, 0.0f,
                     (255.0f - colorIntensity * 255.0f) * (1.0f / 255.0f));
        self->model->SetUnkAC(1);
        self->model->FUN_003721e0(&mtx);
        self->model->FUN_00372170(0);
    }
}
#endif

GLOBAL_ASM("asm/EnBomChu_Draw.s")

void EffectSsBubble_Spawn(PlayState* play, Vec3f* pos, f32 a, f32 b, f32 c, f32 d);
void FUN_003de944(EnBomChu* self, PlayState* play); // N64: EnBomChu_Move

void FUN_003150fc(EnBomChu* self, CollisionPoly* floorPoly, PlayState* play); // N64: EnBomChu_UpdateFloorPoly
void FUN_00314fe4(PlayState* play, EnBomChu* self); // N64: Actor_PlaySfx_SurfaceBomb

#if 0
void FUN_003e4ea4(EnBomChu* self, PlayState* play) { // N64: EnBomChu_WaitForRelease
    Player* player = GET_PLAYER(play);

    if (self->timer != 0) {
        self->timer--;
    }

    self->unk_1AA = 0;

    if (self->timer == 0) {
        FUN_00333ddc(self, play);
        return;
    }

    if (Actor_HasNoParent(&self->actor, play)) {
        self->actor.world.pos = player->actor.world.pos;
        Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);
        self->actor.shape.rot.y = player->actor.shape.rot.y;

        self->axisForwards.x = Math_SinS(self->actor.shape.rot.y);
        self->axisForwards.y = 0.0f;
        self->axisForwards.z = Math_CosS(self->actor.shape.rot.y);

        self->axisUp.x = 0.0f;
        self->axisUp.y = 1.0f;
        self->axisUp.z = 0.0f;

        self->axisLeft.x = Math_SinS(self->actor.shape.rot.y + 0x4000);
        self->axisLeft.y = 0.0f;
        self->axisLeft.z = Math_CosS(self->actor.shape.rot.y + 0x4000);

        self->actor.speedXZ = 8.0f;
        FUN_003150fc(self, self->actor.floorPoly, play);
        self->actor.flags |= 1;
        FUN_00314fe4(play, self);
        self->actionFunc = FUN_003de944;
    }
}
#endif

void FUN_00333ddc(EnBomChu* self, PlayState* play) { // N64: EnBomChu_Explode
    s32 i;

    if (self->unk_1AB == 0) {
        Actor* bomb = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOM, self->actor.world.pos.x, self->actor.world.pos.y,
                                  self->actor.world.pos.z, 0, 0, 0, self->unk_1AA + 2, 1);
        self->unk_1AC = 1;
        if (bomb != NULL) {
            *(s16*)((u8*)bomb + 0x26C) = 0;
        }
    }

    self->timer = 1;
    self->actor.speedXZ = 0.0f;

    if (self->actor.yDistToWater > 0.0f) {
        for (i = 0; i < 40; i++) {
            EffectSsBubble_Spawn(play, &self->actor.world.pos, 1.0f, 5.0f, 30.0f, 0.25f);
        }
    }

    self->actionFunc = (EnBomChuActionFunc)0x0013CAF8; // EnBomChu_WaitForKill (raw: inside FUN_0013bb74 data tail)
}

}
